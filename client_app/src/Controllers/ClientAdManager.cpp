#include "ClientAdManager.h"
#include "ClientAdTransferQtModel.h"
#include "ConfigManager.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include <QApplication>
#include <QtCore>
#include <QDir>
#include <QFile>
#include "utils.h"

//////////////////////////////////////////////////////////////////////////
// interval to update clientAd from server
#ifdef NDEBUG
// release
const int UPDATE_CLIENT_AD_INTERVAL = 3600000; // 3,600,000 ms = 1h
#else
const int UPDATE_CLIENT_AD_INTERVAL = 20000; // 20s
#endif


//////////////////////////////////////////////////////////////////////////
// after client launches, delay some time to request clientAd file from server
#ifdef NDEBUG // release
// delay 20s to request clientAd file when client app launching
const int REQUEST_CLIENT_AD_DELAY_INTERVAL = 20000; // 20,000 ms = 20s
#else
const int REQUEST_CLIENT_AD_DELAY_INTERVAL = 2000; // 2s
#endif

//////////////////////////////////////////////////////////////////////////
// verify clientAd file every one minute, if cafe changes it, restore it back
#ifdef NDEBUG // release
const int VERIFY_CLIENT_AD_INTERVAL = 60000; // 60,000 ms = 60s = 1min
#else
const int VERIFY_CLIENT_AD_INTERVAL = 2000; // 2s
#endif



const QString EMBED_CLIENT_AD_FILE_PATH = ":/ad/ad/ad";
const QString AdFolder = "CAd";
const QString CLIENT_AD_FILE_NAME_WITH_EXT = "ad";
const QString BAK_FILE_NAME_WITH_EXT = "ad.bak";

ClientAdManager::~ClientAdManager()
{
    if (m_updateClientAdTimer != NULL)
    {
        if (m_updateClientAdTimer->isActive())
        {
            m_updateClientAdTimer->stop();
        }

        m_updateClientAdTimer->deleteLater();
        m_updateClientAdTimer = NULL;
    }

    if (m_verifyClientAdTimer != NULL)
    {
        if (m_verifyClientAdTimer->isActive())
        {
            m_verifyClientAdTimer->stop();
        }

        m_verifyClientAdTimer->deleteLater();
        m_verifyClientAdTimer = NULL;
    }
}

bool ClientAdManager::init()
{
    qRegisterMetaType<ClientAdTransferReplyQtModel>("ClientAdTransferReplyQtModel");
    m_isSelfChange = false;
    m_localClientAdFilePath = localClientAdFilePath();

    // prepare local clientAd file, if no clientAd file exists,
    // replace with default clientAd file
    if (!QFile::exists(m_localClientAdFilePath))
    {
        restoreDefaultClientAdFile();
    }

    // get local clientAd hash
    _getClientAdHashAsync(ClientAdType_Local);

    // get embed clientAd hash
    _getClientAdHashAsync(ClientAdType_Embed);

    {
        m_updateClientAdTimer = new QTimer();
        m_updateClientAdTimer->setInterval(UPDATE_CLIENT_AD_INTERVAL);
        connect(m_updateClientAdTimer, SIGNAL(timeout()),
            this, SLOT(requestClientAdSlot()));
        m_updateClientAdTimer->start();
    }

    {
        m_verifyClientAdTimer = new QTimer();
        m_verifyClientAdTimer->setInterval(VERIFY_CLIENT_AD_INTERVAL);
        connect(m_verifyClientAdTimer, SIGNAL(timeout()),
            this, SLOT(verifyClientAdSlot()));
        m_verifyClientAdTimer->start();
    }

    // verify async (after 10s) when initing
    QTimer::singleShot( (REQUEST_CLIENT_AD_DELAY_INTERVAL / 2),
        this, SLOT(verifyClientAdSlot()));

    // delay 20s to request the latest clientAd file
    QTimer::singleShot(REQUEST_CLIENT_AD_DELAY_INTERVAL, 
        this, SLOT(requestClientAdSlot()));

    return true;
}

bool ClientAdManager::connectSigSlot()
{
    return true;
}

QString ClientAdManager::adUrl() const
{
    return m_adUrl;
}

QString ClientAdManager::localClientAdFilePath() const
{
    QString clientAdFilePath = _getClientAdDirPath() + "/" + CLIENT_AD_FILE_NAME_WITH_EXT;

    return QDir::toNativeSeparators(clientAdFilePath);
}

QString ClientAdManager::localBakFilePath() const
{
    QString clientAdDirPath = QApplication::applicationDirPath() + "/" + AdFolder;
    QString bakFilePath = clientAdDirPath + "/" + BAK_FILE_NAME_WITH_EXT;

    return QDir::toNativeSeparators(bakFilePath);
}

void ClientAdManager::requestClientAdSlot()
{
    ClientAdTransferRequestQtModel qtModel(
        CConfigManager::clientId(), 
        m_localClientAdHash);
    cc2csSender::send(cc2cs::ClientAdTransferRequestCommandC(qtModel));
}

void ClientAdManager::verifyClientAdSlot()
{
    if (m_isSelfChange)
    {
        return;
    }

    QtConcurrent::run(
        this,
        &ClientAdManager::_verifyLocalClientAdImp);
}

void ClientAdManager::_verifyLocalClientAdImp()
{
    if (m_isSelfChange)
    {
        return;
    }

    // case 1: the local clientAd file was deleted, restore with default clientAd file
    if (! QFile::exists(m_localClientAdFilePath))
    {
        restoreDefaultClientAdFile();
        return;
    }

    // case 2: the local file exists, verify it
    // client has recved server clientAd info
    if (!CConfigManager::svrClientAdHash().isEmpty())
    {
        _getClientAdHashImp(ClientAdType_Local); // the local file might be changed, need force refresh
        if (QString::compare(m_localClientAdHash, CConfigManager::svrClientAdHash(), Qt::CaseInsensitive) != 0)
        {
            if (QString::compare(m_localClientAdHash, m_embedClientAdHash, Qt::CaseInsensitive) != 0)
            {
                restoreDefaultClientAdFile();
            }
        } else {
            // match, dont need to do anything
        }
    } else {
        if (QString::compare(m_localClientAdHash, m_embedClientAdHash, Qt::CaseInsensitive) != 0)
        {
            restoreDefaultClientAdFile();
        }
    }
}

void ClientAdManager::_getClientAdHashAsync(int typeVal)
{
    QtConcurrent::run(
        this,
        &ClientAdManager::_getClientAdHashImp,
        typeVal);
}

void ClientAdManager::_getClientAdHashImp(int typeVal)
{
    ClientAdType type = (ClientAdType)typeVal;

    QString srcFilePath;
    QString* hashStrPtr = NULL;
    switch(type)
    {
    case ClientAdType_Local:
        srcFilePath = m_localClientAdFilePath;
        hashStrPtr = &m_localClientAdHash;
        break;
    case ClientAdType_Embed:
        srcFilePath = EMBED_CLIENT_AD_FILE_PATH;
        hashStrPtr = &m_embedClientAdHash;
        break;
    }

    if (QFile::exists(srcFilePath) &&
        hashStrPtr != NULL)
    {
        *hashStrPtr = Utils::getFileHash(srcFilePath);
    }
}

void ClientAdManager::restoreDefaultClientAdFile()
{
    m_isSelfChange = true;
    {
        CConfigManager::setSvrClientAdHash("");
        CConfigManager::setSvrClientAdUrl("");

        if (QFile::exists(m_localClientAdFilePath))
        {
            QFile::remove(m_localClientAdFilePath);
        }

        if (!QFile::exists(EMBED_CLIENT_AD_FILE_PATH))
        {
            return;
        }

        QFile embedClientAdFile(EMBED_CLIENT_AD_FILE_PATH);
        QFile localClientAdFile(m_localClientAdFilePath);
        if (localClientAdFile.open(QFile::WriteOnly))
        {
            QByteArray embedFileBytes;

            if (embedClientAdFile.open(QFile::ReadOnly))
            {
                int size = 4*1024*1024;
                while(!embedClientAdFile.atEnd())
                {
                    embedFileBytes.append(embedClientAdFile.read(size));
                }
            }
            localClientAdFile.write(embedFileBytes);
            localClientAdFile.flush();
            localClientAdFile.close();
        }

        embedClientAdFile.close();
    }
    m_isSelfChange = false;

    emit clientAdFileChanged();
}

void ClientAdManager::procClientAdReplyC( const ClientAdTransferReplyQtModel& qtModel )
{
    QString newClientAdHash = qtModel.ClientAdHash_;
    CConfigManager::setSvrClientAdHash(newClientAdHash);

    if (QString::compare(newClientAdHash, m_localClientAdHash, Qt::CaseInsensitive) == 0)
    {
        return;
    }

    // server has no new clientAd file, and the client is using the default one
    // then skip
    if (newClientAdHash.isEmpty() &&
        QString::compare(m_localClientAdHash, m_embedClientAdHash, Qt::CaseInsensitive) == 0)
    {
        return;
    }

    if (!newClientAdHash.isEmpty())
    {
        // update ad url and ad file content
        m_adUrl = qtModel.ClientAdUrl_;
        CConfigManager::setSvrClientAdUrl(m_adUrl);
        const QByteArray& newClientAdContent = qtModel.ClientAdContent_;
        _saveNewClientAdFile(newClientAdContent);
    }
    else
    {
        restoreDefaultClientAdFile();
    }
}

void ClientAdManager::_saveNewClientAdFile( const QByteArray& clientAdContent )
{
    m_isSelfChange = true;
    {
        // delete old bak file
        QString bakFilePath = localBakFilePath();
        if (QFile::exists(bakFilePath))
        {
            QFile::remove(bakFilePath);
        }

        // backup the current clientAd file
        if (QFile::exists(m_localClientAdFilePath))
        {
            QFile::rename(m_localClientAdFilePath, bakFilePath);
        }

        // save the new clientAd file
        QFile localClientAdFile(m_localClientAdFilePath);
        if (localClientAdFile.open(QFile::WriteOnly))
        {
            localClientAdFile.write(clientAdContent);
            localClientAdFile.flush();
            localClientAdFile.close();
        }
    }
    m_isSelfChange = false;

    emit clientAdFileChanged();
}

QString ClientAdManager::_getClientAdDirPath() const
{
    static QString s_clientAdDirPath;
    if (! s_clientAdDirPath.isEmpty())
    {
        return s_clientAdDirPath;
    }

    s_clientAdDirPath = QApplication::applicationDirPath() + "/" + AdFolder;
    QDir clientAdDir(s_clientAdDirPath);
    // if it already exists, return
    if (clientAdDir.exists())
    {
        return s_clientAdDirPath;
    }

    // the folder doesn't exist, attempt to create one
    clientAdDir.mkpath(s_clientAdDirPath);

    // if failed to create the folder, mark the "clientAd_dir" as empty, so next time we'll continue to try
    if (! clientAdDir.exists())
    {
        s_clientAdDirPath = "";
    }

    return s_clientAdDirPath;
}



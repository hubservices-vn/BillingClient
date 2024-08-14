#include "FlashManager.h"
#include "FlashTransferQtModel.h"
#include "ConfigManager.h"
#include "cl2csSender.h"
#include "cl2csCmds.h"
#include <QApplication>
#include <QtCore>
#include <QDir>
#include <QFile>
#include "utils.h"

//////////////////////////////////////////////////////////////////////////
// interval to update flash from server
#ifdef NDEBUG
// release
const int UPDATE_FLASH_INTERVAL = 3600000; // 3,600,000 ms = 1h
#else
const int UPDATE_FLASH_INTERVAL = 20000; // 20s
#endif


//////////////////////////////////////////////////////////////////////////
// after client launches, delay some time to request flash file from server
#ifdef NDEBUG // release
// delay 20s to request flash file when client app launching
const int REQUEST_FLASH_DELAY_INTERVAL = 20000; // 20,000 ms = 20s
#else
const int REQUEST_FLASH_DELAY_INTERVAL = 2000; // 2s
#endif

//////////////////////////////////////////////////////////////////////////
// verify flash file every one minute, if cafe changes it, restore it back
#ifdef NDEBUG // release
const int VERIFY_FLASH_INTERVAL = 60000; // 60,000 ms = 60s = 1min
#else
const int VERIFY_FLASH_INTERVAL = 2000; // 2s
#endif



const QString EMBED_FLASH_FILE_PATH = ":/ccBackground/ccBackground/default.swf";
const QString BackgroundImageFolder = "ccBackground";
const QString FLASH_FILE_NAME_WITH_EXT = "lockscreen.swf";
const QString BAK_FILE_NAME_WITH_EXT = "lockscreen.bak";

FlashManager::~FlashManager()
{
    if (m_updateFlashTimer != NULL)
    {
        if (m_updateFlashTimer->isActive())
        {
            m_updateFlashTimer->stop();
        }

        m_updateFlashTimer->deleteLater();
        m_updateFlashTimer = NULL;
    }

    if (m_verifyFlashTimer != NULL)
    {
        if (m_verifyFlashTimer->isActive())
        {
            m_verifyFlashTimer->stop();
        }

        m_verifyFlashTimer->deleteLater();
        m_verifyFlashTimer = NULL;
    }
}

bool FlashManager::init()
{
    m_isSelfChange = false;
    m_localFlashFilePath = localFlashFilePath();

    // prepare local flash file, if no flash file exists,
    // replace with default flash file
    if (!QFile::exists(m_localFlashFilePath))
    {
        restoreDefaultFlashFile();
    }

    // get local flash hash
    _getFlashHashAsync(FlashType_Local);

    // get embed flash hash
    _getFlashHashAsync(FlashType_Embed);

    {
        m_updateFlashTimer = new QTimer();
        m_updateFlashTimer->setInterval(UPDATE_FLASH_INTERVAL);
        connect(m_updateFlashTimer, SIGNAL(timeout()),
            this, SLOT(requestFlashSlot()));
        m_updateFlashTimer->start();
    }

    {
        m_verifyFlashTimer = new QTimer();
        m_verifyFlashTimer->setInterval(VERIFY_FLASH_INTERVAL);
        connect(m_verifyFlashTimer, SIGNAL(timeout()),
            this, SLOT(verifyFlashSlot()));
        m_verifyFlashTimer->start();
    }

    // verify async (after 10s) when initing
    QTimer::singleShot( (REQUEST_FLASH_DELAY_INTERVAL / 2),
        this, SLOT(verifyFlashSlot()));

    // delay 20s to request the latest flash file
    QTimer::singleShot(REQUEST_FLASH_DELAY_INTERVAL, 
        this, SLOT(requestFlashSlot()));

    return true;
}

bool FlashManager::connectSigSlot()
{
    return true;
}

QString FlashManager::localFlashFilePath() const
{
    QString flashFilePath = _getFlashDirPath() + "/" + FLASH_FILE_NAME_WITH_EXT;

    return QDir::toNativeSeparators(flashFilePath);
}

QString FlashManager::localBakFilePath() const
{
    QString flashDirPath = QApplication::applicationDirPath() + "/" + BackgroundImageFolder;
    QString bakFilePath = flashDirPath + "/" + BAK_FILE_NAME_WITH_EXT;

    return QDir::toNativeSeparators(bakFilePath);
}

void FlashManager::requestFlashSlot()
{
    FlashTransferRequestQtModel qtModel(
        CConfigManager::clientId(), 
        m_localFlashHash);
    cl2csSender::send(cl2cs::FlashTransferRequestCommandC(qtModel));
}

void FlashManager::verifyFlashSlot()
{
    if (m_isSelfChange)
    {
        return;
    }

    QtConcurrent::run(
        this,
        &FlashManager::_verifyLocalFlashImp);
}

void FlashManager::_verifyLocalFlashImp()
{
    if (m_isSelfChange)
    {
        return;
    }

    // case 1: the local flash file was deleted, restore with default flash file
    if (! QFile::exists(m_localFlashFilePath))
    {
        restoreDefaultFlashFile();
        return;
    }

    // case 2: the local file exists, verify it
    // client has recved server flash info
    if (CConfigManager::svrFlashVersion() >= 0)
    {
        _getFlashHashImp(FlashType_Local); // the local file might be changed, need force refresh
        if (CConfigManager::svrFlashHash().isEmpty())
        {
            if (QString::compare(m_localFlashHash, m_embedFlashHash, Qt::CaseInsensitive) != 0)
            {
                restoreDefaultFlashFile();
            }
        }
        // client flash hash doesn't match server flash hash
        else 
        {
            if (QString::compare(m_localFlashHash, CConfigManager::svrFlashHash(), Qt::CaseInsensitive) != 0)
            {
                restoreDefaultFlashFile();
            }
        }
    }
}

void FlashManager::_getFlashHashAsync(int typeVal)
{
    QtConcurrent::run(
        this,
        &FlashManager::_getFlashHashImp,
        typeVal);
}

void FlashManager::_getFlashHashImp(int typeVal)
{
    FlashType type = (FlashType)typeVal;

    QString srcFilePath;
    QString* hashStrPtr = NULL;
    switch(type)
    {
    case FlashType_Local:
        srcFilePath = m_localFlashFilePath;
        hashStrPtr = &m_localFlashHash;
        break;
    case FlashType_Embed:
        srcFilePath = EMBED_FLASH_FILE_PATH;
        hashStrPtr = &m_embedFlashHash;
        break;
    }

    if (QFile::exists(srcFilePath) &&
        hashStrPtr != NULL)
    {
        *hashStrPtr = Utils::getFileHash(srcFilePath);
    }
}

void FlashManager::restoreDefaultFlashFile()
{
    m_isSelfChange = true;
    {
        CConfigManager::setSvrFlashVersion(-1);
        CConfigManager::setSvrFlashHash("");

        if (QFile::exists(m_localFlashFilePath))
        {
            QFile::remove(m_localFlashFilePath);
        }

        if (!QFile::exists(EMBED_FLASH_FILE_PATH))
        {
            return;
        }

        QFile embedFlashFile(EMBED_FLASH_FILE_PATH);
        QFile localFlashFile(m_localFlashFilePath);
        if (localFlashFile.open(QFile::WriteOnly))
        {
            QByteArray embedFileBytes;

            if (embedFlashFile.open(QFile::ReadOnly))
            {
                int size = 4*1024*1024;
                while(!embedFlashFile.atEnd())
                {
                    embedFileBytes.append(embedFlashFile.read(size));
                }
            }
            localFlashFile.write(embedFileBytes);
            localFlashFile.flush();
            localFlashFile.close();
        }

        embedFlashFile.close();
    }
    m_isSelfChange = false;

    emit flashFileChanged();
}

void FlashManager::procFlashReplyC( const FlashTransferReplyQtModel& qtModel )
{
    int newFlashVer = qtModel.NewVer_;
    if (newFlashVer == CConfigManager::svrFlashVersion())
    {
        return;
    }
    CConfigManager::setSvrFlashVersion(newFlashVer);

    QString newFlashHash = qtModel.FlashHash_;
    CConfigManager::setSvrFlashHash(newFlashHash);

    if (QString::compare(newFlashHash, m_localFlashHash, Qt::CaseInsensitive) == 0)
    {
        return;
    }

    // server has no new flash file, and the client is using the default one
    // then skip
    if (newFlashVer == 0 &&
        newFlashHash.isEmpty() &&
        QString::compare(m_localFlashHash, m_embedFlashHash, Qt::CaseInsensitive) == 0)
    {
        return;
    }

    if (newFlashVer > 0)
    {
        const QByteArray& newFlashContent = qtModel.FlashContent_;
        _saveNewFlashFile(newFlashContent);
    }
    else
    {
        restoreDefaultFlashFile();
    }
}

void FlashManager::_saveNewFlashFile( const QByteArray& flashContent )
{
    m_isSelfChange = true;
    {
        // delete old bak file
        QString bakFilePath = localBakFilePath();
        if (QFile::exists(bakFilePath))
        {
            QFile::remove(bakFilePath);
        }

        // backup the current flash file
        if (QFile::exists(m_localFlashFilePath))
        {
            QFile::rename(m_localFlashFilePath, bakFilePath);
        }

        // save the new flash file
        QFile localFlashFile(m_localFlashFilePath);
        if (localFlashFile.open(QFile::WriteOnly))
        {
            localFlashFile.write(flashContent);
            localFlashFile.flush();
            localFlashFile.close();
        }
    }
    m_isSelfChange = false;

    emit flashFileChanged();
}

QString FlashManager::_getFlashDirPath() const
{
    static QString s_flashDirPath;
    if (! s_flashDirPath.isEmpty())
    {
        return s_flashDirPath;
    }

    s_flashDirPath = QApplication::applicationDirPath() + "/" + BackgroundImageFolder;
    QDir flashDir(s_flashDirPath);
    // if it already exists, return
    if (flashDir.exists())
    {
        return s_flashDirPath;
    }

    // the folder doesn't exist, attempt to create one
    flashDir.mkpath(s_flashDirPath);

    // if failed to create the folder, mark the "flash_dir" as empty, so next time we'll continue to try
    if (! flashDir.exists())
    {
        s_flashDirPath = "";
    }

    return s_flashDirPath;
}




#include "ClientWallpaperManager.h"
#include "ClientWallpaperTransferQtModel.h"
#include "cc2csNetworkManager.h"
#include "ConfigManager.h"
#include <QFile>
#include <QDir>
#include "Utils.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "PathManager.h"
#include "WallpaperSetter.h"
#include "CommonSettings.h"
#include "log.h"
#include <QSysInfo>
#include <QDesktopWidget>
#include <QApplication>
#include "UserAccountManager.h"
#include "QFastHash.h"
#include "GFileUtils.h"
#include <QSettings>
#include "RestartExplorerHelper.h"
#include <Shlobj.h>
#include <time.h>

// defined in WallpaperSetter.cpp
extern const int DEFAULT_WALLPAPER_OPTION;


//////////////////////////////////////////////////////////////////////////
// interval to update clientAd from server
#ifdef NDEBUG
// release
const int UPDATE_CLIENT_WALLPAPER_INTERVAL = 1800000; // 1,800,000 ms = 30mins
#else
const int UPDATE_CLIENT_WALLPAPER_INTERVAL = 20000; // 20s
#endif


//////////////////////////////////////////////////////////////////////////
// after client launches, delay some time to request clientAd file from server
#ifdef NDEBUG // release
// delay 20s to request clientAd file when client app launching
const int REQUEST_CLIENT_WALLPAPER_DELAY_INTERVAL = 2000; // 2,000 ms = 2s
#else
const int REQUEST_CLIENT_WALLPAPER_DELAY_INTERVAL = 1000; //
#endif


const QString EmbedWallpaperFolder = ":/deskwp/deskwp";
const QString EmbedDefaultWallpaperFileName = "gb_embed_default.jpg";
const QString WallpaperFolder = "deskwp";
const qint64 Default_Refresh_Interval = 900; // in secs

ClientWallpaperManager::~ClientWallpaperManager()
{
    if (m_updateWallpaperTimer != NULL)
    {
        if (m_updateWallpaperTimer->isActive())
        {
            m_updateWallpaperTimer->stop();
        }

        m_updateWallpaperTimer->deleteLater();
        m_updateWallpaperTimer = NULL;
    }

    if (m_scheduleSetWallpaperTimer != NULL)
    {
        if (m_scheduleSetWallpaperTimer->isActive())
        {
            m_scheduleSetWallpaperTimer->stop();
        }

        m_scheduleSetWallpaperTimer->deleteLater();
        m_scheduleSetWallpaperTimer = NULL;
    }
}

bool ClientWallpaperManager::init()
{
    m_updateWallpaperTimer = new QTimer();
    m_updateWallpaperTimer->setInterval(UPDATE_CLIENT_WALLPAPER_INTERVAL);
    connect(m_updateWallpaperTimer, SIGNAL(timeout()),
        this, SLOT(checkWallpaperSlot()));

    m_scheduleSetWallpaperTimer = new QTimer();
    m_scheduleSetWallpaperTimer->setSingleShot(true);
    connect(m_scheduleSetWallpaperTimer, SIGNAL(timeout()),
        this, SLOT(refreshWallpaperTimeoutSlot()));

    // if user restricted wallpaper through group policy, disable it
    _cleanupGroupPolicyWallpaper();

    _loadEmbeddedWallpapers();
    _refreshWallpaper(TE_Init);

    return true;
}

bool ClientWallpaperManager::connectSigSlot()
{
    // after server online, send command to product data from server
    {
        cc2csNetworkManager* networkMgr = cc2csNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    {
        connect(UserAccountManager::GetInstance(), SIGNAL(accLoggedIn()), 
            this, SLOT(onUserLoggedinSlot()));
        connect(UserAccountManager::GetInstance(), SIGNAL(accLoggedOut(int, int)), 
            this, SLOT(onUserLoggedOutSlot()));
    }

    return true;
}

void ClientWallpaperManager::serverConnectedSlot()
{
    LOG_DEBUG4 << "check deskwp in " << REQUEST_CLIENT_WALLPAPER_DELAY_INTERVAL;
    QTimer::singleShot(REQUEST_CLIENT_WALLPAPER_DELAY_INTERVAL,
        this, SLOT(checkWallpaperSlot()));
}

void ClientWallpaperManager::procWallpaperCheckReply( const ClientWallpaperCheckReplyQtModel& replyModel )
{
    int infoSize = replyModel.InfoList_.size();
    int actionSize = replyModel.ActionList_.size();
    if (infoSize != actionSize)
    {
        LOG_ERROR << "mismatch. info: " << infoSize
            << " act: " << actionSize;
        return;
    }

    for (int i = 0; i < actionSize; ++i)
    {
        int actionVal = replyModel.ActionList_[i];
        const WallpaperMetaInfoItem& infoItem = replyModel.InfoList_[i];

        switch(actionVal)
        {
        case WallpaperAction_Add:
            {
                _addWallpaperMetaInfo(infoItem);
            }
            break;
        case WallpaperAction_Upd:
            {
                _updWallpaperMetaInfo(infoItem);
            }
            break;
        case WallpaperAction_Del:
            {
                _deleteWallpaper(infoItem);
            }
            break;
        }
    }

    foreach(const QString& hash, m_toDownloadMap.keys())
    {
        ClientWallpaperTransferRequestQtModel qtModel;
        qtModel.ClientId_ = CConfigManager::clientId();
        qtModel.LocalHash_ = hash;
        cc2csSender::send(cc2cs::ClientWallpaperTransferRequestCommandC(qtModel));
    }
}

void ClientWallpaperManager::procWallpaperTransferReply( const ClientWallpaperTransferReplyQtModel& replyModel )
{
    QString hash = replyModel.FileHash_.toLower();
    if ( ! m_toDownloadMap.contains(hash))
    {
        LOG_ERROR << "not in dl list " << qPrintable(Utils::symEncrypt(hash));
        return;
    }

    WallpaperMetaInfo* info = m_toDownloadMap.take(hash);
    if (info == NULL)
    {
        LOG_ERROR << "null meta for " << qPrintable(Utils::symEncrypt(hash));
        return;
    }

    info->hash = hash;
    info->fileContent = replyModel.FileContent_;
    info->fileRelativePath = replyModel.FileName_;
    info->interval = replyModel.Interval_;
    info->isOnInit = replyModel.IsOnInit_;
    info->isOnLogin = replyModel.IsOnLogin_;
    info->isOnLogout = replyModel.IsOnLogout_;
    info->expiry = replyModel.Expiry_;
    info->imageOption = replyModel.ImageOption_;

    // cache it to local map
    m_wallpaperMap[hash] = info;

    if (m_currentWallpaperHash.isEmpty())
    {
        _refreshWallpaper(TE_Init);
    }
}

void ClientWallpaperManager::_loadEmbeddedWallpapers()
{
    LOG_DEBUG4 << "load def deskwp";

    QString localDefFilePath = _getLocalDestFilePath(EmbedDefaultWallpaperFileName);
    if (QFile::exists(localDefFilePath))
    {
        QFile::remove(localDefFilePath);
    }

    QString embedFilePath = EmbedWallpaperFolder + "/" + EmbedDefaultWallpaperFileName;
    if (!QFile::exists(embedFilePath))
    {
        LOG_ERROR << "can not find def deskwp";
        return;
    }

    qDeleteAll(m_wallpaperMap.values());
    m_wallpaperMap.clear();
    WallpaperMetaInfo* metaInfo = new WallpaperMetaInfo();

    QFile embedFile(embedFilePath);
    QFile targetLocalFile(localDefFilePath);
    if (targetLocalFile.open(QFile::WriteOnly))
    {
        if (embedFile.open(QFile::ReadOnly))
        {
            int size = 100*1024;
            while(!embedFile.atEnd())
            {
                metaInfo->fileContent.append(embedFile.read(size));
            }
        }
        targetLocalFile.write(metaInfo->fileContent);
        targetLocalFile.flush();
        targetLocalFile.close();
    }

    embedFile.close();

    metaInfo->hash = QFastHash::getBuffHash(metaInfo->fileContent).toLower();
    metaInfo->fileRelativePath = EmbedDefaultWallpaperFileName;
    metaInfo->isOnInit = true;
    metaInfo->isOnLogin = false;
    metaInfo->isOnLogout = true;
    metaInfo->imageOption = DEFAULT_WALLPAPER_OPTION;
    metaInfo->interval = Default_Refresh_Interval;
    m_wallpaperMap[metaInfo->hash] = metaInfo;
}

void ClientWallpaperManager::checkWallpaperSlot()
{
    LOG_DEBUG4 << "check deskwp";
    ClientWallpaperCheckRequestQtModel qtModel;
    qtModel.ClientId_ = CConfigManager::clientId();
    qtModel.OSVersion_ = QSysInfo::WindowsVersion;
    QDesktopWidget* desktop = QApplication::desktop();
    QRect screenRect = desktop->availableGeometry(desktop->primaryScreen());
    qtModel.ScreenWidth_ = screenRect.width();
    qtModel.ScreenHeight_ = screenRect.height();

    foreach(WallpaperMetaInfo* metaInfo, m_wallpaperMap.values())
    {
        if (metaInfo == NULL || ( ! metaInfo->isValid() ))
        {
            continue;
        }

        qtModel.LocalInfoList_.append(
            WallpaperMetaInfoItem(metaInfo->hash));
    }

    cc2csSender::send(cc2cs::ClientWallpaperCheckRequestCommandC(qtModel));
}

QString ClientWallpaperManager::_getLocalDestFilePath( const QString& fileRelativePath )
{
    return QDir::toNativeSeparators(PathManager::getWallpaperFolder() + "/" + fileRelativePath);
}

void ClientWallpaperManager::_refreshWallpaper(TriggerEvent te)
{
    LOG_DEBUG4 << "refresh deskwp";
    QList<WallpaperMetaInfo*> wallpaperList = m_wallpaperMap.values();
    if (wallpaperList.size() == 0)
    {
        LOG_ERROR << "empty deskwp map";
        return;
    }

    // don't allow to refresh when user is logged in a WindowsXP computer
    bool allowRefresh = true;
    QSysInfo::WinVersion wv = QSysInfo::WindowsVersion;
    if (wv <= QSysInfo::WV_2003)
    {
        if (UserAccountManager::isUserLoggedIn())
        {
            allowRefresh = false;
        }
    }

    if (allowRefresh == false)
    {
        return;
    }

    qsrand(::time(NULL));
    int index = qrand() % wallpaperList.size();
    if (index < 0 || index >= wallpaperList.size())
    {
        LOG_ERROR << "invalid index: " << index
            << ", size: " << wallpaperList.size();
        return;
    }

    WallpaperMetaInfo* metaInfo = NULL;
    for (int i = index; i < wallpaperList.size(); ++i)
    {
        LOG_DEBUG4 << "check inx " << i;
        WallpaperMetaInfo* candiInfo = wallpaperList[i];
        if (candiInfo == NULL 
            || ( ! candiInfo->isValid() )
            || candiInfo->fileRelativePath.isEmpty())
        {
            LOG_DEBUG << "invalid candi meta";
            continue;
        }

        bool isMatch = (
            (te == TE_Init && candiInfo->isOnInit == true)
            || (te == TE_Login && candiInfo->isOnLogin== true)
            || (te == TE_Logout && candiInfo->isOnLogout == true)
            );
        if ( ! isMatch)
        {
            continue;
        }

        metaInfo = candiInfo;
        break;
    }

    if (metaInfo == NULL 
        || ( ! metaInfo->isValid() )
        || metaInfo->fileRelativePath.isEmpty())
    {
        LOG_ERROR << "invalid final meta. te " << te;
        return;
    }

    QString localFilePath = _getLocalDestFilePath(metaInfo->fileRelativePath);

    if (QFile::exists(localFilePath))
    {
        QString localHash = QFastHash::getFileHash(localFilePath);

        // local cache is invalid, remove it
        if (metaInfo->hash.compare(localHash, Qt::CaseInsensitive) != 0)
        {
            bool succeeded = GFileUtils::removeFile(localFilePath);
            if ( ! succeeded)
            {
                LOG_ERROR << "can not remove file " << qPrintable(Utils::symEncrypt(localFilePath));
            }
        }
    }

    // restore local file if it does not exist
    if ( ! QFile::exists(localFilePath) )
    {
        _saveFile(localFilePath, metaInfo->fileContent);

        if ( ! QFile::exists(localFilePath) )
        {
            LOG_ERROR << "can not get file " << qPrintable(Utils::symEncrypt(localFilePath));
            return;
        }
    }

    WallpaperSetter setter;
    LOG_DEBUG4 << "set deskwp with opt " << metaInfo->imageOption;
    setter.setWallpaper(localFilePath, metaInfo->imageOption);

    m_currentWallpaperHash = metaInfo->hash.toLower();

    int revisedInterval = _getProperRefreshInterval(metaInfo->interval);
    LOG_DEBUG4 << "next deskwp in " << revisedInterval;
    m_scheduleSetWallpaperTimer->start(revisedInterval * 1000);
}

void ClientWallpaperManager::_addWallpaperMetaInfo( const WallpaperMetaInfoItem& infoItem )
{
    LOG_DEBUG << "add meta " << qPrintable(Utils::symEncrypt(infoItem.Hash_));
    WallpaperMetaInfo* info = new WallpaperMetaInfo();
    info->hash = infoItem.Hash_;

    m_toDownloadMap[infoItem.Hash_] = info;
}

void ClientWallpaperManager::_updWallpaperMetaInfo( const WallpaperMetaInfoItem& infoItem )
{
    QString hash = infoItem.Hash_.toLower();
    LOG_DEBUG << "upd meta " << qPrintable(Utils::symEncrypt(infoItem.Hash_));
    if ( ! m_wallpaperMap.contains(hash))
    {
        LOG_WARN << "can not find local meta, quit";
        return;
    }
    WallpaperMetaInfo* info = m_wallpaperMap[hash];

    int localIntervalBefore = info->interval;

    info->interval = infoItem.Interval_;
    info->isOnInit = infoItem.IsOnInit_;
    info->isOnLogin = infoItem.IsOnLogin_;
    info->isOnLogout = infoItem.IsOnLogout_;
    info->expiry = infoItem.Expiry_;
    info->imageOption = infoItem.ImageOption_;
}

void ClientWallpaperManager::_deleteWallpaper( const WallpaperMetaInfoItem& infoItem )
{
    QString hash = infoItem.Hash_.toLower();
    if ( m_wallpaperMap.contains(hash) )
    {
        delete m_wallpaperMap.take(hash);
    }

    if ( m_toDownloadMap.contains(hash))
    {
        delete m_toDownloadMap.take(hash);
    }

    // current image is being updated
    if (m_currentWallpaperHash.compare(hash) == 0)
    {
        m_currentWallpaperHash = "";

        if (m_scheduleSetWallpaperTimer->isActive())
        {
            m_scheduleSetWallpaperTimer->stop();
        }

        // change immediately (current image is invalid)
        _refreshWallpaper(TE_Init);
    }
}

bool ClientWallpaperManager::_saveFile( const QString& destFilePath, const QByteArray& fileContent )
{
    if (fileContent.isEmpty())
    {
        return false;
    }

    QFile targetLocalFile(destFilePath);
    if (targetLocalFile.open(QFile::WriteOnly))
    {
        targetLocalFile.write(fileContent);
        targetLocalFile.flush();
        targetLocalFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

qint64 ClientWallpaperManager::_getProperRefreshInterval( qint64 interval )
{
    if (interval < 0)
    {
        return Default_Refresh_Interval;
    }

    return interval;
}

void ClientWallpaperManager::refreshWallpaperTimeoutSlot()
{
    _refreshWallpaper(TE_Init);
}

void ClientWallpaperManager::onUserLoggedinSlot()
{
    _refreshWallpaper(TE_Login);
}

void ClientWallpaperManager::onUserLoggedOutSlot()
{
    _refreshWallpaper(TE_Logout);
}

void ClientWallpaperManager::_cleanupGroupPolicyWallpaper()
{
    if ( ! _hasGroupPolicyWallpaperSet())
    {
        // safe, user didn't restrict the wallpaper setting through group policy
        return;
    }

    LOG_DEBUG << "user restricted deskwp via gpedit";

    // remove the group policy wallpaper settings
    {
        QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
        QSettings settings( keyHive, QSettings::NativeFormat );
        settings.remove(QString("Wallpaper"));
        settings.remove(QString("WallpaperStyle"));
    }

    // relaunch explorer
    _refreshExplorer(/*killExplorer*/true);
}

bool ClientWallpaperManager::_hasGroupPolicyWallpaperSet()
{
    QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    QSettings settings( keyHive, QSettings::NativeFormat );
    return (
        settings.contains("Wallpaper")
        || settings.contains("WallpaperStyle")
        );
}

void ClientWallpaperManager::_refreshExplorer(bool isKillExplorer)
{
    if (isKillExplorer)
    {
        RestartExplorerHelper::restartExplorer();
    }
    else
    {
        SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, NULL);
        SHChangeNotify(SHCNE_ALLEVENTS, SHCNF_FLUSH, 0, 0);
    }
}



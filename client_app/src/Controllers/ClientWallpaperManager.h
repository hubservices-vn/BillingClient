#ifndef CLIENT_WALLPAPER_MANAGER_H
#define CLIENT_WALLPAPER_MANAGER_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <time.h>
#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;


class WallpaperMetaInfo
{
public:
    WallpaperMetaInfo()
    {
        reset();
    }

    bool isValid() const
    {
        return (
            ( ! fileRelativePath.isEmpty()) 
            && ( ! hash.isEmpty() )
            );
    }

    bool isExpired() const
    {
        if (expiry <= 0)
        {
            return false;
        }

        return (expiry <= ::time(NULL));
    }
    void reset()
    {
        fileRelativePath = "";
        hash = "";
        fileContent.clear();
        interval = -1;
        isOnInit = false;
        isOnLogin = false;
        isOnLogout = false;
        expiry = -1;
        imageOption = -1;
    }
public:
    QString fileRelativePath;
    QString hash;
    QByteArray fileContent;
    qint64 interval;
    bool isOnInit;
    bool isOnLogin;
    bool isOnLogout;
    qint64 expiry;
    int imageOption;
};


class WallpaperMetaInfoItem;
class ClientWallpaperCheckReplyQtModel;
class ClientWallpaperTransferReplyQtModel;
class ClientWallpaperManager 
    : public QObject,
    public Singleton_T<ClientWallpaperManager>
{
    Q_OBJECT
public:
    ~ClientWallpaperManager();
public:
    bool init();
    bool connectSigSlot();

public: // cmd handlers
    void procWallpaperCheckReply(const ClientWallpaperCheckReplyQtModel& replyModel);
    void procWallpaperTransferReply(const ClientWallpaperTransferReplyQtModel& replyModel);

public slots:
    void serverConnectedSlot();

    void checkWallpaperSlot();

    void refreshWallpaperTimeoutSlot();

    void onUserLoggedinSlot();
    void onUserLoggedOutSlot();
private:
    void _cleanupGroupPolicyWallpaper();
    bool _hasGroupPolicyWallpaperSet();
    void _refreshExplorer(bool isKillExplorer);

    void _loadEmbeddedWallpapers();

    QString _getLocalDestFilePath(const QString& fileRelativePath);

    enum TriggerEvent
    {
        TE_NA,
        TE_Init,
        TE_Login,
        TE_Logout,
    };
    void _refreshWallpaper(TriggerEvent te);

    bool _saveFile(const QString& destFilePath, const QByteArray& fileContent);
private: // manage the local cache map
    void _addWallpaperMetaInfo(const WallpaperMetaInfoItem& infoItem);
    void _updWallpaperMetaInfo(const WallpaperMetaInfoItem& infoItem);
    void _deleteWallpaper(const WallpaperMetaInfoItem& infoItem);

private: // helper
    qint64 _getProperRefreshInterval(qint64 interval);
private:
    // sync latest images from server
    QTimer* m_updateWallpaperTimer;
    // rotate-set 
    QTimer* m_scheduleSetWallpaperTimer;
    QMap<QString/*fileHash*/, WallpaperMetaInfo*> m_wallpaperMap;

    QMap<QString/*fileHash*/, WallpaperMetaInfo*> m_toDownloadMap;

    QString m_currentWallpaperHash;
};

#endif // CLIENT_WALLPAPER_MANAGER_H
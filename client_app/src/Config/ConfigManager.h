#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <QString>
#include <QLocale>
#include <QSettings>

#include "ClientAppSettings.h"
#include "stdtypes.h"

class CConfigManager
{
public:
    static bool init();

    static bool loadRegData();

public: // debugging only
    static bool isDisableSecurityRestrictions();

    // different environments
    static bool isDeveloperEnv();


public: // accessors (peseudo)
    static QString serverAddress( );
    static bool setServerAddress( const QString& address);

    static int clientId();
    static bool setClientId(int clientId);

    static QString adminNameEnc();
    static QString adminNameRaw();

    static QString adminCodeHash ();

    static QString tempLockCode();
    static bool setTempLockCode(const QString& rawTempLockCode);

    static bool inTempLockMode();
    static bool setInTempLockMode(bool val);

    static bool showTrayMsg();
    static bool setShowTrayMsg(bool val);

    static int trayMsgDuration();
    static bool setTrayMsgDuration(int val);

    static QString securityProfile();
    static bool setSecurityProfile(const QString& val);

    static int svrFlashVersion();
    static bool setSvrFlashVersion(int val);

    static QString svrFlashHash();
    static bool setSvrFlashHash(const QString& val);

    static QTime autoPcShutdownTime();
    static bool setAutoPcShutdownTime(const QTime &val);

    static QString autoPcShutdownMsg();
    static bool setAutoPcShutdownMsg(const QString &val);

    static QString svrClientAdHash();
    static bool setSvrClientAdHash(const QString &val);

    static QString svrClientAdUrl();
    static bool setSvrClientAdUrl(const QString &val);
private:
    // registry operation
    // read-only
    static QVariant _getRegData(const QSettings& setting, const QString& key);
    // if registry doesn't have key, then create the key and set default value
    static QVariant _getRegData(QSettings& setting, const QString& key, const QVariant& defaultValue);

private:
    static QString m_productKeyHive;

    static QString m_serverAddr;
    static int m_clientId;

    static QString m_tempLockCode;
    static bool m_inTempLockMode;

    static bool m_isShowTrayMsg; // whether or not show the system tray message
    static int m_trayMsgDuration; // the duration of message displayed in system tray

    static QString m_securityProfile;

    // cache the flash content version and hash
    static int m_svrFlashVersion;
    static QString m_svrFlashHash;

    //static int m_svrClientAdVersion;
    static QString m_svrClientAdHash;
    static QString m_svrClientAdUrl;

    // the minutes to (0:00:00 AM) is stored to db
    static QTime m_autoPcShutdownTime;
    static QString m_autoPcShutdownMsg;
};


#endif // CONFIGMANAGER_H

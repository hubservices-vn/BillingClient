#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <QString>
#include <QLocale>
#include <QSettings>
#include <QDateTime>

#include "ClientSvcSettings.h"
#include "stdtypes.h"
using namespace std;

//
// in "configs" table of main db
//
const QString WEB_CONTROL_LAST_UPDATE_DT = "webCtrlLastUpdateDT";


class CConfigManager
{
public:
    static bool init();

    static bool loadRegData();
    static bool loadDBData();

private:
//    bool _upgradeData();

public: // debugging only
    static bool isDisableSecurityRestrictions();
    // different environments
    static bool isDeveloperEnv();

public: // accessors (peseudo) from DB
    static qint64 lastUpdateWebCtrlDTVal();
    static QDateTime lastUpdateWebCtrlDT();
    static bool setLastUpdateWebCtrlDT(qint64 val);
    static bool setLastUpdateWebCtrlDT(const QDateTime& dt);

public: // accessors (peseudo) from registry

    static QString adminNameEnc ();
    static bool setAdminNameEnc ( const QString& val );

    static QString adminCodeHash ();
    static bool setAdminCodeHash ( const QString& val );

    static QString serverAddress( );
    static bool setServerAddress( const QString& address);
    
    static int clientId();
    static bool setClientId(int clientId);

    static QLocale::Language languageVal();
    static QString language();
    static bool setLanguage(QLocale::Language lang);
    static bool setLanguage(const QString& lang);
    
    static bool showTrayMsg();
    static bool setShowTrayMsg(bool val);
    
    static int trayMsgDuration();
    static bool setTrayMsgDuration(int val);

    static QString tempLockCode();
    static bool setTempLockCode(const QString& rawTempLockCode);

    static bool inTempLockMode();
    static bool setInTempLockMode(bool val);

    static QString securityProfile();
    static bool setSecurityProfile(const QString& val);

    static int shutdownAvailableClientMinutes();
    static bool setShutdownAvailableClientMinutes(int val);

    static int clearAppAvailableClientMinutes();
    static bool setClearAppAvailableClientMinutes(int val);

    static QTime autoPcShutdownTime();
    static bool setAutoPcShutdownTime(const QTime &val);

    static bool setCafeId(int val);
    //static QString clientToken();
    //static bool generateClientToken();

    //static int clientDataVersion();
    //static bool setClientDataVersion(int val);

private:
    // registry operation
    // read-only
    static QVariant _getRegData(const QSettings& setting, const QString& key);
    // if registry doesn't have key, then create the key and set default value
    static QVariant _getRegData(QSettings& setting, const QString& key, const QVariant& defaultValue);
    // DB operation
    static bool _addDBConfiguration( const QString& name, const QVariant& value );
    static bool _modifyDBConfiguration(const QString& name, const QVariant& value, bool enabled);
    static bool _hasActiveDBConfig( const QString& name);
    static QVariant _readDBConfig( const QString& name);
    static QVariant _readDBConfig( const QString& name, const QVariant& defaultVal);
    //private:
    //    static bool _upgradeDbVersoin(int oldVer, int newVer);
    //    // 0-1 data format changes:
    //    // the user data hash changes from hash( hash(pwd, salt), salt) to hash( hash(pwd, name+salt), name+salt)
    //    static bool _upgradeDb0To1();


private:
    static QString m_productKeyHive;

    //
    // value in registry
    //
    static QString m_serverAddr;
    static QString m_adminNameEnc;
    static QString m_adminCodeHash;
    static int m_clientId;
    static QString m_language;
    
    static bool m_isShowTrayMsg; // whether or not show the system tray message
    static int m_trayMsgDuration; // the duration of message displayed in system tray

    static QString m_tempLockCode;
    static bool m_inTempLockMode;
    static QString m_securityProfile;
    //static QString m_token;

    static int m_shutdownAvailableClientMinutes;
    static int m_clearAppAvailableClientMinutes;

    // the minutes to (0:00:00 AM) is stored to db
    static QTime m_autoPcShutdownTime;
    
    //
    // value in "configs" table of main DB
    //
    static uint64_t m_lastUpdateWebCtrlDTVal;

};


#endif // CONFIGMANAGER_H

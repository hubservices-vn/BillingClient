#include "ConfigManager.h"
#include <string>
#include<QProcessEnvironment>
#include <QString>
#include <QDir>
#include <QSettings>
#include "Utils.h"
#include <QApplication>
#include <QFile>
#include <QSysInfo>
#include <QDateTime>
#include "log.h"
#include "DateTimeUtils.h"

using namespace std;


//
// in registry
//
const QString ADMIN_NAME = "AdminName";
const QString ADMIN_CODE = "AdminCode"; // create by client when first launch
const QString SERVER_ADDRESS = "ServerAddress" ; // create by client when first launch
const QString TEMP_LOCK_CODE = "ScreenLockCode"; // the password of "temp screen lock" dialog
const QString IN_TEMP_LOCK_MODE = "ScreenLockMode"; // whether the computer is in "temp screen lock" mode
const QString SECURITY_PROFILE = "SecurityProfile"; // the cache of security profile
const QString SHUTDOWN_CLIENT_MINUTES = "shutdownClientMin";
const QString CLEAR_APP_MINUTES = "clearAppMin";
const QString AUTO_SCHEDULED_SHUTDOWN_TIME = "autoScheduledShutdownTime";
const QString AUTO_SCHEDULED_SHUTDOWN_MSG = "autoScheduledShutdownMsg";

enum CFG_TB_KEYS
{
    // the stored values are integer
    CFG_I_START                             = 0x0000,
    CFG_I_FlashVersion                      = 0x0001,

    // the stored values are string
    CFG_S_START                             = 0x2000,
    CFG_S_FlashHash                         = 0x2001,
    CFG_S_ClientAdHash                      = 0x2002,
    CFG_S_ClientAdUrl                       = 0x2003,
};
//////////////////////////////////////////////////////////////////////////


QString CConfigManager::m_productKeyHive = 
"HKEY_LOCAL_MACHINE\\Software\\" + 
G_COMPANY + "\\" + 
CLIENT_DATA_REGKEY_NAME;


// value in registry
int CConfigManager::m_clientId = -1;
QString CConfigManager::m_serverAddr = "";
int CConfigManager::m_trayMsgDuration = -1;
bool CConfigManager::m_isShowTrayMsg = true;
QString CConfigManager::m_securityProfile = "";

QString CConfigManager::m_tempLockCode = ""; // default temp lock code should be empty
bool CConfigManager::m_inTempLockMode = false;

int CConfigManager::m_svrFlashVersion = -1;
QString CConfigManager::m_svrFlashHash = "";

const int DEFAULT_TRAY_MSG_DURATION = 10000;/*ms*/ // the default message duration of system tray is 10s
const bool DEFAULT_SHOW_TRAY_MSG = true; // by default, we show tray messages;

QTime CConfigManager::m_autoPcShutdownTime;
QString CConfigManager::m_autoPcShutdownMsg;

QString CConfigManager::m_svrClientAdHash = "";
QString CConfigManager::m_svrClientAdUrl = "";

bool CConfigManager::init()
{
    // memory data init
    m_trayMsgDuration = DEFAULT_TRAY_MSG_DURATION;

    return true;
}

bool CConfigManager::loadRegData()
{
    QSettings gcbSettings( m_productKeyHive, QSettings::NativeFormat );

    // registry data init
    {
        // server addr
        m_serverAddr = _getRegData(gcbSettings, SERVER_ADDRESS).toString();

        // security profile
        QString secProfileEnc = _getRegData(gcbSettings, SECURITY_PROFILE).toString();
        m_securityProfile = Utils::symDecrypt(secProfileEnc);

        // flash hash
        QString flashHashEnc = _getRegData(gcbSettings, QString::number(CFG_S_FlashHash)).toString();
        m_svrFlashHash = Utils::symDecrypt(flashHashEnc);

        // client ad hash
        QString clientAdHashEnc = _getRegData(gcbSettings, QString::number(CFG_S_ClientAdHash)).toString();
        m_svrClientAdHash = Utils::symDecrypt(clientAdHashEnc);

        QString clientAdUrlEnc = _getRegData(gcbSettings, QString::number(CFG_S_ClientAdUrl)).toString();
        m_svrClientAdUrl = Utils::symDecrypt(clientAdUrlEnc);
    }
    

    return true;
}

QString CConfigManager::serverAddress()
{
    return m_serverAddr;
}

bool CConfigManager::setServerAddress( const QString& address )
{
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(SERVER_ADDRESS, address );
    m_serverAddr = address;

    return true;
}

int CConfigManager::clientId()
{
    return m_clientId;
}

bool CConfigManager::setClientId( int clientId )
{
    m_clientId = clientId;
    return true;
}

bool CConfigManager::isDisableSecurityRestrictions()
{
#ifndef NDEBUG
    return true;
#endif

    return isDeveloperEnv();
}

bool CConfigManager::isDeveloperEnv()
{
#ifndef NDEBUG
    return true;
#endif

    return false;
}

bool CConfigManager::showTrayMsg()
{
    return m_isShowTrayMsg;
}

bool CConfigManager::setShowTrayMsg( bool val )
{
    m_isShowTrayMsg = val;
    return true;
}

int CConfigManager::trayMsgDuration()
{
    if (m_trayMsgDuration > 0)
    {
        return m_trayMsgDuration;
    }
    
    return DEFAULT_TRAY_MSG_DURATION;
}

bool CConfigManager::setTrayMsgDuration( int val )
{
    if (m_trayMsgDuration != val)
    {
        m_trayMsgDuration = val;
    }
    return true;
}

QVariant CConfigManager::_getRegData( const QSettings& setting, const QString& key )
{
    if (!setting.contains(key))
    {
        return QVariant();
    }
    else
    {
        return setting.value(key);
    }
}

QVariant CConfigManager::_getRegData( QSettings& setting, const QString& key, const QVariant& defaultValue )
{
    if (!setting.contains(key))
    {
        setting.setValue( key, defaultValue);
        return defaultValue;
    }
    else
    {
        return setting.value(key);
    }
}

QString CConfigManager::adminNameEnc()
{
    QSettings gcbSettings( m_productKeyHive, QSettings::NativeFormat );
    return _getRegData(gcbSettings, ADMIN_NAME, DEFAULT_ADMIN_NAME).toString();
}

QString CConfigManager::adminNameRaw()
{
    return Utils::symDecrypt(adminNameEnc());
}

QString CConfigManager::adminCodeHash()
{
    QSettings gcbSettings( m_productKeyHive, QSettings::NativeFormat );
    return _getRegData(gcbSettings, ADMIN_CODE, DEFAULT_ADMIN_CODE).toString();
}

QString CConfigManager::tempLockCode()
{
    return m_tempLockCode;
}

bool CConfigManager::setTempLockCode( const QString& rawTempLockCode )
{
    m_tempLockCode = rawTempLockCode;
    return true;
}

bool CConfigManager::inTempLockMode()
{
    return m_inTempLockMode;
}

bool CConfigManager::setInTempLockMode( bool val )
{
    m_inTempLockMode = val;
    return true;
}

QString CConfigManager::securityProfile()
{
    return m_securityProfile;
}

bool CConfigManager::setSecurityProfile( const QString& rawSecProfile )
{
    QString profileEnc;
    if (!rawSecProfile.isEmpty())
    {
        profileEnc = Utils::symEncrypt(rawSecProfile);
    }

    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(SECURITY_PROFILE, profileEnc );
    m_securityProfile = rawSecProfile;

    return true;
}

int CConfigManager::svrFlashVersion()
{
    return m_svrFlashVersion;
}

bool CConfigManager::setSvrFlashVersion( int val )
{
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(QString::number(CFG_I_FlashVersion), val );
    m_svrFlashVersion = val;

    return true;
}

QString CConfigManager::svrFlashHash()
{
    return m_svrFlashHash;
}

bool CConfigManager::setSvrFlashHash( const QString& val )
{
    QString flashHashEnc;
    if (!val.isEmpty())
    {
        flashHashEnc = Utils::symEncrypt(val);
    }

    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(QString::number(CFG_S_FlashHash), flashHashEnc);
    m_svrFlashHash = val;

    return true;
}

QTime CConfigManager::autoPcShutdownTime()
{
    return m_autoPcShutdownTime;
}

bool CConfigManager::setAutoPcShutdownTime( const QTime &time )
{
    int timeVal = DateTimeUtils::timeToMinutes(time);
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue( AUTO_SCHEDULED_SHUTDOWN_TIME, timeVal );
    m_autoPcShutdownTime = time;
    return true;
}

QString CConfigManager::autoPcShutdownMsg()
{
    return m_autoPcShutdownMsg;
}

bool CConfigManager::setAutoPcShutdownMsg( const QString &val )
{
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue( AUTO_SCHEDULED_SHUTDOWN_MSG, val );
    m_autoPcShutdownMsg = val;
    return true;
}

QString CConfigManager::svrClientAdHash()
{
    return m_svrClientAdHash;
}

bool CConfigManager::setSvrClientAdHash(const QString &val)
{
    QString clientAdHashEnc;
    if (!val.isEmpty())
    {
        clientAdHashEnc = Utils::symEncrypt(val);
    }

    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(QString::number(CFG_S_ClientAdHash), clientAdHashEnc);
    m_svrClientAdHash = val;

    return true;
}

QString CConfigManager::svrClientAdUrl()
{
    return m_svrClientAdUrl;
}

bool CConfigManager::setSvrClientAdUrl(const QString &val)
{
    QString clientAdUrlEnc;
    if (!val.isEmpty())
    {
        clientAdUrlEnc = Utils::symEncrypt(val);
    }

    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(QString::number(CFG_S_ClientAdUrl), clientAdUrlEnc);
    m_svrClientAdUrl = val;

    return true;
}

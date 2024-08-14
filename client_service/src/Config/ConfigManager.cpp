#include "ConfigManager.h"
#include <string>
#include<QProcessEnvironment>
#include <QString>
#include <QDir>
#include <QSettings>
#include "Utils.h"
#include <QCoreApplication>
#include <QFile>
#include <QSysInfo>
#include <QDateTime>
#include "DBManager.h"
#include "log.h"
#include "DateTimeUtils.h"


//////////////////////////////////////////////////////////////////////////
enum CFG_TB_KEYS
{
    // the stored values are integer
    CFG_I_START = 0x0000,
    CFG_I_WebControlUpdateDT = 0x0001,

    // the stored values are string
    CFG_S_START = 0x2000,
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// values stored in registry
const QString ADMIN_NAME = "AdminName";
const QString ADMIN_CODE = "AdminCode"; // create by client when first launch
const QString SERVER_ADDRESS = "ServerAddress" ; // create by client when first launch
const QString RUNNING_STATE = "RunningState" ; 
//const QString CLIENT_ID = "ClientId" ; // the client sequence which is used to uniquely identify the client in server, maintained manually
const QString LANGUAGE = "Language";
const QString TEMP_LOCK_CODE = "ScreenLockCode"; // the password of "temp screen lock" dialog
const QString IN_TEMP_LOCK_MODE = "ScreenLockMode"; // whether the computer is in "temp screen lock" mode
const QString SECURITY_PROFILE = "SecurityProfile"; // the cache of security profile
const QString CLIENT_TOKEN = "ClientToken"; // each time the client starts, it generates an unique clientToken based on the current timestamp
const QString CLIENT_DATA_VERSION = "ClientDataVersion"; // this is NOT the same as client app version, every time the data format updates, this value changes
const QString SHUTDOWN_CLIENT_MINUTES = "shutdownClientMin";
const QString CLEAR_APP_MINUTES = "clearAppMin";
const QString AUTO_SCHEDULED_SHUTDOWN_TIME = "autoScheduledShutdownTime";
const QString CAFE_ID = "cafeId";
//////////////////////////////////////////////////////////////////////////

QString CConfigManager::m_productKeyHive = 
"HKEY_LOCAL_MACHINE\\Software\\" + 
G_COMPANY + "\\" + 
CLIENT_DATA_REGKEY_NAME;


// value of "configs" table of main DB
uint64_t CConfigManager::m_lastUpdateWebCtrlDTVal = 0;

// value in registry
int CConfigManager::m_clientId = -1;
QString CConfigManager::m_serverAddr = "";
QString CConfigManager::m_adminNameEnc = "";
QString CConfigManager::m_adminCodeHash = "";
QString CConfigManager::m_language = ""; // default is en
int CConfigManager::m_trayMsgDuration = -1;
bool CConfigManager::m_isShowTrayMsg = true;
QString CConfigManager::m_tempLockCode = ""; // default temp lock code should be empty
bool CConfigManager::m_inTempLockMode = false;
QString CConfigManager::m_securityProfile = "";
//QString CConfigManager::m_token = "";

int CConfigManager::m_shutdownAvailableClientMinutes = -255; // default is -1
int CConfigManager::m_clearAppAvailableClientMinutes = -255; // default is 10

const int DEFAULT_SHUTDOWN_AVAILABLE_PC_MIN = -1; // -1 means this feature is disabled
const int DEFAULT_CLEAR_APP_ON_AVAILABLE_PC_MIN = 10; // -1 means this feature is disabled

const int DEFAULT_TRAY_MSG_DURATION = 10000;/*ms*/ // the default message duration of system tray is 10s
const bool DEFAULT_SHOW_TRAY_MSG = true; // by default, we show tray messages;

QTime CConfigManager::m_autoPcShutdownTime;

bool CConfigManager::init()
{
    // memory data init
    m_trayMsgDuration = DEFAULT_TRAY_MSG_DURATION;

    //result = result && _upgradeData();
    return true;
}

bool CConfigManager::loadRegData()
{
    QSettings gcbSettings( m_productKeyHive, QSettings::NativeFormat );

    // registry data init
    m_serverAddr = _getRegData(gcbSettings, SERVER_ADDRESS).toString();
    m_adminNameEnc = _getRegData(gcbSettings, ADMIN_NAME, DEFAULT_ADMIN_NAME).toString();
    m_adminCodeHash = _getRegData(gcbSettings, ADMIN_CODE, DEFAULT_ADMIN_CODE).toString();

    QString tempLockEnc = _getRegData(gcbSettings, TEMP_LOCK_CODE).toString();
    m_tempLockCode = Utils::symDecrypt(tempLockEnc);
    m_inTempLockMode = _getRegData(gcbSettings, IN_TEMP_LOCK_MODE).toBool();
    QString secProfileEnc = _getRegData(gcbSettings, SECURITY_PROFILE).toString();
    m_securityProfile = Utils::symDecrypt(secProfileEnc);

    m_shutdownAvailableClientMinutes = _getRegData(gcbSettings, SHUTDOWN_CLIENT_MINUTES, DEFAULT_SHUTDOWN_AVAILABLE_PC_MIN).toInt();
    m_shutdownAvailableClientMinutes = _getRegData(gcbSettings, CLEAR_APP_MINUTES, DEFAULT_CLEAR_APP_ON_AVAILABLE_PC_MIN).toInt();
    
    return true;
}

bool CConfigManager::loadDBData()
{
    // DB data init
    m_lastUpdateWebCtrlDTVal = _readDBConfig(WEB_CONTROL_LAST_UPDATE_DT, /*default*/0).toULongLong();

    return true;
}

//bool CConfigManager::_upgradeData()
//{
//    int previousDataVer = clientDataVersion();
//    int curDataVer = CURRENT_DATA_VERSION;
//    return _upgradeDbVersoin(previousDataVer, curDataVer);
//}

QString CConfigManager::adminNameEnc()
{
    return m_adminNameEnc;
}

bool CConfigManager::setAdminNameEnc( const QString& val )
{
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue( ADMIN_NAME, val);
    m_adminNameEnc = val;
    return true;
}

QString CConfigManager::adminCodeHash()
{
    return m_adminCodeHash;
}

bool CConfigManager::setAdminCodeHash( const QString& adminCodeHash )
{
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue( ADMIN_CODE, adminCodeHash);
    m_adminCodeHash = adminCodeHash;
    return true;
}

QString CConfigManager::serverAddress()
{
    // keep this, sometimes after client app set the IP(to registry), client service failed
    // to get it, so keep it reading from registry
    if (m_serverAddr.isEmpty())
    {
        QSettings gcbSettings( m_productKeyHive, QSettings::NativeFormat );
        m_serverAddr = _getRegData(gcbSettings, SERVER_ADDRESS).toString();
    }

    return m_serverAddr;
}

bool CConfigManager::setServerAddress( const QString& address )
{
    //QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    //settings.setValue(SERVER_ADDRESS, address );
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

QLocale::Language CConfigManager::languageVal()
{
    QString clientLang = language();

    if (clientLang == "zh")
    {
        return QLocale::Chinese;
    }
    else if (clientLang == "vi")
    {
        return QLocale::Vietnamese;
    }
    else if (clientLang == "en")
    {
        return QLocale::English;
    }

    return QLocale::English;
}

QString CConfigManager::language()
{
    return m_language;
}

bool CConfigManager::setLanguage( QLocale::Language lang )
{
    QString langStr;
    if (lang == QLocale::Chinese)
    {
        langStr = "zh";
    }
    else if (lang == QLocale::Vietnamese)
    {
        langStr == "vi";
    }
    else //if (lang == QLocale::English) // make the default language en-us if it's not specified
    {
        langStr = "en";
    }

    return setLanguage(langStr);
}

bool CConfigManager::setLanguage( const QString& lang )
{
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue( LANGUAGE, lang );
    m_language = lang;
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

QString CConfigManager::tempLockCode()
{
    return m_tempLockCode;
}

bool CConfigManager::setTempLockCode( const QString& rawTempLockCode )
{
    QString codeEnc;
    if (!rawTempLockCode.isEmpty())
    {
        codeEnc = Utils::symEncrypt(rawTempLockCode);
    }

    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(TEMP_LOCK_CODE, codeEnc );
    m_tempLockCode = rawTempLockCode;

    return true;
}

bool CConfigManager::inTempLockMode()
{
    return m_inTempLockMode;
}

bool CConfigManager::setInTempLockMode( bool val )
{
    int valToStore = (int)val;
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(IN_TEMP_LOCK_MODE, valToStore );
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

int CConfigManager::shutdownAvailableClientMinutes()
{
    return m_shutdownAvailableClientMinutes;
}

bool CConfigManager::setShutdownAvailableClientMinutes(int val)
{
    // have to store it as string, instead of DWORD
    // as the value may be negative, which won't be accepted by regedit
    QString valStr = QString::number(val);
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(SHUTDOWN_CLIENT_MINUTES, valStr );
    m_shutdownAvailableClientMinutes = val;
    return true;
}

int CConfigManager::clearAppAvailableClientMinutes()
{
    return m_clearAppAvailableClientMinutes;
}

bool CConfigManager::setClearAppAvailableClientMinutes(int val)
{
    QString valStr = QString::number(val);
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(CLEAR_APP_MINUTES, valStr );
    m_clearAppAvailableClientMinutes = val;
    return true;
}

bool CConfigManager::setCafeId(int val)
{
    QString valStr = QString::number(val);
    QSettings settings( m_productKeyHive, QSettings::NativeFormat );
    settings.setValue(CAFE_ID, valStr );
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
QDateTime CConfigManager::lastUpdateWebCtrlDT()
{
    return LegacyDateTimeUtils::uint64InSecToDateTime(m_lastUpdateWebCtrlDTVal);
}

qint64 CConfigManager::lastUpdateWebCtrlDTVal()
{
    return m_lastUpdateWebCtrlDTVal;
}

bool CConfigManager::setLastUpdateWebCtrlDT( qint64 val )
{
    bool result = false;
    if (_hasActiveDBConfig(WEB_CONTROL_LAST_UPDATE_DT))
    {
        result = _modifyDBConfiguration(WEB_CONTROL_LAST_UPDATE_DT, val, true);
    }
    else
    {
        result = _addDBConfiguration(WEB_CONTROL_LAST_UPDATE_DT, val);
    }

    if (result == true)
    {
        m_lastUpdateWebCtrlDTVal = val;
    }

    return result;
}

bool CConfigManager::setLastUpdateWebCtrlDT( const QDateTime& dt )
{
    return setLastUpdateWebCtrlDT(LegacyDateTimeUtils::dateTimeToUInt64InSec(dt));
}

bool CConfigManager::_addDBConfiguration( const QString& name, const QVariant& value )
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    if (!query.prepare(QString("insert into %1 (name, value, enabled) values(?, ?, 1)").arg(CONFIG_TABLE)))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(name);
    query.addBindValue(value);

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    return true;
}

bool CConfigManager::_modifyDBConfiguration(const QString& name, const QVariant& value, bool enabled)
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);
    QString queryStr = QString(" \
                               UPDATE %1 \
                               SET value=?, enabled=? where name=?")
        .arg(CONFIG_TABLE);

    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(value);
    query.addBindValue((int)enabled);
    query.addBindValue(name);

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    return true;
}

QVariant CConfigManager::_readDBConfig( const QString& name)
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    QString queryStr = QString("select value from %1 where name = ? and enabled = 1").arg(CONFIG_TABLE);
    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return QVariant();
    }

    query.addBindValue(name);

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return QVariant();
    }

    while(query.next())
    {
        return query.value(0);
    }

    return QVariant();
}

QVariant CConfigManager::_readDBConfig( const QString& name, const QVariant& defaultVal )
{
    if (_hasActiveDBConfig(name) == false)
    {
        _addDBConfiguration(name, defaultVal);
        return defaultVal;
    }

    return _readDBConfig(name);
}


bool CConfigManager::_hasActiveDBConfig(const QString& name)
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    QString queryStr = QString("select value from %1 where name = ? and enabled = 1").arg(CONFIG_TABLE);
    if (!query.prepare(queryStr))
    {
        QString text = query.lastError().text();
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(name);

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    while(query.next())
    {
        return true;
    }

    return false;
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





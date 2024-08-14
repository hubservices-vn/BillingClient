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
#include "ClientCommonSettings.h"

QString CConfigManager::m_productKeyHive = 
"HKEY_LOCAL_MACHINE\\Software\\" + G_COMPANY + "\\" + CLIENT_DATA_REGKEY_NAME;

//
// in registry
//
const QString ADMIN_NAME = "AdminName";
const QString ADMIN_CODE = "AdminCode"; // create by client when first launch
const QString SERVER_ADDRESS = "ServerAddress" ; // create by client when first launch
const QString TEMP_LOCK_CODE = "ScreenLockCode"; // the password of "temp screen lock" dialog
const QString IN_TEMP_LOCK_MODE = "ScreenLockMode"; // whether the computer is in "temp screen lock" mode

enum CFG_TB_KEYS
{
    // the stored values are integer
    CFG_I_START                             = 0x0000,
    CFG_I_FlashVersion                      = 0x0001,

    // the stored values are string
    CFG_S_START                             = 0x2000,
    CFG_S_FlashHash                         = 0x2001,
};
//////////////////////////////////////////////////////////////////////////


int CConfigManager::m_clientId = -1;

int CConfigManager::m_svrFlashVersion = -1;
QString CConfigManager::m_svrFlashHash;


bool CConfigManager::init()
{
    return true;
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

bool CConfigManager::inTempLockMode()
{
    QSettings gcbSettings( m_productKeyHive, QSettings::NativeFormat );
    return _getRegData(gcbSettings, IN_TEMP_LOCK_MODE, false).toBool();
}

int CConfigManager::svrFlashVersion()
{
    return m_svrFlashVersion;
}

bool CConfigManager::setSvrFlashVersion( int val )
{
    m_svrFlashVersion = val;
    return true;
}

QString CConfigManager::svrFlashHash()
{
    return m_svrFlashHash;
}

bool CConfigManager::setSvrFlashHash( const QString& val )
{
    m_svrFlashHash = val;
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

bool CConfigManager::isDisableSecurityRestrictions()
{
#ifndef NDEBUG
    return true;
#endif

    return isDeveloperEnv();
}

bool CConfigManager::isNoFullScreen()
{
#ifndef NDEBUG
    return true;
#endif

    return false;
}

bool CConfigManager::isDeveloperEnv()
{
#ifndef NDEBUG
    return true;
#endif

    return false;
}

bool CConfigManager::isNoFlash()
{
#ifndef NDEBUG
    return true;
#endif
    return false;
}

int CConfigManager::clientId()
{
    return m_clientId;
}



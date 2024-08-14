#include "OSAgentWinQt.h"
#include <ConfigManager.h>

#include <string>
using namespace std;

#include <QString>
#include <QSettings>
#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QVariant>
#include <QUuid>
#include <QFile>

#include "log.h"
#include "Utils.h"
#include <winreg.h>

#include <prochook.h>
#include <assert.h>
#pragma comment(lib,"prochost.lib")

QString OSAgentWinQt::m_productKeyHive = 
    "HKEY_LOCAL_MACHINE\\Software\\" + 
    G_COMPANY + "\\" + 
    CLIENT_DATA_REGKEY_NAME;

OSAgentWinQt::OSAgentWinQt()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: init prochook in OSAgentWinQt ctor";
        return;
    }

    m_prochost = CreateProcHost(this,PROC_HOOK_VERSION);
}

OSAgentWinQt::~OSAgentWinQt()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: dispose prochook in OSAgentWinQt dtor";
        return;
    }

    DestoryProcHost(m_prochost);
    m_prochost=NULL;
}

bool OSAgentWinQt::disableSystemTaskManager()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableSystemTaskManager";
        return true;
    }

    LOG_DEBUG4 << "disable taskmgr in security component";
    m_prochost->AddApp(L"taskmgr.exe",APP_BLOCKED);

    return true;
}

bool OSAgentWinQt::enableSystemTaskManager()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableSystemTaskManager";
        return true;
    }

    LOG_DEBUG4 << "enable taskmgr in security component";
    m_prochost->RemoveApp(L"taskmgr.exe",APP_BLOCKED);

    return true;
}

bool OSAgentWinQt::disableAppRunByProcName (const std::wstring & processName )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableAppRunByProcName";
        return true;
    }

    m_prochost->AddApp(processName.c_str(),APP_BLOCKED);

    return true;
}

bool OSAgentWinQt::enableAppRunByProcName (const std::wstring & processName )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAppRunByProcName";
        return true;
    }

    m_prochost->RemoveApp(processName.c_str(),APP_BLOCKED);

    return true;
}

// "network connections" is special, it should always be "No"
// otherwise there will be 2 "network connections" icon in control panel
bool OSAgentWinQt::disableNetworkConnections ()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableNetworkConnections";
        return true;
    }

    m_prochost->AddApp(L"ncpa.cpl", APP_BLOCKED);
    {
        // step 2: unregister netshell.dll
        QStringList args;
        args << "/s" << "/u" << "netshell.dll";
        QProcess::startDetached("regsvr32", args);
    }

    return true;
}

// "network connections" is special, it should always be "No"
// otherwise there will be 2 "network connections" icon in control panel
bool OSAgentWinQt::enableNetworkConnections ()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableNetworkConnections";
        return true;
    }

    m_prochost->RemoveApp(L"ncpa.cpl", APP_BLOCKED);
    {
        // step 2: register netshell.dll
        QStringList args;
        args << "/s" << "netshell.dll";
        QProcess::startDetached("regsvr32", args);
    }

    return true;
}

// "network connections" is special, it should always be "No"
// otherwise there will be 2 "network connections" icon in control panel
bool OSAgentWinQt::disableControlPanelItem ( const std::wstring & name )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableControlPanelItem";
        return true;
    }

    m_prochost->AddApp(name.c_str(), APP_BLOCKED);

    return true;
}

// "network connections" is special, it should always be "No"
// otherwise there will be 2 "network connections" icon in control panel
bool OSAgentWinQt::enableControlPanelItem ( const std::wstring & name )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableControlPanelItem";
        return true;
    }

    m_prochost->RemoveApp(name.c_str(), APP_BLOCKED);

    return true;
}


bool OSAgentWinQt::disableRegedit()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableRegedit";
        return true;
    }

    LOG_DEBUG4 << "disable regedit in security component";
    m_prochost->AddApp(L"regedit.exe",APP_BLOCKED);

    return true;
}

bool OSAgentWinQt::enableRegedit()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableRegedit";
        return true;
    }

    LOG_DEBUG4 << "enable regedit in security component";
    m_prochost->RemoveApp(L"regedit.exe",APP_BLOCKED);

    return true;
}

// this key exists only in HKEY_LOCAL_MACHINE, so can't handle within HKEY_LOCAL_MACHINE
bool OSAgentWinQt::disableAdministrativeTools()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableAdministrativeTools";
        return true;
    }

    QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\";
    QSettings settings( keyHive, QSettings::NativeFormat );
    bool exists = settings.childGroups().contains("{D20EA4E1-3957-11d2-A40B-0C5020524153}");

    if (exists)
    {
        // backup the old value
        QString keyHive2 = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\{D20EA4E1-3957-11d2-A40B-0C5020524153}";
        QSettings settings2( keyHive2, QSettings::NativeFormat );
        QVariant oldValue = settings2.value("Default");

        // delete the old key
        settings.remove("{D20EA4E1-3957-11d2-A40B-0C5020524153}");

        // create the new one
        QString keyHive3 = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\-{D20EA4E1-3957-11d2-A40B-0C5020524153}";
        QSettings settings3( keyHive3, QSettings::NativeFormat );
        settings3.setValue("Default", oldValue); 
    }

    return true;
}

// this key exists only in HKEY_LOCAL_MACHINE, so can't handle within HKEY_LOCAL_MACHINE
bool OSAgentWinQt::enableAdministrativeTools()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAdministrativeTools";
        return true;
    }

    QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace";
    QSettings settings( keyHive, QSettings::NativeFormat );
    bool exists = settings.childGroups().contains("-{D20EA4E1-3957-11d2-A40B-0C5020524153}");

    if (exists)
    {
        // backup the old value
        QString keyHive2 = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\-{D20EA4E1-3957-11d2-A40B-0C5020524153}";
        QSettings settings2( keyHive2, QSettings::NativeFormat );        
        QVariant oldValue = settings2.value("Default");

        // delete the old key
        settings.remove("-{D20EA4E1-3957-11d2-A40B-0C5020524153}");

        // create the new one
        QString keyHive3 = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\{D20EA4E1-3957-11d2-A40B-0C5020524153}";
        QSettings settings3( keyHive3, QSettings::NativeFormat );
        settings3.setValue("Default", oldValue); 
    }

    return true;
}

bool OSAgentWinQt::disableCDRomAutorun()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableCDRomAutorun";
        return true;
    }

    QString keyHive = "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
    QSettings settings( keyHive, QSettings::NativeFormat );
    settings.setValue( QString("NoDriveTypeAutoRun"), 181);

    return true;
}

bool OSAgentWinQt::enableCDRomAutorun()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableCDRomAutorun";
        return true;
    }

    QString keyHive = "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
    QSettings settings( keyHive, QSettings::NativeFormat );
    settings.setValue( QString("NoDriveTypeAutoRun"), 145);

    return true;
}

// TODO: in windows 7, need to set more registry options
//bool OSAgentWinQt::hideSelfFromAppList()
//{
//    if (CConfigManager::isDisableSecurityRestrictions() == true)
//    {
//        LOG_DEBUG4 << "simulated action: hideSelfFromAppList";
//        return true;
//    }
//
//    QString selfAppName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
//    std::wstring selfAppNameW = selfAppName.toStdWString();
//
//    m_prochost->Install();
//    m_prochost->Enable(TRUE);
//
//    LOG_DEBUG4 << "add self: " << Utils::UnicodeToUtf8(selfAppNameW) << " to exclude list";
//    m_prochost->AddApp(selfAppNameW.c_str(),APP_EXCLUDED);// add ourself to the white list
//
//    LOG_DEBUG4 << "add gbClientApp.exe to exclude list";
//    m_prochost->AddApp(L"gbClientApp.exe",APP_EXCLUDED); 
//
//    // don't allow user run tasklist/taskkill to kill our app
//    m_prochost->AddApp(L"tasklist.exe", APP_BLOCKED);
//    m_prochost->AddApp(L"taskkill.exe", APP_BLOCKED);
//
//    // disallow "tsKill" built-in
//    m_prochost->AddApp(L"tskill.exe", APP_BLOCKED);
//
//    // disallow "gpedit.msc", "services.msc"
//    m_prochost->AddApp(L"gpedit.msc", APP_BLOCKED);
//    m_prochost->AddApp(L"services.msc", APP_BLOCKED);
//    // disallow "mmc.exe"
//    m_prochost->AddApp(L"mmc.exe", APP_BLOCKED);
//    // disallow "net.exe", "net1.exe" to prevent "net start" or "net stop"
//    m_prochost->AddApp(L"net.exe", APP_BLOCKED);
//    m_prochost->AddApp(L"net1.exe", APP_BLOCKED);
//
//    m_prochost->SetPermission(ACTION_CREATE_DESKTOP,true);
//    m_prochost->SetPermission(ACTION_CHANGE_TIME,true);
//    m_prochost->SetPermission(ACTION_ENUM_PROCESS,true);
//
//    return true;
//}

bool OSAgentWinQt::disableAppRunByHash(const QString& fileName, const QString& hashStr, const QString& itemSizeStr, const QString& hashAlgStr)
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableAppRunByHash";
        return true;
    }

    m_prochost->AddAppHash(hashStr.toStdWString().c_str(),APP_BLOCKED);
    return true;
}

bool OSAgentWinQt::enableAppRunByHash(const QString& hashStr)
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAppRunByHash";
        return true;
    }

    m_prochost->RemoveAppHash(hashStr.toStdWString().c_str(),APP_BLOCKED);
    return true;
}

bool OSAgentWinQt::enableAllProcSecurity()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAllProcSecurity";
        return true;
    }

    LOG_DEBUG4 << "enable process protection for local admin";
    m_prochost->Enable(false);

    return true;
}

bool OSAgentWinQt::applyDefaultProcSecurity()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: applyDefaultProcSecurity";
        return true;
    }

    LOG_DEBUG4 << "disable process access for normal user";
    m_prochost->Enable(true);

    return true;
}

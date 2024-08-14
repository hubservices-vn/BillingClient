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
#include <shlobj.h>
#include <winreg.h>

#include <prochook.h>
#include <assert.h>
#pragma comment(lib,"prochost.lib")

QString OSAgentWinQt::m_productKeyHive = 
"HKEY_LOCAL_MACHINE\\Software\\" + 
G_COMPANY + "\\" + 
CLIENT_DATA_REGKEY_NAME;

namespace
{
    void enableDebugPrivileges()
    {
        HANDLE hcurrent=GetCurrentProcess();
        HANDLE hToken;
        BOOL bret = OpenProcessToken(hcurrent,40,&hToken);
        if ( ! bret)
        {
            LOG_ERROR << "failed to open proc token";
            return;
        }
        LUID luid;
        bret = LookupPrivilegeValue(NULL,SE_DEBUG_NAME, &luid);
        if ( ! bret)
        {
            LOG_ERROR << "failed to LookupPrivilegeValue";
            return;
        }

        TOKEN_PRIVILEGES NewState,PreviousState;
        DWORD ReturnLength;
        NewState.PrivilegeCount =1;
        NewState.Privileges[0].Luid =luid;
        NewState.Privileges[0].Attributes=2;
        bret = AdjustTokenPrivileges(hToken,FALSE,&NewState,28,&PreviousState,&ReturnLength);
        if ( ! bret)
        {
            LOG_ERROR << "failed to AdjustTokenPrivileges";
            return;
        }
    }
}

OSAgentWinQt::OSAgentWinQt()
{
    //if (CConfigManager::isDisableSecurityRestrictions() == true)
    //{
    //    LOG_DEBUG4 << "simulated action: init prochook in OSAgentWinQt ctor";
    //    return;
    //}

    m_prochost = CreateProcHost(this,PROC_HOOK_VERSION);
    if (m_prochost == NULL)
    {
        LOG_ERROR << "can not create prochost instance";
    }
    else
    {
        LOG_DEBUG << "prochost inited successfully";
    }

    m_procHostValid = (m_prochost != NULL);
}

OSAgentWinQt::~OSAgentWinQt()
{
    //if (CConfigManager::isDisableSecurityRestrictions() == true)
    //{
    //    LOG_DEBUG4 << "simulated action: dispose prochook in OSAgentWinQt dtor";
    //    return;
    //}

    LOG_DEBUG << "disposing prochost";
    DestoryProcHost(m_prochost);
    m_prochost=NULL;
}

bool OSAgentWinQt::registerAsAutoRun()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: registerAsAutoRun";
        return true;
    }

    QString keyHive = "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings settings( keyHive, QSettings::NativeFormat );
    QString app = QCoreApplication::applicationFilePath();
    app = QDir::toNativeSeparators(app);
    settings.setValue( APP_AUTORUN_REGKEY_NAME, app);
    return true;
}

bool OSAgentWinQt::unregisterAsAutoRun()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: unregisterAsAutoRun";
        return true;
    }

    QString keyHive = "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings settings( keyHive, QSettings::NativeFormat );
    settings.remove( APP_AUTORUN_REGKEY_NAME );
    return true;
}

bool OSAgentWinQt::disableSystemTaskManager()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableSystemTaskManager";
        return true;
    }

    LOG_DEBUG4 << "disable taskmgr in security component";

    _hideTaskmgr(); // disable by registry

    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->AddApp(L"taskmgr.exe",APP_BLOCKED);
    m_prochost->AddApp(L"perfmon.exe",APP_BLOCKED);

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

    _showTaskmgr(); // enable by registry

    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->RemoveApp(L"taskmgr.exe",APP_BLOCKED);
    m_prochost->RemoveApp(L"perfmon.exe",APP_BLOCKED);

    return true;
}

bool OSAgentWinQt::disableAppRunByProcName (const std::wstring & processName )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableAppRunByProcName";
        return true;
    }

    LOG_DEBUG4 << "action: disableAppRunByProcName: " << qPrintable(QString::fromStdWString(processName));
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
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

    LOG_DEBUG4 << "action: enableAppRunByProcName: " << qPrintable(QString::fromStdWString(processName));
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->RemoveApp(processName.c_str(),APP_BLOCKED);

    LOG_DEBUG4 << "action: enableAppRunByProcName done";

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

    LOG_DEBUG4 << "action: disableNetworkConnections";

    // step 1: make sure the "ncpa.cpl" is hidden
    QString keyHive2 = "HKEY_CURRENT_USER\\Control Panel\\don't load";
    QSettings settings2( keyHive2, QSettings::NativeFormat );
    settings2.setValue( "ncpa.cpl", "No");

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

    LOG_DEBUG4 << "action: enableNetworkConnections";

    // step 1: make sure the "ncpa.cpl" is hidden
    QString keyHive2 = "HKEY_CURRENT_USER\\Control Panel\\don't load";
    QSettings settings2( keyHive2, QSettings::NativeFormat );
    settings2.setValue( "ncpa.cpl", "No");

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

    LOG_DEBUG4 << "action: disableControlPanelItem";

    QString keyHive2 = "HKEY_CURRENT_USER\\Control Panel\\don't load";
    QSettings settings2( keyHive2, QSettings::NativeFormat );
    settings2.setValue( QString::fromStdWString(name), "No");

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

    LOG_DEBUG4 << "action: enableControlPanelItem";

    if (::wcsicmp(name.c_str(), L"ncpa.cpl") == 0)
    {
        return true;
    }

    QString keyHive2 = "HKEY_CURRENT_USER\\Control Panel\\don't load";
    QSettings settings2( keyHive2, QSettings::NativeFormat );
    settings2.remove( QString::fromStdWString(name));

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
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
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
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
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

    LOG_DEBUG4 << "action: disableAdministrativeTools";

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

    LOG_DEBUG4 << "action: enableAdministrativeTools";

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

bool OSAgentWinQt::disableClose()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableClose";

        return true;
    }

	QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";

    QSettings settings(keyHive, QSettings::NativeFormat);

	settings.setValue("NoClose", 1);

	return true;
}

bool OSAgentWinQt::enableClose()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableClose";

        return true;
    }

	QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";

    QSettings settings(keyHive, QSettings::NativeFormat);

	settings.remove("NoClose");

	return true;
}

bool OSAgentWinQt::disableLogOff()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLogOff";

        return true;
    }

	QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";

    QSettings settings(keyHive, QSettings::NativeFormat);

	settings.setValue("StartMenuLogOff", 1);

	return true;
}

bool OSAgentWinQt::enableLogOff()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableLogOff";

        return true;
    }

	QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";

    QSettings settings(keyHive, QSettings::NativeFormat);

	settings.remove("StartMenuLogOff");

	return true;
}

bool OSAgentWinQt::disableSwitchUser()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableSwitchUser";

        return true;
    }

	if (QSysInfo::windowsVersion() == QSysInfo::WV_XP)
	{
		QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";

		QSettings settings(keyHive, QSettings::NativeFormat);

		settings.setValue("AllowMultipleTSSessions", 0);
	}
	else
	{
		QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";

		QSettings settings(keyHive, QSettings::NativeFormat);

		settings.setValue("HideFastUserSwitching", 1);
	}

	return true;
}

bool OSAgentWinQt::enableSwitchUser()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableSwitchUser";

        return true;
    }

	if (QSysInfo::windowsVersion() == QSysInfo::WV_XP)
	{
		QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";

		QSettings settings(keyHive, QSettings::NativeFormat);

		settings.setValue("AllowMultipleTSSessions", 1);
	}
	else
	{
		QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";

		QSettings settings(keyHive, QSettings::NativeFormat);

		settings.remove("HideFastUserSwitching");
	}

	return true;
}

bool OSAgentWinQt::disableLock()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLock";

        return true;
    }
		
	QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";

	QSettings settingsLocalMachine(keyHive, QSettings::NativeFormat);

	settingsLocalMachine.setValue("DisableLockWorkstation", 1);

	keyHive = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";

	QSettings settingsCurrentUser(keyHive, QSettings::NativeFormat);

	settingsCurrentUser.setValue("DisableLockWorkstation", 1);

	return true;
}

bool OSAgentWinQt::enableLock()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableLock";

        return true;
    }

	QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";

	QSettings settingsLocalMachine(keyHive, QSettings::NativeFormat);

	settingsLocalMachine.remove("DisableLockWorkstation");

	keyHive = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";

	QSettings settingsCurrentUser(keyHive, QSettings::NativeFormat);

	settingsCurrentUser.remove("DisableLockWorkstation");

	return true;
}

bool OSAgentWinQt::disableRunCommand()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableRunCommand";

        return true;
    }

	QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";

    QSettings settings(keyHive, QSettings::NativeFormat);

	settings.setValue("NoRun", 1);

	return true;
}

bool OSAgentWinQt::enableRunCommand()
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableRunCommand";

        return true;
    }

	QString keyHive = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";

    QSettings settings(keyHive, QSettings::NativeFormat);

	return true;
}

void OSAgentWinQt::refreshExplorer()
{
	SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, NULL);

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
    //SHChangeNotify(SHCNE_ALLEVENTS, SHCNF_IDLIST, 0, 0);
}

// TODO: in windows 7, need to set more registry options
bool OSAgentWinQt::initSecurityProtection()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: hideSelfFromAppList";
        return true;
    }

    LOG_DEBUG4 << "action: initSecurityProtection";

    if (m_prochost == NULL)
    {
        m_procHostValid = false;
        LOG_ERROR << "can not find prochost, skipping...";
        return false;
    }

    m_procHostValid = m_prochost->Install();
    if (m_procHostValid == false)
    {
        LOG_ERROR << "failed to install security protect, skipping...";
        return false;
    }
    LOG_DEBUG4 << "init prochost succeeded, continue";
    m_prochost->Enable(TRUE);

    QString selfAppName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    std::wstring selfAppNameW = selfAppName.toStdWString();

    //LOG_DEBUG4 << "add self: " << Utils::UnicodeToUtf8(selfAppNameW) << " to exclude list";
    m_prochost->AddApp(selfAppNameW.c_str(),APP_EXCLUDED);// add ourself to the white list

    //LOG_DEBUG4 << "add gbClientService.exe to exclude list";
    m_prochost->AddApp(SVC_EXE_NAMEWITHEXT.toStdWString().c_str(),APP_EXCLUDED); 

    // disallow user to run tasklist/taskkill to kill our app
    m_prochost->AddApp(L"tasklist.exe", APP_BLOCKED);
    m_prochost->AddApp(L"taskkill.exe", APP_BLOCKED);

    // disallow "tsKill" built-in
    m_prochost->AddApp(L"tskill.exe", APP_BLOCKED);

    // disallow "mmc.exe" (gpedit, services, etc)
    m_prochost->AddApp(L"mmc.exe", APP_BLOCKED);
    m_prochost->AddApp(L"filemgmt.dll", APP_BLOCKED);
    m_prochost->AddApp(L"gpedit.dll", APP_BLOCKED);

    // disallow "net.exe", "net1.exe" to prevent "net start" or "net stop"
    m_prochost->AddApp(L"net.exe", APP_BLOCKED);
    m_prochost->AddApp(L"net1.exe", APP_BLOCKED);

    m_prochost->SetPermission(ACTION_CREATE_DESKTOP,true);
    m_prochost->SetPermission(ACTION_CHANGE_TIME,true);
    m_prochost->SetPermission(ACTION_ENUM_PROCESS,true);

    return true;
}

bool OSAgentWinQt::disableAppRunByHash(const QString& fileName, const QString& hashStr, const QString& itemSizeStr, const QString& hashAlgStr)
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableAppRunByHash";
        return true;
    }

    LOG_DEBUG4 << "action: disableAppRunByHash " << qPrintable(hashStr);
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
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

    LOG_DEBUG4 << "action: enableAppRunByHash " << qPrintable(hashStr);
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->RemoveAppHash(hashStr.toStdWString().c_str(),APP_BLOCKED);
    return true;
}

bool OSAgentWinQt::disableAppRunBySig( const QString& fileName, const QString& sig )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableAppRunBySig";
        return true;
    }

    LOG_DEBUG4 << "action: disableAppRunBySig " << qPrintable(sig);
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->AddAppSign(sig.toStdWString().c_str(),APP_BLOCKED);
    return true;
}

bool OSAgentWinQt::enableAppRunBySig( const QString& sig )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAppRunBySig";
        return true;
    }

    LOG_DEBUG4 << "action: enableAppRunBySig " << qPrintable(sig);
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->RemoveAppSign(sig.toStdWString().c_str(),APP_BLOCKED);
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
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }


    // enable registry settings
    _showTaskmgr();

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
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->Enable(true);

    return true;
}


bool OSAgentWinQt::_hideTaskmgr( void )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: _hideTaskmgr";
        return true;
    }

    LOG_DEBUG4 << "_hideTaskmgr";
    QString keyHive = "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    QSettings settings( keyHive, QSettings::NativeFormat );
    settings.setValue( QString("DisableTaskMgr"), 1);

    QString keyHive2 = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    QSettings settings2( keyHive2, QSettings::NativeFormat );
    settings2.setValue( QString("DisableTaskMgr"), 1);

    return true;
}

bool OSAgentWinQt::_showTaskmgr( void )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: _showTaskmgr";
        return true;
    }

    LOG_DEBUG4 << "_showTaskmgr";
    QString keyHive = "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    QSettings settings( keyHive, QSettings::NativeFormat );
    settings.setValue( QString("DisableTaskMgr"), 0);

    QString keyHive2 = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    QSettings settings2( keyHive2, QSettings::NativeFormat );
    settings2.setValue( QString("DisableTaskMgr"), 0);
    return true;
}

bool OSAgentWinQt::disableProcHost()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAllProcSecurity";
        return true;
    }

    LOG_DEBUG4 << "enable process protection for local admin";
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return true;
    }
    m_prochost->Enable(false);

    return true;
}

bool OSAgentWinQt::enableProcHost()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableAllProcSecurity";
        return true;
    }

    LOG_DEBUG4 << "enable process protection for local admin";
    if (m_procHostValid == false)
    {
        LOG_ERROR << "prochost invalid, skip";
        return false;
    }
    m_prochost->Enable(true);

    return true;
}

/*WINDOWS 7 SECTIONS*/
#include <InitGuid.h>
#include <GPEdit.h>
#include <UserEnv.h>
#pragma comment(lib,"userenv.lib")

bool OSAgentWinQt::updateGroupPolicy(QString gpoPath, QString key, unsigned long val)
{
    enableDebugPrivileges();

    LOG_DEBUG4 << "Start changing group policy object " ;
    HRESULT hr = S_OK;
    LSTATUS rStatus;
    IGroupPolicyObject* pLGPO;
    HKEY user_key, dsrkey;

    // Create an instance of the IGroupPolicyObject class
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoInitializeEx failed; hr = " << std::hex << hr;
        return false;
    }

    hr = CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_INPROC_SERVER,
        IID_IGroupPolicyObject, (LPVOID*)&pLGPO);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoCreateInstance failed; hr = " << std::hex << hr;
        return false;
    }

    // We need the machine LGPO (if C++, no need to go through the lpVtbl table)
    hr = pLGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "OpenLocalMachineGPO failed; hr = " << std::hex << hr;
        return false;
    }

    hr = pLGPO->GetRegistryKey(GPO_SECTION_USER, &user_key);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "get GPO_SECTION_USER failed; hr = " << std::hex << hr;
        return false;
    }

    if( RegOpenKeyEx( user_key,
        gpoPath.toStdWString().c_str(), 0,
        KEY_WRITE, &dsrkey) != ERROR_SUCCESS )
    {
        //LOG_DEBUG4 << "Cannot find reg key " << (gpoPath).toStdString() << " create now ";
        rStatus = RegCreateKeyEx(user_key, gpoPath.toStdWString().c_str(),
            0, NULL, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &dsrkey, NULL);
        if (rStatus != ERROR_SUCCESS)
        {
            LOG_ERROR << "can not create key, error: " << rStatus;
        }
    }
    // Create the value
    rStatus = RegSetValueEx(dsrkey, key.toStdWString().c_str(), 0, REG_DWORD, (BYTE*)&val, sizeof(val));
    if (rStatus != ERROR_SUCCESS)
    {
        LOG_ERROR << "can not set value, error: " << rStatus;
        return false;
    }

    RegCloseKey(dsrkey);
    //LOG_DEBUG4 << "Set reg key " << (policy + "\\" + key).toStdString() << " status: " << rStatus;

    // Apply policy and free resources
    GUID ext_guid = REGISTRY_EXTENSION_GUID;
    // This next one can be any GUID you want
    GUID snap_guid = { 0x3d271cfc, 0x2bc6, 0x4ac2, {0xb6, 0x33, 0x3b, 0xdf, 0xf5, 0xbd, 0xab, 0x2a} };
    hr = pLGPO->Save(FALSE, TRUE, &ext_guid, &snap_guid);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "failed to save LGPO: hr: " << std::hex << hr;
        return false;
    }
    RegCloseKey(user_key);
    pLGPO->Release();

    //LOG_DEBUG4 << "Done" << std::hex << hr;
    return true;
}

bool OSAgentWinQt::updateGroupPolicy(QString policy, QString key, QString val)
{
    enableDebugPrivileges();

    LOG_DEBUG4 << "Start changing group policy object " ;
    HRESULT hr;
    LSTATUS rStatus;
    IGroupPolicyObject* pLGPO;
    HKEY user_key, dsrkey;

    // Create an instance of the IGroupPolicyObject class
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoInitializeEx failed; hr = " << std::hex << hr;
        return false;
    }

    hr = CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_INPROC_SERVER,
        IID_IGroupPolicyObject, (LPVOID*)&pLGPO);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoCreateInstance failed; hr = " << std::hex << hr;
        return false;
    }

    // We need the machine LGPO (if C++, no need to go through the lpVtbl table)
    hr = pLGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "OpenLocalMachineGPO failed; hr = " << std::hex << hr;
        return false;
    }

    hr = pLGPO->GetRegistryKey(GPO_SECTION_USER, &user_key);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "get GPO_SECTION_USER failed; hr = " << std::hex << hr;
        return false;
    }

    if( RegOpenKeyEx( user_key,
        policy.toStdWString().c_str(), 0,
        KEY_WRITE, &dsrkey) != ERROR_SUCCESS )
    {
        //LOG_DEBUG4 << "Cannot find reg key " << (policy).toStdString() << " create now ";
        RegCreateKeyEx(user_key, policy.toStdWString().c_str(),
            0, NULL, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &dsrkey, NULL);
    }
    // Create the value
    rStatus = RegSetValueEx(dsrkey, key.toStdWString().c_str(), 0, REG_SZ, (BYTE*)val.toStdWString().c_str(), sizeof(TCHAR) * (val.length() + 1));
    if (rStatus != ERROR_SUCCESS)
    {
        LOG_ERROR << "can not set value, error: " << rStatus;
        return false;
    }

    RegCloseKey(dsrkey);

    // Apply policy and free resources
    GUID ext_guid = REGISTRY_EXTENSION_GUID;
    // This next one can be any GUID you want
    GUID snap_guid = { 0x3d271cfc, 0x2bc6, 0x4ac2, {0xb6, 0x33, 0x3b, 0xdf, 0xf5, 0xbd, 0xab, 0x2a} };
    hr = pLGPO->Save(FALSE, TRUE, &ext_guid, &snap_guid);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "failed to save LGPO: hr: " << std::hex << hr;
        return false;
    }

    RegCloseKey(user_key);
    pLGPO->Release();
    //LOG_DEBUG4 << "Done" << std::hex << hr;
    return true;
}

bool OSAgentWinQt::removeGroupPolicy(QString policy, QString key)
{
    enableDebugPrivileges();

    LOG_DEBUG4 << "Start removing group policy object value";
    HRESULT hr;
    LSTATUS rStatus;
    IGroupPolicyObject* pLGPO;
    HKEY user_key, dsrkey;

    // Create an instance of the IGroupPolicyObject class
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoInitializeEx failed; hr = " << std::hex << hr;
        return false;
    }

    hr = CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_INPROC_SERVER,
        IID_IGroupPolicyObject, (LPVOID*)&pLGPO);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoCreateInstance failed; hr = " << std::hex << hr;
        return false;
    }

    // We need the machine LGPO (if C++, no need to go through the lpVtbl table)
    hr = pLGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "OpenLocalMachineGPO failed; hr = " << std::hex << hr;
        return false;
    }

    hr = pLGPO->GetRegistryKey(GPO_SECTION_USER, &user_key);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "get GPO_SECTION_USER failed; hr = " << std::hex << hr;
        return false;
    }

    if( RegOpenKeyEx( user_key,
        policy.toStdWString().c_str(), 0,
        KEY_SET_VALUE, &dsrkey) == ERROR_SUCCESS )
    {
        //LOG_DEBUG4 << "Found reg key " << (policy).toStdString() << " delete now ";
        rStatus = RegDeleteValue(dsrkey, key.toStdWString().c_str());
        if (rStatus != ERROR_SUCCESS)
        {
            LOG_ERROR << "regdeletevalue failed: " << rStatus;
        }
        RegCloseKey(dsrkey);
    }
    else
    {
        //LOG_DEBUG4 << "Cannot find reg key " << (policy).toStdString();
    }
    
    // Apply policy and free resources
    GUID ext_guid = REGISTRY_EXTENSION_GUID;
    // This next one can be any GUID you want
    GUID snap_guid = { 0x3d271cfc, 0x2bc6, 0x4ac2, {0xb6, 0x33, 0x3b, 0xdf, 0xf5, 0xbd, 0xab, 0x2a} };
    hr = pLGPO->Save(FALSE, TRUE, &ext_guid, &snap_guid);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "failed to save LGPO: hr: " << std::hex << hr;
        return false;
    }

    RegCloseKey(user_key);
    pLGPO->Release();
    //LOG_DEBUG4 << "Done" << std::hex << hr;
    return true;
}

bool OSAgentWinQt::removeGroupPolicyKey(QString policy, QString key)
{
    enableDebugPrivileges();

    LOG_DEBUG4 << "Start removing group policy object key";
    DWORD val_size=sizeof(DWORD);
    HRESULT hr;
    LSTATUS rStatus;
    IGroupPolicyObject* pLGPO;
    HKEY user_key, dsrkey;

    // Create an instance of the IGroupPolicyObject class
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoInitializeEx failed; hr = " << std::hex << hr;
        return false;
    }

    hr = CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_INPROC_SERVER,
        IID_IGroupPolicyObject, (LPVOID*)&pLGPO);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "CoCreateInstance failed; hr = " << std::hex << hr;
        return false;
    }

    // We need the machine LGPO (if C++, no need to go through the lpVtbl table)
    hr = pLGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "OpenLocalMachineGPO failed; hr = " << std::hex << hr;
        return false;
    }

    hr = pLGPO->GetRegistryKey(GPO_SECTION_USER, &user_key);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "get GPO_SECTION_USER failed; hr = " << std::hex << hr;
        return false;
    }

    if( RegOpenKeyEx( user_key,
        policy.toStdWString().c_str(), 0,
        KEY_SET_VALUE, &dsrkey) == ERROR_SUCCESS )
    {
        //LOG_DEBUG4 << "Found reg key " << (policy).toStdString() << " delete subkey now ";
        rStatus = RegDeleteKey(dsrkey,
            key.toStdWString().c_str());
        if (rStatus != ERROR_SUCCESS)
        {
            LOG_ERROR << "regdeletevalue failed: " << rStatus;
        }
        RegCloseKey(dsrkey);
    }
    else
    {
        //LOG_DEBUG4 << "Cannot find reg key " << (policy).toStdString();
    }
    
    // Apply policy and free resources
    GUID ext_guid = REGISTRY_EXTENSION_GUID;
    // This next one can be any GUID you want
    GUID snap_guid = { 0x3d271cfc, 0x2bc6, 0x4ac2, {0xb6, 0x33, 0x3b, 0xdf, 0xf5, 0xbd, 0xab, 0x2a} };
    hr = pLGPO->Save(FALSE, TRUE, &ext_guid, &snap_guid);
    if ( FAILED(hr) )
    {
        LOG_ERROR << "failed to save LGPO: hr: " << std::hex << hr;
        return false;
    }

    RegCloseKey(user_key);
    pLGPO->Release();
    //LOG_DEBUG4 << "Done" << std::hex << hr;
    return true;
}

void OSAgentWinQt::refreshGroupPolicy()
{
    RefreshPolicy(FALSE);
}


// from windows 7 on wards, all items in control panel can be disabled when added to a disallow list.
bool OSAgentWinQt::disableControlPanelItemWindows7 ( const QStringList& listItems)
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableControlPanelItem For Windows 7";
        return true;
    }

    LOG_DEBUG4 << "action: disableControlPanelItem - Windows7";

    //QString keyHive2 = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
    //QSettings settings2( keyHive2, QSettings::NativeFormat );
    //settings2.setValue( QString("DisallowCpl"), 1);

    //// first remove the previous group if it has
    //if (settings2.childGroups().contains("DisallowCpl"))
    //{
    //    settings2.beginGroup("DisallowCpl");
    //    settings2.remove("");
    //    settings2.endGroup();
    //}
    //settings2.beginGroup("DisallowCpl");
    //for (int i = 0; i < listItems.size(); i++)
    //{
    //    QString value = listItems.at(i);
    //    settings2.setValue(QString::number(i + 1), value);
    //    LOG_DEBUG4 << "DisallowCpl " << i << " -> " << value.toStdString();
    //}
    //settings2.endGroup();
    updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "DisallowCpl", 1);
    removeGroupPolicyKey("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "DisallowCpl");
    for (int i = 0; i < listItems.size(); i++)
    {
        QString value = listItems.at(i);
        updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\DisallowCpl", QString::number(i + 1), value);
        //settings2.setValue(QString::number(i + 1), value);
        //LOG_DEBUG4 << "DisallowCpl " << i + 1 << " -> " << value.toStdString();
    }

    return true;
}

bool OSAgentWinQt::disableProgramsAndFeaturesWindows7 ()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableProgramsAndFeatures For Windows 7";
        return true;
    }

    LOG_DEBUG4 << "action: disableProgramsAndFeatures For Windows 7";

    /*QString keyHive2 = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Programs";
    QSettings settings( keyHive2, QSettings::NativeFormat );
    settings.setValue( QString("NoProgramsCPL"), 1);*/
    updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Programs", "NoProgramsCPL", 1);

    return true;
}

bool OSAgentWinQt::enableProgramsAndFeaturesWindows7 ()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableProgramsAndFeatures For Windows 7";
        return true;
    }

    LOG_DEBUG4 << "action: enableProgramsAndFeatures For Windows 7";

    /*QString keyHive2 = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Programs";
    QSettings settings( keyHive2, QSettings::NativeFormat );
    settings.remove( QString("NoProgramsCPL"));*/
    removeGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Programs", "NoProgramsCPL");

    return true;
}

bool OSAgentWinQt::disableCloseWindows7()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLogOff Windows 7";

        return true;
    }
    updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoClose", 1);
    return true;
}

bool OSAgentWinQt::enableCloseWindows7()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLogOff Windows 7";

        return true;
    }
    removeGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoClose");
    return true;
}

bool OSAgentWinQt::disableLogOffWindows7()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLogOff Windows 7";

        return true;
    }
    //L"Software\\Policies\\Microsoft\\Windows\\Explorer"
    //L"PowerButtonAction"
    updateGroupPolicy("Software\\Policies\\Microsoft\\Windows\\Explorer", "PowerButtonAction", 512);
    updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoLogOff", 1);
    updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "StartMenuLogOff", 1);
    return true;
}

bool OSAgentWinQt::enableLogOffWindows7()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: enableLogOff Windows 7";

        return true;
    }
    //L"Software\\Policies\\Microsoft\\Windows\\Explorer"
    //L"PowerButtonAction"
    removeGroupPolicy("Software\\Policies\\Microsoft\\Windows\\Explorer", "PowerButtonAction");
    removeGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoLogOff");
    removeGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "StartMenuLogOff");
    return true;
}

bool OSAgentWinQt::disableLockWindows7()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLock";

        return true;
    }

    updateGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableLockWorkstation", 1);
    return true;
}

bool OSAgentWinQt::enableLockWindows7()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: disableLock";

        return true;
    }

    removeGroupPolicy("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableLockWorkstation");
    return true;
}


#include "SecurityProfileManager.h"
#include "Client.h"
#include "SecuritySettings.h"
#include <QStringList>
#include <QSysInfo>

#include "OSAgentWinQt.h"
#include "UserAccountManager.h"
#include "SecurityProfile.h"
#include "CommonSettings.h"
#include "log.h"
#include "ConfigManager.h"
#include "AppControlManager.h"
#include "cc2csNetworkManager.h"
#include "SecurityProfileQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"


const int DEFAULT_PROFILE_ID = 1;

bool SecurityProfileManager::init()
{
    QString securityItems = CConfigManager::securityProfile();
    m_profile = new SecurityProfile(/*profileId*/DEFAULT_PROFILE_ID, securityItems);
    return true;
}

bool SecurityProfileManager::startup()
{
    bool ret = applySecurityProfile(m_profile);
    return ret;
}

bool SecurityProfileManager::connectSigSlot()
{
    {
        UserAccountManager* userAccMgr = UserAccountManager::GetInstance();
        // user logged in, fetch security settings from server
        connect(userAccMgr, SIGNAL(accLoggedIn()), 
            this, SLOT(accLoggedInSlot()));

        // user clicks "Logout" button from client,
        // 1. if the user is LocalAdmin, need to re-apply the default profile and restart explorer;
        // 2. if there's pending "explorer restart", restart it
        connect(userAccMgr, SIGNAL(accLoggedOut(int, int)), 
            this, SLOT(accLoggedOutSlot(int, int)));
    }

    {
        // after server online, send command to fetch security profile from server
        cc2csNetworkManager* networkMgr = cc2csNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    return true;
}

bool SecurityProfileManager::enableAllRegistrySettings()
{
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    //
    // general security items
    //
    LOG_DEBUG4 << "enable cdRom auto-run";
    agent->enableCDRomAutorun();
    
    //
    // the control panel is done from different registry location, so separate them out
    //
    enableAllControlPanelItems();

    enableAllPowerManagementItems();

    return true;
}

bool SecurityProfileManager::applySecurityProfile( SecurityProfile* profile)
{
    LOG_DEBUG4 << "Apply Security profile";
    // don't do any restrictions to local admin account
    UserGroupType accType = UserAccountManager::GetInstance()->currentUserType();
    if (accType == UserGroup_LocalSuperAdmin)
    {
        LOG_DEBUG4 << "Account is local admin, don't restrict";
        return true;
    }

    OSAgentWin* agent = OSAgentWinQt::GetInstance();
    if (agent == NULL)
    {
        return false;
    }

    //agent->disableProcHost();
    //
    // general security items
    //
    if (profile == NULL || profile->isGS_AllowCDRomAutorun() == false)
    {
        agent->disableCDRomAutorun();
    }
    else
    {
        agent->enableCDRomAutorun();
    }

    if (profile == NULL || profile->isGS_AllowTaskManager() == false)
    {
        agent->disableSystemTaskManager(); // by regedit
    }
    else
    {
        agent->enableSystemTaskManager();
    }

    //
    // the control panel is done from different registry location, so separate them out
    //
    applyControlPanelSecurity(profile);

    if (profile == NULL || profile->isGS_AllowRegistry() == false)
    {
        agent->disableRegedit();  // by prochook
    }
    else
    {
        agent->enableRegedit();
    }

    applyPowerManagementSecurity(profile);

    //agent->enableProcHost();
    return true;
}

// "access":    "Accessibility Options"
// "appwiz":    "add or remove program"
// "desk"  :    "display properties"
// "inetcpl":   "internet properties"
// "main"  :    "mouse"
// "mmsys" :    "sound"
bool SecurityProfileManager::applyControlPanelSecurity(SecurityProfile* profile)
{
    QSysInfo::WinVersion wv = QSysInfo::WindowsVersion;
    LOG_DEBUG << "Windows Version: " << wv << " is >= Windows 7? " << (wv >= QSysInfo::WV_WINDOWS7);
    if (wv < QSysInfo::WV_WINDOWS7)
    {
        LOG_DEBUG << "Apply general control panel security";
        return applyControlPanelSecurityGeneral(profile);
    } else {
        LOG_DEBUG << "Apply windows 7 control panel security";
        return applyControlPanelSecurityWindows7(profile);
    }
}

bool SecurityProfileManager::applyControlPanelSecurityGeneral(SecurityProfile* profile)
{
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    QList<std::wstring> itemsToDisable;
    QList<std::wstring> itemsToEnable;

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: applyControlPanelSecurity";
        return true;
    }

    LOG_DEBUG << "Begin to apply control panel security settings";

    //if (wv <= QSysInfo::WV_2003)
    {
        if (profile == NULL || profile->isCP_AllowUserAccount() == false)
        {
            itemsToDisable.append(L"nusrmgr.cpl"); // "user accounts" control panel item
        }
        else
        {
            itemsToEnable.append(L"nusrmgr.cpl");
        }

        if (profile == NULL || profile->isCP_AllowAddOrRemoveProgram() == false)
        {
            itemsToDisable.append(L"appwiz.cpl"); // "add or remove programs" control panel item
        }
        else
        {
            itemsToEnable.append(L"appwiz.cpl");
        }

        if (profile == NULL || profile->isCP_AllowDateAndTime() == false)
        {
            itemsToDisable.append(L"timedate.cpl"); // "Date and Time" control panel item
        }
        else
        {
            itemsToEnable.append(L"timedate.cpl");
        }

        //if (profile == NULL || profile->isCP_AllowNetworkConnections() == false)
        //{
        //    itemsToDisable.append(L"ncpa.cpl"); // "network connections" control panel item
        //}

        if (profile == NULL || profile->isCP_AllowRegionalSetting() == false)
        {
            itemsToDisable.append(L"intl.cpl"); // "regional and language settings" control panel item
        }
        else
        {
            itemsToEnable.append(L"intl.cpl");
        }

        if (profile == NULL || profile->isCP_AllowSystem() == false)
        {
            itemsToDisable.append(L"sysdm.cpl"); // "system" control panel item
        }
        else
        {
            itemsToEnable.append(L"sysdm.cpl");
        }
    }

    foreach(const std::wstring& item, itemsToDisable)
    {
        agent->disableControlPanelItem(item);
    }

    foreach(const std::wstring& item, itemsToEnable)
    {
        agent->enableControlPanelItem(item);
    }

    if (profile == NULL || profile->isCP_AllowNetworkConnections() == false)
    {
        agent->disableNetworkConnections();
    }
    else
    {
        agent->enableNetworkConnections();
    }

    // this is done from a different registry key, so need to separate from the above
    if (profile == NULL || profile->isCP_AllowAdministrativeTools() == false)
    {
        agent->disableAdministrativeTools();
    }
    else
    {
        agent->enableAdministrativeTools();
    }

    return true;
}

bool SecurityProfileManager::applyControlPanelSecurityWindows7(SecurityProfile* profile)
{
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    QStringList itemsToDisable;

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: applyControlPanelSecurity Windows7";
        return true;
    }

    LOG_DEBUG << "Begin to apply control panel security settings - Windows7";
    
    // for windows 7, we only use blacklist-style to disable items
    // any item not in this list will be enabled
    {
        // User account
        if (profile == NULL || profile->isCP_AllowUserAccount() == false)
        {
            itemsToDisable.append(QString("Microsoft.UserAccounts"));
        }

        if (profile == NULL || profile->isCP_AllowAdministrativeTools() == false)
        {
            itemsToDisable.append(QString("Microsoft.AdministrativeTools"));
        }

         // "Date and Time" control panel item
        if (profile == NULL || profile->isCP_AllowDateAndTime() == false)
        {
            itemsToDisable.append(QString("Microsoft.DateAndTime"));
        }

        // Regional Setting
        if (profile == NULL || profile->isCP_AllowRegionalSetting() == false)
        {
            itemsToDisable.append(QString("Microsoft.RegionAndLanguage"));
        }

        if (profile == NULL || profile->isCP_AllowSystem() == false)
        {
            itemsToDisable.append(QString("Microsoft.System"));
        }

        if (profile == NULL || profile->isCP_AllowAddOrRemoveProgram() == false)
        {
            itemsToDisable.append(QString("Microsoft.ProgramsAndFeatures"));
            agent->disableProgramsAndFeaturesWindows7();
        } else {
            agent->enableProgramsAndFeaturesWindows7();
        }

        if (profile == NULL || profile->isCP_AllowNetworkConnections() == false)
        {
            itemsToDisable.append(QString("Microsoft.NetworkAndSharingCenter"));
        }   
    }

    agent->disableControlPanelItemWindows7(itemsToDisable);

    return true;
}

bool SecurityProfileManager::enableAllControlPanelItems()
{
    QSysInfo::WinVersion wv = QSysInfo::WindowsVersion;
    LOG_DEBUG << "Windows Version: " << wv << " is >= Windows 7? " << (wv >= QSysInfo::WV_WINDOWS7);
    if (wv < QSysInfo::WV_WINDOWS7)
    {
        LOG_DEBUG << "Enable all control panel security general";
        return enableAllControlPanelItemsGeneral();
    } else {
        LOG_DEBUG << "Enable all control panel security windows 7";
        return enableAllControlPanelItemsWindows7();
    }
}

bool SecurityProfileManager::enableAllControlPanelItemsGeneral()
{
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    QList<std::wstring> itemsToEnable;
    bool isWinXP = true;
    bool isWin7 = true;
    bool isVista = true;

    if (isWinXP)
    {
        itemsToEnable.append(L"nusrmgr.cpl"); // "user accounts" control panel item
        itemsToEnable.append(L"appwiz.cpl"); // "add or remove programs" control panel item
        itemsToEnable.append(L"timedate.cpl"); // "Date and Time" control panel item
        //itemsToEnable.append(L"ncpa.cpl"); // "network connections" control panel item
        itemsToEnable.append(L"intl.cpl"); // "regional and language settings" control panel item
        itemsToEnable.append(L"sysdm.cpl"); // "system" control panel item
    }

    foreach(const std::wstring& item, itemsToEnable)
    {
        agent->enableControlPanelItem(item);
    }

    agent->enableNetworkConnections();
    agent->enableAdministrativeTools();
    return true;
}

bool SecurityProfileManager::enableAllControlPanelItemsWindows7()
{
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    // for windows 7, we only use blacklist-style to disable items
    // any item not in this list will be enabled
    agent->disableControlPanelItemWindows7(QStringList());
    agent->enableProgramsAndFeaturesWindows7();
    return true;
}

bool SecurityProfileManager::applyPowerManagementSecurity(SecurityProfile* profile)
{
    QSysInfo::WinVersion wv = QSysInfo::WindowsVersion;
    LOG_DEBUG << "Windows Version: " << wv << " is >= Windows 7? " << (wv >= QSysInfo::WV_WINDOWS7);
    if (wv < QSysInfo::WV_WINDOWS7)
    {
        LOG_DEBUG << "Apply power management security general";
        return applyPowerManagementSecurityGeneral(profile);
    } else {
        LOG_DEBUG << "Apply power management security windows 7";
        return applyPowerManagementSecurityWindows7(profile);
    }
}

bool SecurityProfileManager::applyPowerManagementSecurityGeneral(SecurityProfile* profile)
{
	if (CConfigManager::isDisableSecurityRestrictions() == true)
	{
		LOG_DEBUG4 << "simulated action: applyPowerManagementSecurity";
		return true;
	}

    LOG_DEBUG << "Begin to apply power management security settings";
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

	if (profile == NULL || profile->isPM_AllowShutdownRestart() == false)
    {
		agent->disableClose();
	}
	else
	{
		agent->enableClose();
	}

	if (profile == NULL || profile->isPM_AllowLock() == false)
    {
		agent->disableLock();
	}
	else
	{
		agent->enableLock();
	}

	if (profile == NULL || profile->isPM_AllowSwitchUser() == false)
    {
		agent->disableSwitchUser();
	}
	else
	{
		agent->enableSwitchUser();
	}

    if (profile == NULL || profile->isPM_AllowLogoff() == false)
    {
        agent->disableLogOff();
    }
    else
    {
        agent->enableLogOff();
    }

	agent->refreshExplorer();

	return true;
}

bool SecurityProfileManager::applyPowerManagementSecurityWindows7(SecurityProfile* profile)
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "simulated action: applyPowerManagementSecurity";
        return true;
    }

    LOG_DEBUG << "Begin to apply power management security settings";
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    if (profile == NULL || profile->isPM_AllowLogoff() == false)
    {
        agent->disableLogOffWindows7();
    }
    else
    {
        agent->enableLogOffWindows7();
    }

    if (profile == NULL || profile->isPM_AllowShutdownRestart() == false)
    {
        agent->disableCloseWindows7();
    }
    else
    {
        agent->enableCloseWindows7();
    }

    if (profile == NULL || profile->isPM_AllowLock() == false)
    {
        agent->disableLockWindows7();
    }
    else
    {
        agent->enableLockWindows7();
    }

    if (profile == NULL || profile->isPM_AllowSwitchUser() == false)
    {
        agent->disableSwitchUser();
    }
    else
    {
        agent->enableSwitchUser();
    }

    agent->refreshExplorer();

    agent->refreshGroupPolicy();

    return true;
}

bool SecurityProfileManager::enableAllPowerManagementItems()
{
    QSysInfo::WinVersion wv = QSysInfo::WindowsVersion;
    LOG_DEBUG << "Windows Version: " << wv << " is >= Windows 7? " << (wv >= QSysInfo::WV_WINDOWS7);
    if (wv < QSysInfo::WV_WINDOWS7)
    {
        LOG_DEBUG << "Enable all power management security general";
        return enableAllPowerManagementItemsGeneral();
    } else {
        LOG_DEBUG << "Enable all power management security Windows 7";
        return enableAllPowerManagementItemsWindows7();
    }
}

bool SecurityProfileManager::enableAllPowerManagementItemsGeneral()
{
	OSAgentWin* agent = OSAgentWinQt::GetInstance();

	agent->enableClose();
    agent->enableLogOff();
	agent->enableLock();
	agent->enableSwitchUser();
	agent->refreshExplorer();

	return true;
}

bool SecurityProfileManager::enableAllPowerManagementItemsWindows7()
{
    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    agent->enableLogOffWindows7();
    agent->enableCloseWindows7();
    agent->enableLockWindows7();
    agent->enableSwitchUser();
    agent->refreshExplorer();
    agent->refreshGroupPolicy();

    return true;
}

void SecurityProfileManager::accLoggedOutSlot(int userId, int accTypeVal)
{
    if (accTypeVal == (int)UserGroup_LocalSuperAdmin)
    {
        // disable process security for user
        LOG_DEBUG4 << "Admin logged out";
        LOG_DEBUG4 << "re-apply default process protection setting";
        OSAgentWinQt::GetInstance()->applyDefaultProcSecurity();

        LOG_DEBUG4 << "re-Apply all security settings";
        applySecurityProfile(m_profile);

        LOG_DEBUG4 << "re-Apply all application prohibitions";
        AppControlManager::GetInstance()->applyAllApplicationControls();
    }
}

void SecurityProfileManager::accLoggedInSlot()
{
    UserGroupType accType = UserAccountManager::GetInstance()->currentUserType();
    if (accType == UserGroup_LocalSuperAdmin)
    {
        LOG_DEBUG4 << "Admin logged in. Enable all security settings";
        enableAllRegistrySettings();

        // enable process security for admin
        OSAgentWinQt::GetInstance()->enableAllProcSecurity();
    }
}

void SecurityProfileManager::serverConnectedSlot()
{
    // magic number, since there's only ONE profile
    SecuProfileRequestQtModel qtModel(CConfigManager::clientId(), 1, UserGroup_Member);
    cc2csSender::send(cc2cs::SecuProfileRequestCommandC(qtModel));
}

void SecurityProfileManager::procSecuProfileReply( const SecuProfileReplyQtModel &qtModel )
{
    int profileId = qtModel.ProfileId_;
    QString securitySettings = qtModel.SecuritySettings_;
    if (m_profile == NULL)
    {
        LOG_ERROR << "security profile is NULL, shouldn't happen";
        m_profile = new SecurityProfile(/*profileId*/DEFAULT_PROFILE_ID, CConfigManager::securityProfile());
    }
    else
    {
        QString itemCache = m_profile->securitySettings();
        // no change for security profile
        if (QString::compare(itemCache, securitySettings, Qt::CaseInsensitive) == 0)
        {
            return;
        }
    }

    // there is some change
    m_profile->setSecuritySettings(securitySettings);
    CConfigManager::setSecurityProfile(securitySettings);
    LOG_DEBUG4 << "Security profile changed: " << qPrintable(securitySettings);

    applySecurityProfile(m_profile);
}

void SecurityProfileManager::clientEndedConnectionSlot()
{
    LOG_DEBUG4 << "Admin exit client app. Enable all security settings";
    enableAllRegistrySettings();

    // enable process security for admin
    OSAgentWinQt::GetInstance()->enableAllProcSecurity();
}

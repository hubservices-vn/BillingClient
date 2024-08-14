#include "SecurityProfile.h"
#include "SecuritySettings.h"
#include <QStringList>


SecurityProfile::SecurityProfile( int id, const QString& securitySettings, bool enabled) 
                                 : m_id(id), m_securitySettings(securitySettings), m_enabled(true)
{
    updateSecurityItems();
}

void SecurityProfile::updateSecurityItems()
{
    m_securityItems.clear();
    QStringList list = m_securitySettings.split('|', QString::SkipEmptyParts, Qt::CaseInsensitive);
    int size = list.size();
    for (int i = 0; i < size; ++i)
    {
        m_securityItems.append(list.at(i).toInt());
    }

    qSort(m_securityItems);
}

bool SecurityProfile::isGS_AllowCDRomAutorun()
{
    return isSecurityItemAllow(GS_AllowCDRomAutorun);
}

bool SecurityProfile::isGS_AllowTaskManager()
{
    return isSecurityItemAllow(GS_AllowTaskManager);
}
//
//bool SecurityProfile::isGS_AllowCAD()
//{
//    return isSecurityItemAllow(GS_AllowCAD);
//}

bool SecurityProfile::isGS_AllowRegistry()
{
    return isSecurityItemAllow(GS_AllowRegistry);
}
//
//bool SecurityProfile::isSM_AllowRunMenu()
//{
//    return isSecurityItemAllow(SM_AllowRunMenu);
//}
//
//bool SecurityProfile::isSM_AllowControlPanel()
//{
//    return isSecurityItemAllow(SM_AllowControlPanel);
//}

bool SecurityProfile::isCP_AllowAddOrRemoveProgram()
{
    return isSecurityItemAllow(CP_AllowAddOrRemoveProgram);
}

bool SecurityProfile::isCP_AllowAdministrativeTools()
{
    return isSecurityItemAllow(CP_AllowAdministrativeTools);
}

bool SecurityProfile::isCP_AllowDateAndTime()
{
    return isSecurityItemAllow(CP_AllowDateAndTime);
}

bool SecurityProfile::isCP_AllowNetworkConnections()
{
    return isSecurityItemAllow(CP_AllowNetworkConnections);
}

bool SecurityProfile::isCP_AllowRegionalSetting()
{
    return isSecurityItemAllow(CP_AllowRegionalSetting);
}

bool SecurityProfile::isCP_AllowSystem()
{
    return isSecurityItemAllow(CP_AllowSystem);
}

bool SecurityProfile::isCP_AllowUserAccount()
{
    return isSecurityItemAllow(CP_AllowUserAccount);
}

bool SecurityProfile::isPM_AllowShutdownRestart()
{
	return isSecurityItemAllow(PM_AllowShutdownRestart);
}

bool SecurityProfile::isPM_AllowLock()
{
	return isSecurityItemAllow(PM_AllowLock);
}

bool SecurityProfile::isPM_AllowSwitchUser()
{
	return isSecurityItemAllow(PM_AllowSwitchUser);
}

bool SecurityProfile::isPM_AllowLogoff()
{
    return isSecurityItemAllow(PM_AllowLogoff);
}

bool SecurityProfile::isSecurityItemAllow( SecuritySettings item )
{
    int value = (int)item;

    int size = m_securityItems.size();
    for (int i = 0; i < size; ++i)
    {
        if (value == m_securityItems.at(i))
        {
            return true;
        }
    }

    return false;
}

void SecurityProfile::setSecuritySettings( const QString& val )
{
    m_securitySettings = val; 
    updateSecurityItems();
}

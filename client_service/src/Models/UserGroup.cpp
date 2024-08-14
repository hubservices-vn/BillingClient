#include "UserGroup.h"
#include "CommonSettings.h"

UserGroup::UserGroup( int id, const QString &name, /*int priceStructureID,*/ int applicationProfileID, 
                     int securityProfileID, int warnThreshhold, WarnType warnType, int timeoutAction, bool enabled ) 
                     : m_id(id), m_name(name), /*m_priceStructureID(priceStructureID),*/ 
                     m_applicationProfileID(applicationProfileID), m_securityProfileID(securityProfileID),
                     m_warnThreshhold(warnThreshhold), m_warnType(warnType), 
                     m_timeoutAction(timeoutAction), m_enabled(enabled)
{
}

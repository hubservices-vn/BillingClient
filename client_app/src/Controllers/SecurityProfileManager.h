#ifndef SECURITYPROFILEMANAGER_H
#define SECURITYPROFILEMANAGER_H


#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>


class SecurityProfile;
class SecuProfileReplyQtModel;
class SecurityProfileManager : public QObject, public Singleton_T<SecurityProfileManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    bool applySecurityProfile(SecurityProfile* profile);

    bool enableAllRegistrySettings();

    bool startup();

public:
    void procSecuProfileReply( const SecuProfileReplyQtModel &qtModel );

signals:

public slots:
    // after server online, send command to fetch security profile from server
    void serverConnectedSlot();

    // user logged in, fetch security settings from server
    void accLoggedInSlot();

    // user clicks "Logout" button from client,
    // clear the security profile data and disable all security
    void accLoggedOutSlot(int userId, int accTypeVal);

    // re-enable all security settings
    void clientEndedConnectionSlot();
private:
    bool applyControlPanelSecurity(SecurityProfile* profile);
    bool enableAllControlPanelItems();
	bool applyPowerManagementSecurity(SecurityProfile* profile);
	bool enableAllPowerManagementItems();

    bool applyControlPanelSecurityGeneral(SecurityProfile* profile);
    bool applyControlPanelSecurityWindows7(SecurityProfile* profile);
    bool enableAllControlPanelItemsGeneral();
    bool enableAllControlPanelItemsWindows7();
    bool applyPowerManagementSecurityGeneral(SecurityProfile* profile);
    bool applyPowerManagementSecurityWindows7(SecurityProfile* profile);
    bool enableAllPowerManagementItemsGeneral();
    bool enableAllPowerManagementItemsWindows7();
private:
    SecurityProfile* m_profile;
};

#endif // SECURITYPROFILEMANAGER_H

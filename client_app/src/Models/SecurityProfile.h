#ifndef SECURITYPROFILE_H
#define SECURITYPROFILE_H


#include <QObject>

enum SecuritySettings;
class SecurityProfile : public QObject
{
    Q_OBJECT
public:
    explicit SecurityProfile(int id, const QString& securitySettings, bool enabled = true);

    // security settings
    // GS = general security
    bool isGS_AllowCDRomAutorun();
    bool isGS_AllowTaskManager();
    //bool isGS_AllowCAD();
    bool isGS_AllowRegistry();
    // SM = start menu
    //bool isSM_AllowRunMenu();
    //bool isSM_AllowControlPanel();
    // CP = control panel
    bool isCP_AllowAddOrRemoveProgram();
    bool isCP_AllowAdministrativeTools();
    bool isCP_AllowDateAndTime();
    bool isCP_AllowNetworkConnections();
    bool isCP_AllowRegionalSetting();
    bool isCP_AllowSystem();
    bool isCP_AllowUserAccount();

	// PM = power management
	bool isPM_AllowShutdownRestart();
	bool isPM_AllowLock();
	bool isPM_AllowSwitchUser();
    bool isPM_AllowLogoff();

public: // accessors
    int id() const { return m_id; }
    void setId(int val) { m_id = val; }

    QString securitySettings() const { return m_securitySettings; }
    void setSecuritySettings(const QString& val);

    bool enabled() const { return m_enabled; }
    void setEnabled(bool val) { m_enabled = val; }

private:

    bool isSecurityItemAllow(SecuritySettings item);

    // general
    void updateSecurityItems();

private:
    int m_id;

    QString m_securitySettings; // format of 15;25;33;34;1;2;3;4;6;7;8;13;14; the numbers are defined in the securitySettings.h

    QList<int> m_securityItems;

    bool m_enabled;
};

#endif // SECURITYPROFILE_H

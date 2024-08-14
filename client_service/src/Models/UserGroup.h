#ifndef USERGROUP_H
#define USERGROUP_H

#include <QObject>

enum WarnType;
class UserGroup : public QObject
{
    Q_OBJECT
public:
    explicit UserGroup(int id, const QString& name, /*int priceStructureID,*/ int applicationProfileID, 
        int securityProfileID, int warnThreshhold, WarnType warnType, int timeoutAction, bool enabled);

public: // accessors
    int id() const { return m_id; }
    void setId(int val) { m_id = val; }

    QString nName() const { return m_name; }
    void setName(const QString& val) { m_name = val; }

    /*int priceStructureID() const { return m_priceStructureID; }
    void setPriceStructureID(int val) { m_priceStructureID = val; }*/

    int applicationProfileID() const { return m_applicationProfileID; }
    void setApplicationProfileID(int val) { m_applicationProfileID = val; }

    int securityProfileID() const { return m_securityProfileID; }
    void setSecurityProfileID(int val) { m_securityProfileID = val; }

    bool enabled() const { return m_enabled; }
    void setEnabled(bool val) { m_enabled = val; }

    int warnThreshhold() const { return m_warnThreshhold; }
    void setWarnThreshhold(int val) { m_warnThreshhold = val; }

    WarnType warnType() const { return m_warnType; }
    void setWarnType(WarnType val) { m_warnType = val; }

    int timeoutAction() const { return m_timeoutAction; }
    void setTimeoutAction(int val) { m_timeoutAction = val; }

private:
    int m_id;
    QString m_name;
    //int m_priceStructureID;
    int m_applicationProfileID;
    int m_securityProfileID;
    bool m_enabled;
    int m_warnThreshhold;
    WarnType m_warnType; 
    int m_timeoutAction; 
};

#endif // USERGROUP_H

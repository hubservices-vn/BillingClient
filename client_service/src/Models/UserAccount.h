#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <QObject>

enum PwdHashType;
enum UserGroupType;
class UserAccount
{
public: // methods
    explicit UserAccount(int id, const QString &name, UserGroupType accType);

public: // accessors
    int Id() const { return m_id; }
    void SetId(int val) { m_id = val; }

    QString UserName() const { return m_userName; }
    void SetUserName(const QString& val) { m_userName = val; }

    UserGroupType accType() const { return m_accType; }
    void setAccType(UserGroupType val) { m_accType = val; }
    double calculateTopupBonus( double& totalTopupAmountForBonus );
private:
    int m_id;
    QString m_userName;
    UserGroupType m_accType;
};

#endif // USERACCOUNT_H

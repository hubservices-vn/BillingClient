#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <QObject>

enum PwdHashType;
enum UserGroupType;
class UserAccount
{
public: // methods
    explicit UserAccount(int id, const QString &name, UserGroupType accType, 
        double realMoney, double freeMoney, double debt, int pointBalance);

public: // accessors
    int Id() const { return m_id; }
    void SetId(int val) { m_id = val; }

    QString UserName() const { return m_userName; }
    void SetUserName(const QString& val) { m_userName = val; }

    UserGroupType accType() const { return m_accType; }
    void setAccType(UserGroupType val) { m_accType = val; }

    int PointBalance() const { return m_pointBalance; }
    void SetPointBalance(int val) { m_pointBalance = val; }

    double Balance() const { 
        if (m_freeMoney >= 0 && m_realMoney >= 0)
        {
            return m_freeMoney + m_realMoney; 
        } else {
            return 0;
        }
    }

    double RealMoney() const {return m_realMoney;}
    void setRealMoney(double val) { m_realMoney = val;}

    double FreeMoney() const {return m_freeMoney;}
    void setFreeMoney(double val) { m_freeMoney = val;}

    double Debt() const {return m_debt;}
    void setDebt(double val) { m_debt = val;}
private:
    int m_id;
    QString m_userName;
    UserGroupType m_accType;
    double m_realMoney;
    double m_freeMoney;
    double m_debt;
    int m_pointBalance;
};

#endif // USERACCOUNT_H

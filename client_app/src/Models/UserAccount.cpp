#include "UserAccount.h"
#include "Utils.h"
#include "CommonSettings.h"

UserAccount::UserAccount( int id, const QString& name, UserGroupType accType, double realMoney, double freeMoney, double debt, int pointBalance)
: m_id(id), m_userName(name), m_accType(accType), m_realMoney(realMoney), m_freeMoney(freeMoney), m_debt(debt), m_pointBalance(pointBalance)
{
}

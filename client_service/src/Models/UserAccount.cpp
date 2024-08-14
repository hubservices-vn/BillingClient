#include "UserAccount.h"
#include "Utils.h"
#include "CommonSettings.h"

UserAccount::UserAccount( int id, const QString& name, UserGroupType accType)
: m_id(id), m_userName(name), m_accType(accType)
{
}

double UserAccount::calculateTopupBonus( double& totalTopupAmountForBonus )
{
    throw std::exception("The method or operation is not implemented.");
}

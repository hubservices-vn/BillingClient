#include "UserAccountManager.h"
#include "Client.h"
#include "CommonSettings.h"
#include "UserAccount.h"
#include "log.h"
#include "UserLoginQtModel.h"
#include "UserLogoutQtModel.h"
#include "UserUpdateQtModel.h"
#include "ChangeAccPwdQtModel.h"
#include "TopupByCouponQtModel.h"
#include <QApplication>
#include <QMessageBox>

UserAccount* UserAccountManager::m_userAccount = NULL;

bool UserAccountManager::init()
{
    m_userAccount = NULL;
    qRegisterMetaType<TopupByCouponReplyQtModel>("TopupByCouponReplyQtModel");
    return true;
}

bool UserAccountManager::connectSigSlot()
{
    return true;
}

int UserAccountManager::currentUserId()
{
    if (m_userAccount == NULL)
    {
        return -1;
    }
    else
    {
        return m_userAccount->Id();
    }
}

QString UserAccountManager::currentUserName()
{
    if (m_userAccount == NULL)
    {
        return QString();
    }

    return m_userAccount->UserName();
}

double UserAccountManager::currentUserBalance()
{
    if (m_userAccount == NULL)
    {
        return 0;
    }

    return m_userAccount->Balance();
}

double UserAccountManager::currentUserDebt()
{
    if (m_userAccount == NULL)
    {
        return 0;
    }

    return m_userAccount->Debt();
}

int UserAccountManager::currentUserPointBalance()
{
    if (m_userAccount == NULL)
    {
        return 0;
    }

    return m_userAccount->PointBalance();
}

UserGroupType UserAccountManager::currentUserType()
{
    if (m_userAccount == NULL)
    {
        return UserGroup_NA;
    }

    return m_userAccount->accType();
}

int UserAccountManager::currentUserTypeVal()
{
    if (m_userAccount == NULL)
    {
        return (int)(UserGroup_NA);
    }

    return (int)(m_userAccount->accType());
}

bool UserAccountManager::isUserLoggedIn()
{
    return m_userAccount != NULL;
}

void UserAccountManager::procLoginReply( const UserLoginReplyQtModel &qtModel )
{
    if (m_userAccount != NULL)
    {
        delete m_userAccount;
        m_userAccount = NULL;
    }

    // store the user account
    if (qtModel.Result_ == LoginResult_OK)
    {
        m_userAccount = new UserAccount(
            qtModel.AccId_, qtModel.AccName_, qtModel.AccType_, qtModel.RealMoney_, qtModel.FreeMoney_, qtModel.Debt_, qtModel.PointBalance_);

        emit accLoggedIn();
    }

    // notify other UI components that login reply has arrived
    emit loginReplyRecved(qtModel.Result_, qtModel.AccType_);
}

void UserAccountManager::procLogoutReply( const UserLogoutReplyQtModel &qtModel )
{
    int userId = -1;
    UserGroupType accType = UserGroup_NA;
    if (m_userAccount != NULL)
    {
        userId = m_userAccount->Id();
        accType = m_userAccount->accType();

        delete m_userAccount;
        m_userAccount = NULL;
    }

    emit accLoggedOut(userId, accType);
}

void UserAccountManager::procUserUpdate( const UserUpdateQtModel &qtModel )
{
    if (m_userAccount != NULL)
    {
        if (qtModel.UserId == m_userAccount->Id())
        {
            m_userAccount->SetUserName(qtModel.UserName);
            m_userAccount->setRealMoney(qtModel.Money);
            m_userAccount->setFreeMoney(qtModel.FreeMoney);
            m_userAccount->setDebt(qtModel.Debt);
            m_userAccount->SetPointBalance(qtModel.PointBalance);
            emit userAccUpdated();
        }
    }
}

// forward to client UI
void UserAccountManager::procChangePwdReply( const ChangeAccPwdReplyQtModel &qtModel )
{
    emit changeAccPwdReplyRecved(qtModel.Result_);
}

void UserAccountManager::procTopupByCouponReply( const TopupByCouponReplyQtModel &qtModel )
{
    emit topupByCouponReplyRecved(qtModel.Result);
}
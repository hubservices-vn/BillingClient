#include "UserAccountManager.h"
#include "Client.h"
#include "SecurityProfileManager.h"
#include "CommonSettings.h"
#include "UserAccount.h"
#include "Utils.h"
#include "ConfigManager.h"
#include "log.h"
#include "DateTimeUtils.h"
#include "cs2ssNetworkManager.h"
#include "UserLoginQtModel.h"
#include "cs2ssSender.h"
#include "cs2ccSender.h"
#include "UserLogoutQtModel.h"
#include "SessionBeginQtModel.h"
#include "cs2ssCmds.h"
#include "ss2csCmds.h"
#include "SessionManager.h"
#include "ChangeAccPwdQtModel.h"
#include "cs2ccCmds.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"
#include "TopupByCouponQtModel.h"

UserAccount* UserAccountManager::m_userAccount = NULL;

bool UserAccountManager::init()
{
    m_userAccount = NULL;
    return true;
}

bool UserAccountManager::connectSigSlot()
{
    {

    }

    // user ends the session, clear account
    {
        SessionManager* sessionMgr = SessionManager::GetInstance();
        connect(sessionMgr, SIGNAL(sessionEnded()),
            this, SLOT(sessionEndedSlot()));
    }

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

void UserAccountManager::procLoginRequest( UserLoginRequestQtModel &qtModel )
{
    if (cs2ssNetworkManager::GetInstance()->isServerConnected())
    {
        // normal user login, send to server for auth
        qtModel.ClientId = CConfigManager::clientId();
        qtModel.AdminCode = CConfigManager::adminCodeHash();
        cs2ssSender::send(cs2ss::UserLoginCommand(qtModel));
    }
    // only do local auth when client is offline
    else
    {
        // if user is "local admin account", auth locally and unlock client directly
        if (isLocalAdminAccount(qtModel.UserName, qtModel.RawPwd))
        {
            _loginLocalAdmin();
        }
        // user tries to login offline, but the account is NOT admin account, deny it
        else
        {
            _loginReplyImp(/*userId*/-1, /*userName*/"", UserGroup_NA, LoginResult_NoUser, 0, 0, 0, 0);
        }
    }
}

void UserAccountManager::procLoginReply( UserLoginReplyQtModel &model )
{
    _loginReplyImp(model.AccId_, model.AccName_, model.AccType_, model.Result_, model.RealMoney_, model.FreeMoney_, model.Debt_, model.PointBalance_);
}

void UserAccountManager::clientEndedConnectionSlot()
{
    if (cs2ssNetworkManager::GetInstance()->isServerConnected() == false)
    {
        SessionManager::GetInstance()->endSession(/*lockUI*/false);
    }
    logoutFromClient();

    // relay to server
    UserLogoutQtModel qtModel;
    qtModel.ClientId = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::UserLogOutCommand(qtModel));
}

void UserAccountManager::procLogoutRequest( UserLogoutQtModel &qtModel )
{
    if (cs2ssNetworkManager::GetInstance()->isServerConnected() == false)
    {
        SessionManager::GetInstance()->endSession();
    }
    logoutFromClient();

    // relay to server
    qtModel.ClientId = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::UserLogOutCommand(qtModel));
}

void UserAccountManager::procLogoutReply( UserLogoutReplyQtModel &model )
{
    logoutFromClient();
}

void UserAccountManager::sessionEndedSlot()
{
    logoutFromClient();
}

bool UserAccountManager::logoutFromClient()
{
    // notify the client to lock the computer and goback to the fullscreen dialog
    UserLogoutReplyQtModel qtModel;
    cs2ccSender::send(cs2cc::UserLogoutReplyCommandC(qtModel));

    if (m_userAccount == NULL)
    {
        return true;
    }

    int userId = m_userAccount->Id();
    QString userName = m_userAccount->UserName();
    UserGroupType accType = m_userAccount->accType();

    LOG_DEBUG4 
        << "Account [ID: " << userId << "], " 
        << "[Name: " << qPrintable(userName) << "], " 
        << "[Type: " << (int)accType
        << "Logged out";

    if (m_userAccount != NULL)
    {
        delete m_userAccount;
        m_userAccount = NULL;
    }

    emit accLoggedOut(userId, accType);
    return true;
}

bool UserAccountManager::isLocalAdminAccount( 
    const QString &userName, const QString &rawPwd )
{
    QString pwdHash = Utils::hashUserData(userName, rawPwd, HashType_GCB_MD5);

    return (QString::compare(pwdHash, 
        CConfigManager::adminCodeHash(), Qt::CaseInsensitive) == 0);
}

bool UserAccountManager::_loginReplyImp(int userId, const QString &userName, 
                                        UserGroupType accType, LoginResult result, double realMoney, double freeMoney, double debt, int pointBalance )
{
    if (m_userAccount != NULL)
    {
        delete m_userAccount;
        m_userAccount = NULL;
    }

    // store the user account
    // no need raise signal to unlock the client now
    // as we are still waiting for the UnlockClient command (and may also the security profile command)
    if (result == LoginResult_OK)
    {
        // store to memory
        m_userAccount = new UserAccount(userId, userName, accType);
        LOG_DEBUG4 << "Account Logged in: "
            << ". ID: " << userId
            << ". Name: " << qPrintable(userName)
            << ". Type: " << (int)accType;
        emit accLoggedIn();
    }

    // forward to UI app
    UserLoginReplyQtModel qtModel;
    qtModel.AccId_ = userId;
    qtModel.AccName_ = userName;
    qtModel.AccType_ = accType;
    qtModel.Result_ = result;
    qtModel.RealMoney_ = realMoney;
    qtModel.FreeMoney_ = freeMoney;
    qtModel.Debt_ = debt;
    qtModel.PointBalance_ = pointBalance;

    cs2ccSender::send(cs2cc::UserLoginReplyCommandC(qtModel));

    cs2clSender::send(cs2cl::UserLoginReplyCommandC(qtModel));

    return true;
}

bool UserAccountManager::_loginLocalAdmin()
{
    int userId = 1; // hard-coded, the local admin account should always have id 1
    QString accName = tr("ADMIN");
    UserGroupType accType = UserGroup_LocalSuperAdmin;
    LoginResult result = LoginResult_OK;
    _loginReplyImp(userId, accName, accType, result, 0, 0, 0, 0);

    // unlock the client UI app
    SessionBeginQtModel qtModel;
    cs2ccSender::send(ss2cs::SessionBeginCommand(qtModel));
    cs2clSender::send(cs2cl::SessionBeginCommandC(qtModel));

    return true;
}

void UserAccountManager::procChangePwdRequest( ChangeAccPwdRequestQtModel &qtModel )
{
    // relay to server
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::ChangeAccPwdRequestCommand(qtModel));
}

// forward to client UI
void UserAccountManager::procChangePwdReply( const ChangeAccPwdReplyQtModel &qtModel )
{
    cs2ccSender::send(cs2cc::ChangePwdReplyCommandC(qtModel));
}

void UserAccountManager::tryResumeAccount( int accId, UserGroupType accType, const QString &accName )
{
    if (accId < 0 || accType < 0)
    {
        return;
    }

    if (m_userAccount != NULL)
    {
        return;
    }

    m_userAccount = new UserAccount(accId, accName, accType);

    emit accLoggedIn();
}

void UserAccountManager::procTopupByCouponRequest( TopupByCouponRequestQtModel & model )
{
    // relay to server
    model.ClientId = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::TopupByCouponRequestCommand(model));
}

void UserAccountManager::procTopupByCouponReply( TopupByCouponReplyQtModel & model )
{
    // relay to client app
    cs2ccSender::send(cs2cc::TopupByCouponReplyCommandC(model));
}

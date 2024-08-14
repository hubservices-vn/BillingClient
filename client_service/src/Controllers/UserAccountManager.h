#ifndef USERACCOUNTMANAGER_H
#define USERACCOUNTMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

enum UserGroupType;
enum LoginResult;
class UserAccount;
// these 2 come from client UI app
class UserLoginRequestQtModel;
class UserLogoutQtModel;
class TopupByCouponRequestQtModel;
class ChangeAccPwdRequestQtModel;

// these 2 come from server reply
class UserLoginReplyQtModel;
class UserLogoutReplyQtModel;
class TopupByCouponReplyQtModel;
class ChangeAccPwdReplyQtModel;

class UserAccountManager : public QObject, public Singleton_T<UserAccountManager>
{
    Q_OBJECT
public:

    bool init();
    bool connectSigSlot();

    // helper
    static UserAccount* CurrentUser() { return m_userAccount; }
    static void SetCurrentUser(UserAccount* val) { m_userAccount = val; }

    bool isLocalAdminAccount(const QString &userName, const QString &rawPwd);

    static bool isUserLoggedIn();

    static QString currentUserName();
    // if the user account is NULL, will return -1
    static int currentUserId();

    static UserGroupType currentUserType();

    static int currentUserTypeVal();

    bool logoutFromClient();

    // client service crashed, while a member is still using the computer
    // after UI App connected to service, the client APP reports the account data
    void tryResumeAccount( int accId, UserGroupType accType, const QString &accName );

signals:
    void accLoggedIn();
    void accLoggedOut(int userId, int accTypeVal);

public slots:
    void clientEndedConnectionSlot();
    // clear the current account
    void sessionEndedSlot();

    void procLoginRequest(UserLoginRequestQtModel &qtModel);
    void procLogoutRequest( UserLogoutQtModel &qtModel );

    void procLogoutReply( UserLogoutReplyQtModel &model );
    void procLoginReply( UserLoginReplyQtModel &model );

    void procChangePwdRequest(ChangeAccPwdRequestQtModel &qtModel);
    void procChangePwdReply(const ChangeAccPwdReplyQtModel &qtModel);
    void procTopupByCouponRequest(TopupByCouponRequestQtModel & model);    
    void procTopupByCouponReply( TopupByCouponReplyQtModel & model );
private: // private methods
    bool _loginReplyImp(int userId, const QString &userName, 
        UserGroupType accType, LoginResult result, double realMoney, double freeMoney, double debt, int pointBalance );

    bool _loginLocalAdmin();
    
private:
    static UserAccount* m_userAccount;
};

#endif // USERACCOUNTMANAGER_H

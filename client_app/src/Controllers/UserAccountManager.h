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
// these 2 come from server reply
class UserLoginReplyQtModel;
class UserLogoutReplyQtModel;
class ChangeAccPwdReplyQtModel;
class UserUpdateQtModel;

class TopupByCouponReplyQtModel;
class UserAccountManager : public QObject, public Singleton_T<UserAccountManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // cmd handlers
    void procLoginReply( const UserLoginReplyQtModel &qtModel );
    void procLogoutReply( const UserLogoutReplyQtModel &qtModel );
    void procUserUpdate( const UserUpdateQtModel &qtModel );
    void procChangePwdReply(const ChangeAccPwdReplyQtModel &qtModel);

    void procTopupByCouponReply( const TopupByCouponReplyQtModel &qtModel );

public: // helper
    static UserAccount* CurrentUser() { return m_userAccount; }
    static void SetCurrentUser(UserAccount* val) { m_userAccount = val; }

    static bool isUserLoggedIn();

    static QString currentUserName();
    // if the user account is NULL, will return -1
    static int currentUserId();

    static UserGroupType currentUserType();

    static int currentUserTypeVal();

    static double currentUserBalance();
    static double currentUserDebt();
    static int currentUserPointBalance();

signals:
    void loginReplyRecved(/*LoginResult*/int result, 
        /*UserGroupType*/int accType);

    void changeAccPwdReplyRecved(/*ChangeAccPwdResult*/int result);

    void accLoggedIn();
    void accLoggedOut(int userId, int accTypeVal);

    void topupByCouponReplyRecved(/*TopupByCouponResult*/int result);
    void userAccUpdated();
    public slots:
    

private: // private methods

private:
    static UserAccount* m_userAccount;
};

#endif // USERACCOUNTMANAGER_H

#ifndef USERACCOUNTMANAGER_H
#define USERACCOUNTMANAGER_H

#include "CcCore/CcSingleton.h"
#include <QObject>

// these 2 come from server reply
class UserLoginReplyQtModel;
class UserLogoutReplyQtModel;
class UserAccountManager : public QObject, public Singleton_T<UserAccountManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // cmd handlers
    void procLoginReply( const UserLoginReplyQtModel &qtModel );
    void procLogoutReply( const UserLogoutReplyQtModel &qtModel );

signals:
    void loginReplyRecved(/*LoginResult*/int resultVal, 
        /*UserGroupType*/int accTypeVal);

    void logoutReplyRecved(/*LoginResult*/bool result);

	void accLoggedIn();
    void accLoggedOut(int x, int y);

public slots:

private:
};

#endif // USERACCOUNTMANAGER_H

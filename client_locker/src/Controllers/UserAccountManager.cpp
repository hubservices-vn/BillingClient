#include "UserAccountManager.h"
#include "Client.h"
#include "CommonSettings.h"
#include "log.h"
#include "UserLoginQtModel.h"
#include "UserLogoutQtModel.h"

bool UserAccountManager::init()
{
    return true;
}

bool UserAccountManager::connectSigSlot()
{
    return true;
}

void UserAccountManager::procLoginReply( const UserLoginReplyQtModel &qtModel )
{
    // notify other UI components that login reply has arrived
    emit loginReplyRecved(qtModel.Result_, qtModel.AccType_);
	emit accLoggedIn();
}

void UserAccountManager::procLogoutReply( const UserLogoutReplyQtModel &qtModel )
{
    emit logoutReplyRecved(qtModel.Result);
	emit accLoggedOut(0, 0);
}

#include "WebHistoryManager.h"
#include "UserAccountManager.h"
#include "WebHistoryQtModel.h"
#include "ConfigManager.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"


bool WebHistoryManager::init()
{
    return true;
}

bool WebHistoryManager::connectSigSlot()
{
    return true;
}

// forward this message to server
void WebHistoryManager::procWebHistoryReportC( WebHistoryReportQtModel &qtModel )
{
    qtModel.ClientId_ = CConfigManager::clientId();
    qtModel.AccId_ = UserAccountManager::currentUserId();
    qtModel.AccType_ = UserAccountManager::currentUserType();

    cs2ssSender::send(cs2ss::WebHistoryReportCommand(qtModel));
}

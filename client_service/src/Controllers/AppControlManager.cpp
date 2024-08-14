#include "AppControlManager.h"

#include "AppControlQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "ConfigManager.h"

bool AppControlManager::init()
{
    return true;
}

bool AppControlManager::connectSigSlot()
{
    return true;
}

void AppControlManager::procAppControlReply( const AppControlReplyQtModel &qtModel )
{
    cs2ccSender::send(cs2cc::AppControlReplyCommandC(qtModel));
}

void AppControlManager::procAppControlWhiteReply(const AppControlReplyQtModel &qtModel)
{
    cs2ccSender::send(cs2cc::AppControlReplyWhiteCommandC(qtModel));
}

void AppControlManager::procAppControlRequestC( AppControlRequestQtModel &qtModel )
{
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::AppControlRequestCommand(qtModel));
}

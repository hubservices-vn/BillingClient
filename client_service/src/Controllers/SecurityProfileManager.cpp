#include "SecurityProfileManager.h"

#include "log.h"
#include "SecurityProfileQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "cs2ccNetworkManager.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "ConfigManager.h"

const int DEFAULT_PROFILE_ID = 1;

bool SecurityProfileManager::init()
{
    return true;
}

bool SecurityProfileManager::connectSigSlot()
{
    return true;
}

void SecurityProfileManager::procSecuProfileReply( const SecuProfileReplyQtModel &qtModel )
{
    cs2ccSender::send(cs2cc::SecurityProfileReplyCommandC(qtModel));
}

void SecurityProfileManager::procSecuProfileRequestC( SecuProfileRequestQtModel &qtModel )
{
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::SecuProfileRequestCommand(qtModel));
}

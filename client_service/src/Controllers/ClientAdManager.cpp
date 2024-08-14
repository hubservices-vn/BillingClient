#include "ClientAdManager.h"
#include "ClientAdTransferQtModel.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"

bool ClientAdManager::init()
{
    return true;
}

bool ClientAdManager::connectSigSlot()
{
    return true;
}

void ClientAdManager::procClientAdReplyC(const ClientAdTransferReplyQtModel &qtModel)
{
    cs2ccSender::send(cs2cc::ClientAdTransferReplyCommandC(qtModel));
}

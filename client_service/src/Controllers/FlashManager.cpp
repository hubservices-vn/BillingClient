#include "FlashManager.h"
#include "FlashTransferQtModel.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"

bool FlashManager::init()
{
    return true;
}

bool FlashManager::connectSigSlot()
{
    return true;
}

void FlashManager::procFlashReplyC(const FlashTransferReplyQtModel &qtModel)
{
    cs2clSender::send(cs2cl::FlashTransferReplyCommandC(qtModel));
}

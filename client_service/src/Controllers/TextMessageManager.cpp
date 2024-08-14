#include "TextMessageManager.h"
#include "cs2ccSender.h"
#include "TextMessageQtModel.h"
#include "cs2ccCmds.h"


bool TextMessageManager::init()
{
    return true;
}

bool TextMessageManager::connectSigSlot()
{
    return true;
}

void TextMessageManager::procTextMsgRecved( const TextMessageQtModel &qtModel )
{
    cs2ccSender::send(cs2cc::TextMessageCommandC(qtModel));
}

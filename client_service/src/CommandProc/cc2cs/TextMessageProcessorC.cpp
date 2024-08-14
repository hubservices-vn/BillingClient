#include "TextMessageProcessorC.h"
#include "TextMessageQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ConfigManager.h"
#include "log.h"


// forward text message to server side directly
void cc2cs::TextMessageProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
{
    TextMessageQtModel qtModel;
    TextMessageQtModel::fromProtoBuf(ptr, qtModel);

    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::TextMessageCommand(qtModel));
}

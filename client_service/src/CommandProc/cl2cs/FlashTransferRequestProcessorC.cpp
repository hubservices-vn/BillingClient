#include "FlashTransferRequestProcessorC.h "
#include "FlashTransferQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ConfigManager.h"

namespace cl2cs
{
    void FlashTransferRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        FlashTransferRequestQtModel qtModel;
        FlashTransferRequestQtModel::fromProtoBuf(ptr, qtModel);

        qtModel.ClientId_ = CConfigManager::clientId();
        cs2ssSender::send(cs2ss::FlashTransferRequestCommand(qtModel));
    }

}
#include "ClientAdTransferRequestProcessorC.h "
#include "ClientAdTransferQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ConfigManager.h"

namespace cc2cs
{
    void ClientAdTransferRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientAdTransferRequestQtModel qtModel;
        ClientAdTransferRequestQtModel::fromProtoBuf(ptr, qtModel);

        qtModel.ClientId_ = CConfigManager::clientId();
        cs2ssSender::send(cs2ss::ClientAdTransferRequestCommand(qtModel));
    }

}
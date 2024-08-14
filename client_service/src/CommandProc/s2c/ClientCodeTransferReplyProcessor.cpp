#include "ClientCodeTransferReplyProcessor.h"
#include "ClientCodeTransferQtModel.h"
#include "ClientEventDllUpdater.h"


namespace s2c
{
    void ClientCodeTransferReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientCodeTransferReplyQtModel qtModel;
        ClientCodeTransferReplyQtModel::fromProtoBuf(ptr, qtModel);

        ClientEventDllUpdater::GetInstance()->procClientCodeReplyC(qtModel);
    }

}
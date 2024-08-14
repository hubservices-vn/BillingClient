#include "ClientAdTransferReplyProcessor.h"
#include "ClientAdTransferQtModel.h"
#include "ClientAdManager.h"


namespace s2c
{
    void ClientAdTransferReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientAdTransferReplyQtModel qtModel;
        ClientAdTransferReplyQtModel::fromProtoBuf(ptr, qtModel);

        ClientAdManager::GetInstance()->procClientAdReplyC(qtModel);
    }

}
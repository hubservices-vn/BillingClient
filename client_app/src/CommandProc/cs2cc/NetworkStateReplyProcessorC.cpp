#include "NetworkStateReplyProcessorC.h"
#include "NetworkStateQtModel.h"
#include "cc2csNetworkManager.h"
#include "log.h"

namespace cs2cc
{
    void NetworkStateReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        NetworkStateReplyQtModel qtModel;
        NetworkStateReplyQtModel::fromProtoBuf(ptr, qtModel);

        cc2csNetworkManager::GetInstance()->procNetworkStateReply(qtModel);
    }
}




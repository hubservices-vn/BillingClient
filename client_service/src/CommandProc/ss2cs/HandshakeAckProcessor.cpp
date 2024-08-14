#include "HandshakeAckProcessor.h"
#include "HandshakeQtModel.h"
#include "cs2ssNetworkManager.h"
#include "log.h"

namespace ss2cs
{
    void HandshakeAckProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        HandshakeAckQtModel qtModel;
        HandshakeAckQtModel::fromProtoBuf(ptr, qtModel);

        cs2ssNetworkManager::GetInstance()->procHandshakeAck(qtModel);
    }

}

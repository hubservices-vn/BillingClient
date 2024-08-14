#include "CCHandshakeAckProcessorC.h"
#include "CCHandshakeQtModel.h"
#include "cc2csNetworkManager.h"


namespace cs2cc
{

    void CCHandshakeAckProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        CCHandshakeAckQtModel qtModel;
        CCHandshakeAckQtModel::fromProtoBuf(ptr, qtModel);

        cc2csNetworkManager::GetInstance()->procCCHandshakeAck(qtModel);
    }

}
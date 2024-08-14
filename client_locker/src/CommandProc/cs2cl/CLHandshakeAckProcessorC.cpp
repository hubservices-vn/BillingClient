#include "CLHandshakeAckProcessorC.h"
#include "CLHandshakeQtModel.h"
#include "cl2csNetworkManager.h"


namespace cs2cl
{

    void CLHandshakeAckProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        CLHandshakeAckQtModel qtModel;
        CLHandshakeAckQtModel::fromProtoBuf(ptr, qtModel);

        cl2csNetworkManager::GetInstance()->procCLHandshakeAck(qtModel);
    }


}
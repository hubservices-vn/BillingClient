#include "CLHandshakeProcessorC.h"
#include "CLHandshakeQtModel.h"
#include "cs2clNetworkManager.h"


namespace cl2cs
{
    void CLHandshakeProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        CLHandshakeQtModel qtModel;
        CLHandshakeQtModel::fromProtoBuf(ptr, qtModel);

        cs2clNetworkManager::GetInstance()->procCLHandshake(socket, qtModel);
    }
}


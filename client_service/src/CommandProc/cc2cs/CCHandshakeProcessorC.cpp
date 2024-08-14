#include "CCHandshakeProcessorC.h"
#include "CCHandshakeQtModel.h"
#include "cs2ccNetworkManager.h"

namespace cc2cs
{
    void CCHandshakeProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        CCHandshakeQtModel qtModel;
        CCHandshakeQtModel::fromProtoBuf(ptr, qtModel);

        cs2ccNetworkManager::GetInstance()->procCCHandshake(socket, qtModel);
    }
}
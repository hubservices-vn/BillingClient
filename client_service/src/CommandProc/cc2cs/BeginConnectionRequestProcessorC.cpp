#include "BeginConnectionRequestProcessorC.h"
#include "ClientConnectionQtModel.h"
#include "log.h"
#include "cs2ccNetworkManager.h"


namespace cc2cs
{

    void BeginConnectionRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        BeginConnectionRequestQtModel qtModel;
        BeginConnectionRequestQtModel::fromProtoBuf(ptr, qtModel);

        cs2ccNetworkManager::GetInstance()->procBeginConnectionRequest(qtModel);
    }

}

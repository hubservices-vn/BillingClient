#include "EndConnectionRequestProcessorC.h"
#include "ClientConnectionQtModel.h"
#include "cs2ccNetworkManager.h"

namespace cc2cs
{

    void EndConnectionRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        EndConnectionRequestQtModel qtModel;
        EndConnectionRequestQtModel::fromProtoBuf(ptr, qtModel);

        cs2ccNetworkManager::GetInstance()->procEndConnectionRequestC(qtModel);
    }

}
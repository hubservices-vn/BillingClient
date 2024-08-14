#include "NetworkStateProcessorC.h"
#include "cs2ccNetworkManager.h"
#include "NetworkStateQtModel.h"
#include "log.h"


namespace cc2cs
{
    void NetworkStateProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        NetworkStateQtModel qtModel;
        NetworkStateQtModel::fromProtoBuf(ptr, qtModel);

        cs2ccNetworkManager::GetInstance()->procNetworkStateRequest();
    }

}
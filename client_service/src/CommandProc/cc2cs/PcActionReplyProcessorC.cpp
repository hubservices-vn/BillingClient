#include "PcActionReplyProcessorC.h"
#include "PcActionQtModel.h"
#include "SystemManager.h"


namespace cc2cs
{

    void RemoteControlReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        RemoteControlReplyQtModel qtModel;
        RemoteControlReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procRemoteControlReplyC(qtModel);
    }

}
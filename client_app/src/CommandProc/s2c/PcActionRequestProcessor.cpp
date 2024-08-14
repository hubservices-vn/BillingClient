#include "PcActionRequestProcessor.h"
#include "PcActionQtModel.h"
#include "SystemManager.h"


namespace s2c
{

    void PcActionRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        PcActionRequestQtModel qtModel;
        PcActionRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procPcAction(qtModel);
    }

}
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

    void VolumeQueryRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        VolumeQueryRequestQtModel qtModel;
        VolumeQueryRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procVolumeQueryRequest(qtModel);
    }

    void VolumeControlRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        VolumeControlRequestQtModel qtModel;
        VolumeControlRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procVolumeControlRequest(qtModel);
    }

}
#include "UpdateClientRequestProcessor.h"
#include "SystemManager.h"
#include "UpdateClientQtModel.h"

namespace s2c
{

    void UpdateClientRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UpdateClientRequestQtModel qtModel;
        UpdateClientRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procUpdateClientRequest(qtModel);
    }

}

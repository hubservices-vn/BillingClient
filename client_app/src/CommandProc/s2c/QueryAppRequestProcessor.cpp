#include "QueryAppRequestProcessor.h"
#include "SystemManager.h"
#include "QueryTermAppQtModel.h"


namespace s2c
{

    void QueryAppRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        QueryAppRequestQtModel qtModel;
        QueryAppRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procQueryAppRequest(qtModel);
    }

}
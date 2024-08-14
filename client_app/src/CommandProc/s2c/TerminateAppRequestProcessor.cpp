#include "TerminateAppRequestProcessor.h"
#include "QueryTermAppQtModel.h"
#include "SystemManager.h"
#include "log.h"


namespace s2c
{
    void TerminateAppRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        TerminateAppRequestQtModel qtModel;
        TerminateAppRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procTermAppRequest(qtModel);
    }


}

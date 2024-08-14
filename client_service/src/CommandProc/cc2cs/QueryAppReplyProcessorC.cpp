#include "QueryAppReplyProcessorC.h"
#include "QueryTermAppQtModel.h"
#include "SystemManager.h"
#include "log.h"


namespace cc2cs
{

    void QueryAppReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        QueryAppReplyQtModel qtModel;
        QueryAppReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procQueryAppReplyC(qtModel);
    }

}


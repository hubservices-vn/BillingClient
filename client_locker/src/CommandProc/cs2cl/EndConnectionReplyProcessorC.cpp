#include "EndConnectionReplyProcessorC.h"
#include "log.h"
#include "ClientConnectionQtModel.h"
#include "SystemManager.h"


namespace cs2cc
{

    void EndConnectionReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        EndConnectionReplyQtModel qtModel;
        EndConnectionReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procEndConnectionReply(qtModel);
    }

}

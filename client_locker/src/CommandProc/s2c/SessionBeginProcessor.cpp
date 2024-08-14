#include "SessionBeginProcessor.h"
#include "SessionBeginQtModel.h"
#include "log.h"
#include "SessionManager.h"

namespace s2c
{
    void SessionBeginProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SessionBeginQtModel qtModel;
        SessionBeginQtModel::fromProtoBuf(ptr, qtModel);

        SessionManager::GetInstance()->procSessionBegin(qtModel);
    }
}
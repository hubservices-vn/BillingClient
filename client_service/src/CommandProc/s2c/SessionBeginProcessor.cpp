#include "SessionBeginProcessor.h"
#include "SessionBeginQtModel.h"
#include "SessionManager.h"
#include "log.h"

namespace s2c
{
    void SessionBeginProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SessionBeginQtModel qtModel;
        SessionBeginQtModel::fromProtoBuf(ptr, qtModel);

        SessionManager::GetInstance()->procSessionBegin(qtModel);
    }
}
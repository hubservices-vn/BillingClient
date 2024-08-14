#include "SessionStatusProcessor.h"
#include "SessionStatusQtModel.h"
#include "SessionManager.h"
#include "log.h"

namespace s2c
{
    void SessionStatusProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SessionStatusQtModel qtModel;
        SessionStatusQtModel::fromProtoBuf(ptr, qtModel);

        SessionManager::GetInstance()->procSessionStatus(qtModel);
    }

}

#include "SessionEndProcessor.h"
#include "SessionEndQtModel.h"
#include "SessionManager.h"
#include "UserAccountManager.h"
#include "log.h"


namespace s2c
{
    void SessionEndProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SessionEndQtModel qtModel;
        SessionEndQtModel::fromProtoBuf(ptr, qtModel);

        SessionManager::GetInstance()->procSessionEnd(qtModel);
    }
}
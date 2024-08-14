#include "UserLoginReplyProcessor.h"
#include "UserLoginQtModel.h"
#include "UserAccountManager.h"
#include "log.h"

namespace s2c
{
    void UserLoginReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UserLoginReplyQtModel qtModel;
        UserLoginReplyQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procLoginReply(qtModel);
    }
}


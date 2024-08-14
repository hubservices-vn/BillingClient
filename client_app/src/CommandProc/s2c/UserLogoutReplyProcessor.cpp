#include "UserLogoutReplyProcessor.h"
#include "UserLogoutQtModel.h"
#include "UserAccountManager.h"
#include "log.h"


namespace s2c
{
    void UserLogoutReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UserLogoutReplyQtModel qtModel;
        UserLogoutReplyQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procLogoutReply(qtModel);
    }
}



#include "AccountAuthReplyProcessor.h"
#include "UserLoginQtModel.h"
#include "SystemManager.h"


namespace ss2cs
{

    void AccountAuthReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        AccountAuthReplyQtModel qtModel;
        AccountAuthReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procAccountAuthReply(qtModel);
    }

}
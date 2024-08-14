#include "ChangePwdReplyProcessor.h"
#include "log.h"
#include "ChangeAccPwdQtModel.h"
#include "UserAccountManager.h"



namespace s2c
{

    void ChangePwdReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ChangeAccPwdReplyQtModel qtModel;
        ChangeAccPwdReplyQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procChangePwdReply(qtModel);
    }

}
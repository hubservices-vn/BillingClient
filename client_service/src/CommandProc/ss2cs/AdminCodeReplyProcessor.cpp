#include "AdminCodeReplyProcessor.h"
#include "ClientSettingManager.h"
#include "AdminCodeQtModel.h"
#include "log.h"


namespace ss2cs
{

    void AdminCodeReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        AdminCodeReplyQtModel qtModel;
        AdminCodeReplyQtModel::fromProtoBuf(ptr, qtModel);

        ClientSettingManager::GetInstance()->procAdminCodeReply(qtModel);
    }

}
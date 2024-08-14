#include "ClientSettingReplyProcessor.h"
#include "log.h"
#include "SystemSettingQtModel.h"
#include "ClientSettingManager.h"



namespace s2c
{

    void ClientSettingReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SystemSettingReplyQtModel qtModel;
        SystemSettingReplyQtModel::fromProtoBuf(ptr, qtModel);

        ClientSettingManager::GetInstance()->procSystemSettingReply(qtModel);
    }

}
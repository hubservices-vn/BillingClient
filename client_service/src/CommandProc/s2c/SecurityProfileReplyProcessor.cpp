#include "SecurityProfileReplyProcessor.h"
#include "SecurityProfileQtModel.h"
#include "SecurityProfileManager.h"

namespace s2c
{

    void SecurityProfileReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SecuProfileReplyQtModel qtModel;
        SecuProfileReplyQtModel::fromProtoBuf(ptr, qtModel);

        SecurityProfileManager::GetInstance()->procSecuProfileReply(qtModel);
    }

}
#include "WebControlReplyProcessor.h"
#include "log.h"
#include "WebControlQtModel.h"
#include "WebControlManager2.h"



namespace ss2cs
{

    void WebControlReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        WebControlReplyQtModel qtModel;
        WebControlReplyQtModel::fromProtoBuf(ptr, qtModel);

        WebControlManager2::GetInstance()->procWebControlReply(qtModel);
    }

}
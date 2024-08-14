#include "HeartbeatReplyProcessor.h"
#include "log.h"
#include "HeartBeatQtModel.h"
#include "HeartBeatManager.h"



namespace ss2cs
{

    void HeartbeatReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        HeartBeatQtModel qtModel;
        HeartBeatQtModel::fromProtoBuf(ptr, qtModel);

        SvrHeartBeatSender::GetInstance()->procHeartBeatReply(qtModel);
    }

}

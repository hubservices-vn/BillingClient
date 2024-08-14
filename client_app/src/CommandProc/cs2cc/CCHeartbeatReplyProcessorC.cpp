#include "CCHeartbeatReplyProcessorC.h"
#include "log.h"
#include "HeartBeatQtModel.h"
#include "HeartbeatSender.h"



namespace cs2cc
{
    void CCHeartbeatReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        HeartBeatQtModel qtModel;
        HeartBeatQtModel::fromProtoBuf(ptr, qtModel);

        HeartBeatSender::GetInstance()->procHeartBeatReply(qtModel);
    }

}

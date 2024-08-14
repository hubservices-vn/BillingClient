#include "CCHeartbeatProcessorC.h"
#include "log.h"
#include "HeartBeatQtModel.h"
#include "HeartBeatManager.h"



namespace cc2cs
{

    void CCHeartbeatProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        HeartBeatQtModel qtModel;
        HeartBeatQtModel::fromProtoBuf(ptr, qtModel);

        ClientHeartBeatChecker::GetInstance()->procCCHeartBeat(qtModel);
    }

}

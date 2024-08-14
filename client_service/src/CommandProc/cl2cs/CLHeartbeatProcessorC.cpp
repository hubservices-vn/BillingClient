#include "CLHeartbeatProcessorC.h"
#include "log.h"
#include "HeartBeatQtModel.h"
#include "HeartBeatManager.h"



namespace cl2cs
{

    void CLHeartbeatProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        HeartBeatQtModel qtModel;
        HeartBeatQtModel::fromProtoBuf(ptr, qtModel);

        ClientHeartBeatChecker::GetInstance()->procCLHeartBeat(qtModel);
    }

}

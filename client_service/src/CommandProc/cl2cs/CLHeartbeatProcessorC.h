#ifndef CL_HEARTBEAT_PROCESSORC_H
#define CL_HEARTBEAT_PROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\HeartBeatPack.pb.h"

namespace cl2cs
{
    class CLHeartbeatProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CLHeartbeatProcessorC_p, 
        Models::HeartBeatPack,
        CMD_ENCRYPT
    > 
    CLHeartbeatProcessorC;
}


#endif // CL_HEARTBEAT_PROCESSORC_H

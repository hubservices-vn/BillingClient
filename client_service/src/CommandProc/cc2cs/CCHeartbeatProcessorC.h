#ifndef CC_HEARTBEAT_PROCESSORC_H
#define CC_HEARTBEAT_PROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\HeartBeatPack.pb.h"

namespace cc2cs
{
    class CCHeartbeatProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CCHeartbeatProcessorC_p, 
        Models::HeartBeatPack,
        CMD_ENCRYPT
    > 
    CCHeartbeatProcessorC;
}


#endif // CC_HEARTBEAT_PROCESSORC_H

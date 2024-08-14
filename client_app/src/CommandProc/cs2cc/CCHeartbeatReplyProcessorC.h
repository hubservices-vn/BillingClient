#ifndef HEARTBEATREPLYPROCESSOR_H
#define HEARTBEATREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\HeartBeatPack.pb.h"

namespace cs2cc
{
    class CCHeartbeatReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CCHeartbeatReplyProcessorC_p, 
        Models::HeartBeatPack,
        CMD_ENCRYPT
    > 
    CCHeartbeatReplyProcessorC;
}


#endif // HEARTBEATREPLYPROCESSOR_H

#ifndef HEARTBEATREPLYPROCESSOR_H
#define HEARTBEATREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\HeartBeatPack.pb.h"

namespace ss2cs
{
    class HeartbeatReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        HeartbeatReplyProcessor_p, 
        Models::HeartBeatPack,
        CMD_ENCRYPT
    > 
    HeartbeatReplyProcessor;
}


#endif // HEARTBEATREPLYPROCESSOR_H

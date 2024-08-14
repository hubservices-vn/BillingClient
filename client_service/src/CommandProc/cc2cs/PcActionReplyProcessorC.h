#ifndef PC_ACTION_REPLY_PROCESSORC_H
#define PC_ACTION_REPLY_PROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/PCActionPack.pb.h"


namespace cc2cs
{
    class RemoteControlReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        RemoteControlReplyProcessorC_p, 
        Models::RemoteControlReplyPack,
        CMD_ENCRYPT
    > 
    RemoteControlReplyProcessorC;
}



#endif // PC_ACTION_REPLY_PROCESSORC_H

#ifndef END_CONNECTION_REPLY_PROCESSORC_H
#define END_CONNECTION_REPLY_PROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientConnectionPack.pb.h"


namespace cs2cc
{
    class EndConnectionReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        EndConnectionReplyProcessorC_p, 
        Models::EndConnectionReplyPack,
        CMD_ENCRYPT
    >
    EndConnectionReplyProcessorC;
}



#endif // END_CONNECTION_REPLY_PROCESSORC_H

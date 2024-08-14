#ifndef CLIENTADTRANSFERREPLYPROCESSOR_H
#define CLIENTADTRANSFERREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ClientAdTransferPack.pb.h"

namespace s2c
{
    class ClientAdTransferReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientAdTransferReplyProcessor_p , 
        Models::ClientAdTransferReplyPack,
        /*encrypt*/false
    > 
    ClientAdTransferReplyProcessor;
}


#endif // CLIENTADTRANSFERREPLYPROCESSOR_H

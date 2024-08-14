#ifndef CLIENTCODETRANSFERREPLYPROCESSOR_H
#define CLIENTCODETRANSFERREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ClientCodeTransferPack.pb.h"

namespace s2c
{
    class ClientCodeTransferReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientCodeTransferReplyProcessor_p , 
        Models::ClientCodeTransferReplyPack,
        /*encrypt*/false
    > 
    ClientCodeTransferReplyProcessor;
}


#endif // CLIENTCODETRANSFERREPLYPROCESSOR_H

#ifndef CLIENTTRANSFERREQUESTPROCESSORC_H
#define CLIENTTRANSFERREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientAdTransferPack.pb.h"


namespace cc2cs
{
    class ClientAdTransferRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientAdTransferRequestProcessorC_p, 
        Models::ClientAdTransferRequestPack,
        CMD_ENCRYPT
    > 
    ClientAdTransferRequestProcessorC;
}



#endif // CLIENTTRANSFERREQUESTPROCESSORC_H

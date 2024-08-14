#ifndef CLIENTCODEDLLUPDATEDPROCESSORC_H
#define CLIENTCODEDLLUPDATEDPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ClientCodeDllUpdatedPack.pb.h"


namespace cs2cl
{
    class ClientCodeDllUpdatedProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientCodeDllUpdatedProcessorC_p , 
        Models::ClientCodeDllUpdatedPack ,
        CMD_ENCRYPT
    >
    ClientCodeDllUpdatedProcessorC;
}

#endif // CLIENTCODEDLLUPDATEDPROCESSORC_H

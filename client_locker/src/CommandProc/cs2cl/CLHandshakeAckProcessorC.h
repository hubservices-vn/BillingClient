#ifndef CLHANDSHAKEACKPROCESSORC_H
#define CLHANDSHAKEACKPROCESSORC_H


#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\CLHandshakePack.pb.h"


namespace cs2cl
{
    class CLHandshakeAckProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CLHandshakeAckProcessorC_p, 
        Models::CLHandshakeAckPack,
        CMD_ENCRYPT
    >
    CLHandshakeAckProcessorC;
}


#endif // CLHANDSHAKEACKPROCESSORC_H

#ifndef CCHANDSHAKEACKPROCESSORC_H
#define CCHANDSHAKEACKPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\CCHandshakePack.pb.h"


namespace cs2cc
{
    class CCHandshakeAckProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CCHandshakeAckProcessorC_p , 
        Models::CCHandshakeAckPack ,
        CMD_ENCRYPT
    >
    CCHandshakeAckProcessorC;
}

#endif // CCHANDSHAKEACKPROCESSORC_H

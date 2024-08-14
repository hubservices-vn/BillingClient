#ifndef HANDSHAKECOMMANDPROCESSOR_H
#define HANDSHAKECOMMANDPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\HandshakePack.pb.h"

namespace ss2cs
{
    class HandshakeAckProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        HandshakeAckProcessor_p, 
        Models::HandshakeAckPack,
        CMD_ENCRYPT
    > 
    HandshakeAckProcessor;
}

#endif // HANDSHAKECOMMANDPROCESSOR_H

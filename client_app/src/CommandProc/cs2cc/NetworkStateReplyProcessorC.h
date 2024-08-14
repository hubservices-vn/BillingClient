#ifndef NETWORKSTATEREPLYPROCESSOR_H
#define NETWORKSTATEREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/NetworkStatePack.pb.h"


namespace cs2cc
{
    class NetworkStateReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        NetworkStateReplyProcessor_p, 
        Models::NetworkStateReplyPack,
        CMD_ENCRYPT
    >
    NetworkStateReplyProcessor;
}

#endif // NETWORKSTATEREPLYPROCESSOR_H

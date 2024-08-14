#ifndef NETWORKSTATEPROCESSOR_H
#define NETWORKSTATEPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/NetworkStatePack.pb.h"

namespace cc2cs
{
    class NetworkStateProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        NetworkStateProcessor_p, 
        Models::NetworkStatePack,
        CMD_ENCRYPT
    > 
    NetworkStateProcessorC;
}


#endif // NETWORKSTATEPROCESSOR_H

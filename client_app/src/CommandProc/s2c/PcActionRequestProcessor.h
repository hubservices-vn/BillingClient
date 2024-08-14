#ifndef PCACTIONPROCESSOR_H
#define PCACTIONPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/PCActionPack.pb.h"

namespace s2c
{
    class PcActionRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        PcActionRequestProcessor_p, 
        Models::PCActionRequestPack,
        CMD_ENCRYPT
    > 
    PcActionRequestProcessor;
}


#endif // PCACTIONPROCESSOR_H

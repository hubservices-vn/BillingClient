#ifndef SYSTEMMESSAGEPROCESSOR_H
#define SYSTEMMESSAGEPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/SystemMessagePack.pb.h"


namespace s2c
{
    class SystemMessageProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        SystemMessageProcessor_p, 
        Models::SystemMessagePack,
        CMD_ENCRYPT
    > 
    SystemMessageProcessor;
}

#endif // SYSTEMMESSAGEPROCESSOR_H

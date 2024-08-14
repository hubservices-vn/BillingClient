#ifndef ORDERREPLYPROCESSOR_H
#define ORDERREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/OrderPack.pb.h"

namespace ss2cs
{
    class OrderReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        OrderReplyProcessor_p, 
        Models::OrderReplyPack,
        CMD_ENCRYPT
    > 
    OrderReplyProcessor;
}

#endif // ORDERREPLYPROCESSOR_H

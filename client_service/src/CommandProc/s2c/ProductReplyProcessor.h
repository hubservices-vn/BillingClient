#ifndef PRODUCTREPLYPROCESSOR_H
#define PRODUCTREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ProductPack.pb.h"

namespace s2c
{
    class ProductReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ProductReplyProcessor_p, 
        Models::ProductReplyPack,
        CMD_ENCRYPT
    > 
    ProductReplyProcessor;
}



#endif // PRODUCTREPLYPROCESSOR_H

#ifndef PRODUCTQUERYPROCESSORC_H
#define PRODUCTQUERYPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ProductPack.pb.h"

namespace cc2cs
{
    class ProductQueryProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ProductQueryProcessorC_p, 
        Models::ProductRequestPack,
        CMD_ENCRYPT
    > 
    ProductQueryProcessorC;
}


#endif // PRODUCTQUERYPROCESSORC_H

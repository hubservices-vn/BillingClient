#ifndef QUERYAPPREPLYPROCESSOR_H
#define QUERYAPPREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\QueryAppPack.pb.h"


namespace cc2cs
{
    class QueryAppReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        QueryAppReplyProcessorC_p,
        Models::QueryAppReplyPack,
        CMD_ENCRYPT
    >
    QueryAppReplyProcessorC;
}



#endif // QUERYAPPREPLYPROCESSOR_H

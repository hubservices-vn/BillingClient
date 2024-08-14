#ifndef QUERYAPPREQUESTPROCESSOR_H
#define QUERYAPPREQUESTPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\QueryAppPack.pb.h"

namespace s2c
{
    class QueryAppRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        QueryAppRequestProcessor_p, 
        Models::QueryAppRequestPack,
        CMD_ENCRYPT
    > 
    QueryAppRequestProcessor;
}


#endif // QUERYAPPREQUESTPROCESSOR_H

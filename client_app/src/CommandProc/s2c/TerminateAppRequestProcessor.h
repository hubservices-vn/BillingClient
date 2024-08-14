#ifndef TERMINATEAPPREQUESTPROCESSOR_H
#define TERMINATEAPPREQUESTPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\QueryAppPack.pb.h"

namespace s2c
{
    class TerminateAppRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        TerminateAppRequestProcessor_p, 
        Models::TerminateAppRequestPack,
        CMD_ENCRYPT
    > 
    TerminateAppRequestProcessor;
}



#endif // TERMINATEAPPREQUESTPROCESSOR_H

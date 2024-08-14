#ifndef APPCONTROLREPLYPROCESSOR_H
#define APPCONTROLREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ApplicationProhibitionPack.pb.h"


namespace s2c
{
    class AppControlReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AppControlReplyProcessor_p, 
        Models::AppControlReplyPack,
        CMD_ENCRYPT
    > 
    AppControlReplyProcessor;
}

namespace s2c
{
    class AppControlReplyWhiteProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AppControlReplyWhiteProcessor_p, 
        Models::AppControlReplyPack,
        CMD_ENCRYPT
    > 
    AppControlReplyWhiteProcessor;
}

#endif // APPCONTROLREQUESTPROCESSOR_H

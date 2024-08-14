#ifndef ADMINCODEREPLYPROCESSOR_H
#define ADMINCODEREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/AdminCodePack.pb.h"


namespace ss2cs
{
    class AdminCodeReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AdminCodeReplyProcessor_p, 
        Models::AdminCodeReplyPack,
        CMD_ENCRYPT
    > 
    AdminCodeReplyProcessor;
}




#endif // ADMINCODEREPLYPROCESSOR_H

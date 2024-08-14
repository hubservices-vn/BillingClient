#ifndef USERLOGOUTREPLYPROCESSOR_H
#define USERLOGOUTREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\UserLogoutPack.pb.h"

namespace s2c
{
    class UserLogoutReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        UserLogoutReplyProcessor_p, 
        Models::UserLogoutReplyPack,
        CMD_ENCRYPT
    >
    UserLogoutReplyProcessor;
}

#endif // USERLOGOUTREPLYPROCESSOR_H

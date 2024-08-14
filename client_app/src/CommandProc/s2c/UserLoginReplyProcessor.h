#ifndef USERLOGINREPLYPROCESSOR_H
#define USERLOGINREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\UserLoginPack.pb.h"

namespace s2c
{
    class UserLoginReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        UserLoginReplyProcessor_p, 
        Models::UserLoginReplyPack,
        CMD_ENCRYPT
    >
    UserLoginReplyProcessor;
}


#endif // USERLOGINREPLYPROCESSOR_H

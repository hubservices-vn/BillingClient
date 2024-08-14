#ifndef USERLOGOUTPROCESSOR_H
#define USERLOGOUTPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\UserLogoutPack.pb.h"

namespace cc2cs
{
    class UserLogoutProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        UserLogoutProcessor_p, 
        Models::UserLogoutPack,
        CMD_ENCRYPT
    > 
    UserLogoutProcessorC;
}

#endif // USERLOGOUTPROCESSOR_H

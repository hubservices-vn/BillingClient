#ifndef USERLOGINPROCESSOR_H
#define USERLOGINPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/UserLoginPack.pb.h"


namespace cl2cs
{
    class UserLoginProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        UserLoginProcessor_p, 
        Models::UserLoginPack,
        CMD_ENCRYPT
    > 
    UserLoginProcessorC;
}


#endif // USERLOGINPROCESSOR_H

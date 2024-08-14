#ifndef ACCOUNTAUTHREPLYPROCESSOR_H
#define ACCOUNTAUTHREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/UserLoginPack.pb.h"


namespace ss2cs
{
    class AccountAuthReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AccountAuthReplyProcessor_p, 
        Models::AccountAuthReplyPack,
        CMD_ENCRYPT
    > 
    AccountAuthReplyProcessor;
}



#endif // ACCOUNTAUTHREPLYPROCESSOR_H

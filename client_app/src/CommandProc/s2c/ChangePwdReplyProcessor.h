#ifndef CHANGEPWDREPLYPROCESSOR_H
#define CHANGEPWDREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ChangePwdPack.pb.h"


namespace s2c
{
    class ChangePwdReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ChangePwdReplyProcessor_p, 
        Models::ChangePwdReplyPack,
        CMD_ENCRYPT
    > 
    ChangePwdReplyProcessor;
}


#endif // CHANGEPWDREPLYPROCESSOR_H

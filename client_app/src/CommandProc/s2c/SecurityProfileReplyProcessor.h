#ifndef SECURITYPROFILEREPLYPROCESSOR_H
#define SECURITYPROFILEREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\SecurityProfilePack.pb.h"


namespace s2c
{
    class SecurityProfileReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        SecurityProfileReplyProcessor_p, 
        Models::SecuProfileReplyPack,
        CMD_ENCRYPT
    > 
    SecurityProfileReplyProcessor;
}


#endif // SECURITYPROFILEREPLYPROCESSOR_H

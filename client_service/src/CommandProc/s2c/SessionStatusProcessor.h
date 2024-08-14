#ifndef SESSIONSTATUSPROCESSOR_H
#define SESSIONSTATUSPROCESSOR_H
#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\SessionStatusPack.pb.h"

namespace s2c
{
    class SessionStatusProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        SessionStatusProcessor_p, 
        Models::SessionStatusPack,
        CMD_ENCRYPT
    > 
    SessionStatusProcessor;
}

#endif // SESSIONSTATUSPROCESSOR_H

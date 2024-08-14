#ifndef SESSIONBEGINPROCESSOR_H
#define SESSIONBEGINPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\SessionPack.pb.h"

namespace s2c
{
    class SessionBeginProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        SessionBeginProcessor_p, 
        Models::SessionBeginPack,
        CMD_ENCRYPT
    > 
    SessionBeginProcessor;
}

#endif // SESSIONBEGINPROCESSOR_H

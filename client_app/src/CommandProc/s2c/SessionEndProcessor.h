#ifndef SESSIONENDPROCESSOR_H
#define SESSIONENDPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"

namespace Models {
    class SessionEndPack;
}

namespace s2c
{
    class SessionEndProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        SessionEndProcessor_p, 
        Models::SessionEndPack,
        CMD_ENCRYPT
    > 
    SessionEndProcessor;
}

#endif // SESSIONENDPROCESSOR_H

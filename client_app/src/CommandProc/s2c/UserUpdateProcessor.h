#ifndef USERUPDATEPROCESSOR_H
#define USERUPDATEPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\UserUpdatePack.pb.h"

namespace s2c
{
    class UserUpdateProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        UserUpdateProcessor_p, 
        Models::UserUpdatePack,
        CMD_ENCRYPT
    >
    UserUpdateProcessor;
}

#endif // USERUPDATEPROCESSOR_H

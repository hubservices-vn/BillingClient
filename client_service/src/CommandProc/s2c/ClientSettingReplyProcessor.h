#ifndef CLIENTSETTINGREPLYPROCESSOR_H
#define CLIENTSETTINGREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\ClientSettingPack.pb.h"


namespace s2c
{
    class ClientSettingReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientSettingReplyProcessor_p, 
        Models::SystemSettingReplyPack,
        CMD_ENCRYPT
    > 
    ClientSettingReplyProcessor;
}


#endif // CLIENTSETTINGREPLYPROCESSOR_H

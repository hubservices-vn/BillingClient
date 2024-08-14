#ifndef UPDATECLIENTREQUESTPROCESSOR_H
#define UPDATECLIENTREQUESTPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/UpdateClientPack.pb.h"

namespace s2c
{
    class UpdateClientRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        UpdateClientRequestProcessor_p, 
        Models::UpdateClientRequestPack,
        CMD_ENCRYPT
    > 
    UpdateClientRequestProcessor;
}



#endif // UPDATECLIENTREQUESTPROCESSOR_H

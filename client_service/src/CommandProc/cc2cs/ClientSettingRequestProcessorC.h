#ifndef CLIENTSETTINGREQUESTPROCESSORC_H
#define CLIENTSETTINGREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientSettingPack.pb.h"

namespace cc2cs
{
    class ClientSettingRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientSettingRequestProcessorC_p, 
        Models::SystemSettingRequestPack,
        CMD_ENCRYPT
    > 
    ClientSettingRequestProcessorC;
}


#endif // CLIENTSETTINGREQUESTPROCESSORC_H

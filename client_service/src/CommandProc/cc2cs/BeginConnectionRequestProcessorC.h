#ifndef BEGINCONNECTIONREQUESTPROCESSORC_H
#define BEGINCONNECTIONREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientConnectionPack.pb.h"

namespace cc2cs
{
    class BeginConnectionRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        BeginConnectionRequestProcessorC_p, 
        Models::BeginConnectionRequestPack,
        CMD_ENCRYPT
    > 
    BeginConnectionRequestProcessorC;
}



#endif // BEGINCONNECTIONREQUESTPROCESSORC_H

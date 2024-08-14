#ifndef ENDCONNECTIONREQUESTPROCESSORC_H
#define ENDCONNECTIONREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientConnectionPack.pb.h"

namespace cc2cs
{
    class EndConnectionRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        EndConnectionRequestProcessorC_p, 
        Models::EndConnectionRequestPack,
        CMD_ENCRYPT
    > 
    EndConnectionRequestProcessorC;
}



#endif // ENDCONNECTIONREQUESTPROCESSORC_H

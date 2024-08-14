#ifndef CLHANDSHAKEPROCESSORC_H
#define CLHANDSHAKEPROCESSORC_H


#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\CLHandshakePack.pb.h"

namespace cl2cs
{
    class CLHandshakeProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CLHandshakeProcessorC_p , 
        Models::CLHandshakePack ,
        CMD_ENCRYPT
    > 
    CLHandshakeProcessorC;
}


#endif // CLHANDSHAKEPROCESSORC_H

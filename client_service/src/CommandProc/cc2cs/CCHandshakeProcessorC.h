#ifndef CCHANDSHAKEPROCESSORC_H
#define CCHANDSHAKEPROCESSORC_H


#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\CCHandshakePack.pb.h"

namespace cc2cs
{
    class CCHandshakeProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        CCHandshakeProcessorC_p , 
        Models::CCHandshakePack ,
        CMD_ENCRYPT
    > 
    CCHandshakeProcessorC;
}
#endif // CCHANDSHAKEPROCESSORC_H

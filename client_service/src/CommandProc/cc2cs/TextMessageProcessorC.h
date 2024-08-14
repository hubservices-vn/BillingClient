#ifndef TEXTMESSAGEPROCESSORC_H
#define TEXTMESSAGEPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/TextMessagePack.pb.h"

namespace cc2cs
{
    class TextMessageProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        TextMessageProcessorC_p, 
        Models::TextMessagePack,
        CMD_ENCRYPT
    > 
    TextMessageProcessorC;
}



#endif // TEXTMESSAGEPROCESSORC_H

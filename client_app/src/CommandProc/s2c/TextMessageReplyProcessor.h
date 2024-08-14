#ifndef TEXTMESSAGEREPLYPROCESSOR_H
#define TEXTMESSAGEREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/TextMessagePack.pb.h"

namespace s2c
{
    class TextMessageReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        TextMessageReplyProcessor_p, 
        Models::TextMessagePack,
        CMD_ENCRYPT
    > 
    TextMessageReplyProcessor;
}


#endif // TEXTMESSAGEREPLYPROCESSOR_H

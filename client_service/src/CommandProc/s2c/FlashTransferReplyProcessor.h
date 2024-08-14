#ifndef FLASHTRANSFERREPLYPROCESSOR_H
#define FLASHTRANSFERREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\FlashTransferPack.pb.h"

namespace s2c
{
    class FlashTransferReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        FlashTransferReplyProcessor_p , 
        Models::FlashTransferReplyPack,
        /*encrypt*/false
    > 
    FlashTransferReplyProcessor;
}


#endif // SCREENSHOTREQUESTPROCESSOR_H

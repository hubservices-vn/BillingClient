#ifndef FLASHTRANSFERREPLYPROCESSOR_H
#define FLASHTRANSFERREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\FlashTransferPack.pb.h"

namespace s2c
{
    class FlashTransferReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        FlashTransferReplyProcessorC_p , 
        Models::FlashTransferReplyPack,
        /*encrypt*/false
    > 
    FlashTransferReplyProcessorC;
}


#endif // SCREENSHOTREQUESTPROCESSOR_H

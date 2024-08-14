#ifndef FLASHTRANSFERREQUESTPROCESSORC_H
#define FLASHTRANSFERREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/FlashTransferPack.pb.h"


namespace cl2cs
{
    class FlashTransferRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        FlashTransferRequestProcessorC_p, 
        Models::FlashTransferRequestPack,
        CMD_ENCRYPT
    > 
    FlashTransferRequestProcessorC;
}



#endif // SCREENSHOTREPLYPROCESSORC_H

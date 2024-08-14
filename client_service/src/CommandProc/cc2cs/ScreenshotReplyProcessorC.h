#ifndef SCREENSHOTREPLYPROCESSORC_H
#define SCREENSHOTREPLYPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/CaptureScreenPack.pb.h"


namespace cc2cs
{
    class ScreenshotReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ScreenshotReplyProcessorC_p, 
        Models::ScreenCaptureReplyPack,
        /*encrypt*/false
    > 
    ScreenshotReplyProcessorC;
}



#endif // SCREENSHOTREPLYPROCESSORC_H

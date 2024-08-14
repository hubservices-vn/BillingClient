#ifndef SCREENSHOTREQUESTPROCESSOR_H
#define SCREENSHOTREQUESTPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\CaptureScreenPack.pb.h"


namespace s2c
{
    class ScreenshotRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ScreenshotRequestProcessor_p , 
        Models::ScreenCaptureRequestPack ,
        CMD_ENCRYPT
    > 
    ScreenshotRequestProcessor;
}


#endif // SCREENSHOTREQUESTPROCESSOR_H

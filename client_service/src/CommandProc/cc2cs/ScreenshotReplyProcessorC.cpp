#include "ScreenshotReplyProcessorC.h"
#include "ScreencaptureQtModel.h"
#include "SystemManager.h"


namespace cc2cs
{

    void ScreenshotReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ScreencaptureReplyQtModel qtModel;
        ScreencaptureReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procScreenCaptureReplyC(qtModel);
    }

}
#include "ScreenshotRequestProcessor.h"
#include "log.h"
#include "ScreencaptureQtModel.h"
#include "SystemManager.h"



namespace s2c
{

    void ScreenshotRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ScreencaptureRequestQtModel qtModel;
        ScreencaptureRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procScreenCaptureRequest(qtModel);
    }

}
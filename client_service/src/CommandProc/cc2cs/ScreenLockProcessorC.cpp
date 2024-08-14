#include "ScreenLockProcessorC.h"
#include "ScreenLockQtModel.h"
#include "SystemManager.h"
#include "log.h"

namespace cc2cs
{

    void ScreenLockRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ScreenLockRequestQtModel qtModel;
        ScreenLockRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procScreenLockRequest(qtModel);
    }

}

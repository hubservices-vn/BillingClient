#include "ScreenUnlockProcessorC.h"
#include "ScreenLockQtModel.h"
#include "SystemManager.h"
#include "log.h"

namespace cl2cs
{
    void ScreenUnlockRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ScreenUnlockRequestQtModel qtModel;
        ScreenUnlockRequestQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procScreenUnlockRequest(qtModel);
    }

}

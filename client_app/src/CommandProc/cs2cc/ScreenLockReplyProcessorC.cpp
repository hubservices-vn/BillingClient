#include "ScreenLockReplyProcessorC.h"
#include "SystemManager.h"
#include "ScreenLockQtModel.h"

namespace cs2cc
{

    void ScreenLockReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ScreenLockReplyQtModel qtModel;
        ScreenLockReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procScreenLockReply(qtModel);
    }

}

#include "ScreenUnlockReplyProcessorC.h"


#include "SystemManager.h"
#include "ScreenLockQtModel.h"

namespace cs2cl
{

    void ScreenUnlockReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ScreenUnlockReplyQtModel qtModel;
        ScreenUnlockReplyQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procScreenUnlockReply(qtModel);
    }

}


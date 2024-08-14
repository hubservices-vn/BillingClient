#include "FlashTransferReplyProcessor.h"
#include "FlashTransferQtModel.h"
#include "FlashManager.h"


namespace s2c
{
    void FlashTransferReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        FlashTransferReplyQtModel qtModel;
        FlashTransferReplyQtModel::fromProtoBuf(ptr, qtModel);

        FlashManager::GetInstance()->procFlashReplyC(qtModel);
    }

}
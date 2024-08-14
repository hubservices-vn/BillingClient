#include "AppControlReplyProcessor.h"
#include "log.h"
#include "AppControlQtModel.h"
#include "AppControlManager.h"


namespace s2c
{

    void AppControlReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        AppControlReplyQtModel qtModel;
        AppControlReplyQtModel::fromProtoBuf(ptr, qtModel);

        AppControlManager::GetInstance()->procAppControlReply(qtModel);
    }


    void AppControlReplyWhiteProcessor_p::Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr)
    {
        AppControlReplyQtModel qtModel;
        AppControlReplyQtModel::fromProtoBuf(ptr, qtModel);

        AppControlManager::GetInstance()->procAppControlWhiteReply(qtModel);
    }

}

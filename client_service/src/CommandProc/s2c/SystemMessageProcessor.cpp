#include "SystemMessageProcessor.h"
#include "SystemMessageQtModel.h"
#include "SystemManager.h"
#include "log.h"


namespace s2c
{
    void SystemMessageProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SystemMessageQtModel qtModel;
        SystemMessageQtModel::fromProtoBuf(ptr, qtModel);

        SystemManager::GetInstance()->procSystemMessage(qtModel);
    }


}

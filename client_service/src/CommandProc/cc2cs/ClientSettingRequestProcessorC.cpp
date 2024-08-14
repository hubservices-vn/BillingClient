#include "ClientSettingRequestProcessorC.h"
#include "SystemSettingQtModel.h"
#include "ClientSettingManager.h"

namespace cc2cs
{

    void ClientSettingRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SystemSettingRequestQtModel qtModel;
        SystemSettingRequestQtModel::fromProtoBuf(ptr, qtModel);

        ClientSettingManager::GetInstance()->procSystemSettingRequestC(qtModel);
    }

}
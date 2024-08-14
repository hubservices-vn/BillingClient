#include "UserLogoutProcessorC.h"
#include "UserLogoutQtModel.h"
#include "UserAccountManager.h"
#include "log.h"

namespace cc2cs
{
    void UserLogoutProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UserLogoutQtModel qtModel;
        UserLogoutQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procLogoutRequest(qtModel);
    }
}
#include "UserLoginProcessorC.h"
#include "UserLoginQtModel.h"
#include "UserAccountManager.h"
#include "log.h"


namespace cl2cs
{
    void UserLoginProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UserLoginRequestQtModel qtModel;
        UserLoginRequestQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procLoginRequest(qtModel);
    }

}
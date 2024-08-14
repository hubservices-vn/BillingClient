#include "UserUpdateProcessor.h"
#include "UserUpdateQtModel.h"
#include "UserAccountManager.h"
#include "log.h"


namespace s2c
{
    void UserUpdateProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UserUpdateQtModel qtModel;
        UserUpdateQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procUserUpdate(qtModel);
    }
}



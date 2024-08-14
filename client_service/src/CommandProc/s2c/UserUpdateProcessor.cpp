#include "UserUpdateProcessor.h"
#include "UserUpdateQtModel.h"
#include "UserAccountManager.h"
#include "log.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"

namespace s2c
{
    void UserUpdateProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        UserUpdateQtModel qtModel;
        UserUpdateQtModel::fromProtoBuf(ptr, qtModel);
        cs2ccSender::send(cs2cc::UserUpdateCommandC(qtModel));
    }
}



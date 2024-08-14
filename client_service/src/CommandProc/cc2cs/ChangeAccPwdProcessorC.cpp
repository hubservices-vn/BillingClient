#include "ChangeAccPwdProcessorC.h"
#include "log.h"
#include "ChangeAccPwdQtModel.h"
#include "ConfigManager.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"

namespace cc2cs
{
    void ChangeAccPwdProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ChangeAccPwdRequestQtModel qtModel;
        ChangeAccPwdRequestQtModel::fromProtoBuf(ptr, qtModel);

        qtModel.ClientId_ = CConfigManager::clientId();
        cs2ssSender::send(cs2ss::ChangeAccPwdRequestCommand(qtModel));
    }
}


#include "OfflineSessionReplyProcessor.h"
#include "OfflineSessionQtModel.h"
#include "OfflineSessionManager.h"


namespace ss2cs
{
    void OfflineSessionReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        OfflineSessionReplyQtModel qtModel;
        OfflineSessionReplyQtModel::fromProtoBuf(ptr, qtModel);

        OfflineSessionManager::GetInstance()->procOfflineSessionReply(qtModel);
    }

}

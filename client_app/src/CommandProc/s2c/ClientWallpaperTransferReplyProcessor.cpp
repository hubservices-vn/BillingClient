#include "ClientWallpaperTransferReplyProcessor.h"
#include "ClientWallpaperTransferQtModel.h"
#include "ClientWallpaperManager.h"

namespace s2c
{
    void ClientWallpaperCheckReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientWallpaperCheckReplyQtModel qtModel;
        ClientWallpaperCheckReplyQtModel ::fromProtoBuf(ptr, qtModel);

        ClientWallpaperManager::GetInstance()->procWallpaperCheckReply(qtModel);
    }


    void ClientWallpaperTransferReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientWallpaperTransferReplyQtModel qtModel;
        ClientWallpaperTransferReplyQtModel ::fromProtoBuf(ptr, qtModel);

        ClientWallpaperManager::GetInstance()->procWallpaperTransferReply(qtModel);
    }

}
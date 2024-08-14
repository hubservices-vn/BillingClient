#include "ClientWallpaperTransferReplyProcessor.h"
#include "ClientWallpaperTransferQtModel.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"

namespace s2c
{
    void ClientWallpaperCheckReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientWallpaperCheckReplyQtModel qtModel;
        ClientWallpaperCheckReplyQtModel::fromProtoBuf(ptr, qtModel);

        cs2ccSender::send(cs2cc::ClientWallpaperCheckReplyCommandC(qtModel));
    }

    void ClientWallpaperTransferReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientWallpaperTransferReplyQtModel qtModel;
        ClientWallpaperTransferReplyQtModel::fromProtoBuf(ptr, qtModel);

        cs2ccSender::send(cs2cc::ClientWallpaperTransferReplyCommandC(qtModel));
    }

}
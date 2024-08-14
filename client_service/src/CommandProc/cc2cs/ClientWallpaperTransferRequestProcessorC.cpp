#include "ClientWallpaperTransferRequestProcessorC.h"
#include "ClientWallpaperTransferQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ConfigManager.h"

namespace cc2cs
{
    void ClientWallpaperCheckRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientWallpaperCheckRequestQtModel qtModel;
        ClientWallpaperCheckRequestQtModel::fromProtoBuf(ptr, qtModel);
        qtModel.ClientId_ = CConfigManager::clientId();

        cs2ssSender::send(cs2ss::ClientWallpaperCheckRequestCommand(qtModel));
    }


    void ClientWallpaperTransferRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientWallpaperTransferRequestQtModel qtModel;
        ClientWallpaperTransferRequestQtModel::fromProtoBuf(ptr, qtModel);
        qtModel.ClientId_ = CConfigManager::clientId();

        cs2ssSender::send(cs2ss::ClientWallpaperTransferRequestCommand(qtModel));
    }

}
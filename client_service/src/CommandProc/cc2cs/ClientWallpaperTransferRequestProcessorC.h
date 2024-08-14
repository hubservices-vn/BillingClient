#ifndef CLIENT_WALLPAPER_TRANSFER_REQUEST_PROCESSOR_C_H
#define CLIENT_WALLPAPER_TRANSFER_REQUEST_PROCESSOR_C_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientWallpaperTransferPack.pb.h"


namespace cc2cs
{
    class ClientWallpaperCheckRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientWallpaperCheckRequestProcessorC_p, 
        Models::ClientWallpaperCheckRequestPack,
        CMD_ENCRYPT
    >
    ClientWallpaperCheckRequestProcessorC;



    class ClientWallpaperTransferRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientWallpaperTransferRequestProcessorC_p, 
        Models::ClientWallpaperTransferRequestPack,
        CMD_ENCRYPT
    > 
    ClientWallpaperTransferRequestProcessorC;
}



#endif // CLIENT_WALLPAPER_TRANSFER_REQUEST_PROCESSOR_C_H

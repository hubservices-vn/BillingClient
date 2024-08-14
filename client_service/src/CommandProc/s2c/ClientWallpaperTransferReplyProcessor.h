#ifndef CLIENT_WALLPAPER_TRANSFER_REPLY_PROCESSOR_H
#define CLIENT_WALLPAPER_TRANSFER_REPLY_PROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ClientWallpaperTransferPack.pb.h"

namespace s2c
{
    class ClientWallpaperCheckReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientWallpaperCheckReplyProcessor_p , 
        Models::ClientWallpaperCheckReplyPack,
        /*encrypt*/CMD_ENCRYPT
    > 
    ClientWallpaperCheckReplyProcessor;



    class ClientWallpaperTransferReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ClientWallpaperTransferReplyProcessor_p , 
        Models::ClientWallpaperTransferReplyPack,
        /*encrypt*/false
    > 
    ClientWallpaperTransferReplyProcessor;
}


#endif // CLIENTADTRANSFERREPLYPROCESSOR_H

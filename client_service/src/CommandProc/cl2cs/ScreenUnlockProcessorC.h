#ifndef SCREEN_UNLOCK_PROCESSORC_H
#define SCREEN_UNLOCK_PROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ScreenLockPack.pb.h"


namespace cl2cs
{
    class ScreenUnlockRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ScreenUnlockRequestProcessorC_p, 
        Models::ScreenUnlockRequestPack,
        CMD_ENCRYPT
    >
    ScreenUnlockRequestProcessorC;
}



#endif // SCREEN_UNLOCK_PROCESSORC_H

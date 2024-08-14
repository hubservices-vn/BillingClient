#ifndef SCREENLOCK_PROCESSORC_H
#define SCREENLOCK_PROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ScreenLockPack.pb.h"


namespace cc2cs
{
    class ScreenLockRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ScreenLockRequestProcessorC_p, 
        Models::ScreenLockRequestPack,
        CMD_ENCRYPT
    > 
    ScreenLockRequestProcessorC;
}



#endif // SCREENLOCK_PROCESSORC_H

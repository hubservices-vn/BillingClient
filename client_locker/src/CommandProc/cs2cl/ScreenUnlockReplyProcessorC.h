#ifndef SCREENUNLOCKREPLYPROCESSORC_H
#define SCREENUNLOCKREPLYPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ScreenLockPack.pb.h"


namespace cs2cl
{
    class ScreenUnlockReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ScreenUnlockReplyProcessorC_p , 
        Models::ScreenUnlockReplyPack ,
        CMD_ENCRYPT
    >
    ScreenUnlockReplyProcessorC;
}

#endif // SCREENUNLOCKREPLYPROCESSORC_H

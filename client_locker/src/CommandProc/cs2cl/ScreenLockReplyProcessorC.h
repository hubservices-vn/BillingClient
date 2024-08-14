#ifndef SCREENLOCKREPLYPROCESSORC_H
#define SCREENLOCKREPLYPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ScreenLockPack.pb.h"


namespace cs2cl
{
    class ScreenLockReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ScreenLockReplyProcessorC_p, 
        Models::ScreenLockReplyPack,
        CMD_ENCRYPT
    >
    ScreenLockReplyProcessorC;
}


#endif // SCREENLOCKREPLYPROCESSORC_H

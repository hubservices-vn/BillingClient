#ifndef CHANGEACCPWDPROCESSORC_H
#define CHANGEACCPWDPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ChangePwdPack.pb.h"

namespace cc2cs
{
    class ChangeAccPwdProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        ChangeAccPwdProcessorC_p, 
        Models::ChangePwdRequestPack,
        CMD_ENCRYPT
    > 
    ChangeAccPwdProcessorC;
}



#endif // CHANGEACCPWDPROCESSORC_H

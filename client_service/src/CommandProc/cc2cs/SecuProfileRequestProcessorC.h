#ifndef SECUPROFILEREQUESTPROCESSORC_H
#define SECUPROFILEREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/SecurityProfilePack.pb.h"

namespace cc2cs
{
    class SecuProfileRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        SecuProfileRequestProcessorC_p,
        Models::SecuProfileRequestPack,
        CMD_ENCRYPT
    >
    SecuProfileRequestProcessorC;
}


#endif // SECUPROFILEREQUESTPROCESSORC_H

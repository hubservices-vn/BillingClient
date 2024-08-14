#ifndef OFFLINESESSIONREPLYPROCESSOR_H
#define OFFLINESESSIONREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\OfflineSessionPack.pb.h"

namespace ss2cs
{
    class OfflineSessionReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        OfflineSessionReplyProcessor_p, 
        Models::OfflineSessionReplyPack,
        CMD_ENCRYPT
    > 
    OfflineSessionReplyProcessor;
}




#endif // OFFLINESESSIONREPLYPROCESSOR_H

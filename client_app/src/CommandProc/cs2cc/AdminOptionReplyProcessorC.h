#ifndef ADMINOPTIONREPLYPROCESSORC_H
#define ADMINOPTIONREPLYPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/AdminOptionPack.pb.h"


namespace cs2cc
{
    class AdminOptionReplyProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AdminOptionReplyProcessorC_p, 
        Models::AdminOptionReplyPack,
        CMD_ENCRYPT
    >
    AdminOptionReplyProcessorC;
}



#endif // ADMINOPTIONREPLYPROCESSORC_H

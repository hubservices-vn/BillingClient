#ifndef ADMINOPTIONREQUESTPROCESSORC_H
#define ADMINOPTIONREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/AdminOptionPack.pb.h"

namespace cc2cs
{
    class AdminOptionRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AdminOptionRequestProcessorC_p, 
        Models::AdminOptionRequestPack,
        CMD_ENCRYPT
    > 
    AdminOptionRequestProcessorC;
}



#endif // ADMINOPTIONREQUESTPROCESSORC_H

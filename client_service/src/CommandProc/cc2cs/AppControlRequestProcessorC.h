#ifndef APPCONTROLREQUESTPROCESSORC_H
#define APPCONTROLREQUESTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/ApplicationProhibitionPack.pb.h"

namespace cc2cs
{
    class AppControlRequestProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        AppControlRequestProcessorC_p, 
        Models::AppControlRequestPack,
        CMD_ENCRYPT
    > 
    AppControlRequestProcessorC;
}


#endif // APPCONTROLREQUESTPROCESSORC_H

#ifndef WEBCONTROLREPLYPROCESSOR_H
#define WEBCONTROLREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\WebControlPack.pb.h"

namespace ss2cs
{
    class WebControlReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        WebControlReplyProcessor_p, 
        Models::WebControlReplyPack,
        /*encrypt*/false
    > 
    WebControlReplyProcessor;
}



#endif // WEBCONTROLREPLYPROCESSOR_H

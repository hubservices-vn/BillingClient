#include "OrderReplyProcessor.h"
#include "log.h"



namespace ss2cs
{

    // we don't handle order reply for now
    void OrderReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        //OrderReplyQtModel qtModel;
        //fromProtoBuf(ptr, qtModel);
    }

}

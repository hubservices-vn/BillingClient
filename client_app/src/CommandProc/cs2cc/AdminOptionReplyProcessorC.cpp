#include "AdminOptionReplyProcessorC.h"
#include "AdminOptionQtModelC.h"
#include "log.h"
#include "AdminOptionManager.h"

namespace cs2cc
{

    void AdminOptionReplyProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        AdminOptionReplyQtModelC qtModel;
        AdminOptionReplyQtModelC::fromProtoBuf(ptr, qtModel);

        AdminOptionManager::GetInstance()->procAdminOptionReply(qtModel);
    }

}

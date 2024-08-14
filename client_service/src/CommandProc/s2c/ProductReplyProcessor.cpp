#include "ProductReplyProcessor.h"
#include "log.h"
#include "ProductQtModel.h"
#include "ProductManager.h"

namespace s2c
{

    void ProductReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ProductReplyQtModel qtModel;
        ProductReplyQtModel::fromProtoBuf(ptr, qtModel);

        ProductManager::GetInstance()->procProdReply(qtModel);
    }

}

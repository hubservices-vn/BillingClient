#include "ProductQueryProcessorC.h"
#include "ProductQtModel.h"
#include "ProductManager.h"
#include "log.h"


namespace cc2cs
{

    void ProductQueryProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ProductRequestQtModel qtModel;
        ProductRequestQtModel::fromProtoBuf(ptr, qtModel);

        ProductManager::GetInstance()->procProdRequestC(qtModel);
    }

}
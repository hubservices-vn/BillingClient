#include "OrderRequestProcessorC.h"
#include "OrderQtModel.h"
#include "OrderManager.h"
#include "log.h"


namespace cc2cs
{

    void OrderRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        OrderRequestQtModel qtModel;
        OrderRequestQtModel::fromProtoBuf(ptr, qtModel);

        OrderManager::GetInstance()->procOrderRequestC(qtModel);
    }

}
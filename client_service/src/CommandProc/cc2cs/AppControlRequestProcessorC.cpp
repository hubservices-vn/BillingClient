#include "AppControlRequestProcessorC.h"
#include "AppControlQtModel.h"
#include "AppControlManager.h"


namespace cc2cs
{

    void AppControlRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        AppControlRequestQtModel qtModel;
        AppControlRequestQtModel::fromProtoBuf(ptr, qtModel);

        AppControlManager::GetInstance()->procAppControlRequestC(qtModel);
    }

}
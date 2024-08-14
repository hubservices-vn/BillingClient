#include "SecuProfileRequestProcessorC.h"
#include "SecurityProfileQtModel.h"
#include "SecurityProfileManager.h"

namespace cc2cs
{

    void SecuProfileRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        SecuProfileRequestQtModel qtModel;
        SecuProfileRequestQtModel::fromProtoBuf(ptr, qtModel);

        SecurityProfileManager::GetInstance()->procSecuProfileRequestC(qtModel);
    }

}
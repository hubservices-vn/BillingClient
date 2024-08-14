#include "AdminOptionRequestProcessorC.h"
#include "log.h"
#include "AdminOptionQtModelC.h"
#include "ClientSettingManager.h"


namespace cc2cs
{

    void AdminOptionRequestProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        AdminOptionRequestQtModelC qtModel;
        AdminOptionRequestQtModelC::fromProtoBuf(ptr, qtModel);

        ClientSettingManager::GetInstance()->procAdminOptionChangeRequest(qtModel);
    }

}
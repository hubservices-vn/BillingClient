#include "WebHistoryReportProcessorC.h"
#include "WebHistoryManager.h"
#include "WebHistoryQtModel.h"


namespace cc2cs
{

    void WebHistoryReportProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        WebHistoryReportQtModel qtModel;
        WebHistoryReportQtModel::fromProtoBuf(ptr, qtModel);

        WebHistoryManager::GetInstance()->procWebHistoryReportC(qtModel);
    }

}

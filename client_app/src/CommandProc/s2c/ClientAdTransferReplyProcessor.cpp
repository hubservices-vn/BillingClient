#include "ClientAdTransferReplyProcessor.h"
#include "ClientAdTransferQtModel.h"
#include "cc2csNetworkManager.h"
#include "ClientAdManager.h"

namespace s2c
{

    void ClientAdTransferReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientAdTransferReplyQtModel qtModel;
        ClientAdTransferReplyQtModel ::fromProtoBuf(ptr, qtModel);


		bool result = QMetaObject::invokeMethod(
			ClientAdManager::GetInstance(),
			"procClientAdReplyC",
			Qt::QueuedConnection,
			Q_ARG(ClientAdTransferReplyQtModel, qtModel));

    }

}
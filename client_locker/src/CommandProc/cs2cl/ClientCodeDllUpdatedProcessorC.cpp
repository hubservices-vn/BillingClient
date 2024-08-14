#include "ClientCodeDllUpdatedProcessorC.h"
#include "ClientCodeDllUpdatedQtModel.h"
//#include "cc2csNetworkManager.h"
#include "ClientEventManager.h"

namespace cs2cl
{

    void ClientCodeDllUpdatedProcessorC_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        ClientCodeDllUpdatedQtModel qtModel;
        ClientCodeDllUpdatedQtModel ::fromProtoBuf(ptr, qtModel);

		/*QMetaObject::invokeMethod(
			ClientEventManager::GetInstance(),
			"onLogin");*/

		bool result = QMetaObject::invokeMethod(
			ClientEventManager::GetInstance(),
			"clientCodeDllUpdated",
			Qt::QueuedConnection,
			Q_ARG(ClientCodeDllUpdatedQtModel, qtModel));

    }

}
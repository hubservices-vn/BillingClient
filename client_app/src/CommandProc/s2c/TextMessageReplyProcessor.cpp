#include "TextMessageReplyProcessor.h"
#include "log.h"
#include "TextMessageQtModel.h"
#include "TextMessageManager.h"



namespace s2c
{

    void TextMessageReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        TextMessageQtModel qtModel;
        TextMessageQtModel::fromProtoBuf(ptr, qtModel);

        QMetaObject::invokeMethod(
            TextMessageManager::GetInstance(),
            "procTextMsgRecved",
            Qt::QueuedConnection,
            Q_ARG(TextMessageQtModel, qtModel));
    }

}
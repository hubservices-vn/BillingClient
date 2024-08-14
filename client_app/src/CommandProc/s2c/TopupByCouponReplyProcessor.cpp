#include "TopupByCouponReplyProcessor.h"
#include "TopupByCouponQtModel.h"
#include "UserAccountManager.h"
#include "log.h"


namespace s2c
{
    void TopupByCouponReplyProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        TopupByCouponReplyQtModel qtModel;
        TopupByCouponReplyQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procTopupByCouponReply(qtModel);
    }
}



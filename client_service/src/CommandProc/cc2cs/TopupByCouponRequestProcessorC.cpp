#include "TopupByCouponRequestProcessorC.h"
#include "TopupByCouponQtModel.h"
#include "UserAccountManager.h"
#include "log.h"

namespace cc2cs
{
    void TopupByCouponRequestProcessor_p::Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr )
    {
        TopupByCouponRequestQtModel qtModel;
        TopupByCouponRequestQtModel::fromProtoBuf(ptr, qtModel);

        UserAccountManager::GetInstance()->procTopupByCouponRequest(qtModel);
    }
}
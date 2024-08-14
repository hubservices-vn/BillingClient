#ifndef TOPUPBYCOUPONREQUESTPROCESSOR_H
#define TOPUPBYCOUPONREQUESTPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\TopupByCouponPack.pb.h"

namespace cc2cs
{
    class TopupByCouponRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        TopupByCouponRequestProcessor_p, 
        Models::TopupByCouponRequestPack,
        CMD_ENCRYPT
    > 
    TopupByCouponRequestProcessorC;
}

#endif // TOPUPBYCOUPONREQUESTPROCESSOR_H

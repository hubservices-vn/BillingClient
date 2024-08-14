#ifndef TOPUPBYCOUPONREPLYPROCESSOR_H
#define TOPUPBYCOUPONREPLYPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc\TopupByCouponPack.pb.h"

namespace s2c
{
    class TopupByCouponReplyProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        TopupByCouponReplyProcessor_p, 
        Models::TopupByCouponReplyPack,
        CMD_ENCRYPT
    >
    TopupByCouponReplyProcessor;
}

#endif // TOPUPBYCOUPONREPLYPROCESSOR_H

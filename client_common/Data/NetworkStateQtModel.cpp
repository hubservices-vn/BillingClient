#include "NetworkStateQtModel.h"
#include "Utils.h"
#include <sstream>
#include "pbc/NetworkStatePack.pb.h"
#include "NetworkStatus.h"
#include "log.h"

std::string NetworkStateQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "NetworkStateRequest";
    return stream.str();
}

ProtobufMsgPtr_s NetworkStateQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::NetworkStatePack> pbPtr(new Models::NetworkStatePack);
    return pbPtr;
}

void NetworkStateQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, NetworkStateQtModel& qtModel )
{
    Models::NetworkStatePack *pack =
        static_cast<Models::NetworkStatePack*>(pbPtr.data());

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}



std::string NetworkStateReplyQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "NetworkStateReply"
        << ". ClientId: " << ClientId
        << ". State: " << qPrintable(NetworkResulttoQString(State));
    return stream.str();
}

ProtobufMsgPtr_s NetworkStateReplyQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::NetworkStateReplyPack>
        ptr(new Models::NetworkStateReplyPack);
    ptr->set_clientid(ClientId);
    ptr->set_state((int)State);
    return ptr;
}

void NetworkStateReplyQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, NetworkStateReplyQtModel& qtModel )
{
    Models::NetworkStateReplyPack *pack =
        static_cast<Models::NetworkStateReplyPack*>(pbPtr.data());
    qtModel.ClientId = pack->clientid();
    qtModel.State = (NetworkResult)pack->state();

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}

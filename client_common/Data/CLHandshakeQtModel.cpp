#include "CLHandshakeQtModel.h"
#include "pbc/CLHandshakePack.pb.h"

#include "Utils.h"
#include <sstream>
#include "log.h"
#include "ConfigManager.h"


std::string CLHandshakeQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "CLHandshake"
        << ". Ver: " << qPrintable(Version_)
        << ". TS: " << Timestamp_;
    return stream.str();
}

ProtobufMsgPtr_s CLHandshakeQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::CLHandshakePack> pbPtr(new Models::CLHandshakePack);
    
    pbPtr->set_ver(Utils::QStringToUtf8(Version_));
    QString tsStr = QString::number(Timestamp_);
    QString tsStrEnc = Utils::symEncrypt(tsStr);
    pbPtr->set_timestamp(Utils::QStringToUtf8(tsStrEnc));
    return pbPtr;
}

void CLHandshakeQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, CLHandshakeQtModel& qtModel )
{
    Models::CLHandshakePack *pack =
        static_cast<Models::CLHandshakePack*>(pbPtr.data());

    qtModel.Version_ = Utils::Utf8ToQString(pack->ver());
    QString tsStrEnc = Utils::Utf8ToQString(pack->timestamp());
    QString tsStr = Utils::symDecrypt(tsStrEnc);
    qtModel.Timestamp_ = tsStr.toLongLong();

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}




std::string CLHandshakeAckQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "CLHandshakeAck"
        << ". ClientId:" << ClientId_;
    return stream.str();
}

ProtobufMsgPtr_s CLHandshakeAckQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::CLHandshakeAckPack> pbPtr(new Models::CLHandshakeAckPack);

    pbPtr->set_client_id(CConfigManager::clientId());
    return pbPtr;
}

void CLHandshakeAckQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, CLHandshakeAckQtModel& qtModel )
{
    Models::CLHandshakeAckPack *pack =
        static_cast<Models::CLHandshakeAckPack*>(pbPtr.data());

    qtModel.ClientId_ = pack->client_id();
    
    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}

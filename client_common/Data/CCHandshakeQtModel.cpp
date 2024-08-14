#include "CCHandshakeQtModel.h"
#include "pbc\CCHandshakePack.pb.h"

#include "Utils.h"
#include <sstream>
#include "log.h"
#include "ConfigManager.h"

std::string CCHandshakeQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "CC_Handshake"
        << ". Ver: " << qPrintable(Version_)
        << ". TS: " << Timestamp_;
    return stream.str();
}

ProtobufMsgPtr_s CCHandshakeQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::CCHandshakePack> 
        pbPtr(new Models::CCHandshakePack);

    pbPtr->set_ver(Utils::QStringToUtf8(Version_));
    QString tsStr = QString::number(Timestamp_);
    QString tsStrEnc = Utils::symEncrypt(tsStr);
    pbPtr->set_timestamp(Utils::QStringToUtf8(tsStrEnc));
    return pbPtr;
}

void CCHandshakeQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, CCHandshakeQtModel& qtModel )
{
    Models::CCHandshakePack *pack =
        static_cast<Models::CCHandshakePack*>(pbPtr.data());

    qtModel.Version_ = Utils::Utf8ToQString(pack->ver());
    QString tsStrEnc = Utils::Utf8ToQString(pack->timestamp());
    QString tsStr = Utils::symDecrypt(tsStrEnc);
    qtModel.Timestamp_ = tsStr.toLongLong();

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}



std::string CCHandshakeAckQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "CCHandshakeAck"
        << ". ClientId:" << ClientId_;
    return stream.str();
}

ProtobufMsgPtr_s CCHandshakeAckQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::CCHandshakeAckPack> pbPtr(new Models::CCHandshakeAckPack);

    pbPtr->set_client_id(CConfigManager::clientId());
    return pbPtr;
}

void CCHandshakeAckQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, CCHandshakeAckQtModel& qtModel )
{
    Models::CCHandshakeAckPack *pack =
        static_cast<Models::CCHandshakeAckPack*>(pbPtr.data());

    qtModel.ClientId_ = pack->client_id();

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}
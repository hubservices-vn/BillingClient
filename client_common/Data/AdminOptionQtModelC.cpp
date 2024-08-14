#include "AdminOptionQtModelC.h"
#include "Utils.h"
#include <sstream>
#include "pbc/AdminOptionPack.pb.h"
#include "log.h"

std::string AdminOptionRequestQtModelC::toStdString() const
{
    std::stringstream stream;
    stream << "AdminOption"
        << ". OpType_" << OpType_
        << ". ServerAddress_" << qPrintable(ServerAddress_)
        << ". NewAdminName_" << qPrintable(NewAdminName_)
        << ". OldAdminPwdEnc_" << qPrintable(Utils::symEncrypt(OldAdminPwdRaw_))
        << ". NewAdminPwdEnc_" << qPrintable(Utils::symEncrypt(NewAdminPwdRaw_));
    return stream.str();
}

ProtobufMsgPtr_s AdminOptionRequestQtModelC::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::AdminOptionRequestPack> pbPtr(new Models::AdminOptionRequestPack);
    pbPtr->set_type(OpType_);
    pbPtr->set_server_address(ServerAddress_.toStdString());
    pbPtr->set_new_admin_name(Utils::QStringToUtf8(NewAdminName_));
    pbPtr->set_old_admin_pwd_enc(Utils::QStringToUtf8(Utils::symEncrypt(OldAdminPwdRaw_)));
    pbPtr->set_new_admin_pwd_enc(Utils::QStringToUtf8(Utils::symEncrypt(NewAdminPwdRaw_)));
    return pbPtr;
}

void AdminOptionRequestQtModelC::fromProtoBuf( ProtobufMsgPtr_s pbPtr, AdminOptionRequestQtModelC& qtModel )
{
    Models::AdminOptionRequestPack *pack =
        static_cast<Models::AdminOptionRequestPack*>(pbPtr.data());
    qtModel.OpType_ = pack->type();
    qtModel.ServerAddress_ = QString::fromStdString(pack->server_address());
    qtModel.NewAdminName_ = Utils::Utf8ToQString(pack->new_admin_name());
    qtModel.OldAdminPwdRaw_ = Utils::symDecrypt(Utils::Utf8ToQString(pack->old_admin_pwd_enc()));
    qtModel.NewAdminPwdRaw_ = Utils::symDecrypt(Utils::Utf8ToQString(pack->new_admin_pwd_enc()));

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}

std::string AdminOptionReplyQtModelC::toStdString() const
{
    std::stringstream stream;
    stream << "AdminOptionReply"
        << ". OpType_" << OpType_
        << ". Result_" << Result_
        << ". ServerAddress_" << qPrintable(ServerAddress_)
        << ". NewAdminName_" << qPrintable(NewAdminName_);
    return stream.str();
}

ProtobufMsgPtr_s AdminOptionReplyQtModelC::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::AdminOptionReplyPack> pbPtr(new Models::AdminOptionReplyPack);
    pbPtr->set_type(OpType_);
    pbPtr->set_result((int)Result_);
    pbPtr->set_server_address(ServerAddress_.toStdString());
    pbPtr->set_new_admin_name(Utils::QStringToUtf8(NewAdminName_));

    return pbPtr;
}

void AdminOptionReplyQtModelC::fromProtoBuf( ProtobufMsgPtr_s pbPtr, AdminOptionReplyQtModelC& qtModel )
{
    Models::AdminOptionReplyPack *pack =
        static_cast<Models::AdminOptionReplyPack*>(pbPtr.data());
    qtModel.OpType_ = pack->type();
    qtModel.Result_ = (AdminOperationResult)pack->result();
    qtModel.ServerAddress_ = QString::fromStdString(pack->server_address());
    qtModel.NewAdminName_ = Utils::Utf8ToQString(pack->new_admin_name());

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}

#include "ScreenLockQtModel.h"
#include <sstream>
#include "pbc/ScreenLockPack.pb.h"
#include "Utils.h"
#include "log.h"

std::string ScreenLockRequestQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "ScreenLockRequest"
        << " .Mode_: " << Mode_
        << " .PasswordEnc_: " << qPrintable(Utils::symEncrypt(Password_))
        << " .AccountName_: " << qPrintable(AccountName_)
        << " .AccountPwdEnc_: " << qPrintable(Utils::symEncrypt(AccountPwdRaw_));
    return stream.str();
}


ProtobufMsgPtr_s ScreenLockRequestQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::ScreenLockRequestPack> pbPtr(new Models::ScreenLockRequestPack);
    pbPtr->set_mode(Mode_);
    pbPtr->set_pwd(Utils::QStringToUtf8(Password_));
    pbPtr->set_acc_name(Utils::QStringToUtf8(AccountName_));
    pbPtr->set_acc_pwd_raw(Utils::QStringToUtf8(AccountPwdRaw_));
    return pbPtr;
}

void ScreenLockRequestQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, ScreenLockRequestQtModel& qtModel )
{
    Models::ScreenLockRequestPack *pack =
        static_cast<Models::ScreenLockRequestPack*>(pbPtr.data());
    qtModel.Mode_ = (ScreenLockMode)pack->mode();
    qtModel.Password_ = Utils::Utf8ToQString(pack->pwd());
    qtModel.AccountName_ = Utils::Utf8ToQString(pack->acc_name());
    qtModel.AccountPwdRaw_ = Utils::Utf8ToQString(pack->acc_pwd_raw());

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}

std::string ScreenLockReplyQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "ScreenLockReply"
        << " .Mode_: " << Mode_
        << " .Result_: " << Result_;
    return stream.str();
}

ProtobufMsgPtr_s ScreenLockReplyQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::ScreenLockReplyPack> pbPtr(new Models::ScreenLockReplyPack);
    pbPtr->set_mode(Mode_);
    pbPtr->set_result(Result_);
    return pbPtr;
}

void ScreenLockReplyQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, ScreenLockReplyQtModel& qtModel )
{
    Models::ScreenLockReplyPack *pack =
        static_cast<Models::ScreenLockReplyPack*>(pbPtr.data());
    qtModel.Mode_ = (ScreenLockMode)pack->mode();
    qtModel.Result_ = pack->result();

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}




std::string ScreenUnlockRequestQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "ScreenUnlockRequest"
        << " .Mode_: " << Mode_
        << " .PasswordEnc_: " << qPrintable(Utils::symEncrypt(Password_))
        << " .AccountName_: " << qPrintable(AccountName_)
        << " .AccountPwdEnc_: " << qPrintable(Utils::symEncrypt(AccountPwdRaw_));
    return stream.str();
}

ProtobufMsgPtr_s ScreenUnlockRequestQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::ScreenUnlockRequestPack> pbPtr(new Models::ScreenUnlockRequestPack);
    pbPtr->set_mode(Mode_);
    pbPtr->set_pwd(Utils::QStringToUtf8(Password_));
    pbPtr->set_acc_name(Utils::QStringToUtf8(AccountName_));
    pbPtr->set_acc_pwd_raw(Utils::QStringToUtf8(AccountPwdRaw_));
    return pbPtr;
}

void ScreenUnlockRequestQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, ScreenUnlockRequestQtModel& qtModel )
{
    Models::ScreenUnlockRequestPack *pack =
        static_cast<Models::ScreenUnlockRequestPack*>(pbPtr.data());
    qtModel.Mode_ = (ScreenLockMode)pack->mode();
    qtModel.Password_ = Utils::Utf8ToQString(pack->pwd());
    qtModel.AccountName_ = Utils::Utf8ToQString(pack->acc_name());
    qtModel.AccountPwdRaw_ = Utils::Utf8ToQString(pack->acc_pwd_raw());

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}

std::string ScreenUnlockReplyQtModel::toStdString() const
{
    std::stringstream stream;
    stream << "ScreenUnlockReply"
        << " .Mode_: " << Mode_
        << " .Result_: " << Result_;
    return stream.str();
}

ProtobufMsgPtr_s ScreenUnlockReplyQtModel::toProtoBuf()
{
    LOG_DEBUG << "Serialize: " << toStdString();
    QSharedPointer<Models::ScreenUnlockReplyPack> pbPtr(new Models::ScreenUnlockReplyPack);
    pbPtr->set_mode(Mode_);
    pbPtr->set_result(Result_);
    return pbPtr;
}

void ScreenUnlockReplyQtModel::fromProtoBuf( ProtobufMsgPtr_s pbPtr, ScreenUnlockReplyQtModel& qtModel )
{
    Models::ScreenUnlockReplyPack *pack =
        static_cast<Models::ScreenUnlockReplyPack*>(pbPtr.data());
    qtModel.Mode_ = (ScreenLockMode)pack->mode();
    qtModel.Result_ = pack->result();

    LOG_DEBUG << "Deserialize " << qtModel.toStdString();
}


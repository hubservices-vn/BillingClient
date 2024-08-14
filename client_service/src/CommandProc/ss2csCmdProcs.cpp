#include "ss2csCmdProcs.h"
#include <QSharedPointer>
#include "ICommandProcessor.h"
#include "s2cCmdIDs.h"

#include "HandshakeAckProcessor.h"
#include "OfflineSessionReplyProcessor.h"
#include "s2c\UserLoginReplyProcessor.h"
#include "s2c\UserLogoutReplyProcessor.h"
#include "s2c\SessionBeginProcessor.h"
#include "s2c\SessionEndProcessor.h"
#include "s2c\SessionStatusProcessor.h"
#include "s2c\ProductReplyProcessor.h"
#include "s2c\SecurityProfileReplyProcessor.h"
#include "s2c\AppControlReplyProcessor.h"
#include "AdminCodeReplyProcessor.h"
#include "s2c\ChangePwdReplyProcessor.h"
#include "s2c\QueryAppRequestProcessor.h"
#include "s2c\TerminateAppRequestProcessor.h"
#include "ss2cs\WebControlReplyProcessor.h"
#include "s2c\PcActionRequestProcessor.h"
#include "s2c\TextMessageReplyProcessor.h"
#include "s2c\ScreenshotRequestProcessor.h"
#include "ss2cs\HeartbeatReplyProcessor.h"
#include "ss2cs\AccountAuthReplyProcessor.h"
#include "s2c\ClientSettingReplyProcessor.h"
#include "s2c\UpdateClientRequestProcessor.h"
#include "s2c\FlashTransferReplyProcessor.h"
#include "s2c\ClientAdTransferReplyProcessor.h"
#include "s2c\ClientCodeTransferReplyProcessor.h"
#include "s2c\SystemMessageProcessor.h"
#include "s2c\TopupByCouponReplyProcessor.h"
#include "s2c\UserUpdateProcessor.h"
#include "s2c\ClientWallpaperTransferReplyProcessor.h"

typedef QSharedPointer<ICommandProcessor> CmdProcPtr_s;

void ss2csCmdProcs::registerProcs()
{
    // handshake ack
    this->Register(
        s2c::ID_HandshakeAck, 
        CmdProcPtr_s(new ss2cs::HandshakeAckProcessor));

    // login reply
    this->Register(
        s2c::ID_LoginReply, 
        CmdProcPtr_s(new s2c::UserLoginReplyProcessor));

    // logout reply
    this->Register(
        s2c::ID_LogoutReply, 
        CmdProcPtr_s(new s2c::UserLogoutReplyProcessor));

    // session begin
    this->Register(
        s2c::ID_SessionBegin, 
        CmdProcPtr_s(new s2c::SessionBeginProcessor));


    // session end
    this->Register(
        s2c::ID_SessionEnd, 
        CmdProcPtr_s(new s2c::SessionEndProcessor));


    // session status
    this->Register(
        s2c::ID_SessionStatus, 
        CmdProcPtr_s(new s2c::SessionStatusProcessor));


    // offline session reply
    this->Register(
        s2c::ID_OfflineSessionReply, 
        CmdProcPtr_s(new ss2cs::OfflineSessionReplyProcessor));


    // product reply
    this->Register(
        s2c::ID_ProductReply, 
        CmdProcPtr_s(new s2c::ProductReplyProcessor));


    // app control reply
    this->Register(
        s2c::ID_AppControlReply, 
        CmdProcPtr_s(new s2c::AppControlReplyProcessor));

    this->Register(
        s2c::ID_AppControlWhiteReply, 
        CmdProcPtr_s(new s2c::AppControlReplyWhiteProcessor));

    // security profile reply
    this->Register(
        s2c::ID_SecurityProfileReply, 
        CmdProcPtr_s(new s2c::SecurityProfileReplyProcessor));


    // security profile reply
    this->Register(
        s2c::ID_SystemSettingReply, 
        CmdProcPtr_s(new s2c::ClientSettingReplyProcessor));


    // admin code reply
    this->Register(
        s2c::ID_AdminCodeReply, 
        CmdProcPtr_s(new ss2cs::AdminCodeReplyProcessor));

    this->Register(
        s2c::ID_ChangePwdReply, 
        CmdProcPtr_s(new s2c::ChangePwdReplyProcessor));

    this->Register(
        s2c::ID_QueryAppRequest, 
        CmdProcPtr_s(new s2c::QueryAppRequestProcessor));

    this->Register(
        s2c::ID_TerminateAppRequest, 
        CmdProcPtr_s(new s2c::TerminateAppRequestProcessor));

    this->Register(
        s2c::ID_WebControlReply, 
        CmdProcPtr_s(new ss2cs::WebControlReplyProcessor));

    this->Register(
        s2c::ID_TextMessage, 
        CmdProcPtr_s(new s2c::TextMessageReplyProcessor));

    this->Register(
        s2c::ID_PCActionRequest, 
        CmdProcPtr_s(new s2c::PcActionRequestProcessor));

    this->Register(
        s2c::ID_ScreenCapture, 
        CmdProcPtr_s(new s2c::ScreenshotRequestProcessor));

    this->Register(
        s2c::ID_HeartbeatReply, 
        CmdProcPtr_s(new ss2cs::HeartbeatReplyProcessor));

    this->Register(
        s2c::ID_AccountAuthReply, 
        CmdProcPtr_s(new ss2cs::AccountAuthReplyProcessor));

    this->Register(
        s2c::ID_UpdateClientRequest, 
        CmdProcPtr_s(new s2c::UpdateClientRequestProcessor));

    this->Register(
        s2c::ID_FlashTransferReply, 
        CmdProcPtr_s(new s2c::FlashTransferReplyProcessor));

    this->Register(
        s2c::ID_SystemMessage, 
        CmdProcPtr_s(new s2c::SystemMessageProcessor));

    this->Register(
        s2c::ID_TopupByCouponReply,
        CmdProcPtr_s(new s2c::TopupByCouponReplyProcessor));

    this->Register(
        s2c::ID_QueryVolumeRequest,
        CmdProcPtr_s(new s2c::VolumeQueryRequestProcessor));

    this->Register(
        s2c::ID_ControlVolumeRequest,
        CmdProcPtr_s(new s2c::VolumeControlRequestProcessor));

    this->Register(
        s2c::ID_ClientCodeTransferReply, 
        CmdProcPtr_s(new s2c::ClientCodeTransferReplyProcessor));

    this->Register(
        s2c::ID_ClientAdTransferReply, 
        CmdProcPtr_s(new s2c::ClientAdTransferReplyProcessor));

    this->Register(
        s2c::ID_UserUpdate, 
        CmdProcPtr_s(new s2c::UserUpdateProcessor));

    this->Register(
        s2c::ID_ClientWallpaperCheckReply, 
        CmdProcPtr_s(new s2c::ClientWallpaperCheckReplyProcessor));

    this->Register(
        s2c::ID_ClientWallpaperTransferReply, 
        CmdProcPtr_s(new s2c::ClientWallpaperTransferReplyProcessor));
}

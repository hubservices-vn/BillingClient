#include "cs2ccCmdProcs.h"
#include <QSharedPointer>
#include "ICommandProcessor.h"
#include "s2cCmdIDs.h"

#include "NetworkStateReplyProcessorC.h"
#include "s2c\SessionBeginProcessor.h"
#include "s2c\SessionEndProcessor.h"
#include "s2c\UserLoginReplyProcessor.h"
#include "s2c\UserLogoutReplyProcessor.h"
#include "s2c\SessionStatusProcessor.h"
#include "s2c\ProductReplyProcessor.h"
#include "ScreenLockReplyProcessorC.h"
#include "ScreenUnlockReplyProcessorC.h"
#include "AdminOptionReplyProcessorC.h"
#include "s2c\TextMessageReplyProcessor.h"
#include "s2c\ChangePwdReplyProcessor.h"
#include "s2c\ScreenshotRequestProcessor.h"
#include "s2c\UpdateClientRequestProcessor.h"
#include "s2c\SecurityProfileReplyProcessor.h"
#include "s2c\AppControlReplyProcessor.h"
#include "s2c\ClientSettingReplyProcessor.h"
#include "s2c\PcActionRequestProcessor.h"
#include "s2c\TerminateAppRequestProcessor.h"
#include "s2c\QueryAppRequestProcessor.h"
#include "s2c\SystemMessageProcessor.h"
#include "CCHandshakeAckProcessorC.h"
#include "cs2cc\ClientCodeDllUpdatedProcessorC.h"
#include "s2c\TopupByCouponReplyProcessor.h"
#include "s2c\ClientAdTransferReplyProcessor.h"
#include "cs2cc\CCHeartbeatReplyProcessorC.h"
#include "s2c\UserUpdateProcessor.h"
#include "s2c\ClientWallpaperTransferReplyProcessor.h"

typedef QSharedPointer<ICommandProcessor> CmdProcPtr_s;

void cs2ccCmdProcs::registerProcs()
{
    this->Register(
        cs2c::IDC_CC_HandshakeAck,
        CmdProcPtr_s(new cs2cc::CCHandshakeAckProcessorC));

    // network state reply (push from server)
    this->Register(
        cs2c::IDC_NetworkStateReply,
        CmdProcPtr_s(new cs2cc::NetworkStateReplyProcessor));

    this->Register(
        s2c::ID_HeartbeatReply, 
        CmdProcPtr_s(new cs2cc::CCHeartbeatReplyProcessorC));

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

    // session update
    this->Register(
        cs2c::IDC_SessionDisplay,
        CmdProcPtr_s(new s2c::SessionStatusProcessor));

    this->Register(
        cs2c::IDC_AdminOptionReply,
        CmdProcPtr_s(new cs2cc::AdminOptionReplyProcessorC));

    this->Register(
        s2c::ID_ProductReply,
        CmdProcPtr_s(new s2c::ProductReplyProcessor));

    this->Register(
        cs2c::IDC_ScreenLockReply,
        CmdProcPtr_s(new cs2cc::ScreenLockReplyProcessorC));

    this->Register(
        cs2c::IDC_ScreenUnlockReply,
        CmdProcPtr_s(new cs2cc::ScreenUnlockReplyProcessorC));

    this->Register(
        s2c::ID_TextMessage,
        CmdProcPtr_s(new s2c::TextMessageReplyProcessor));

    this->Register(
        s2c::ID_ChangePwdReply,
        CmdProcPtr_s(new s2c::ChangePwdReplyProcessor));

    this->Register(
        s2c::ID_ScreenCapture,
        CmdProcPtr_s(new s2c::ScreenshotRequestProcessor));

    this->Register(
        s2c::ID_UpdateClientRequest,
        CmdProcPtr_s(new s2c::UpdateClientRequestProcessor));

    this->Register(
        s2c::ID_SecurityProfileReply,
        CmdProcPtr_s(new s2c::SecurityProfileReplyProcessor));

    this->Register(
        s2c::ID_AppControlReply,
        CmdProcPtr_s(new s2c::AppControlReplyProcessor));

    this->Register(
        s2c::ID_AppControlWhiteReply,
        CmdProcPtr_s(new s2c::AppControlReplyWhiteProcessor));

    this->Register(
        s2c::ID_SystemSettingReply,
        CmdProcPtr_s(new s2c::ClientSettingReplyProcessor));

    this->Register(
        s2c::ID_PCActionRequest,
        CmdProcPtr_s(new s2c::PcActionRequestProcessor));

    this->Register(
        s2c::ID_TerminateAppRequest,
        CmdProcPtr_s(new s2c::TerminateAppRequestProcessor));

    this->Register(
        s2c::ID_QueryAppRequest,
        CmdProcPtr_s(new s2c::QueryAppRequestProcessor));

    this->Register(
        s2c::ID_SystemMessage, 
        CmdProcPtr_s(new s2c::SystemMessageProcessor));

	this->Register(
		cs2c::IDC_ClientCodeDllUpdated,
		CmdProcPtr_s(new cs2cc::ClientCodeDllUpdatedProcessorC));


    
    this->Register(
        s2c::ID_TopupByCouponReply,
        CmdProcPtr_s(new s2c::TopupByCouponReplyProcessor));

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

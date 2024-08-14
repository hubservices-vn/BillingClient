#ifndef CS2CCCMDS_H
#define CS2CCCMDS_H

#include "s2cCmdIDs.h"
#include "CommandPkt.h"
#include "CmdSettings.h"

class HeartBeatQtModel;
class CCHandshakeAckQtModel;
class NetworkStateReplyQtModel;
class UserLoginReplyQtModel;
class UserLogoutReplyQtModel;
class SessionBeginQtModel;
class SessionStatusQtModel;
class SessionEndQtModel;
class AdminOptionReplyQtModelC;
class ProductReplyQtModel;
class ScreenLockReplyQtModel;
class ScreenUnlockReplyQtModel;
class TextMessageQtModel;
class ChangeAccPwdReplyQtModel;
class ScreencaptureRequestQtModel;
class EndConnectionReplyQtModel;
class UpdateClientRequestQtModel;
class SecuProfileReplyQtModel;
class AppControlReplyQtModel;
class SystemSettingReplyQtModel;
class QueryAppRequestQtModel;
class TerminateAppRequestQtModel;
class PcActionRequestQtModel;
class SystemMessageQtModel;
class ClientCodeDllUpdatedQtModel;
class ClientAdTransferReplyQtModel;

class TopupByCouponReplyQtModel;
class ClientAdTransferReplyQtModel;
class UserUpdateQtModel;
class ClientWallpaperCheckReplyQtModel;
class ClientWallpaperTransferReplyQtModel;

namespace cs2cc
{
    typedef
        CommandPkt<CCHandshakeAckQtModel, cs2c::IDC_CC_HandshakeAck, CMD_ENCRYPT>
        CCHandshakeAckCommandC;

    typedef CommandPkt<
        HeartBeatQtModel, s2c::ID_HeartbeatReply, CMD_ENCRYPT>
        HeartbeatCommand;

    typedef
        CommandPkt<NetworkStateReplyQtModel, cs2c::IDC_NetworkStateReply, CMD_ENCRYPT>
        NetworkStateReplyCommand;

    typedef
        CommandPkt<UserLoginReplyQtModel, s2c::ID_LoginReply, CMD_ENCRYPT>
        UserLoginReplyCommandC;

    typedef
        CommandPkt<UserLogoutReplyQtModel, s2c::ID_LogoutReply, CMD_ENCRYPT>
        UserLogoutReplyCommandC;

    typedef
        CommandPkt<SessionBeginQtModel, s2c::ID_SessionBegin, CMD_ENCRYPT>
        SessionBeginCommandC;

    typedef
        CommandPkt<SessionStatusQtModel, cs2c::IDC_SessionDisplay, CMD_ENCRYPT>
        SessionDisplayCommandC;

    typedef
        CommandPkt<SessionEndQtModel, s2c::ID_SessionEnd, CMD_ENCRYPT>
        SessionEndCommandC;

    typedef
        CommandPkt<AdminOptionReplyQtModelC, cs2c::IDC_AdminOptionReply, CMD_ENCRYPT>
        AdminOptionReplyCommandC;

    typedef
        CommandPkt<ProductReplyQtModel, s2c::ID_ProductReply, CMD_ENCRYPT>
        ProductReplyCommandC;

    typedef
        CommandPkt<ScreenLockReplyQtModel, cs2c::IDC_ScreenLockReply, CMD_ENCRYPT>
        ScreenLockReplyCommandC;

    typedef
        CommandPkt<ScreenUnlockReplyQtModel, cs2c::IDC_ScreenUnlockReply, CMD_ENCRYPT>
        ScreenUnlockReplyCommandC;

    typedef
        CommandPkt<TextMessageQtModel, s2c::ID_TextMessage, CMD_ENCRYPT>
        TextMessageCommandC;

    typedef
        CommandPkt<ChangeAccPwdReplyQtModel, s2c::ID_ChangePwdReply, CMD_ENCRYPT>
        ChangePwdReplyCommandC;

    typedef
        CommandPkt<ScreencaptureRequestQtModel, s2c::ID_ScreenCapture, CMD_ENCRYPT>
        ScreenCaptureRequestCommandC;

    typedef CommandPkt<
        UpdateClientRequestQtModel, s2c::ID_UpdateClientRequest, CMD_ENCRYPT>
        UpdateClientRequestCommandC;

    typedef CommandPkt<
        SecuProfileReplyQtModel, s2c::ID_SecurityProfileReply, CMD_ENCRYPT>
        SecurityProfileReplyCommandC;

    typedef CommandPkt<
        AppControlReplyQtModel, s2c::ID_AppControlReply, CMD_ENCRYPT>
        AppControlReplyCommandC;

    typedef CommandPkt<
        AppControlReplyQtModel, s2c::ID_AppControlWhiteReply, CMD_ENCRYPT>
        AppControlReplyWhiteCommandC;

    typedef CommandPkt<
        SystemSettingReplyQtModel, s2c::ID_SystemSettingReply, CMD_ENCRYPT>
        SystemSettingReplyCommandC;

    typedef CommandPkt<
        QueryAppRequestQtModel, s2c::ID_QueryAppRequest, CMD_ENCRYPT>
        QueryAppRequestCommandC;

    typedef CommandPkt<
        TerminateAppRequestQtModel, s2c::ID_TerminateAppRequest, CMD_ENCRYPT>
        TerminateAppRequestCommandC;

    typedef CommandPkt<
        PcActionRequestQtModel, s2c::ID_PCActionRequest, CMD_ENCRYPT>
        PCActionRequestCommandC;

    typedef CommandPkt<
        SystemMessageQtModel, s2c::ID_SystemMessage, CMD_ENCRYPT>
        SystemMessageCommandC;

    typedef CommandPkt<
        ClientCodeDllUpdatedQtModel, cs2c::IDC_ClientCodeDllUpdated, CMD_ENCRYPT>
        ClientCodeDllUpdatedCommandC;

    typedef CommandPkt<
        TopupByCouponReplyQtModel, s2c::ID_TopupByCouponReply, CMD_ENCRYPT>
        TopupByCouponReplyCommandC;

    typedef CommandPkt<
        ClientAdTransferReplyQtModel, s2c::ID_ClientAdTransferReply, /*encrypt*/false>
        ClientAdTransferReplyCommandC;
        
    typedef CommandPkt<
        ClientAdTransferReplyQtModel, s2c::ID_ClientAdTransferReply, /*encrypt*/false>
        ClientAdTransferReplyCommandC;
		
    typedef CommandPkt<
        UserUpdateQtModel, s2c::ID_UserUpdate, CMD_ENCRYPT>
        UserUpdateCommandC;

    typedef CommandPkt<
        ClientWallpaperCheckReplyQtModel, s2c::ID_ClientWallpaperCheckReply, CMD_ENCRYPT>
        ClientWallpaperCheckReplyCommandC;

    // file transfer, no need encryption
    typedef CommandPkt<
        ClientWallpaperTransferReplyQtModel, s2c::ID_ClientWallpaperTransferReply, /*encrypt*/false>
        ClientWallpaperTransferReplyCommandC;
}


#endif // CS2CCCMDS_H

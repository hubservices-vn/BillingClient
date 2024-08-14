#ifndef CC2CSCMDS_H
#define CC2CSCMDS_H

#include "c2sCmdIDs.h"
#include "CommandPkt.h"
#include "CmdSettings.h"

class HeartBeatQtModel;
class NetworkStateQtModel;
class TextMessageQtModel;
class ChangeAccPwdRequestQtModel;
class AdminOptionRequestQtModelC;
class UserLogoutQtModel;
class ProductRequestQtModel;
class EndConnectionRequestQtModel;
class OrderRequestQtModel;
class ScreenLockRequestQtModel;
class BeginConnectionRequestQtModel;
class ScreencaptureReplyQtModel;
class WebHistoryReportQtModel;
class SecuProfileRequestQtModel;
class AppControlRequestQtModel;
class SystemSettingRequestQtModel;
class QueryAppReplyQtModel;
class CCHandshakeQtModel;
class TopupByCouponRequestQtModel;
class ClientAdTransferRequestQtModel;
class RemoteControlReplyQtModel;
class ClientWallpaperCheckRequestQtModel;
class ClientWallpaperTransferRequestQtModel;
namespace cc2cs
{
    typedef
        CommandPkt<CCHandshakeQtModel, c2cs::IDC_CC_Handshake, CMD_ENCRYPT>
        CCHandshakeCommandC;

    typedef
        CommandPkt<NetworkStateQtModel, c2cs::IDC_NetworkState, CMD_ENCRYPT>
        NetworkStateQueryCommand;
    typedef CommandPkt<
        HeartBeatQtModel, c2s::ID_Heartbeat, CMD_ENCRYPT>
        HeartbeatCommand;

    typedef
        CommandPkt<UserLogoutQtModel, c2s::ID_Logout, CMD_ENCRYPT>
        UserLogoutCommandC;

    typedef
        CommandPkt<ChangeAccPwdRequestQtModel, c2s::ID_ChangeAccPwdRequest, CMD_ENCRYPT>
        ChangeAccPwdRequestCommand;

    typedef
        CommandPkt<AdminOptionRequestQtModelC, c2cs::IDC_AdminOption, CMD_ENCRYPT>
        AdminOptionRequestCommand;

    typedef
        CommandPkt<ProductRequestQtModel, c2s::ID_ProductRequest, CMD_ENCRYPT>
        ProductRequestCommandC;

    typedef
        CommandPkt<OrderRequestQtModel, c2s::ID_OrderRequest, CMD_ENCRYPT>
        OrderRequestCommandC;

    typedef
        CommandPkt<EndConnectionRequestQtModel, c2s::ID_EndConnection, CMD_ENCRYPT>
        EndConnectionRequestCommandC;

    typedef
        CommandPkt<ScreenLockRequestQtModel, c2cs::IDC_ScreenLock, CMD_ENCRYPT>
        ScreenLockCommandC;

    typedef
        CommandPkt<TextMessageQtModel, c2s::ID_TextMessage, CMD_ENCRYPT>
        TextMessageCommand;

    typedef
        CommandPkt<BeginConnectionRequestQtModel, c2s::ID_BeginConnection, CMD_ENCRYPT>
        BeginConnectionRequestCommandC;

    typedef
        CommandPkt<ScreencaptureReplyQtModel, c2s::ID_ScreenCaptureReply, /*encrypt*/false>
        ScreenCaptureReplyCommandC;

    typedef
        CommandPkt<WebHistoryReportQtModel, c2s::ID_WebHistoryReport, /*encrypt*/false>
        WebHistoryReportCommandC;

    typedef
        CommandPkt<SecuProfileRequestQtModel, c2s::ID_SecuProfileRequest, CMD_ENCRYPT>
        SecuProfileRequestCommandC;

    typedef
        CommandPkt<AppControlRequestQtModel, c2s::ID_AppControlRequest, CMD_ENCRYPT>
        AppControlRequestCommandC;

    typedef
        CommandPkt<SystemSettingRequestQtModel, c2s::ID_SystemSettingRequest, CMD_ENCRYPT>
        SystemSettingRequestCommandC;

    typedef
        CommandPkt<QueryAppReplyQtModel, c2s::ID_QueryAppReply, CMD_ENCRYPT>
        QueryAppReplyCommandC;

    typedef
        CommandPkt<TopupByCouponRequestQtModel, c2s::ID_TopupByCouponRequest, CMD_ENCRYPT>
        TopupByCouponRequestCommandC;

    typedef CommandPkt<
        ClientAdTransferRequestQtModel, c2s::ID_ClientAdTransferRequest, CMD_ENCRYPT>
        ClientAdTransferRequestCommandC;

    typedef CommandPkt<
        RemoteControlReplyQtModel, c2s::ID_RemoteControlReply, CMD_ENCRYPT>
        RemoteControlReplyCommandC;

    typedef CommandPkt<
        ClientWallpaperCheckRequestQtModel, c2s::ID_ClientWallpaperCheckRequest, CMD_ENCRYPT>
        ClientWallpaperCheckRequestCommandC;

    typedef CommandPkt<
        ClientWallpaperTransferRequestQtModel, c2s::ID_ClientWallpaperTransferRequest, CMD_ENCRYPT>
        ClientWallpaperTransferRequestCommandC;
}



#endif // CC2CSCMDS_H

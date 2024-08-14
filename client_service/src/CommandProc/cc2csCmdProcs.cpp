#include "cc2csCmdProcs.h"
#include <QSharedPointer>
#include "ICommandProcessor.h"
#include "c2sCmdIDs.h"
#include "UserLogoutProcessorC.h"
#include "NetworkStateProcessorC.h"
#include "TextMessageProcessorC.h"
#include "ChangeAccPwdProcessorC.h"
#include "AdminOptionRequestProcessorC.h"
#include "ProductQueryProcessorC.h"
#include "OrderRequestProcessorC.h"
#include "BeginConnectionRequestProcessorC.h"
#include "EndConnectionRequestProcessorC.h"
#include "ScreenshotReplyProcessorC.h"
#include "WebHistoryReportProcessorC.h"
#include "AppControlRequestProcessorC.h"
#include "ClientSettingRequestProcessorC.h"
#include "QueryAppReplyProcessorC.h"
#include "ScreenLockProcessorC.h"
#include "CCHandshakeProcessorC.h"
#include "SecuProfileRequestProcessorC.h"
#include "ClientAdTransferRequestProcessorC.h"
#include "TopupByCouponRequestProcessorC.h"
#include "ClientAdTransferRequestProcessorC.h"
#include "CCHeartbeatProcessorC.h"
#include "PcActionREplyProcessorC.h"
#include "ClientWallpaperTransferRequestProcessorC.h"
typedef QSharedPointer<ICommandProcessor> CmdProcPtr_s;

void cc2csCmdProcs::registerProcs()
{
    this->Register(
        c2cs::IDC_CC_Handshake,
        CmdProcPtr_s(new cc2cs::CCHandshakeProcessorC));

    this->Register(
        c2s::ID_Heartbeat,
        CmdProcPtr_s(new cc2cs::CCHeartbeatProcessorC));

    this->Register(
        c2cs::IDC_NetworkState,
        CmdProcPtr_s(new cc2cs::NetworkStateProcessorC));

    // begin connection request
    this->Register(
        c2s::ID_BeginConnection,
        CmdProcPtr_s(new cc2cs::BeginConnectionRequestProcessorC));

    // end connection request
    this->Register(
        c2s::ID_EndConnection,
        CmdProcPtr_s(new cc2cs::EndConnectionRequestProcessorC));

    // logout request
    this->Register(
        c2s::ID_Logout,
        CmdProcPtr_s(new cc2cs::UserLogoutProcessorC));

    // text message
    this->Register(
        c2s::ID_ChangeAccPwdRequest,
        CmdProcPtr_s(new cc2cs::ChangeAccPwdProcessorC));

    this->Register(
        c2cs::IDC_AdminOption,
        CmdProcPtr_s(new cc2cs::AdminOptionRequestProcessorC));

    this->Register(
        c2s::ID_ProductRequest,
        CmdProcPtr_s(new cc2cs::ProductQueryProcessorC));

    this->Register(
        c2s::ID_OrderRequest,
        CmdProcPtr_s(new cc2cs::OrderRequestProcessorC));

    this->Register(
        c2cs::IDC_ScreenLock,
        CmdProcPtr_s(new cc2cs::ScreenLockRequestProcessorC));

    this->Register(
        c2s::ID_TextMessage,
        CmdProcPtr_s(new cc2cs::TextMessageProcessorC));

    this->Register(
        c2s::ID_ScreenCaptureReply,
        CmdProcPtr_s(new cc2cs::ScreenshotReplyProcessorC));

    this->Register(
        c2s::ID_WebHistoryReport,
        CmdProcPtr_s(new cc2cs::WebHistoryReportProcessorC));

    this->Register(
        c2s::ID_AppControlRequest,
        CmdProcPtr_s(new cc2cs::AppControlRequestProcessorC));

    this->Register(
        c2s::ID_SystemSettingRequest,
        CmdProcPtr_s(new cc2cs::ClientSettingRequestProcessorC));

    this->Register(
        c2s::ID_QueryAppReply,
        CmdProcPtr_s(new cc2cs::QueryAppReplyProcessorC));

    this->Register(
        c2s::ID_SecuProfileRequest,
        CmdProcPtr_s(new cc2cs::SecuProfileRequestProcessorC));

    this->Register(
        c2s::ID_TopupByCouponRequest,
        CmdProcPtr_s(new cc2cs::TopupByCouponRequestProcessorC));

    this->Register(
        c2s::ID_ClientAdTransferRequest,
        CmdProcPtr_s(new cc2cs::ClientAdTransferRequestProcessorC));

    this->Register(
        c2s::ID_RemoteControlReply,
        CmdProcPtr_s(new cc2cs::RemoteControlReplyProcessorC));

    this->Register(
        c2s::ID_ClientWallpaperCheckRequest,
        CmdProcPtr_s(new cc2cs::ClientWallpaperCheckRequestProcessorC));

    this->Register(
        c2s::ID_ClientWallpaperTransferRequest,
        CmdProcPtr_s(new cc2cs::ClientWallpaperTransferRequestProcessorC));
}

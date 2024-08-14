#include "cs2clCmdProcs.h"
#include <QSharedPointer>
#include "ICommandProcessor.h"
#include "s2cCmdIDs.h"

#include "s2c\SessionBeginProcessor.h"
#include "s2c\SessionEndProcessor.h"
#include "s2c\UserLoginReplyProcessor.h"
#include "s2c\UserLogoutReplyProcessor.h"
#include "cs2cl\CLHandshakeAckProcessorC.h"
#include "cs2cl\ScreenLockReplyProcessorC.h"
#include "cs2cl\ScreenUnlockReplyProcessorC.h"
#include "cs2cl\EndConnectionReplyProcessorC.h"
#include "cs2cl\ClientCodeDllUpdatedProcessorC.h"
#include "s2c\FlashTransferReplyProcessor.h"

typedef QSharedPointer<ICommandProcessor> CmdProcPtr_s;

void cs2clCmdProcs::registerProcs()
{
    this->Register(
        cs2c::IDC_CL_HandshakeAck,
        CmdProcPtr_s(new cs2cl::CLHandshakeAckProcessorC));

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

    // end connection
    this->Register(
        s2c::ID_EndConnectionReply,
        CmdProcPtr_s(new cs2cc::EndConnectionReplyProcessorC));

    this->Register(
        cs2c::IDC_ScreenLockReply,
        CmdProcPtr_s(new cs2cl::ScreenLockReplyProcessorC));

    this->Register(
        cs2c::IDC_ScreenUnlockReply,
        CmdProcPtr_s(new cs2cl::ScreenUnlockReplyProcessorC));

    this->Register(
        s2c::ID_FlashTransferReply,
        CmdProcPtr_s(new s2c::FlashTransferReplyProcessorC));

	this->Register(
		cs2c::IDC_ClientCodeDllUpdated,
		CmdProcPtr_s(new cs2cl::ClientCodeDllUpdatedProcessorC));
}

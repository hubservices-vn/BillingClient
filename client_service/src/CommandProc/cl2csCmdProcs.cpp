#include "cl2csCmdProcs.h"
#include <QSharedPointer>
#include "ICommandProcessor.h"
#include "c2sCmdIDs.h"
#include "cl2cs\UserLoginProcessorC.h"
#include "cl2cs\FlashTransferRequestProcessorC.h"
#include "cl2cs\ScreenUnlockProcessorC.h"
#include "cl2cs\CLHandshakeProcessorC.h"

#include "cl2cs\CLHeartbeatProcessorC.h"

typedef QSharedPointer<ICommandProcessor> CmdProcPtr_s;

void cl2csCmdProcs::registerProcs()
{
    this->Register(
        c2cs::IDC_CL_Handshake,
        CmdProcPtr_s(new cl2cs::CLHandshakeProcessorC));

    // login request
    this->Register(
        c2s::ID_Heartbeat,
        CmdProcPtr_s(new cl2cs::CLHeartbeatProcessorC));

    // login request
    this->Register(
        c2s::ID_Login,
        CmdProcPtr_s(new cl2cs::UserLoginProcessorC));

    this->Register(
        c2cs::IDC_ScreenUnlock,
        CmdProcPtr_s(new cl2cs::ScreenUnlockRequestProcessorC));

    this->Register(
        c2s::ID_FlashTransferRequest,
        CmdProcPtr_s(new cl2cs::FlashTransferRequestProcessorC));
}

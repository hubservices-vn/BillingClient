#ifndef CC2CSCMDS_H
#define CC2CSCMDS_H

#include "c2sCmdIDs.h"
#include "CommandPkt.h"
#include "CmdSettings.h"

class HeartBeatQtModel;
class CLHandshakeQtModel;
class ScreenUnlockRequestQtModel;
class UserLoginRequestQtModel;
class FlashTransferRequestQtModel;
namespace cl2cs
{
    typedef
        CommandPkt<CLHandshakeQtModel, c2cs::IDC_CL_Handshake, CMD_ENCRYPT>
        CLHandshakeCommandC;

    typedef CommandPkt<
        HeartBeatQtModel, c2s::ID_Heartbeat, CMD_ENCRYPT>
        HeartbeatCommand;

    typedef
        CommandPkt<UserLoginRequestQtModel, c2s::ID_Login, CMD_ENCRYPT>
        UserLoginCommandC;

    typedef
        CommandPkt<FlashTransferRequestQtModel, c2s::ID_FlashTransferRequest, CMD_ENCRYPT>
        FlashTransferRequestCommandC;

    typedef
        CommandPkt<ScreenUnlockRequestQtModel, c2cs::IDC_ScreenUnlock, CMD_ENCRYPT>
        ScreenUnlockRequestCommandC;
}



#endif // CC2CSCMDS_H

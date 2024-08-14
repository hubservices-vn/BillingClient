#ifndef CS2CL_CMDS_H
#define CS2CL_CMDS_H

#include "s2cCmdIDs.h"
#include "CommandPkt.h"
#include "CmdSettings.h"

class HeartBeatQtModel;
class NetworkStateReplyQtModel;
class UserLoginReplyQtModel;
class SessionBeginQtModel;
class SessionEndQtModel;
class ScreenLockReplyQtModel;
class ScreenUnlockReplyQtModel;
class FlashTransferReplyQtModel;
class CLHandshakeAckQtModel;
class EndConnectionReplyQtModel;
class ClientCodeDllUpdatedQtModel;
namespace cs2cl
{
    typedef
        CommandPkt<CLHandshakeAckQtModel, cs2c::IDC_CL_HandshakeAck, CMD_ENCRYPT>
        CLHandshakeAckCommandC;

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
        CommandPkt<SessionBeginQtModel, s2c::ID_SessionBegin, CMD_ENCRYPT>
        SessionBeginCommandC;

    typedef
        CommandPkt<SessionEndQtModel, s2c::ID_SessionEnd, CMD_ENCRYPT>
        SessionEndCommandC;

    typedef
        CommandPkt<ScreenLockReplyQtModel, cs2c::IDC_ScreenLockReply, CMD_ENCRYPT>
        ScreenLockReplyCommandC;

    typedef
        CommandPkt<ScreenUnlockReplyQtModel, cs2c::IDC_ScreenUnlockReply, CMD_ENCRYPT>
        ScreenUnlockReplyCommandC;

    typedef
        CommandPkt<FlashTransferReplyQtModel, s2c::ID_FlashTransferReply, /*encrypt*/false>
        FlashTransferReplyCommandC;

    typedef
        CommandPkt<EndConnectionReplyQtModel, s2c::ID_EndConnectionReply, CMD_ENCRYPT>
        EndConnectionReplyCommandC;

    typedef 
        CommandPkt<    ClientCodeDllUpdatedQtModel, cs2c::IDC_ClientCodeDllUpdated, CMD_ENCRYPT>
        ClientCodeDllUpdatedCommandC;
}


#endif // CS2CL_CMDS_H

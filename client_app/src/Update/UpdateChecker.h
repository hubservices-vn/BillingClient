#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include "Common/ConstructorMagic.h"
#include "stdtypes.h"

using namespace SDK::Common::ConstructorMagic;

enum UpdaterState
{
    UpdaterState_NA = -1,
    UpdaterState_Done = 1,
    UpdaterState_Failed = 2,
};
std::string UpdaterStateToStdString(UpdaterState state);

class UpdateChecker
{
public:
	static bool HasUpdate();
    static UpdaterState BeginUpdate();

private:
    static bool _SendReplyToUpdater(const wstring_t& cmdReply);
    static bool _ReadRpcCommand(const wstring_t& expectedCmd);

private:
	DISALLOW_IMPLICIT_CONSTRUCTORS(UpdateChecker);
};

#endif // UPDATECHECKER_H
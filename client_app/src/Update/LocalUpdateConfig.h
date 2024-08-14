#ifndef AUTOUPDATE_LOCALUPDATECONFIG_H__
#define AUTOUPDATE_LOCALUPDATECONFIG_H__

#include <map>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include "LocalFileItem.h"

class LocalUpdateConfig : public Singleton_T<LocalUpdateConfig>
{
public:
	LocalUpdateConfig();
	~LocalUpdateConfig();

public:
	bool	Init();

	float	GetVersion();

private:
	float version_;
};

#endif // AUTOUPDATE_LOCALUPDATECONFIG_H__
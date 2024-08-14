#pragma once
#ifndef ICLIENT_EVENT_H__
#define ICLIENT_EVENT_H__

#define DLL_VERSION 1

namespace ClientEvent 
{
	enum InvokerType
	{
		APP = 0,
		SERVICE,
		LOCKER,
	};
	class IClientEvent
	{
	public:
		virtual void init(InvokerType invokerType, int dllVersion) = 0;
		virtual void onInit() = 0;
		virtual void onUserLogIn() = 0;
		virtual void onUserLogOut() = 0;
	};

}
#endif
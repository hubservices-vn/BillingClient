#pragma once
#include <windows.h>
#include "IClientEvent.h"


using namespace ClientEvent;
class ClientEventLoader
{
typedef void*(*VOIDS_FUNC_T)(InvokerType, int);
public:
	void loadDll(InvokerType invokerType, int dllVersion)
	{
		m_hdll = LoadLibrary(m_dllName);
		m_clientEventInstance = NULL;
		CreateClientEventInstanceFunc = NULL;
		if (m_hdll != NULL)
		{
			CreateClientEventInstanceFunc = (VOIDS_FUNC_T) GetProcAddress( m_hdll, "CreateClientEventClassInstance" );// get the function pointer
			if (CreateClientEventInstanceFunc == NULL)
			{
				lastError = GetLastError();
			} else {
				m_clientEventInstance = static_cast< IClientEvent* > ( CreateClientEventInstanceFunc(invokerType, dllVersion) );	// get pointer to object
				m_clientEventInstance->init(invokerType, dllVersion);
			}
		} else {
			lastError = GetLastError();
		}
	}
	
	void unloadDll()
	{
		if (m_hdll != NULL)
		{
			if (m_clientEventInstance != NULL)
			{
				//delete m_clientEventInstance;
				m_clientEventInstance = NULL;
			}
			FreeLibrary(m_hdll);// free the dll
			m_hdll = NULL;
		}
	}

public:
	ClientEventLoader(LPCWSTR dllName, InvokerType invokerType, int dllVersion)
	{
		m_clientEventInstance = NULL;
		m_dllName = dllName;
		loadDll(invokerType, dllVersion);
	}
	~ClientEventLoader(void)
	{
		unloadDll();
	}
	
	int getLastError()
	{
		return lastError;
	}

	IClientEvent* GetClientEventInstance()
	{
		return m_clientEventInstance;
	}


	HINSTANCE m_hdll;
	VOIDS_FUNC_T CreateClientEventInstanceFunc;
	IClientEvent* m_clientEventInstance;
	int lastError;
	LPCWSTR m_dllName;
};

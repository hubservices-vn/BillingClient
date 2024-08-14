#pragma once


#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>

#pragma comment( lib, "ole32.lib" )
#pragma comment( lib, "oleaut32.lib" )

#include <atlcomcli.h>
#include <comip.h>
#pragma comment( lib, "comsupp.lib" )

class MSFirewallSetting
{
public:
	MSFirewallSetting();
	~MSFirewallSetting();

public:
	HRESULT IsOn(OUT BOOL* fwOn);
	HRESULT TurnOn();
	HRESULT TurnOff();
	HRESULT AppIsEnabled(
		IN const wchar_t* fwName,
		IN const wchar_t* fwProcessImageFileName,
		OUT BOOL* fwAppEnabled
		);
	HRESULT AddApp(
		IN const wchar_t* fwName,
		IN const wchar_t* fwProcessImageFileName
		);
	HRESULT PortIsEnabled(
		IN LONG portNumber,
		IN NET_FW_IP_PROTOCOL ipProtocol,
		OUT BOOL* fwPortEnabled
		);
	HRESULT PortAdd(
		IN LONG portNumber,
		IN NET_FW_IP_PROTOCOL ipProtocol,
		IN const wchar_t* name
		);

protected:
	HRESULT __Initialize();

	HRESULT initialize_result_;
	CComPtr<INetFwProfile> profile_;
};


/*
sample

int __cdecl wmain(int argc, wchar_t* argv[])
{
    HRESULT hr = S_OK;
	MSFirewallSetting firewall;

    // Add Windows Messenger to the authorized application collection.
    hr = firewall.AddApp(
            L"%ProgramFiles%\\Messenger\\msmsgs.exe",
            L"Windows Messenger"
            );
    if (FAILED(hr))
    {
        LOG_ERROR("WindowsFirewallAddApp failed: 0x%08lx\n", hr);
    }

    // Add TCP::80 to list of globally open ports.
    hr = firewall.PortAdd(80, NET_FW_IP_PROTOCOL_TCP, L"WWW");
    if (FAILED(hr))
    {
        LOG_ERROR("WindowsFirewallPortAdd failed: 0x%08lx\n", hr);
    }

    return 0;
}

*/
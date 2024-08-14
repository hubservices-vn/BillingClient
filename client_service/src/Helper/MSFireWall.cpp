#include "stdafx.h"
#include "MSFireWall.h"
#include "winerror.h"
// based on http://msdn.microsoft.com/en-us/library/aa364726%28v=VS.85%29.aspx
namespace
{
	class ComInitor
	{
	public:
		HRESULT initialize_result_;
		ComInitor()
		{
			initialize_result_ 	 = CoInitializeEx(
				0,
				COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
				);
			if (FAILED(initialize_result_))
			{
				//LOG_ERROR("CoInitializeEx failed: 0x%08lx\n", initialize_result_);
			}
		}
		~ComInitor()
		{
			if (SUCCEEDED(initialize_result_))
				CoUninitialize();
		}
	};
}

MSFirewallSetting::MSFirewallSetting():initialize_result_(E_FAIL)
{
	static ComInitor _com_initor;
	initialize_result_ = __Initialize();
}

MSFirewallSetting::~MSFirewallSetting()
{

}

HRESULT MSFirewallSetting::__Initialize()
{
    // Create an instance of the firewall settings manager.
	CComPtr<INetFwMgr> fwMgr;
    HRESULT hr = fwMgr.CoCreateInstance(__uuidof(NetFwMgr),NULL,CLSCTX_INPROC_SERVER);
    if (FAILED(hr))
    {
       //LOG_ERROR("CreateInstance failed: 0x%08lx\n", hr);
       return hr;
    }

    // Retrieve the local firewall policy.
	CComPtr<INetFwPolicy> fwPolicy;
    hr = fwMgr->get_LocalPolicy(&fwPolicy);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_LocalPolicy failed: 0x%08lx\n", hr);
		return hr;
    }

    // Retrieve the firewall profile currently in effect.
	CComPtr<INetFwProfile> profile;
    hr = fwPolicy->get_CurrentProfile(&profile);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_CurrentProfile failed: 0x%08lx\n", hr);
    }
	else
	{
		profile_ = profile;
	}
	return hr;
}

HRESULT MSFirewallSetting::IsOn(OUT BOOL* fwOn)
{
	if (profile_==NULL) return E_FAIL;
    VARIANT_BOOL fwEnabled;
    _ASSERT(fwOn != NULL);
    *fwOn = FALSE;

    // Get the current state of the firewall.
    HRESULT hr = profile_->get_FirewallEnabled(&fwEnabled);
    if (FAILED(hr))
    {
        LOG_DEBUG << "get_FirewallEnabled failed: " << hr;
		return hr;
    }

    // Check to see if the firewall is on.
    if (fwEnabled != VARIANT_FALSE)
    {
        *fwOn = TRUE;
        LOG_DEBUG << "The firewall is on.";
    }
    else
    {
        LOG_DEBUG << "The firewall is off.";
    }
    return hr;
}


HRESULT MSFirewallSetting::TurnOn()
{
	if (profile_==NULL) return E_FAIL;
   BOOL fwOn;

	// Check to see if the firewall is off.
    HRESULT hr = IsOn(&fwOn);
    if (FAILED(hr))
    {
        //LOG_ERROR("WindowsFirewallIsOn failed: 0x%08lx\n", hr);
        return hr;
    }

    // If it is, turn it on.
    if (!fwOn)
    {
        // Turn the firewall on.
        hr = profile_->put_FirewallEnabled(VARIANT_TRUE);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_FirewallEnabled failed: 0x%08lx\n", hr);
            return hr;
        }

        LOG_INFO << "The firewall is now on.";
    }

    return hr;
}


HRESULT MSFirewallSetting::TurnOff()
{
	if (profile_==NULL) return E_FAIL;
	BOOL fwOn;

    // Check to see if the firewall is on.
    HRESULT hr = IsOn(&fwOn);
    if (FAILED(hr))
    {
        //LOG_ERROR("WindowsFirewallIsOn failed: 0x%08lx\n", hr);
       return hr;
    }

    // If it is, turn it off.
    if (fwOn)
    {
        // Turn the firewall off.
        hr = profile_->put_FirewallEnabled(VARIANT_FALSE);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_FirewallEnabled failed: 0x%08lx\n", hr);
            return hr;
        }

        LOG_INFO << "The firewall is now off.";
    }
    return hr;
}


HRESULT MSFirewallSetting::AppIsEnabled(
			IN const wchar_t* fwName,
			IN const wchar_t* fwProcessImageFileName,
            OUT BOOL* fwAppEnabled
            )
{
	if (profile_==NULL) return E_FAIL;
	_ASSERT(fwName != NULL);
	_ASSERT(fwProcessImageFileName != NULL);
	_bstr_t fwBstrName(fwName);
	_bstr_t fwBstrProcessImageFileName(fwProcessImageFileName);
    VARIANT_BOOL fwEnabled;
    CComPtr<INetFwAuthorizedApplication> fwrule;
    CComPtr<INetFwAuthorizedApplications> fwrules;

    *fwAppEnabled = FALSE;

    // Retrieve the authorized application collection.
    HRESULT hr = profile_->get_AuthorizedApplications(&fwrules);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_AuthorizedApplications failed: 0x%08lx\n", hr);
        return hr;
    }

    // Attempt to retrieve the authorized application.
    hr = fwrules->Item(fwBstrProcessImageFileName, &fwrule);
    if (SUCCEEDED(hr))
    {
        // Find out if the authorized application is enabled.
        hr = fwrule->get_Enabled(&fwEnabled);
        if (FAILED(hr))
        {
            //LOG_ERROR("get_Enabled failed: 0x%08lx\n", hr);
			return hr;
        }

        if (fwEnabled != VARIANT_FALSE)
        {
            // The authorized application is enabled.
            *fwAppEnabled = TRUE;

            //LOG_INFO(
            //    L"Authorized application %s is enabled in the firewall.\n",
            //    fwProcessImageFileName
            //    );
        }
        else
        {
            //LOG_INFO(
            //    L"Authorized application %s is disabled in the firewall.\n",
            //    fwProcessImageFileName
            //    );
        }
    }
    else
    {
        // The authorized application was not in the collection.
        hr = S_OK;

        //LOG_INFO(
        //    L"Authorized application %s is disabled in the firewall.\n",
        //    fwProcessImageFileName
        //    );
    }

    return hr;
}


HRESULT MSFirewallSetting::AddApp(
            IN const wchar_t* fwName,
            IN const wchar_t* fwProcessImageFileName
            )
{
	if (profile_==NULL) return E_FAIL;
    // First check to see if the application is already authorized.
	BOOL fwAppEnabled;
    HRESULT hr = AppIsEnabled(
			fwName,
            fwProcessImageFileName,
            &fwAppEnabled
            );
    if (FAILED(hr))
    {
        //LOG_ERROR("WindowsFirewallAppIsEnabled failed: 0x%08lx\n", hr);
        return hr;
    }

    // Only add the application if it isn't already authorized.
    if (fwAppEnabled)
		return hr;

	_ASSERT(fwName != NULL);
	_ASSERT(fwProcessImageFileName != NULL);
	_bstr_t fwBstrName(fwName);
	_bstr_t fwBstrProcessImageFileName(fwProcessImageFileName);
	CComPtr<INetFwAuthorizedApplication> fwrule;
	CComPtr<INetFwAuthorizedApplications> fwrules;

    // Retrieve the authorized application collection.
    hr = profile_->get_AuthorizedApplications(&fwrules);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_AuthorizedApplications failed: 0x%08lx\n", hr);
        return hr;
    }

    // Create an instance of an authorized application.
    hr = fwrule.CoCreateInstance(
            __uuidof(NetFwAuthorizedApplication),
			NULL,
            CLSCTX_INPROC_SERVER
            );
    if (FAILED(hr))
    {
        //LOG_ERROR("CoCreateInstance failed: 0x%08lx\n", hr);
        return hr;
    }

    // Set the process image file name.
    hr = fwrule->put_ProcessImageFileName(fwBstrProcessImageFileName);
    if (FAILED(hr))
    {
        //LOG_ERROR("put_ProcessImageFileName failed: 0x%08lx\n", hr);
		return hr;
    }

    // Set the application friendly name.
    hr = fwrule->put_Name(fwBstrName);
    if (FAILED(hr))
    {
        //LOG_ERROR("put_Name failed: 0x%08lx\n", hr);
		return hr;
    }

    // Add the application to the collection.
    hr = fwrules->Add(fwrule);
    if (FAILED(hr))
    {
        //LOG_ERROR("Add failed: 0x%08lx\n", hr);
		return hr;
    }

    //LOG_INFO(
    //    L"Authorized application %s is now enabled in the firewall.\n",
    //    fwProcessImageFileName
    //    );
    return hr;
}


HRESULT MSFirewallSetting::PortIsEnabled(
            IN LONG portNumber,
            IN NET_FW_IP_PROTOCOL ipProtocol,
            OUT BOOL* fwPortEnabled
            )
{
	if (profile_==NULL) return E_FAIL;
	_ASSERT(fwPortEnabled != NULL);
    VARIANT_BOOL fwEnabled;
    CComPtr<INetFwOpenPort> fwrule;
    CComPtr<INetFwOpenPorts> fwrules;

    *fwPortEnabled = FALSE;

    // Retrieve the globally open ports collection.
    HRESULT hr = profile_->get_GloballyOpenPorts(&fwrules);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_GloballyOpenPorts failed: 0x%08lx\n", hr);
        return hr;
    }

    // Attempt to retrieve the globally open port.
    hr = fwrules->Item(portNumber, ipProtocol, &fwrule);
    if (SUCCEEDED(hr))
    {
        // Find out if the globally open port is enabled.
        hr = fwrule->get_Enabled(&fwEnabled);
        if (FAILED(hr))
        {
           //LOG_ERROR("get_Enabled failed: 0x%08lx\n", hr);
           return hr;
        }

        if (fwEnabled != VARIANT_FALSE)
        {
            // The globally open port is enabled.
            *fwPortEnabled = TRUE;
            //LOG_INFO("Port %ld is open in the firewall.\n", portNumber);
        }
        else
        {
            //LOG_INFO("Port %ld is not open in the firewall.\n", portNumber);
        }
    }
    else
    {
        // The globally open port was not in the collection.
        hr = S_OK;
        //LOG_INFO("Port %ld is not open in the firewall.\n", portNumber);
    }

    return hr;
}


HRESULT MSFirewallSetting::PortAdd(
            IN LONG portNumber,
            IN NET_FW_IP_PROTOCOL ipProtocol,
            IN const wchar_t* name
            )
{
	if (profile_==NULL) return E_FAIL;
	_ASSERT(name != NULL);
    BOOL fwPortEnabled;
    _bstr_t fwBstrName(name);
    CComPtr<INetFwOpenPort> fwrule;
    CComPtr<INetFwOpenPorts> fwrules;

    // First check to see if the port is already added.
    HRESULT hr = PortIsEnabled(
            portNumber,
            ipProtocol,
            &fwPortEnabled
            );
    if (FAILED(hr))
    {
        //LOG_ERROR("WindowsFirewallPortIsEnabled failed: 0x%08lx\n", hr);
        return hr;
    }

    // Only add the port if it isn't already added.
    if (!fwPortEnabled)
    {
        // Retrieve the collection of globally open ports.
        hr = profile_->get_GloballyOpenPorts(&fwrules);
        if (FAILED(hr))
        {
            //LOG_ERROR("get_GloballyOpenPorts failed: 0x%08lx\n", hr);
            return hr;
        }

        // Create an instance of an open port.
        hr = fwrule.CoCreateInstance(
                __uuidof(NetFwOpenPort),
				NULL,
                CLSCTX_INPROC_SERVER
                );
        if (FAILED(hr))
        {
            //LOG_ERROR("CoCreateInstance failed: 0x%08lx\n", hr);
			return hr;
        }

        // Set the port number.
        hr = fwrule->put_Port(portNumber);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_Port failed: 0x%08lx\n", hr);
			return hr;
        }

        // Set the IP protocol.
        hr = fwrule->put_Protocol(ipProtocol);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_Protocol failed: 0x%08lx\n", hr);
			return hr;
        }

        // Set the friendly name of the port.
        hr = fwrule->put_Name(fwBstrName);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_Name failed: 0x%08lx\n", hr);
			return hr;
        }

        // Opens the port and adds it to the collection.
        hr = fwrules->Add(fwrule);
        if (FAILED(hr))
        {
            //LOG_ERROR("Add failed: 0x%08lx\n", hr);
			return hr;
        }

        //LOG_INFO("Port %ld is now open in the firewall.\n", portNumber);
    }

    return hr;
}

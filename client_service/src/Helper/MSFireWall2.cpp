#include "stdafx.h"
#include "MSFireWall2.h"

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
				COINIT_APARTMENTTHREADED
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

MSFirewallSetting2::MSFirewallSetting2():initialize_result_(E_FAIL)
{
	static ComInitor _com_initor;
	initialize_result_ = __Initialize();
}

MSFirewallSetting2::~MSFirewallSetting2()
{

}

HRESULT MSFirewallSetting2::__Initialize()
{
    // Create an instance of the firewall settings manager.
    HRESULT hr = profile_.CoCreateInstance(__uuidof(NetFwPolicy2),NULL,CLSCTX_INPROC_SERVER);
    if (FAILED(hr))
    {
       //LOG_ERROR("CreateInstance failed: 0x%08lx\n", hr);
    }

	return hr;
}

HRESULT MSFirewallSetting2::IsOn(OUT BOOL* fwOn)
{
	if (profile_==NULL) return E_FAIL;
    VARIANT_BOOL fwEnabled;
    _ASSERT(fwOn != NULL);
    *fwOn = FALSE;

    // Get the current state of the firewall.
    HRESULT hr = profile_->get_FirewallEnabled(NET_FW_PROFILE2_PUBLIC,&fwEnabled);
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


HRESULT MSFirewallSetting2::TurnOn()
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
        hr = profile_->put_FirewallEnabled(NET_FW_PROFILE2_PUBLIC,VARIANT_TRUE);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_FirewallEnabled failed: 0x%08lx\n", hr);
            return hr;
        }

        //LOG_INFO("The firewall is now on.\n");
    }

    return hr;
}


HRESULT MSFirewallSetting2::TurnOff()
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
        hr = profile_->put_FirewallEnabled(NET_FW_PROFILE2_PUBLIC,VARIANT_FALSE);
        if (FAILED(hr))
        {
            //LOG_ERROR("put_FirewallEnabled failed: 0x%08lx\n", hr);
            return hr;
        }

        //LOG_INFO("The firewall is now off.\n");
    }
    return hr;
}


HRESULT MSFirewallSetting2::AppIsEnabled(
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
    CComPtr<INetFwRule> fwrule;
    CComPtr<INetFwRules> fwrules;

    *fwAppEnabled = FALSE;

    // Retrieve the authorized application collection.
    HRESULT hr = profile_->get_Rules(&fwrules);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_Rules failed: 0x%08lx\n", hr);
        return hr;
    }

    // Attempt to retrieve the authorized application.
    hr = fwrules->Item(fwBstrName, &fwrule);
    if (SUCCEEDED(hr))
    {
        // Find out if the authorized application is enabled.
        hr = fwrule->get_Enabled(&fwEnabled);
        if (FAILED(hr))
        {
            //LOG_ERROR("get_Enabled failed: 0x%08lx\n", hr);
			return hr;
        }

		_bstr_t fwBstrCurProcessImageFileName;
		hr = fwrule->get_ApplicationName(fwBstrCurProcessImageFileName.GetAddress());
		if (FAILED(hr))
		{
			//LOG_ERROR("get_ProcessImageFileName failed: 0x%08lx\n", hr);
			return hr;
		}
		if (fwBstrCurProcessImageFileName!=fwBstrProcessImageFileName)
		{// file name changed
			fwrules->Remove(fwBstrName);
			return hr;
		}

        if (fwEnabled != VARIANT_FALSE)
        {
            // The authorized application is enabled.
            *fwAppEnabled = TRUE;

            //LOG_INFO(
            //    L"Authorized application %s is enabled in the firewall.\n",
            //    fwName
            //    );
        }
        else
        {
            //LOG_INFO(
            //    L"Authorized application %s is disabled in the firewall.\n",
            //    fwName
            //    );
        }
    }
    else
    {
        // The authorized application was not in the collection.
        hr = S_OK;

        //LOG_INFO(
        //    L"Authorized application %s is disabled in the firewall.\n",
        //    fwName
        //    );
    }

    return hr;
}


HRESULT MSFirewallSetting2::AddApp(
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

	_ASSERT(fwProcessImageFileName != NULL);
	_ASSERT(fwName != NULL);
	_bstr_t fwBstrName(fwName);
	_bstr_t fwBstrProcessImageFileName(fwProcessImageFileName);
	CComPtr<INetFwRule> fwrule;
	CComPtr<INetFwRules> fwrules;

	// Retrieve the authorized application collection.
    hr = profile_->get_Rules(&fwrules);
    if (FAILED(hr))
    {
        //LOG_ERROR("get_AuthorizedApplications failed: 0x%08lx\n", hr);
        return hr;
    }

    // Create an instance of an authorized application.
    hr = fwrule.CoCreateInstance(
            __uuidof(NetFwRule),
			NULL,
            CLSCTX_INPROC_SERVER
            );
    if (FAILED(hr))
    {
        //LOG_ERROR("CoCreateInstance failed: 0x%08lx\n", hr);
        return hr;
    }

    // Set the process image file name.
    hr = fwrule->put_ApplicationName(fwBstrProcessImageFileName);
    if (FAILED(hr))
    {
        //LOG_ERROR("put_ApplicationName failed: 0x%08lx\n", hr);
		return hr;
    }

    // Set the application friendly name.
    hr = fwrule->put_Name(fwBstrName);
    if (FAILED(hr))
    {
        //LOG_ERROR("put_Name failed: 0x%08lx\n", hr);
		return hr;
    }
	hr = fwrule->put_Action(NET_FW_ACTION_ALLOW);
	if ( FAILED(hr) )
	{
		//LOG_ERROR("Failed INetFwRule::put_Action failed with error: 0x %x.\n", hr);
		return hr;
	}
	hr = fwrule->put_Profiles(NET_FW_PROFILE2_DOMAIN | NET_FW_PROFILE2_PRIVATE | NET_FW_PROFILE2_PUBLIC);
	if ( FAILED(hr) )
	{
		//LOG_ERROR("Failed INetFwRule::put_Profiles failed with error: 0x %x.\n", hr);
	}
#if 0
	// Check if INetFwRule2 interface is available (i.e Windows7+)
	// If supported, then use EdgeTraversalOptions
	// Else use the EdgeTraversal boolean flag.
	CComPtr<INetFwRule2> fwrule2;
	if (SUCCEEDED(fwrule->QueryInterface(__uuidof(INetFwRule2), (void**)&fwrule2)))
	{
		hr = fwrule2->put_EdgeTraversalOptions(NET_FW_EDGE_TRAVERSAL_TYPE_ALLOW);
		if ( FAILED(hr) )
		{
			//LOG_ERROR("Failed INetFwRule::put_EdgeTraversalOptions failed with error: 0x %x.\n", hr);
		}
	}
	else
	{

		hr = fwrule->put_EdgeTraversal(VARIANT_TRUE);
		if ( FAILED(hr) )
		{
			//LOG_ERROR(L"Failed INetFwRule::put_EdgeTraversal failed with error: 0x %x.\n", hr);
		}
	}
#endif
	hr = fwrule->put_Enabled(VARIANT_TRUE);
	if ( FAILED(hr) )
	{
		//LOG_ERROR("Failed INetFwRule::put_Enabled failed with error: 0x %x.\n", hr);
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
#include "WindowsVolumeControl.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Winmm.lib")

enum WinOsVersion
{
    VC_NA = 0,
    VC_WinXP = 1,
    VC_WinVista = 2,
};

enum VolumeOperation
{
    VO_NA = 0,
    VO_Mute = 1,
    VO_Volume = 2,
};

class WinXPVolCtrl
{
private:
    class WinXPSoundDevice
    {
    private:
        HMIXER m_hMixer;
        DWORD m_ctrlId;
    public:
        WinXPSoundDevice() : m_hMixer(NULL), m_ctrlId(0)
        {
        }
        ~WinXPSoundDevice()
        {
            close();
        }

        HMIXER getMixer() { return m_hMixer; }

        DWORD getControlId() { return m_ctrlId; }

        bool open(VolumeOperation operation)
        {
            // obtain a handle to the mixer device
            MMRESULT result = ::mixerOpen(&m_hMixer, MIXER_OBJECTF_MIXER, 0, 0, 0);
            if (result != MMSYSERR_NOERROR)
            {
                return false;
            }

            // get the speaker line of the mixer device
            MIXERLINE ml = {0};
            ml.cbStruct = sizeof(MIXERLINE);
            ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
            result = ::mixerGetLineInfo((HMIXEROBJ) m_hMixer,
                &ml, MIXER_GETLINEINFOF_COMPONENTTYPE);
            if (result != MMSYSERR_NOERROR)
            {
                this->close();
                return false;
            }

            // get the volume control of the speaker line
            MIXERLINECONTROLS mlc = {0};
            MIXERCONTROL mc = {0};
            mlc.cbStruct = sizeof(MIXERLINECONTROLS);
            mlc.dwLineID = ml.dwLineID;
            mlc.dwControlType = (operation == VO_Mute) ? MIXERCONTROL_CONTROLTYPE_MUTE : MIXERCONTROL_CONTROLTYPE_VOLUME;
            mlc.cControls = 1;
            mlc.pamxctrl = &mc;
            mlc.cbmxctrl = sizeof(MIXERCONTROL);
            result = ::mixerGetLineControls((HMIXEROBJ) m_hMixer,
                &mlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);
            if (result != MMSYSERR_NOERROR)
            {
                this->close();
                return false;
            }

            m_ctrlId = mc.dwControlID;
            return true;
        }

        bool close()
        {
            if (m_hMixer == NULL)
            {
                return true;
            }

            MMRESULT mr = ::mixerClose(m_hMixer);
            if (mr != MMSYSERR_NOERROR) 
            {
                return false;
            }
            m_hMixer = NULL;
            return true;
        }
    };
public:
    static int getMasterVolume()
    {
        WinXPSoundDevice device;
        bool succeeded = device.open(VO_Volume);
        if ( ! succeeded)
        {
            return 0;
        }

        // get volume state
        MIXERCONTROLDETAILS_UNSIGNED  mcud = {0};

        MIXERCONTROLDETAILS mcd = {0};
        mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
        mcd.hwndOwner = 0;
        mcd.dwControlID = device.getControlId();
        mcd.paDetails = &mcud;
        mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
        mcd.cChannels = 1;
        MMRESULT result = ::mixerGetControlDetails((HMIXEROBJ)device.getMixer(), &mcd,
                                            MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
        if (result != MMSYSERR_NOERROR)
        {
            return 0;
        }

        return static_cast<int>(100 * ((double)mcud.dwValue / 65535.0));
    }

    // volValue is in [0, 100]
    static bool setMasterVolume(int volValue)
    {
        // newVolumeLevel is in range [0, 65535]
        int newVolumeLevel = static_cast<int>((float)(volValue) / (float)100.0 * 65535);

        WinXPSoundDevice device;
        bool succeeded = device.open(VO_Volume);
        if ( ! succeeded)
        {
            return 0;
        }

        // set the volume level
        MIXERCONTROLDETAILS mcd = {0};
        MIXERCONTROLDETAILS_UNSIGNED mcdu = {0};
        mcdu.dwValue = newVolumeLevel;
        mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
        mcd.hwndOwner = 0;
        mcd.dwControlID = device.getControlId();
        mcd.paDetails = &mcdu;
        mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
        mcd.cChannels = 1;
        MMRESULT result = ::mixerSetControlDetails((HMIXEROBJ)device.getMixer(),
            &mcd, MIXER_SETCONTROLDETAILSF_VALUE);

        return (result == MMSYSERR_NOERROR);
    }

    static bool isMuted()
    {
        WinXPSoundDevice device;
        bool succeeded = device.open(VO_Mute);
        if ( ! succeeded)
        {
            return false;
        }

        // get mute state
        MIXERCONTROLDETAILS_BOOLEAN mcb = {0};

        MIXERCONTROLDETAILS mcd = {0};
        mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
        mcd.hwndOwner = 0;
        mcd.dwControlID = device.getControlId();
        mcd.paDetails = &mcb;
        mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mcd.cChannels = 1;
        MMRESULT result = ::mixerGetControlDetails((HMIXEROBJ)device.getMixer(), &mcd,
                                            MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
        if (result != MMSYSERR_NOERROR)
        {
            return false;
        }

        return static_cast<bool>(mcb.fValue);
    }

    static bool setMute(bool isToMute)
    {
        WinXPSoundDevice device;
        bool succeeded = device.open(VO_Mute);
        if ( ! succeeded)
        {
            return false;
        }

        // set mute/un-mute
        MIXERCONTROLDETAILS mcd = {0};
        MIXERCONTROLDETAILS_BOOLEAN mcb = {0};
        mcb.fValue    = isToMute;
        mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
        mcd.hwndOwner = 0;
        mcd.dwControlID = device.getControlId();
        mcd.paDetails = &mcb;
        mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mcd.cChannels = 1;
        MMRESULT result = ::mixerSetControlDetails((HMIXEROBJ)device.getMixer(),
                       &mcd, MIXER_SETCONTROLDETAILSF_VALUE);

        return (result == MMSYSERR_NOERROR);
    }
};


class WinVistaVolCtrl
{
private:
    class WinVistaSoundDevice
    {
    private:
        IAudioEndpointVolume* m_endpointVolume;
    public:
        WinVistaSoundDevice() : m_endpointVolume(NULL)
        {}

        ~WinVistaSoundDevice()
        {
            close();
        }

        IAudioEndpointVolume* getEndPointVolume() { return m_endpointVolume; }

        bool open()
        {
            ::CoInitialize(NULL);
            IMMDeviceEnumerator *deviceEnumerator = NULL;
            HRESULT hr = ::CoCreateInstance(
                __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
            if (hr != S_OK)
            {
                return false;
            }

            IMMDevice *defaultDevice = NULL;
            hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
            deviceEnumerator->Release();
            deviceEnumerator = NULL;
            if (hr != S_OK)
            {
                return false;
            }

            hr = defaultDevice->Activate(
                __uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER,
                NULL, (LPVOID *)&m_endpointVolume);
            defaultDevice->Release();
            defaultDevice = NULL;

            return (hr == S_OK);
        }

        bool close()
        {
            if (m_endpointVolume == NULL)
            {
                return true;
            }

            m_endpointVolume = NULL;
            ::CoUninitialize();
            return true;
        }
    };
public:
    static int getMasterVolume()
    {
        {
            WinVistaSoundDevice device;
            bool succeeded = device.open();
            if ( ! succeeded)
            {
                return 0;
            }

            float currentVolume = 0;
            HRESULT hr = device.getEndPointVolume()->GetMasterVolumeLevelScalar(&currentVolume);
            if (hr != S_OK)
            {
                return 0;
            }

            return (int)(currentVolume * 100);
        }
    }

    // newVolumeLevel is in [0, 100]
    static bool setMasterVolume(int volValue)
    {
        float newVolumeLevel = (float)(volValue % 100) / (float)100.0;

        WinVistaSoundDevice device;
        bool succeeded = device.open();
        if ( ! succeeded)
        {
            return false;
        }

        HRESULT hr = device.getEndPointVolume()->SetMasterVolumeLevelScalar(newVolumeLevel, NULL);
        return (hr == S_OK); // more readable, leave to tail-recursion optimization
    }

    static bool isMuted()
    {
        WinVistaSoundDevice device;
        bool succeeded = device.open();
        if ( ! succeeded)
        {
            return false;
        }

        BOOL isMuted = FALSE;
        HRESULT hr = device.getEndPointVolume()->GetMute(&isMuted);
        if (hr != S_OK)
        {
            return false;
        }

        return static_cast<bool>(isMuted);
    }

    static bool setMute(bool isToMute)
    {
        WinVistaSoundDevice device;
        bool succeeded = device.open();
        if ( ! succeeded)
        {
            return false;
        }

        HRESULT hr = device.getEndPointVolume()->SetMute(static_cast<BOOL>(isToMute), NULL);
        return (hr == S_OK);
    }
};

int WindowsVolumeControl::getMasterVolume()
{
    WinOsVersion vc = _getWinOsVer();
    if (vc == VC_WinVista)
    {
        return WinVistaVolCtrl::getMasterVolume();
    }
    else if (vc == VC_WinXP)
    {
        return WinXPVolCtrl::getMasterVolume();
    }

    return 0;
}

bool WindowsVolumeControl::setMasterVolume(int volValue)
{
    WinOsVersion vc = _getWinOsVer();
    if (vc == VC_WinVista)
    {
        return WinVistaVolCtrl::setMasterVolume(volValue);
    }
    else if (vc == VC_WinXP)
    {
        return WinXPVolCtrl::setMasterVolume(volValue);
    }
    return false;
}

bool WindowsVolumeControl::isMuted()
{
    WinOsVersion vc = _getWinOsVer();
    if (vc == VC_WinVista)
    {
        return WinVistaVolCtrl::isMuted();
    }
    else if (vc == VC_WinXP)
    {
        return WinXPVolCtrl::isMuted();
    }
    return false;
}

bool WindowsVolumeControl::setMute(bool mute)
{
    WinOsVersion vc = _getWinOsVer();
    if (vc == VC_WinVista)
    {
        return WinVistaVolCtrl::setMute(mute);
    }
    else if (vc == VC_WinXP)
    {
        return WinXPVolCtrl::setMute(mute);
    }
    return false;
}

WinOsVersion WindowsVolumeControl::_getWinOsVer()
{
    static WinOsVersion vcType = VC_NA;
    if (vcType != VC_NA)
    {
        return vcType;
    }

    OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx(&osver);
    if (osver.dwMajorVersion == 5)
    {
        vcType = VC_WinXP;
    }
    else if (osver.dwMajorVersion == 6)
    {
        vcType = VC_WinVista;
    }

    return vcType;
}

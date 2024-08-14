
#include "OSAgentWin.h"
#include <Windows.h>
#include <PowrProf.h>
// for windows 7 sdk compatibility
#define PSAPI_VERSION 1
#include <Psapi.h>
#include <Shlwapi.h>
#include <Tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>
#include <LMCONS.H>
#include <winbase.h>
#include <Strsafe.h>

#include <QCoreApplication>

#include "Utils.h"
#include "log.h"
#include <shlobj.h>
#include "ConfigManager.h"
#include "DateTimeUtils.h"
#include "remotesubclass.h"

using namespace std;

typedef void  (*PVOIDHOOKFN)(void);
typedef bool  (*PBOOLHOOKFN)(void);


#pragma comment(lib, "Psapi.lib ")
#pragma comment(lib, "Shlwapi.lib ")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "PowrProf.lib")

#define     PROGRAM_MANAGER L"Program Manager"  // Program manager window name
#define     TASKBAR         L"Shell_TrayWnd"        // Taskbar class name
#define     BUTTON_HND      L"BUTTON"       // Taskbar class name
#define     ID_STARTBUTTON  0x130               // Start button ID
#define     ID_TRAY         0x12F               // System tray ID
#define     ID_CLOCK        0x12F               // System clock ID



OSAgentWin::OSAgentWin()
: m_taskbarHidden(false), m_startmenuHidden(false),
  m_hotkeyDisabled(false), m_cadDisabled(false)
{
    m_subClass.reset(new RemoteSubClass);
}

OSAgentWin::~OSAgentWin()
{
    if (m_taskbarHidden == true)
    {
        showTaskBar();
    }

    if (m_startmenuHidden == true)
    {
        showStartMenu();
    }

    if (m_hotkeyDisabled == true)
    {
        enableSystemHotKey();
    }

    if (m_cadDisabled == true)
    {
        enableCAD();
    }
}

bool OSAgentWin::disableSystemHotKey()
{
    if (m_hotkeyDisabled == true)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: disableSystemHotKey";
        return true;
    }

    //LOG_DEBUG4 << "begin to disable system hot key";

    HMODULE mod = LoadLibrary(L"SystemHook.dll");
    if (mod == NULL)
    {
        return false;
    }
    PBOOLHOOKFN fn = (PBOOLHOOKFN)GetProcAddress(mod, "disableSystemHotkeys");
    if (fn == NULL)
    {
        return false;
    }
    bool result = fn();

    //LOG_DEBUG4 << "disable system hotkey result: " << ((result == true) ? "true" : "false");
    m_hotkeyDisabled = result;

    return result;
}

bool OSAgentWin::enableSystemHotKey()
{
    if (m_hotkeyDisabled == false)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: enableSystemHotKey";
        return true;
    }

    //LOG_DEBUG4 << "begin to enable system hot key";

    HMODULE mod = LoadLibrary(L"SystemHook.dll");
    if (mod == NULL)
    {
        return false;
    }
    PBOOLHOOKFN fn = (PBOOLHOOKFN)GetProcAddress(mod, "enableSystemHotkeys");
    if (fn == NULL)
    {
        return false;
    }
    bool result = fn();

    //LOG_DEBUG4 << "enable system hotkey result: " << ((result == true) ? "true" : "false");
    if (result == true)
    {
        m_hotkeyDisabled = false;
    }
    
    return result;
}

bool OSAgentWin::hideTaskBar( void )
{
    return true;

    if (m_taskbarHidden == true)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: hideTaskBar";
        return true;
    }

    //LOG_DEBUG4 << "begin to hide taskbar";
    HWND barWnd = FindWindow(TASKBAR, NULL);
    if (barWnd == NULL)
    {
        LOG_DEBUG4 << "can't find taskbar";
        return false;
    }

    ShowWindow(barWnd, SW_HIDE);
    UpdateWindow(barWnd);

    m_taskbarHidden = true;
    //LOG_DEBUG4 << "hide task bar succeeded";

    return true;
}

bool OSAgentWin::showTaskBar( void )
{
    return true;

    if (m_taskbarHidden == false)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: showTaskBar";
        return true;
    }

    //LOG_DEBUG4 << "begin to show taskbar";
    HWND barWnd = FindWindow(TASKBAR, NULL);
    if (barWnd == NULL)
    {
        LOG_DEBUG4 << "can't find taskbar";
        return false;
    }

    ShowWindow(barWnd, SW_SHOW);
    UpdateWindow(barWnd);

    m_taskbarHidden = false;
    //LOG_DEBUG4 << "show task bar succeeded";

    return true;
}

bool OSAgentWin::hideStartMenu( void )
{
    return true;

    if (m_startmenuHidden == true)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: hideStartMenu";
        return true;
    }

    //LOG_DEBUG4 << "begin to hide startmenu";
    HWND    hWnd;

    hWnd = GetDlgItem(FindWindow(TASKBAR, NULL), ID_STARTBUTTON);

    if (hWnd == NULL)
    {
        // for win7
        hWnd = FindWindow(BUTTON_HND, NULL);
    }

    if (hWnd == NULL)
    {
        LOG_DEBUG4 << "can't find start menu";
        return false;
    }

    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);

    //LOG_DEBUG4 << "hide start menu succeeded";

    m_startmenuHidden = true;
    return true;
}

bool OSAgentWin::showStartMenu( void )
{
    return true;

    if (m_startmenuHidden == false)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: showStartMenu";
        return true;
    }

    //LOG_DEBUG4 << "begin to show startmenu";
    HWND    hWnd;

    hWnd = GetDlgItem(FindWindow(TASKBAR, NULL), ID_STARTBUTTON);

    if (hWnd == NULL)
    {
        // for win7
        hWnd = FindWindow(BUTTON_HND, NULL);
    }

    if (hWnd == NULL)
    {
        LOG_DEBUG4 << "can't find start menu";
        return false;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    m_startmenuHidden = false;

    //LOG_DEBUG4 << "show start menu succeeded";

    return true;
}

bool OSAgentWin::disableCAD()
{
    if (m_cadDisabled == true)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: disableCAD";
        return true;
    }

    //LOG_DEBUG4 << "begin to disable Ctrl+Alt+Del";

    try
    {
        // suport windows xp only,so far
        if ( (GetVersion() & 0xff) != 5 )
            return true;

        bool result = m_subClass->SetSwitch(true);
        if (result == false)
        {
            LOG_ERROR << "disable CAD failed";
            return false;
        }
    }
    catch(...)
    {
        LOG_ERROR << "disableCAD failed unknown reason";
        return false;
    }

    //LOG_DEBUG4 << "disable Ctrl+Alt+Del succeeded";
    m_cadDisabled = true;

    return true;
}

bool OSAgentWin::enableCAD()
{
    if (m_cadDisabled == false)
    {
        return true;
    }

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        //LOG_DEBUG4 << "Simulated action: enableCAD";
        return true;
    }

    //LOG_DEBUG4 << "begin to enableCAD";

    try
    {
        // suport windows xp only,so far
        if ( (GetVersion() & 0xff) != 5 )
            return true;

        bool result = m_subClass->SetSwitch(false);
        if (result == false)
        {
            LOG_ERROR << "enableCAD failed.";
            return false;
        }
    }
    catch(...)
    {
        LOG_ERROR << "enableCAD failed unknown reason";
        return false;
    }

    //LOG_DEBUG4 << "enable Ctrl+Alt+Del succeeded";
    m_cadDisabled = false;

    return true;
}

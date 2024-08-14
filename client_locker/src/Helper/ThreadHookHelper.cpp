#include "ThreadHookHelper.h"
#include "GuiController.h"
#include "log.h"
#include <winuser.h>

HHOOK ThreadHookHelper::s_mouseHook = NULL;
HHOOK ThreadHookHelper::s_kbdHook = NULL;


ThreadHookHelper::~ThreadHookHelper()
{
    stop();
}

void ThreadHookHelper::start()
{
    //LOG_DEBUG4 << "Begin to hook input";
    if (isHookAlive())
    {
        //LOG_DEBUG4 << "Already hooked, skip";
        return;
    }

    if (s_mouseHook == NULL)
    {
        //LOG_DEBUG << "hook mouse";
        s_mouseHook = SetWindowsHookEx(WH_MOUSE, cbmouse, NULL, GetCurrentThreadId());
    }

    if (s_kbdHook == NULL)
    {
        //LOG_DEBUG << "hook kbd";
        s_kbdHook = SetWindowsHookEx(WH_KEYBOARD, cbkeyboard, NULL, GetCurrentThreadId());
    }

    //LOG_DEBUG4 << "Hook input done";
}

void ThreadHookHelper::stop()
{
    //LOG_DEBUG4 << "Begin to unhook input";
    if (false == isHookAlive())
    {
        //LOG_DEBUG4 << "Already unhooked, skip";
        return;
    }

    if (s_kbdHook)
    {
        //LOG_DEBUG << "unhook kbd";
        BOOL result = UnhookWindowsHookEx(s_kbdHook);
        if (result)
        {
            s_kbdHook = NULL;
        }
    }

    if (s_mouseHook)
    {
        //LOG_DEBUG << "unhook mouse";
        BOOL result = UnhookWindowsHookEx(s_mouseHook);
        if (result)
        {
            s_mouseHook = NULL;
        }
    }

    //LOG_DEBUG4 << "Unhook input done";
}

LRESULT __stdcall ThreadHookHelper::cbkeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || nCode != HC_ACTION )
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    GuiController::GetInstance()->popupLoginDialog();
    return 1;
}

LRESULT __stdcall ThreadHookHelper::cbmouse(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || nCode != HC_ACTION )
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    switch (wParam)
    {
    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        GuiController::GetInstance()->popupLoginDialog();
        return 1;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
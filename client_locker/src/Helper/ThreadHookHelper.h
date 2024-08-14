#ifndef THREADHOOKHELPER_H
#define THREADHOOKHELPER_H
#include <wtypes.h>

class ThreadHookHelper
{
public:
    ~ThreadHookHelper();

    static void start();
    static void stop();

    static bool isHookAlive()
    {
        return (
            s_mouseHook != NULL
            && s_kbdHook != NULL);
    }

public:
    static LRESULT __stdcall cbmouse(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT __stdcall cbkeyboard(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static HHOOK s_mouseHook;
    static HHOOK s_kbdHook;
};

#endif 

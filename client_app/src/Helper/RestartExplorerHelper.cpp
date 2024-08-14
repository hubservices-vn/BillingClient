#include "RestartExplorerHelper.h"
#include "OSAgentWinQt.h"
#include "log.h"
#include "Utils.h"
#include <QMutexLocker>
#include <shellapi.h>
#include "OSAgentWin.h"


void RestartExplorerThread::run()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    OSAgentWin* agent = OSAgentWinQt::GetInstance();
    // initialization
    static int retryCount = 0;
    static int lastPid = -1;

    // reset each time
    retryCount = 0;
    lastPid = -1;
    bool succeeded = true;
    while(true)
    {
        ++retryCount;
        if (retryCount > 100)
        {
            succeeded = false;
            break;
        }

        int currPid = -1;
        bool explorerExists = agent->isProcessRunning(L"explorer.exe", currPid);
        if (!explorerExists)
        {
            succeeded = true;
            break;
        }

        if (lastPid == -1 && currPid != -1)
        {
            lastPid = currPid;
        }

        if (lastPid != currPid)
        {
            succeeded = true;
            return;
        }

        LOG_DEBUG4 << "Attempting to Kill explorer.exe by name";
        agent->killProcessByName(L"explorer.exe");
        ::Sleep(500);
    }

    if (succeeded == false)
    {
        return;
    }

    // start the explorer
    // reset each time
    retryCount = 0;
    succeeded = true;
    while(true)
    {
        ++retryCount;
        if (retryCount > 1000)
        {
            succeeded = false;
            break;
        }

        bool explorerExists = agent->isProcessRunning(L"explorer.exe");
        if (explorerExists)
        {
            succeeded = true;
            break;
        }

        HINSTANCE hinst = ShellExecute(NULL, L"explorer.exe", NULL, NULL, NULL, SW_SHOWNORMAL);
        //BOOL result = CreateProcess( NULL, L"explorer.exe", NULL, NULL, false, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi );
        if (hinst == NULL)
        {
            DWORD error_code = GetLastError();
            LOG_INFO << "start explorer failed. Error code: " << error_code;
        }
        ::Sleep(200);
    }
}

RestartExplorerThread RestartExplorerHelper::m_thread;

void RestartExplorerHelper::restartExplorer()
{
    if (m_thread.isRunning())
    {
        return;
    }

    m_thread.start();
}

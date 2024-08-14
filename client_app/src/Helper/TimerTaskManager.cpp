#include "TimerTaskManager.h"
#include "SysInfoHelper.h"
#include <QProcess>
#include <QTimer>

#include "OSAgentWinQt.h"
#include "log.h"

const int SYS_USER_INIT_DELAY = 2000; // 2,000 ms

void TimerTaskManager::launchSystemUserInit()
{
    QTimer::singleShot(SYS_USER_INIT_DELAY, 
        TimerTaskManager::GetInstance(), SLOT(_launchSystemUserInitSlot()));
}

void TimerTaskManager::_launchSystemUserInitSlot()
{
    bool isExplorerRunning = OSAgentWinQt::GetInstance()->isProcessRunning(QString("explorer.exe").toStdWString());
    if (isExplorerRunning)
    {
        LOG_DEBUG4 << "Explorer running, skip";
        return;
    }

    LOG_DEBUG << "Explorer is NOT running";
    QString userInitExePath = SysInfoHelper::system32Folder() + "\\userinit.exe";
    bool succeeded = QProcess::startDetached(userInitExePath);
    if (!succeeded)
    {
        LOG_ERROR << "Failed to launch uinit";
    }
}

#include "UIAPPManager.h"
#include "Utils.h"
#include "ConfigManager.h"
#include <QDir>
#include <sstream>
#include <tlhelp32.h>
#include <tchar.h>
#include "OSAgentWinQt.h"
#include "ClientCommonSettings.h"
#include "DateTimeUtils.h"
#include <time.h>
#include "TickCountHelper.h"



const qint64 DEF_CHECK_CCCL_INTERVAL_SEC = 10; // 10,000 ms


#ifdef NDEBUG
// release
const qint64 DEF_NORESPONSE_INTERVAL = 30; // 30s
const qint64 DEF_RESTART_INTERVAL = 123; // 123s
#else
// debug
const qint64 DEF_NORESPONSE_INTERVAL = 10; // 10s
const qint64 DEF_RESTART_INTERVAL = 63; // 63s
#endif


bool ProcMonitor::isMonitorHung( qint64 checkDT )
{
    if ( m_lastCheckTime == 0)
    {
        m_lastCheckTime = TickCountHelper::GetInstance()->tickCount();
        return false;
    }

    if (qAbs(checkDT - m_lastCheckTime) > 2 * DEF_CHECK_CCCL_INTERVAL_SEC)
    {
        m_lastCheckTime = TickCountHelper::GetInstance()->tickCount();
        return true;
    }

    m_lastCheckTime = TickCountHelper::GetInstance()->tickCount();
    return false;
}

bool ProcMonitor::isLostResponse(const qint64 checkDT)
{
    if (m_lastPidHBTime <= 0)
    {
        return true;
    }

    qint64 secsPassed = qAbs(m_lastPidHBTime - checkDT);
    bool isLostResponse = secsPassed > DEF_NORESPONSE_INTERVAL;
    if (isLostResponse)
    {
        LOG_DEBUG4 << "nrp: " 
            << m_lastPidHBTime
            << ", " << checkDT;
    }
    return isLostResponse;
}

#ifdef NDEBUG
// release
const qint64 DEF_MIN_PROC_INTERVAL = 300; // 300s
const qint64 DEF_MIN_PROC_COUNT = 3; // check whether process is malicious when there's at least 3 recent HB records in list (prevent user keep killing our process)
const qint64 DEF_MAX_HB_RECORDS = 9; // store at most DEF_MAX_HB_RECORDS historical HB pid records
#else
const qint64 DEF_MIN_PROC_INTERVAL = 60; 
const qint64 DEF_MIN_PROC_COUNT = 3;
const qint64 DEF_MAX_HB_RECORDS = 9;
#endif


ProcMonitor::ProcMonitor() : m_currPid(-1), 
m_firstCheckTime(0), m_lastCheckTime(0),
m_firstPidHBTime(0), m_lastPidHBTime(0)
{
    m_recentHBList.reserve(DEF_MAX_HB_RECORDS);
}

bool ProcMonitor::isMalicious(const qint64 checkDT)
{
    if (m_firstCheckTime == 0)
    {
        m_firstCheckTime = checkDT;
        return false;
    }

    if (m_recentHBList.isEmpty())
    {
        LOG_DEBUG4 << "no rhblst";
        qint64 checkSecsPassed = qAbs(m_firstCheckTime - checkDT);
        bool neverConnected = (checkSecsPassed > DEF_RESTART_INTERVAL);
        if (neverConnected)
        {
            LOG_DEBUG4 << "1st ck: " << m_firstCheckTime
                << " cur ck: " << checkDT;
        }
        return neverConnected;
    }

    int hbRecSize = m_recentHBList.size();
    // check last HB record, see whether it's no response long enough
    // (prevent user suspend process/kill process and rename the exe)
    if (hbRecSize >= 1)
    {
        qint64 currentTickCount = TickCountHelper::GetInstance()->tickCount();
        const ProcHBRecord& pidHBRec = m_recentHBList.last();
        if (pidHBRec.secsSinceLastHB(currentTickCount) > DEF_RESTART_INTERVAL)
        {
            LOG_DEBUG4 
                << "last 1st: " << pidHBRec.FirstDT_
                << " lst: " << pidHBRec.LastDT_
                << " cur ck: " << currentTickCount;
            return true;
        }
    }

    // (prevent user killing process every 30s)
    // hbRecSize - 1 : exclude the current HB record (as it's still running)
    if ((hbRecSize - 1) >= DEF_MIN_PROC_COUNT)
    {
        qint64 totalMaliciousSecs = 0;
        qint64 totalNormalSecs = 0;
        int maliciousCount = 0;
        // exclude the current HB record (as it's still running)
        for (int i = 0; i < (hbRecSize - 1); ++i)
        {
            const ProcHBRecord& pidHBRec = m_recentHBList.at(i);
            if (pidHBRec.procSecs() < DEF_MIN_PROC_INTERVAL)
            {
                ++maliciousCount;
                totalMaliciousSecs += pidHBRec.procSecs();
                LOG_DEBUG4 << "hit. 1st: " 
                    << pidHBRec.FirstDT_
                    << ", lst: " << pidHBRec.LastDT_
                    << ", mct: " << maliciousCount
                    << ", msc: " << totalMaliciousSecs;
            }
            else
            {
                totalNormalSecs += (pidHBRec.procSecs());
            }
        }

        if (maliciousCount <= (double)hbRecSize * 0.4)
        {
            return false;
        }

        if (maliciousCount >= (double)hbRecSize * 0.6)
        {
            LOG_DEBUG4 << "ismls. mct: " << maliciousCount
                << ", rcsz: " << hbRecSize;
            return true;
        }

        if (totalMaliciousSecs * 6 >= totalNormalSecs)
        {
            LOG_DEBUG4 << "ismls. mscs: " << totalMaliciousSecs
                << ", nmscs: " << totalNormalSecs;
            return true;
        }

        return false;
    }

    return false;
}

void ProcMonitor::onHeartbeatRecved( quint32 pid )
{
    qint64 checkDT = TickCountHelper::GetInstance()->tickCount();
    m_lastPidHBTime = checkDT;

    if (pid != m_currPid)
    {
        m_currPid = pid;
        m_firstPidHBTime = checkDT;

        while(m_recentHBList.size() > DEF_MAX_HB_RECORDS - 1)
        {
            m_recentHBList.removeFirst();
        }
        m_recentHBList.append(ProcHBRecord(pid, checkDT, checkDT));
    }
    else
    {
        if ( ! m_recentHBList.isEmpty())
        {
            ProcHBRecord& lastRec = m_recentHBList.last();
            lastRec.LastDT_ = checkDT;
        }
    }
}

void ProcMonitor::reset()
{
    m_currPid = -1;

    m_firstCheckTime = 0;
    m_lastCheckTime = 0;
    m_firstPidHBTime = 0;
    m_lastPidHBTime = 0;

    clearHistoryHBProcRecords();
}

void ProcMonitor::clearHistoryHBProcRecords()
{
    m_recentHBList.clear();
}

bool UIAppManager::init()
{
    m_checkProcTimer.reset(new QTimer());
    m_checkProcTimer->setInterval(DEF_CHECK_CCCL_INTERVAL_SEC * 1000);
    connect(m_checkProcTimer.data(), SIGNAL(timeout()),
        this, SLOT(checkClientProcessSlot()));
    m_checkProcTimer->start();

    return true;
}

bool UIAppManager::connectSigSlot()
{
    return true;
}

void UIAppManager::onAppBeginConn( int pid )
{
    m_adminExit = false;
}

void UIAppManager::onAppEndConn()
{
    m_adminExit = true;

    // admin may launch and exit the app/locker multi times in short period
    // shouldn't treat this as "lost response" or "malicious"
    m_appMonitor.reset();
    m_appMonitor.clearHistoryHBProcRecords();

    m_lockerMonitor.reset();
    m_lockerMonitor.clearHistoryHBProcRecords();
}

void UIAppManager::onAppHBRecved( quint32 pid )
{
    LOG_DEBUG4 << "recv app HB: " << pid;
    m_appMonitor.onHeartbeatRecved(pid);
}

void UIAppManager::onLockerHBRecved( quint32 pid )
{
    LOG_DEBUG4 << "recv locker HB: " << pid;
    m_lockerMonitor.onHeartbeatRecved(pid);
}

const qint64 DELAY_CHECK_PROCESS_SEC = 30; // 30s
// delay 20s after service launches (give enough time for windows desktop, gbilling app and locker to launch)
void UIAppManager::checkClientProcessSlot()
{
    if (m_adminExit)
    {
        LOG_DEBUG << "The UI app is closed by Admin. ignore";
        return;
    }

    LOG_DEBUG4 << "chk gbcps";
    // if OS hasn't been logged in by any user, no need to check
    // the OS is waiting at the welcome screen
    static bool isExplororRun = false;
    if (!isExplororRun)
    {
        if (_GetExplorerPid() != 0)
        {
            isExplororRun = true;
        }
        else
        {
            LOG_DEBUG4 << "no exp";
            return;
        }
    }

    qint64 tickCount = TickCountHelper::GetInstance()->tickCount();
    qint64 svcUpSecs = tickCount;
    if (svcUpSecs < DELAY_CHECK_PROCESS_SEC)
    {
        LOG_DEBUG4 << "skip ck cp. upt: " << svcUpSecs;
        return;
    }

    qint64 checkDT = tickCount;
    if (m_appMonitor.isMonitorHung(checkDT)
        || m_lockerMonitor.isMonitorHung(checkDT))
    {
        LOG_DEBUG4 << "montor hung";
        return;
    }

    if (m_appMonitor.isLostResponse(checkDT))
    {
        LOG_DEBUG4 << "no arp";
        _startProcess(APP_EXE_NAMEWITHEXT, _getCAppFullPath());
    }

    if (m_lockerMonitor.isLostResponse(checkDT))
    {
        LOG_DEBUG4 << "no lrp";
        _startProcess(LOCKER_EXE_NAMEWITHEXT, _getCLockerFullPath());
    }

    if (m_appMonitor.isMalicious(checkDT)
        || m_lockerMonitor.isMalicious(checkDT))
    {
        LOG_DEBUG << "auth-";
        OSAgentWinQt::GetInstance()->restartComputer();
    }
}

QString UIAppManager::_getCAppFullPath()
{
    return QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "\\" + APP_EXE_NAMEWITHEXT);
}

QString UIAppManager::_getCLockerFullPath()
{
    return QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "\\" + LOCKER_EXE_NAMEWITHEXT);
}

void UIAppManager::_startProcess( const QString& procName, const QString& fullPath)
{
    int pid = 0;
    bool isRunning = OSAgentWinQt::GetInstance()->isProcessRunning(procName.toStdWString(), pid);
    if (pid > 0)
    {
        OSAgentWinQt::GetInstance()->killProcessByPid(pid);
    }

    quint32 explorerPid = _GetExplorerPid();
    if (explorerPid == 0)
    {
        LOG_ERROR << "no exp pd";
        return;
    }

    HANDLE explorerToken = _GetProcessToken(explorerPid);
    if (explorerToken == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR << "no exp tk";
        return;
    }

    if (CConfigManager::isDeveloperEnv())
    {
        LOG_DEBUG4 << "simulated action: relaunch process: " << qPrintable(fullPath);
        return;
    }

    STARTUPINFO si={0};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi={0};
    BOOL bResult=CreateProcessAsUser(explorerToken, fullPath.toStdWString().c_str() ,NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
    if (bResult)
    {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    LOG_DEBUG << "after restarted, pid: " << pi.dwProcessId;
}

quint32 UIAppManager::_GetExplorerPid()
{
    quint32 pid = 0;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hsnap == INVALID_HANDLE_VALUE)
    {
        return pid;
    }

    PROCESSENTRY32 pe32={0};
    pe32.dwSize = sizeof(pe32);
    if (Process32First(hsnap, &pe32))
    {
        do
        {
            if (!_tcsicmp(pe32.szExeFile, L"EXPLORER.EXE"))
            {
                //LOG_DEBUG4 << "EXPLORER.EXE pid:" << pe32.th32ProcessID;
                pid = pe32.th32ProcessID;
                break;
            }
        } 
        while (Process32Next(hsnap,&pe32));
    }
    CloseHandle(hsnap);
    return pid;
}

HANDLE UIAppManager::_GetProcessToken( quint32 pid )
{
    HANDLE hToken = NULL;
    HANDLE  hTokenNew = NULL;
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if ( ! hProc)
    {
        LOG_DEBUG4 << "opc failed,last error:" << GetLastError();
        return hTokenNew;
    }

    if(OpenProcessToken(hProc, TOKEN_DUPLICATE|TOKEN_READ, &hToken))
    {
        DWORD dwRet=0;
        if (DuplicateTokenEx( hToken, TOKEN_ALL_ACCESS, NULL, 
            SecurityImpersonation, TokenPrimary, &hTokenNew))
        {
        }
        else
        {
            LOG_DEBUG4 << "Dtke failed,last error:" << GetLastError();
        }
        CloseHandle(hToken);
    }
    else
    {
        LOG_DEBUG4 << "otk failed,last error:" << GetLastError();
    }
    CloseHandle(hProc);

    return hTokenNew;
}


#ifndef UIAPPMANAGER_H
#define UIAPPMANAGER_H
#include <QObject>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;



class ProcMonitor : public QObject
{
    Q_OBJECT
public:
    ProcMonitor();

public:
    void reset();

    void clearHistoryHBProcRecords();

    // this is to solve the "system hung" in diskless system
    // if the client pc (diskless) loses connection to diskless server
    // the whole pc will get hung (OS frozen, thus our program will also get frozen)
    // suppose the hung last 10 minutes, after the connection to diskless resumes
    // OS and our program are unfreezed, and it finds last heart-beat time is 10 minutes ago
    // thus it reboot the computer, this is NOT what we want.
    // the expected behavior is: if system gets hung, we should be able to detect and ignore the hung minutes
    bool isMonitorHung(qint64 checkDT);

    bool isLostResponse(const qint64 checkDT);
    // the process should keep running for some time 
    // (so if hacking keep killing our process and relaunch it, we can detect it)
    bool isMalicious(const qint64 checkDT);

    void onHeartbeatRecved(quint32 pid);

private:
    struct ProcHBRecord 
    {
        ProcHBRecord(qint64 pid, 
            qint64 firstDT, qint64 lastDt)
            : Pid_(pid), FirstDT_(firstDT), LastDT_(lastDt)
        {}

        qint64 procSecs() const
        {
            if ( FirstDT_ == 0
                || LastDT_ == 0)
            {
                return 0;
            }
            return qAbs(FirstDT_ - LastDT_);
        }

        qint64 secsSinceLastHB(qint64 currDT) const
        {
            if ( LastDT_ == 0 )
            {
                return 0xFFFFFFFF;
            }
            return qAbs(LastDT_ - currDT);
        }

        qint64 Pid_;
        qint64 FirstDT_;
        qint64 LastDT_;
    };
    QList<ProcHBRecord> m_recentHBList;

    qint64 m_currPid;

    qint64 m_firstCheckTime; // the 1st time that process is checked (in tick counts)
    qint64 m_lastCheckTime; // the last time when the process is checked (in tick counts)

    qint64 m_firstPidHBTime; // if pid changed, this will be reset
    qint64 m_lastPidHBTime;
};


#include <QScopedPointer>
#include <QTimer>
class UIAppManager : public QObject, public Singleton_T<UIAppManager>
{
    Q_OBJECT
public:
    UIAppManager()
        : m_adminExit(false)
    {}

    bool init();
    bool connectSigSlot();

    // client app sends "Begin Connection" cmd, set adminExit flag to false
    void onAppBeginConn(int pid);

    void onAppEndConn();

    void onAppHBRecved(quint32 pid);
    void onLockerHBRecved(quint32 pid);

public slots:
    void checkClientProcessSlot();

private:
    void _startProcess(const QString& procName, const QString& fullPath);

    QString _getCAppFullPath();
    QString _getCLockerFullPath();

    quint32 _GetExplorerPid();
    HANDLE _GetProcessToken(quint32 pid);
private:
    // if the client app is closed by cafe admin 
    // (via clicking the "Exit" button), then don't restart it
    bool m_adminExit;

private:
    QScopedPointer<QTimer> m_checkProcTimer;

    ProcMonitor m_appMonitor;
    ProcMonitor m_lockerMonitor;
};

#endif // UIAPPMANAGER_H

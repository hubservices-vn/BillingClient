#ifndef OSAGENTWIN_H
#define OSAGENTWIN_H

#pragma warning (disable: 4805 4995)

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QList>

typedef unsigned long DWORD;

class QueryTermApp_AppItem;
class QPixmap;
class OSAgentWin 
{
public:

    virtual ~OSAgentWin();

    virtual bool registerAsAutoRun( void ) = 0;
    virtual bool unregisterAsAutoRun( void ) = 0;
    
    virtual bool disableSystemTaskManager( void ) = 0;
    virtual bool enableSystemTaskManager( void ) = 0;

    virtual bool disableNetworkConnections() = 0;
    virtual bool enableNetworkConnections() = 0;

    virtual bool disableControlPanelItem( const std::wstring & name ) = 0;
    virtual bool enableControlPanelItem( const std::wstring & name ) = 0;
    
    virtual bool disableAppRunByProcName( const std::wstring & processName ) = 0;
    virtual bool enableAppRunByProcName( const std::wstring & processName ) = 0;

    virtual bool disableAppRunByHash (const QString& fileName, 
        const QString& hashStr, const QString& itemSizeStr, const QString& hashAlgStr) = 0;
    virtual bool enableAppRunByHash (const QString& hashStr) = 0;

    virtual bool disableAppRunBySig (const QString& fileName, 
        const QString& sig) = 0;
    virtual bool enableAppRunBySig (const QString& sig) = 0;


    // check whether a process with the name is already running
    virtual bool isProcessRunning( const std::wstring & processName);
    virtual bool isProcessRunning( const std::wstring & processName, int & pid);

    virtual bool killProcessByName( const std::wstring & processName );
    virtual bool killProcessByPid(int pid);
    virtual bool killProcessByWnd(int handle);

    virtual bool killAllRunningApps(const QStringList& excludeList);

    virtual bool getRunningProcessNameListEx(
        /*out*/QList<QueryTermApp_AppItem>& procList, 
        /*in*/const QStringList& excludeAppList);
    virtual QString getProcessNameByPid (DWORD pid);

    virtual bool disableRegedit() = 0;
    virtual bool enableRegedit() = 0;

    virtual bool disableAdministrativeTools() = 0;
    virtual bool enableAdministrativeTools() = 0;

    virtual bool disableCDRomAutorun() = 0;
    virtual bool enableCDRomAutorun() = 0;

    // prevent other apps from viewing self
    virtual bool initSecurityProtection() = 0;
    //virtual bool showSelfFromAppList() = 0;

    virtual bool enableAllProcSecurity() = 0;
    virtual bool applyDefaultProcSecurity() = 0;

	virtual bool disableClose() = 0;
	virtual bool enableClose() = 0;

	virtual bool disableLogOff() = 0;
	virtual bool enableLogOff() = 0;

	virtual bool disableSwitchUser() = 0;
	virtual bool enableSwitchUser() = 0;

	virtual bool disableLock() = 0;
	virtual bool enableLock() = 0;

	virtual bool disableRunCommand() = 0;
	virtual bool enableRunCommand() = 0;

	virtual void refreshExplorer() = 0;

    virtual bool shutdownComputer();
    virtual bool restartComputer();
    virtual bool standbyComputer();
    virtual bool logoffComputer();

    //WINDOWS 7
    virtual bool disableControlPanelItemWindows7 ( const QStringList& listItems) = 0;
    virtual bool disableProgramsAndFeaturesWindows7 () = 0;
    virtual bool enableProgramsAndFeaturesWindows7 () = 0;
    virtual bool enableLogOffWindows7() = 0;
    virtual bool disableLogOffWindows7() = 0;
    virtual bool enableProcHost() = 0;
    virtual bool disableProcHost() = 0;
    virtual bool disableLockWindows7() = 0;
    virtual bool disableCloseWindows7() = 0;
    virtual bool enableLockWindows7() = 0;
    virtual bool enableCloseWindows7() = 0;
    virtual void refreshGroupPolicy() = 0;
protected:
};

#endif // OSAGENTWIN_H

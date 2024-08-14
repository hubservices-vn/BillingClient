#ifndef OSAGENTWINQT_H
#define OSAGENTWINQT_H

#include "OSAgentWin.h"
#include "prochook.h"

class OSAgentWinQt : public OSAgentWin,
    public IProcEventSink
{
public:
    static OSAgentWin* GetInstance()
    {
        static OSAgentWinQt instance;
        return &instance;
    }

public:
    OSAgentWinQt();
    ~OSAgentWinQt();
public:
    bool registerAsAutoRun ( void );
    bool unregisterAsAutoRun ( void );

    bool disableSystemTaskManager ( void );
    bool enableSystemTaskManager ( void );

    bool disableNetworkConnections();
    bool enableNetworkConnections();

    bool disableControlPanelItem ( const std::wstring & name );
    bool enableControlPanelItem ( const std::wstring & name );

    bool disableAppRunByProcName ( const std::wstring & processName );
    bool enableAppRunByProcName ( const std::wstring & processName );

    bool disableAppRunByHash (const QString& fileName, 
        const QString& hashStr, const QString& itemSizeStr, const QString& hashAlgStr);
    bool enableAppRunByHash (const QString& hashStr);
    //bool enableAllAppsRunByHash ();

    bool disableAppRunBySig (const QString& fileName, 
        const QString& sig);
    bool enableAppRunBySig (const QString& sig);

    bool disableRegedit();
    bool enableRegedit();

    bool disableAdministrativeTools();
    bool enableAdministrativeTools();

    bool disableCDRomAutorun() ;
    bool enableCDRomAutorun() ;

	bool disableClose();
	bool enableClose();

	bool disableLogOff();
	bool enableLogOff();

	bool disableSwitchUser();
	bool enableSwitchUser();

	bool disableLock();
	bool enableLock();

	bool disableRunCommand();
	bool enableRunCommand();

	void refreshExplorer();

    bool initSecurityProtection();
    //bool showSelfFromAppList();

    virtual bool enableAllProcSecurity();
    virtual bool applyDefaultProcSecurity();

    //WINDOWS 7
    bool disableControlPanelItemWindows7 ( const QStringList& listItems);
    bool disableProgramsAndFeaturesWindows7 ();
    bool enableProgramsAndFeaturesWindows7 ();
    bool enableLogOffWindows7();
    bool disableLogOffWindows7();
    bool enableProcHost();
    bool disableProcHost();
    bool disableLockWindows7();
    bool disableCloseWindows7();
    bool enableLockWindows7();
    bool enableCloseWindows7();
    void refreshGroupPolicy();
private:
    virtual bool _hideTaskmgr( );
    virtual bool _showTaskmgr( );

private:
    static QString m_productKeyHive;
    IProcHost* m_prochost;
    bool m_procHostValid; // if install prochost failed, this would be false;
    bool updateGroupPolicy(QString policy, QString key, unsigned long val);
    bool updateGroupPolicy(QString policy, QString key, QString val);
protected:
    bool removeGroupPolicy(QString policy, QString key);
    bool removeGroupPolicyKey(QString policy, QString key);
    virtual void OnData(const char* data){}
};

#endif // OSAGENTWINQT_H

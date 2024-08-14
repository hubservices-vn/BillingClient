#ifndef OSAGENTWINQT_H
#define OSAGENTWINQT_H

#include <OSAgentWin.h>
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
    bool disableSystemTaskManager ( void );
    bool enableSystemTaskManager ( void );

    bool disableNetworkConnections();
    bool enableNetworkConnections();

    bool disableControlPanelItem ( const std::wstring & name );
    bool enableControlPanelItem ( const std::wstring & name );
    
    bool disableAppRunByProcName ( const std::wstring & processName );
    bool enableAppRunByProcName ( const std::wstring & processName );

    bool disableAppRunByHash (const QString& fileName, const QString& hashStr, const QString& itemSizeStr, const QString& hashAlgStr);
    bool enableAppRunByHash (const QString& hashStr);
    //bool enableAllAppsRunByHash ();

    bool disableRegedit();
    bool enableRegedit();

    bool disableAdministrativeTools();
    bool enableAdministrativeTools();

    bool disableCDRomAutorun() ;
    bool enableCDRomAutorun() ;

    //bool hideSelfFromAppList();
    //bool showSelfFromAppList();

    virtual bool enableAllProcSecurity();
    virtual bool applyDefaultProcSecurity();

private:
    static QString m_productKeyHive;
    IProcHost* m_prochost;

protected:
    virtual void OnData(const char* data){}
};

#endif // OSAGENTWINQT_H

#ifndef OSAGENTWIN_H
#define OSAGENTWIN_H

#pragma warning (disable: 4805 4995)

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QList>

class QueryTermApp_AppItem;
class QPixmap;

class OSAgentWin //: public OSAgent
{
public:

    virtual ~OSAgentWin();

    virtual bool disableSystemTaskManager                 ( void ) = 0;
    virtual bool enableSystemTaskManager                  ( void ) = 0;

    virtual bool disableNetworkConnections() = 0;
    virtual bool enableNetworkConnections() = 0;

    virtual bool disableControlPanelItem                      ( const std::wstring & name ) = 0;
    virtual bool enableControlPanelItem                       ( const std::wstring & name ) = 0;

    virtual bool disableAppRunByProcName                ( const std::wstring & processName ) = 0;
    virtual bool enableAppRunByProcName          ( const std::wstring & processName ) = 0;

    virtual bool disableAppRunByHash (const QString& fileName, const QString& hashStr, const QString& itemSizeStr, const QString& hashAlgStr) = 0;
    virtual bool enableAppRunByHash (const QString& hashStr) = 0;
    //virtual bool enableAllAppsRunByHash () = 0;

    // check whether a process with the name is already running
    virtual bool isProcessRunning( const std::wstring & processName);
    virtual bool isProcessRunning( const std::wstring & processName, int & pid);

    virtual bool killProcessByPid(int pid);

    virtual bool disableRegedit() = 0;
    virtual bool enableRegedit() = 0;

    virtual bool disableAdministrativeTools() = 0;
    virtual bool enableAdministrativeTools() = 0;

    virtual bool disableCDRomAutorun() = 0;
    virtual bool enableCDRomAutorun() = 0;

    // prevent other apps from viewing self
    //virtual bool hideSelfFromAppList() = 0;
    //virtual bool showSelfFromAppList() = 0;

    virtual bool enableAllProcSecurity() = 0;
    virtual bool applyDefaultProcSecurity() = 0;

    virtual bool setLocalTime(const QDate &date, const QTime &time, 
        int timezoneBias,const std::string& timezoneName);

    virtual bool shutdownComputer();
    virtual bool restartComputer();

    virtual quint32 getExplorerPid();
protected:

};

#endif // OSAGENTWIN_H

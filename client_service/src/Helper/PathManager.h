#ifndef PATHMANAGER_H
#define PATHMANAGER_H

class PathManager
{
public:
    // always this path
    // win7: %ProgramData%\gbClientService or %ProgramData%\gbClientServiceI (for Internal versoin)
    static QString getAppDataFolder();

    static QString getDBFolder();

    // always this path
    // win7: %ProgramData%\gbClientService(I?)\logs
    static QString getLogFolder();

};

class SysPathHelper
{
    // win xp: C:\Documents and Settings\carl\Local Settings\Temp
    // win 7 : C:\Users\carl\AppData\Local\Temp
    static QString getTmpFolder();

    // win xp: C:\Documents and Settings\All Users
    // win 7: c:\programdata
    static QString getAllUserProfileFolder();

    // c:\windows
    static QString getSystemRootFolder();
};

#endif // PATHMANAGER_H

#ifndef PATHMANAGER_H
#define PATHMANAGER_H

#include <QObject>
class PathManager
{
public:
    // always this path
    // win7: %ProgramData%\gbClientService or %ProgramData%\gbClientServiceI (for Internal versoin)
    static QString getAppFolder();

    // always this path
    // win7: %ProgramData%\gbClientService(I?)\logs
    static QString getLogFolder();

};

#endif // PATHMANAGER_H

#ifndef OSAGENTWINQT_H
#define OSAGENTWINQT_H

#include "OSAgentWin.h"
class OSAgentWinQt : public OSAgentWin
{
public:
    static OSAgentWin* GetInstance()
    {
        static OSAgentWinQt instance;
        return &instance;
    }

private:
    OSAgentWinQt();
    ~OSAgentWinQt();
public:

private:
    static QString m_productKeyHive;
};

#endif // OSAGENTWINQT_H

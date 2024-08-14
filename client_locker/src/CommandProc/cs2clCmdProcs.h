#ifndef CS2CCCMDREGISTER_H
#define CS2CCCMDREGISTER_H

#include <QObject>
#include "CommandsRouter.h"
#include "CcCore/CcSingleton.h"

class cs2clCmdProcs : 
    public CommandsRouter,
    public Singleton_T<cs2clCmdProcs>
{
public:
    void registerProcs();
};

#endif // SS2CSCMDREGISTER_H

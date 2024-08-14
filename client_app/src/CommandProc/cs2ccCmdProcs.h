#ifndef CS2CCCMDREGISTER_H
#define CS2CCCMDREGISTER_H

#include <QObject>
#include "CommandsRouter.h"
#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class cs2ccCmdProcs : 
    public CommandsRouter,
    public Singleton_T<cs2ccCmdProcs>
{
public:
    void registerProcs();
};

#endif // SS2CSCMDREGISTER_H

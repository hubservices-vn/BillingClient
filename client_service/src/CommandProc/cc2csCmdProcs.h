#ifndef CC2CSCMDREGISTER_H
#define CC2CSCMDREGISTER_H

#include <QObject>
#include "CommandsRouter.h"

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class cc2csCmdProcs : 
    public CommandsRouter, 
    public Singleton_T<cc2csCmdProcs>
{
public:
    void registerProcs();
};

#endif // SS2CSCMDREGISTER_H

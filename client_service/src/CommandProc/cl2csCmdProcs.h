#ifndef CL2CS_CMDREGISTER_H
#define CL2CS_CMDREGISTER_H

#include <QObject>
#include "CommandsRouter.h"

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class cl2csCmdProcs : 
    public CommandsRouter, 
    public Singleton_T<cl2csCmdProcs>
{
public:
    void registerProcs();
};

#endif // SS2CSCMDREGISTER_H

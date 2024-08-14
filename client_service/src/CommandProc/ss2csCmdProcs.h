#ifndef SS2CSCMDREGISTER_H
#define SS2CSCMDREGISTER_H


#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include "CommandsRouter.h"

class ss2csCmdProcs : 
    public CommandsRouter,
    public Singleton_T<ss2csCmdProcs>
{
public:
    void registerProcs();

};
#endif // SS2CSCMDREGISTER_H

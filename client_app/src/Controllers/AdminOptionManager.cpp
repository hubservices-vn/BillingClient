#include "AdminOptionManager.h"
#include "AdminOptionQtModelC.h"
#include "ConfigManager.h"


bool AdminOptionManager::init()
{
    return true;
}

bool AdminOptionManager::connectSigSlot()
{
    return true;
}

void AdminOptionManager::procAdminOptionReply( const AdminOptionReplyQtModelC &qtModel )
{
    if ((qtModel.OpType_ & AdminOp_ServerAddress) != 0)
    {
        if (qtModel.Result_ == AdminOpResult_OK)
        {
            CConfigManager::setServerAddress(qtModel.ServerAddress_);
        }
    }

    if ((qtModel.OpType_ & AdminOp_LocalAdminAcc) != 0)
    {
        if (qtModel.Result_ == AdminOpResult_OK)
        {
            //CConfigManager::setAdminNameRaw(qtModel.NewAdminName_);
        }
    }

    emit adminOptionReplyRecved(qtModel.OpType_, qtModel.Result_);
}

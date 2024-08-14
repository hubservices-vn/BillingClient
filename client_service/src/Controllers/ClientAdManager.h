#ifndef CLIENTADMANAGER_H
#define CLIENTADMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

class ClientAdTransferReplyQtModel;
class ClientAdManager :
        public QObject,
        public Singleton_T<ClientAdManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public:
    void procClientAdReplyC(const ClientAdTransferReplyQtModel& qtModel);

signals:
    
public slots:
    
};

#endif // CLIENTADMANAGER_H

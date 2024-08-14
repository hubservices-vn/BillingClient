#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

class FlashTransferReplyQtModel;
class FlashManager :
        public QObject,
        public Singleton_T<FlashManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public:
    void procFlashReplyC(const FlashTransferReplyQtModel& qtModel);

signals:
    
public slots:
    
};

#endif // FLASHMANAGER_H

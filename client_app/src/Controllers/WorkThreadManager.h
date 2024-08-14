#ifndef WORKTHREADMANAGER_H
#define WORKTHREADMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class WorkThreadManager : public QObject, public Singleton_T<WorkThreadManager>
{
    Q_OBJECT
public:
    bool initThreads();
    bool connectSigSlot();
    bool startThreads();

public: // accessor
    QThread* cc2csNetworkThread() const { return m_cc2csNetworkThread; }

    QThread* logicThread() const { return m_logicThread; }
signals:
    
public slots:
    
private:
    QThread* m_cc2csNetworkThread;
    
    // used for useraccount/session/product/order manager, etc
    QThread* m_logicThread;
};

#endif // WORKTHREADMANAGER_H

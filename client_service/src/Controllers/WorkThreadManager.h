#ifndef WORKTHREADMANAGER_H
#define WORKTHREADMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QThread>

class WorkThreadManager : public QObject, public Singleton_T<WorkThreadManager>
{
    Q_OBJECT
public: // ctor & dtor
    WorkThreadManager()
        : m_gbProtectThread(NULL)
    {}

    ~WorkThreadManager();

public: // fundamental method
    bool initThreads();
    bool startThreads();

public: // accessor
    QThread* gbProtectThread() const;
signals:

public slots:

private:
    QThread* m_gbProtectThread;
};

#endif // WORKTHREADMANAGER_H

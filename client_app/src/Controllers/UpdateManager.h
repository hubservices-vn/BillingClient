#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H


#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;


#include <QObject>
#include <QThread>
#include <QScopedPointer>

class UpdateTask : public QObject
{
    Q_OBJECT
public:
    explicit UpdateTask(QObject *parent = 0);

public:
    void clearToDelFolder();
    void doUpdatingProcess();

signals:
    
public slots:
    

private:
    void _applyPreviousUpdate();
    void _doNewUpdate();
};


class UpdateThread : public QThread
{
    Q_OBJECT
public:
    UpdateThread();

private:
    void run();
    QScopedPointer<UpdateTask> m_task;
};

class UpdateManager : public QObject, public Singleton_T<UpdateManager>
{
    Q_OBJECT
public:
    bool beginUpdate(int tarVer, const QString& hash);

private:
    QScopedPointer<UpdateThread> m_thread;
};
#endif // UPDATEMANAGER_H

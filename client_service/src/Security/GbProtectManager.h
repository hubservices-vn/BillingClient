#ifndef GBPROTECTMANAGER_H
#define GBPROTECTMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include <QTimer>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class IGBKernel;
class GbProtectManager_p: public QObject
{
    Q_OBJECT
public:
    explicit GbProtectManager_p(QObject *parent = 0);
    ~GbProtectManager_p();

public slots:
    void init();
    void sysHeartbeatSlot();
    void onAppEndConnection();

private:
    IGBKernel* m_gbKernal;
    QScopedPointer<QTimer> m_sysHBTimer;
};




class GbProtectManager 
    : public QObject, 
    public Singleton_T<GbProtectManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    void onAppEndConnection();

private slots:
    void init_imp();

private:
    QScopedPointer<GbProtectManager_p> m_pimp;
};

#endif // GBPROTECTMANAGER_H

#ifndef GCAINFOMANAGER_H
#define GCAINFOMANAGER_H

#include <QObject>


#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class GcaInfoManager : public QObject, public Singleton_T<GcaInfoManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

signals:

public slots:
    void serverConnectedSlot();

    void _delayGetGcaIdAndReport();

private:
    int _getGcaId();

};

#endif // GCAINFOMANAGER_H

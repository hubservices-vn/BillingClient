#ifndef WEBHISTORYMANAGER_H
#define WEBHISTORYMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
class WebHistoryReportQtModel;
class WebHistoryManager : public QObject, public Singleton_T<WebHistoryManager>
{
    Q_OBJECT
public:

public:
    bool init();
    bool connectSigSlot();

    // cmd handler
public:
    void procWebHistoryReportC( WebHistoryReportQtModel &qtModel );

signals:

public slots:

private:

};

#endif // WEBHISTORYMANAGER_H

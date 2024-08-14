#ifndef HOSTFILEHELPER_H
#define HOSTFILEHELPER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include "SmartPtrs.h"


class WebControl;
class HostFileHelper : public QObject, public Singleton_T<HostFileHelper>
{
    Q_OBJECT
public:
    bool updateHostsFile(const QMap<quint64, WebControl*> &contentMap);

private: // host operation
    bool _isGbBeginSection(const QString& line);
    bool _isGbEndSection(const QString& line);
private:
    bool _createTempHostFile(QString& result);

signals:
    
public slots:
    
};

#endif // HOSTFILEHELPER_H

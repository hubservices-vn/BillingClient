#ifndef WEBCONTROLDB_H
#define WEBCONTROLDB_H

#include <QObject>


class WebControl;
class WebControlDB : public QObject
{
    Q_OBJECT
public:
    explicit WebControlDB(QObject *parent = 0);
    ~WebControlDB();

public: // methods
    bool loadDB();
    WebControl* addControl(quint64 controlId, const QString& url, const QDateTime& updateDT);
    bool modifyControl(quint64 controlId, const QString& url, const QDateTime& updateDT);
    bool removeControl(quint64 controlId);

public: // helpers
    WebControl* getControlById(quint64 controlId);

private:
    bool _modifyControlImp(WebControl* control, const QString& url, const QDateTime& updateDT);
    bool _removeControlImp(WebControl* control);

signals:

public slots:

private:
    QMap<quint64/*id*/, WebControl*> m_webMap;
    friend class WebControlMgrThread;
    friend class WebControlManager2;
};

#endif // WEBCONTROLDB_H

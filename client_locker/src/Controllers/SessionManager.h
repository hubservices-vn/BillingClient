#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H


#include <QObject>
#include "CcCore\CcSingleton.h"

class Session;
class SessionBeginQtModel;
class SessionEndQtModel;
class SessionManager : public QObject, public Singleton_T<SessionManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // cmd handler
    void procSessionBegin( const SessionBeginQtModel& qtModel );

    void procSessionEnd(const SessionEndQtModel& qtModel);

    quint64 currentSessionId() const { return m_currentSessionId;}

    void onSessionIdReceived(quint64 ssId) { m_currentSessionId = ssId;};
signals:
    void sessionEnded();
    void sessionStarted();

public slots:

private:
    quint64 m_currentSessionId;
};

#endif // SESSIONMANAGER_H

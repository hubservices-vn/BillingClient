#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

class Session;
class SessionBeginQtModel;
class SessionEndQtModel;
class SessionStatusQtModel;
class SessionManager : public QObject, public Singleton_T<SessionManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // accessors
    Session* session() const { return m_session; }

    // helper accessors or methods
    quint64 currentSessionId() const;

    // if there's no session, return default values (likely, 0)
    // otherewise return the session's detail info
    int totalMinutes() const;
    int minutesUsed() const;
    int minutesLeft() const;
    double timeFee() const;
    double serviceFee() const;

public: // cmd handler
    void procSessionStatus(const SessionStatusQtModel &qtModel);

    void procSessionBegin( const SessionBeginQtModel& qtModel );

    void procSessionEnd(const SessionEndQtModel& qtModel);

public:
    void endSession();

signals:
    void sessionUpdated();
    void sessionEnded();
    void sessionStarted();

public slots:

private:
    Session* m_session;
};

#endif // SESSIONMANAGER_H

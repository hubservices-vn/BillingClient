#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>
#include <math.h>
#include "stdTypes.h"
#include <QTime>

class Session
{
public:
    explicit Session(uint64_t sessionId, int minutesUsed, int minutesLeft, double timeFee, double serviceFee);
    ~Session();

    // methods
    void updateValue( int minsUsed, int minsLeft, double timeFee, double serviceFee);

    uint64_t sessionId() const { return m_sessionId; }
    void setSessionId(uint64_t val) { m_sessionId = val; }

public: // helper
    bool isAlive() const;

public: // accessors
    QString TotalMinutesStr();

    QString minutesUsedStr();
    int minutesUsed() const;

    QString minutesLeftStr();
    int minutesLeft() const;

    //QString MinutesUsedStr();
    int MinutesUsed() const { return m_minutesUsed; }
    void SetMinutesUsed(int val) { m_minutesUsed = val; }

    //QString MinutesLeftStr();
    int MinutesLeft() const { return m_minutesLeft; }
    void SetMinutesLeft(int val) { m_minutesLeft = val; }

    double TimeFee() const { return m_timeFee; }
    void SetTimeFee(double val) { m_timeFee = val; }

    double ServiceFee() const { return m_serviceFee; }
    void SetServiceFee(double val) { m_serviceFee = val; }

private:
    uint64_t m_sessionId; // this is the session id on server DB (store here. when trying to restore a session with server, this will be used to identify the session)
    
    int m_minutesUsed;
    int m_minutesLeft;

    double m_timeFee;
    double m_serviceFee;
};
#endif // SESSION_H

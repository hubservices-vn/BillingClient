#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "StdTypes.h"

class Session
{
public:
    explicit Session(quint64 id, int minutesUsed, int minutesLeft, double timeFee, double serviceFee);

public:// methods
    void updateValue( int minsUsed, int minsLeft, double timeFee, double serviceFee );

public: // accessors
    quint64 id() const { return m_id; }
    void setId(quint64 val) { m_id = val; }

    int minutesUsed() const { return m_minutesUsed; }
    void setMinutesUsed(int val) { m_minutesUsed = val; }

    int minutesLeft() const { return m_minutesLeft; }
    void setMinutesLeft(int val) { m_minutesLeft = val; }

    double timeFee() const { return m_timeFee; }
    void setTimeFee(double val) { m_timeFee = val; }

    double serviceFee() const { return m_serviceFee; }
    void setServiceFee(double val) { m_serviceFee = val; }

private:
    quint64 m_id;
    int m_minutesLeft;
    int m_minutesUsed;
    double m_timeFee;
    double m_serviceFee;
};

#endif // SESSION_H

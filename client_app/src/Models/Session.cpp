#include "Session.h"


Session::Session( quint64 id, int minutesUsed, int minutesLeft, double timeFee, double serviceFee )
: m_id(id), m_minutesUsed(minutesUsed), m_minutesLeft(minutesLeft),
m_timeFee(timeFee), m_serviceFee(serviceFee)
{
}

void Session::updateValue( int minsUsed, int minsLeft, double timeFee, double serviceFee)
{
    m_minutesUsed = minsUsed;
    m_minutesLeft = minsLeft;
    m_timeFee = timeFee;
    m_serviceFee = serviceFee;
}
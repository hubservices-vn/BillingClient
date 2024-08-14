#include "Session.h"
#include "Utils.h"
#include "DateTimeUtils.h"


Session::Session(uint64_t sessionId, int minutesUsed, int minutesLeft, double timeFee, double serviceFee)
: m_sessionId(sessionId), m_minutesUsed(minutesUsed), m_minutesLeft(minutesLeft),
m_timeFee(timeFee), m_serviceFee(serviceFee)
{
}

Session::~Session()
{
}

void Session::updateValue( int minsUsed, int minsLeft, double timeFee, double serviceFee)
{
    m_minutesUsed = minsUsed;
    m_minutesLeft = minsLeft;
    m_timeFee = timeFee;
    m_serviceFee = serviceFee;
}

int Session::minutesUsed() const
{
    return m_minutesUsed;
}

int Session::minutesLeft() const
{
    return m_minutesLeft;
}

bool Session::isAlive() const
{
    return m_minutesLeft > 0;
}


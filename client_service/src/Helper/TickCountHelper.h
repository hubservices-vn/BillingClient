#ifndef TICK_COUNT_HELPER_H
#define TICK_COUNT_HELPER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QTimer>

// this class will record the "Seconds" after client service is running
// it maintains a timer which ticks every second.
// provide a fake "clock" for checking heart-beat 
// will tick every second
// (should NOT trust QDateTime::currentDateTime(), as the system time might be changed
// by some time-sync tool)
class TickCountHelper : public QObject, public Singleton_T<TickCountHelper>
{
    Q_OBJECT
public:
    ~TickCountHelper();
    bool init();

    qint64 tickCount() const { return tickCount_; }

private slots:
    void onTickTimerTimeoutSlot();

private:
    QTimer* tickTimer_;
    qint64 tickCount_;
};

#endif

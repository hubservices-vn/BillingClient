#include "TickCountHelper.h"


TickCountHelper::~TickCountHelper()
{
    if (tickTimer_ != NULL)
    {
        delete tickTimer_;
        tickTimer_ = NULL;
    }
}

bool TickCountHelper::init()
{
    tickCount_ = 0;

    tickTimer_ = new QTimer(this);
    tickTimer_->setInterval(1000);
    connect(tickTimer_, SIGNAL(timeout()),
        this, SLOT(onTickTimerTimeoutSlot()));
    tickTimer_->start();

    return true;
}

void TickCountHelper::onTickTimerTimeoutSlot()
{
    ++tickCount_;
}

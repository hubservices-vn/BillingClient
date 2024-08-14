#ifndef TIMERTASKMANAGER_H
#define TIMERTASKMANAGER_H

#include <QObject>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class TimerTaskManager : public QObject, public Singleton_T<TimerTaskManager>
{
    Q_OBJECT
public:
    static void launchSystemUserInit();

signals:

private slots: // the timer imp slots
    static void _launchSystemUserInitSlot();


private:
};

#endif // TIMERTASKMANAGER_H

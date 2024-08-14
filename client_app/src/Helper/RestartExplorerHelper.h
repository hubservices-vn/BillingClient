#ifndef RESTART_EXPLORER_HELPER_H
#define RESTART_EXPLORER_HELPER_H

#include <QObject>
#include <QThread>


class RestartExplorerThread : public QThread
{
    Q_OBJECT
public:
    RestartExplorerThread() {};

private:
    void run();
};

class RestartExplorerHelper
{
public:
    static void restartExplorer();

private:
    static RestartExplorerThread m_thread;
};

#endif // RESTART_EXPLORER_HELPER_H

#ifndef CS2SSCOMMAND_H
#define CS2SSCOMMAND_H

#include <QByteArray>
#include <QTcpSocket>
#include <QMutexLocker>
#include "cs2ssSocketManager.h"

// some commands are sent from dedicated threads (heartbeat, etc)
namespace
{
    static QMutex s_cs2ssMutex;
}

class cs2ssSender
{
public:
    static void send(const QByteArray& byteArray);

private:
    static cs2ssSocketManager* m_sktMgr;
};

#endif // CS2SSCOMMAND_H

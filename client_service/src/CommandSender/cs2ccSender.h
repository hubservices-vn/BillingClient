#ifndef CS2CCSENDER_H
#define CS2CCSENDER_H

#include <QByteArray>
#include <QLocalSocket>
#include <QMutexLocker>
#include "cs2ccSocketManager.h"

// some commands are sent from dedicated threads (heartbeat, etc)
namespace
{
    static QMutex s_cs2ccMutex;
}


class cs2ccSender
{
public:
    static void send(const QByteArray& byteArray)
    {
        QMutexLocker locker(&s_cs2ccMutex);
        {
            if (m_sktMgr == NULL)
            {
                m_sktMgr = cs2ccSocketManager::GetInstance();
            }

            if (m_sktMgr->isSocketConnected() == false)
            {
                return;
            }

            QTcpSocket* socket = m_sktMgr->socket();
            if (socket == NULL)
            {
                return;
            }

            int bytesWritten = socket->write(byteArray);
            bool succeeded = socket->flush();
            int kk = 3;
            Q_UNUSED(bytesWritten);
            Q_UNUSED(succeeded);
            Q_UNUSED(kk);
        }
    }
private:
    static cs2ccSocketManager* m_sktMgr;
};

#endif // CS2CCSENDER_H

#include "cs2clSender.h"
#include "cs2clSocketManager.h"
#include <QTcpSocket>
#include <QMutexLocker>

cs2clSocketManager* cs2clSender::m_sktMgr = NULL;

// some commands are sent from dedicated threads (heartbeat, etc)
namespace
{
    static QMutex s_cs2clMutex;
}

void cs2clSender::send( const QByteArray& byteArray )
{
    QMutexLocker locker(&s_cs2clMutex);
    {
        if (m_sktMgr == NULL)
        {
            m_sktMgr = cs2clSocketManager::GetInstance();
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

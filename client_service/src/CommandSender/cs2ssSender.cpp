#include "cs2ssSender.h"

cs2ssSocketManager* cs2ssSender::m_sktMgr = NULL;


void cs2ssSender::send( const QByteArray& byteArray )
{
    QMutexLocker locker(&s_cs2ssMutex);
    {
        if (m_sktMgr == NULL)
        {
            m_sktMgr = cs2ssSocketManager::GetInstance();
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
        Q_UNUSED(bytesWritten);
        Q_UNUSED(succeeded);
    }
}

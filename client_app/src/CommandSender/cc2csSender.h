#ifndef CC2CSSENDER_H
#define CC2CSSENDER_H

#include <QByteArray>
#include <QLocalSocket>
#include <QMutexLocker>
#include "cc2csSocketManager.h"

static QMutex s_cmdSendMutex;

class cc2csSender
{
public:
    static void send(const QByteArray& byteArray)
    {
        QMutexLocker locker(&s_cmdSendMutex);
        {
            if (m_sktMgr == NULL)
            {
                m_sktMgr = cc2csSocketManager::GetInstance();
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
    static cc2csSocketManager* m_sktMgr;
};

#endif // CC2CSSENDER_H

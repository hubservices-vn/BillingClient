#ifndef CS2CL_SENDER_H
#define CS2CL_SENDER_H

#include <QByteArray>


class cs2clSocketManager;

class cs2clSender
{
public:
    static void send(const QByteArray& byteArray);

private:
    static cs2clSocketManager* m_sktMgr;
};

#endif // CS2CL_SENDER_H

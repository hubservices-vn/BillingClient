#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QMap>
#include <QDateTime>

// disable the enum-with-prefix warning
#pragma warning(disable: 4482)


#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class Client : public QObject, public Singleton_T<Client>
{
    Q_OBJECT
public:

    bool init();
    bool connectSigSlot();
   
    // start the socket connection, begin to communicate with server
    bool startUp();
    // stop the socket listening
    bool stop();
signals:

public slots:
  

private:
};

#endif // CLIENT_H

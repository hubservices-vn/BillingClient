#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "CcCore/CcSingleton.h"

class Client : public QObject, public Singleton_T<Client>
{
    Q_OBJECT
public:
    ~Client();
    bool init();
    bool connectSigSlot();

public: // methods
    // start the socket connection, begin to communicate with server
    bool startUp();

signals:

public slots:

private:
    // data
};

#endif // CLIENT_H

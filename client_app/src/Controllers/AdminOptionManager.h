#ifndef ADMINOPTIONMANAGER_H
#define ADMINOPTIONMANAGER_H

#include <QObject>
#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class AdminOptionReplyQtModelC;
class AdminOptionManager : public QObject, public Singleton_T<AdminOptionManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public:
    void procAdminOptionReply(const AdminOptionReplyQtModelC &qtModel);

signals:
    void adminOptionReplyRecved(int opTypeVal, int resultVal);
};

#endif // ADMINOPTIONMANAGER_H

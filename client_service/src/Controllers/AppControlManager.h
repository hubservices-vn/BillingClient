#ifndef APPLICATIONPROHIBITIONMANAGER_H
#define APPLICATIONPROHIBITIONMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;


class AppControlReplyQtModel;
class AppControlRequestQtModel;
class AppControlManager : public QObject, public Singleton_T<AppControlManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // cc2cs cmd
    void procAppControlRequestC( AppControlRequestQtModel &qtModel );

public: // ss2cs cmd handlers
    void procAppControlReply( const AppControlReplyQtModel &qtModel );
    void procAppControlWhiteReply( const AppControlReplyQtModel &qtModel );

private:
};

#endif // APPLICATIONPROHIBITIONMANAGER_H

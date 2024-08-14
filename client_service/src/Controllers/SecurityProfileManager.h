#ifndef SECURITYPROFILEMANAGER_H
#define SECURITYPROFILEMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>


class SecuProfileReplyQtModel;
class SecuProfileRequestQtModel;
class SecurityProfileManager : public QObject, public Singleton_T<SecurityProfileManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // cc2cs cmd handler
    void procSecuProfileRequestC( SecuProfileRequestQtModel &qtModel );

public: // ss2cs cmd handler
    void procSecuProfileReply( const SecuProfileReplyQtModel &qtModel );
};

#endif // SECURITYPROFILEMANAGER_H

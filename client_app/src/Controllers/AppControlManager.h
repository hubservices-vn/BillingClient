#ifndef APPLICATIONPROHIBITIONMANAGER_H
#define APPLICATIONPROHIBITIONMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QMap>
#include <QList>
#include <QObject>

class ApplicationProhibition;
class AppControlReplyQtModel;
class AppControlManager : public QObject, public Singleton_T<AppControlManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    const QList<ApplicationProhibition*>& appProhibitList() const { return m_appProhibitList; }

    ApplicationProhibition* getAppProhibitionById(int id);

    void applyAllApplicationControls();

public: // cmd handlers
    void procAppControlReply( const AppControlReplyQtModel &qtModel );

    void procAppControlWhiteReply( const AppControlReplyQtModel &qtModel );

public slots:
    void serverConnectedSlot();

    //void appProhibitBatchCmdRecvedSlot( const QList<ApplicationProhibition>& appList );

    //// server broadcasts an "Application prohibition" data, update the local memory storage and the
    //// corresponding registry settings
    //void appProhibitCmdRecvedSlot(int appProhibitId, int prohibitType, const QString& fileName, 
    //    const QString& params, const QString& oldParams, bool enabled);

private:
    bool _applyApplicationProhibition(ApplicationProhibition* app);

    // remove the restriction to the specified application
    bool _enableApplication(ApplicationProhibition* app);
    
private:
    QList<ApplicationProhibition*> m_appProhibitList;
    //QMap<int /*appId*/, ApplicationProhibition*> m_appControlMap;
};

#endif // APPLICATIONPROHIBITIONMANAGER_H

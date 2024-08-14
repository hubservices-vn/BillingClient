#ifndef CLIENTEVENTDLLUPDATEMANAGER_H
#define CLIENTEVENTDLLUPDATEMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

class ClientEventDllTransferReplyQtModel;
class ClientCodeDllUpdatedQtModel;

class ClientEventDllUpdateManager :
        public QObject,
        public Singleton_T<ClientEventDllUpdateManager>
{
    Q_OBJECT
public:
	~ClientEventDllUpdateManager();
    bool init();
    bool connectSigSlot();

// client cmd handler
	//void procClientEventDllRequest(const ClientEventDllTransferRequestQtModel& qtModel);

signals:
	void clientCodeDllsUpdated(const ClientCodeDllUpdatedQtModel& qtModel);

public slots:
	void checkAndDownloadClientEventDllSlot();
	void versionFileDownloaded(QByteArray& downloadedContent);
	void clientEventDllFileUpdatedSlot();

private:
	void _verifyLocalClientEventDllFile();

	void _getClientEventDllHashAsync();
	void _getClientEventDllHashImp();
	void _handleVerXmlDownloaded();
	void _handleAllFilesDownloaded();

private:
	QByteArray m_clientEventXmlContent;
	QTimer* m_clientEventDllCheckTimer;
	qint64 m_currentVersion;
	qint64 m_currentVersionBeingDownload;
};

#endif // CLIENTEVENTDLLMANAGER_H


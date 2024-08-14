#ifndef CLIENTEVENTMANAGER_H
#define CLIENTEVENTMANAGER_H

#include "CcCore/CcSingleton.h"

#include <QObject>

#include "IClientEvent.h"
#include "ClientEventLoader.h"
#include "UserAccountManager.h"
#include "LogManager.h"
#include "log.h"
#include "ClientCodeDllUpdatedQtModel.h"

class ClientCodeDllUpdatedQtModel;

class ClientEventManager : public QObject, public Singleton_T<ClientEventManager>
{
	Q_OBJECT

typedef QMap<QString, ClientEventLoader*> MapStringDllLoader;

public:
	bool init();
	void UnInitialize();
	bool connectSigSlot();

public slots:
	void onLogin();
	void onLogOut();
	void timerFired();
	void clientCodeDllUpdated(const ClientCodeDllUpdatedQtModel& qtModel);

private:
	bool _verifyLocalClientEventDllFile(QString commonDllFile, QString specificDllFile);
	void _updateLocalDllFile(QString commonDllFile, QString specificDllFile);
	void reloadDllList(QList<QString> listDllToReload);
	bool checkNeedReload( QString dllPath );
	QString getCommonDllPathFromRelativePath( QString relativeDllPath );
	QString getSpecificDllPathFromRelativePath( QString relativeDllPath );
	LPCWSTR LPCWSTRFromQString( QString specificDllPath );

	ClientEventLoader* m_clientEventLoader;
	IClientEvent* m_clientEvent;
	std::wstring m_wstrDllName;
	QString m_specificDllFile;
	QString m_currentFileHash;
	QString m_commonDllFile;
	QString m_xmlContent;
	QList<QString> m_listDllToLoad;
	QList<ClientEventLoader*> m_listDllInstances;
	bool m_listDllReloaded;
	MapStringDllLoader m_mapDllLoaded;
};

#endif // CLIENTEVENTMANAGER_H

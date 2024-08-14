#include "ClientEventManager.h"
#include <QThread>
#include "Utils.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDomDocument>
#include <QtAlgorithms>
#include "ClientEventDllUpdateManager.h"

using namespace ClientEvent;

const QString prefixDll = "Service";

bool ClientEventManager::init()
{
	qRegisterMetaType<ClientCodeDllUpdatedQtModel>("ClientCodeDllUpdatedQtModel");
	return true;
}

bool ClientEventManager::connectSigSlot()
{
	connect(ClientEventDllUpdateManager::GetInstance(), SIGNAL(clientCodeDllsUpdated(const ClientCodeDllUpdatedQtModel&)), this, SLOT(clientCodeDllUpdated(const ClientCodeDllUpdatedQtModel&)));
	connect(UserAccountManager::GetInstance(), SIGNAL(accLoggedIn()), this, SLOT(onLogin()));
	connect(UserAccountManager::GetInstance(), SIGNAL(accLoggedOut(int, int)), this, SLOT(onLogOut()));
	return true;
}


void ClientEventManager::onLogin()
{
	foreach (QString relativePath, m_listDllToLoad)
	{
		MapStringDllLoader::iterator it = m_mapDllLoaded.find(relativePath);
		if (it != m_mapDllLoaded.end())
		{
			ClientEventLoader* loader = it.value();
			if (loader != NULL)
			{
				IClientEvent* instance = loader->GetClientEventInstance();
				if (instance != NULL)
				{
					instance->onUserLogIn();
				}
			}
		}
	}
}


void ClientEventManager::onLogOut()
{
	foreach (QString relativePath, m_listDllToLoad)
	{
		MapStringDllLoader::iterator it = m_mapDllLoaded.find(relativePath);
		if (it != m_mapDllLoaded.end())
		{
			ClientEventLoader* loader = it.value();
			if (loader != NULL)
			{
				IClientEvent* instance = loader->GetClientEventInstance();
				if (instance != NULL)
				{
					instance->onUserLogOut();
				}
			}
		}
	}
}

void ClientEventManager::UnInitialize()
{
	delete m_clientEventLoader;
}

void ClientEventManager::timerFired()
{
}

void ClientEventManager::_updateLocalDllFile(QString commonDllFile, QString specificDllFile)
{
	int retry =  0;
	do
	{
		if (QFile::exists(specificDllFile))
		{
			QFile::remove(specificDllFile);
		}

		bool result = QFile::copy(commonDllFile, specificDllFile);
		if (result)
		{
			return;
		} else {
			
		}
	} while (retry++ < 5);
}


bool ClientEventManager::_verifyLocalClientEventDllFile(QString commonDllFile, QString specificDllFile)
{
	LOG_DEBUG << "Verify local app-dll hash";
	QString clientEventDllFilePath = specificDllFile;

	int retry = 0;
	QString commonDllFileHash;
	// If there's no common dll file, dont update (coz we got nothing to copy from)
	if ( ! QFile::exists(commonDllFile) )
	{
		LOG_DEBUG << "No common client app-dll file";
		return true;
	}
	while (retry++ < 5) 
	{
		commonDllFileHash = Utils::getFileHash(commonDllFile);
		// wait until the common dll is updated
		if (commonDllFileHash != "") 
		{
			break;
		} else {
			Sleep(500);
		}
	}	
	if (commonDllFileHash == "")
	{
		// dont try to update if the common file is not updated;
		LOG_DEBUG << "Cannot get hash of common file, not update, because some problem with common file";
		return true;
	}

	QString currentFileHash;

	// there's no local clientEventDll file, just update
	if ( ! QFile::exists(specificDllFile) )
	{
		LOG_DEBUG << "No specific client app-dll file";
		return false;
	}

	retry = 0;
	while (retry++ < 5) 
	{
		currentFileHash = Utils::getFileHash(specificDllFile);
		// wait until the common dll is updated
		if (currentFileHash != "") 
		{
			break;
		} else {
			Sleep(500);
		}
	}

	// the clientEventDll file might be changed
	if (QString::compare(commonDllFileHash, currentFileHash, Qt::CaseInsensitive) != 0)
	{
		LOG_DEBUG << "Different hash:";
		LOG_DEBUG << "Common hash:";
		LOG_DEBUG << commonDllFileHash.toStdString();
		LOG_DEBUG << "Specific hash:";
		LOG_DEBUG << currentFileHash.toStdString();
		return false;
	}

	// same hash -> no need to update
	return true;
}

void ClientEventManager::reloadDllList(QList<QString> listDllToReload)
{
	LOG_DEBUG << "Loading:" ;
	LOG_DEBUG << "List DLL to reload: ";
	LOG_DEBUG << listDllToReload.size();
	foreach (QString dllName, listDllToReload)
	{
		LOG_DEBUG << dllName.toStdString();
	}
	// unload the dll that are not needed anymore
	for (MapStringDllLoader::iterator it = m_mapDllLoaded.begin(); it != m_mapDllLoaded.end();){
		if (!m_listDllToLoad.contains(it.key()))
		{
			// not needed, unload now
			ClientEventLoader* loader = it.value();
			delete loader;
			it = m_mapDllLoaded.erase(it);
		} else {
			it++;
		}
	}
	// unload the dll that need to be loaded now
	foreach (QString relativePath, listDllToReload)
	{
		if (m_mapDllLoaded.contains(relativePath))
		{
			MapStringDllLoader::iterator it = m_mapDllLoaded.find(relativePath);
			ClientEventLoader* loader = it.value();
			delete loader;
			m_mapDllLoaded.erase(it);
		}
	}
	// now reload the list
	foreach (QString relativePath, listDllToReload)
	{
		QString commonDllPath = getCommonDllPathFromRelativePath(relativePath);
		QString specificDllPath = getSpecificDllPathFromRelativePath(relativePath);
		// overwrite the dll using the common file
		_updateLocalDllFile(commonDllPath, specificDllPath);
		LPCWSTR specificPathWStr = LPCWSTRFromQString(specificDllPath);
		ClientEventLoader* clientEventLoader = new ClientEventLoader(specificPathWStr, SERVICE, DLL_VERSION);
		IClientEvent* instance = clientEventLoader->GetClientEventInstance();
		if (instance != NULL)
		{
			instance->onInit();
		}
		m_mapDllLoaded.insert(relativePath, clientEventLoader);
		LOG_DEBUG << "Loaded dll:";
		LOG_DEBUG << relativePath.toStdString();
		LOG_DEBUG << "Using dll file:";
		LOG_DEBUG << specificDllPath.toStdString();
	}
}

void ClientEventManager::clientCodeDllUpdated(const ClientCodeDllUpdatedQtModel& qtModel)
{
	if (m_xmlContent == qtModel.XmlContent_)
		return;
	m_xmlContent = qtModel.XmlContent_;
	QDomDocument dom;
	QList<QString> listDllToReload;
	if (!dom.setContent(m_xmlContent))
	{
		LOG_DEBUG << "Cannot read xml from download client xml file. Probably xml file corrupted!";
	} else {
		QDomNode node = dom.firstChild();
		while(!node.isNull() && node.nodeName() != "update")
		{
			node = node.nextSibling();
		}
		//cannot find root, file format error
		if(node.isNull() || node.nodeName() != "update")
		{
			LOG_DEBUG << "Cannot find xml root node - update!";
			return ;
		}

		if (!node.hasAttributes())
		{
			LOG_DEBUG << "Malformed root node!";
			return ;
		}
		QDomNamedNodeMap attributes = node.attributes();
		
		QDomNodeList childList = node.childNodes();
		m_listDllToLoad.clear();
		for(int i = 0; i< childList.count(); i++)
		{
			QDomNode childNode = childList.item(i);
			if (childNode.nodeName() == "ClientService")
			{
				QDomNodeList subChildList = childNode.childNodes();
				for (int j = 0; j < subChildList.count(); j++)
				{
					QDomNode subChildNode = subChildList.item(j);
					if (subChildNode.nodeName() == "load")
					{
						QDomNamedNodeMap attributes = subChildNode.attributes();
						if (attributes.contains("path"))
						{
							QString dllPath = attributes.namedItem("path").nodeValue();
							bool needReload = checkNeedReload(dllPath);
							if (needReload)
							{
								listDllToReload << dllPath;
							}
							m_listDllToLoad << dllPath;
						}
					}
				}
			}			
		}
		// now reload the whole list of dll
		reloadDllList(listDllToReload);
	}
}

bool ClientEventManager::checkNeedReload( QString relativeDllPath )
{
	LOG_DEBUG << "Checking if dll need reload:";
	LOG_DEBUG << relativeDllPath.toStdString();
	if (!m_mapDllLoaded.contains(relativeDllPath))
	{
		return true; // need reloaded
	}
	QString commonDllPath = getCommonDllPathFromRelativePath(relativeDllPath);
	LOG_DEBUG << commonDllPath.toStdString();
	QString specificDllPath = getSpecificDllPathFromRelativePath(relativeDllPath);
	LOG_DEBUG << specificDllPath.toStdString();
	if (_verifyLocalClientEventDllFile(commonDllPath, specificDllPath))
	{
		LOG_DEBUG << "Same hash, dont need to reload";
		// same dll, dll is already loaded -> skip
		return false;
	}
	LOG_DEBUG << "Add to reload list";
	return true;	
}

QString ClientEventManager::getCommonDllPathFromRelativePath( QString relativeDllPath )
{
	return QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/" + relativeDllPath);
}

QString ClientEventManager::getSpecificDllPathFromRelativePath( QString relativeDllPath )
{
	QStringList listSplited = QDir::toNativeSeparators(relativeDllPath).split(QDir::separator());
	if (listSplited.size() > 0)
	{			
		QString fileName = listSplited.last();
		QString specificDllFile = QCoreApplication::applicationDirPath() + QString("/") + prefixDll + fileName;
		return specificDllFile;
	}
	return getCommonDllPathFromRelativePath(relativeDllPath);
}

LPCWSTR ClientEventManager::LPCWSTRFromQString( QString str )
{
	wchar_t* wStr = (wchar_t*) malloc (sizeof(wchar_t) * (str.length()+1));
	str.toWCharArray(wStr);
	wStr[str.length()]=0; // Null terminate the string
	return wStr;
}

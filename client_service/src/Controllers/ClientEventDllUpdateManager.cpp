#include "ClientEventDllUpdateManager.h"
#include "ClientCodeDllUpdatedQtModel.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"
#include "Utils.h"

#include "ClientEventDllUpdater.h"
#include <QDomDocument>

#ifndef NDEBUG
// debug
const int CLIENT_EVENT_DLL_CHECK_INTERVAL = 30000; // debug = 30s
#else
const int CLIENT_EVENT_DLL_CHECK_INTERVAL = 3600000; // 3,600,000 ms = 3600s = 1h
#endif

bool ClientEventDllUpdateManager::init()
{
	m_currentVersion = -1;
	ClientEventDllUpdater::GetInstance()->init();

	//_verifyLocalClientEventDllFile();

	m_clientEventDllCheckTimer = new QTimer();
	m_clientEventDllCheckTimer->setInterval(CLIENT_EVENT_DLL_CHECK_INTERVAL);
	connect(m_clientEventDllCheckTimer, SIGNAL(timeout()),
		this, SLOT(checkAndDownloadClientEventDllSlot()));
	m_clientEventDllCheckTimer->start();
	connect(ClientEventDllUpdater::GetInstance(), SIGNAL(xmlFileDownloaded(QByteArray &)), this, SLOT(versionFileDownloaded(QByteArray&)));
	connect(ClientEventDllUpdater::GetInstance(), SIGNAL(clientEventDllFileUpdated()), this, SLOT(clientEventDllFileUpdatedSlot()));

	// run it once (delay 20s)
#ifndef NDEBUG
	// debug
	const int DELAY_CHECK_INTERVAL = 2000; // 2,000ms = 2s
#else
	// release
	const int DELAY_CHECK_INTERVAL = 20000; // 20,000ms = 20s
#endif

	QTimer::singleShot(DELAY_CHECK_INTERVAL,
		this, SLOT(checkAndDownloadClientEventDllSlot()));
	return true;
}

bool ClientEventDllUpdateManager::connectSigSlot()
{
	/*{
		ClientEventDllUpdater* updater = ClientEventDllUpdater::GetInstance();	}*/
    return true;
}

void ClientEventDllUpdateManager::checkAndDownloadClientEventDllSlot()
{
	LOG_DEBUG << "begin to check clientEventDll";
	ClientEventDllUpdater::GetInstance()->checkUpdate();
}

DownloadFileInfo* getDownloadFileInfo(QDomNode& node)
{
	DownloadFileInfo *downloadFileInfo = new DownloadFileInfo();
	QDomNamedNodeMap attributes = node.attributes();
	if (attributes.contains("path"))
	{
		QString filePath = attributes.namedItem("path").nodeValue();
		downloadFileInfo->filePath = QCoreApplication::applicationDirPath() + "/" + filePath;
	}
	if (attributes.contains("url"))
	{
		QString url = attributes.namedItem("url").nodeValue();
		downloadFileInfo->downloadUrl = url;
	}
	if (attributes.contains("md5"))
	{
		QString hash = attributes.namedItem("md5").nodeValue();
		downloadFileInfo->hash = hash;
	}
	if (attributes.contains("size"))
	{
		QString size = attributes.namedItem("size").nodeValue();
		downloadFileInfo->size = size.toInt();
	}
	if (attributes.contains("unzip"))
	{
		QString unzip = attributes.namedItem("unzip").nodeValue();
		downloadFileInfo->needUnzip = unzip.toInt();
	}

	if (QFile::exists(downloadFileInfo->filePath))
	{
		QString fileHash = Utils::getFileHash(downloadFileInfo->filePath);
		// the clientEventDll file might be changed
		if (QString::compare(fileHash, downloadFileInfo->hash, Qt::CaseInsensitive) == 0)
		{
			// hash is not the same, need to redownload this file
			delete downloadFileInfo;
			return NULL;
		}
	}
	return downloadFileInfo;
}

void ClientEventDllUpdateManager::versionFileDownloaded(QByteArray& downloadedContent)
{
	LOG_DEBUG << "Version file downloaded, checking:";
	m_clientEventXmlContent = downloadedContent;
	QDomDocument dom;
	if (!dom.setContent(downloadedContent))
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
		if (attributes.contains("version"))
		{
			LOG_DEBUG << "Downloaded version:";
			LOG_DEBUG << attributes.namedItem("version").toElement().text().toLongLong();
			if (m_currentVersion == attributes.namedItem("version").toElement().text().toLongLong())
			{	

				// event if no change in version, we still can update client programs.
				// this is to help in case there's some problem that makes them unable to load dll in previous load
				// no change in version
				// return;
			}
			m_currentVersionBeingDownload = attributes.namedItem("version").toElement().text().toLongLong();
		}
		DownloadList downloadList;
		QDomNodeList childList = node.childNodes();
		for(int i = 0; i< childList.count(); i++)
		{
			QDomNode childNode = childList.item(i);
			if (childNode.nodeName() == "file")
			{
				DownloadFileInfo* downloadFileInfo = getDownloadFileInfo(childNode);
				if (downloadFileInfo != NULL){
					downloadList.append(*downloadFileInfo);
					delete downloadFileInfo;
				}
			}
			if (childNode.nodeName() == "ClientService")
			{

			}
			if (childNode.nodeName() == "ClientApp")
			{

			}
			if (childNode.nodeName() == "ClientLocker")
			{

			}
		}
		if (downloadList.size() > 0)
		{
			ClientEventDllUpdater::GetInstance()->downloadListDllFile(downloadList);
		} else {
			// all file are updated, fire this so other client program can update
			clientEventDllFileUpdatedSlot();
		}
	}
}



void ClientEventDllUpdateManager::clientEventDllFileUpdatedSlot()
{
	LOG_DEBUG << "Dll file updated, inform all programs now!";
	m_currentVersion = m_currentVersionBeingDownload;
	// all dll files have been downloaded
	QString fileContent = QString(m_clientEventXmlContent);
	ClientCodeDllUpdatedQtModel model(fileContent);
	emit clientCodeDllsUpdated(model);
	cs2ccSender::send(cs2cc::ClientCodeDllUpdatedCommandC(model));	
	cs2clSender::send(cs2cl::ClientCodeDllUpdatedCommandC(model));
}

// Destructor
ClientEventDllUpdateManager::~ClientEventDllUpdateManager()
{
	if (m_clientEventDllCheckTimer != NULL)
	{
		if (m_clientEventDllCheckTimer->isActive())
		{
			m_clientEventDllCheckTimer->stop();
		}

		m_clientEventDllCheckTimer->deleteLater();
		m_clientEventDllCheckTimer = NULL;
	}
}


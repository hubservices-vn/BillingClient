#include "ClientEventDllUpdater.h"
#include "PathManager.h"
#include "ConfigManager.h"
#include "Utils.h"
#include "log.h"
#include "ClientSettingManager.h"
#include "ClientCodeDllUpdatedQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ClientCodeTransferQtModel.h"

/*const QString STR_CLIENT_EVENT_DLL_UPDATE_URL = "http://vnupdate.garenanow.com/gbilling/update/public/gbcode/";*/
const QString STR_DLL_VER_FILE = "version.xml";
const int REQUEST_TIMEOUT = 1000 * 60 * 5;

void ClientEventDllUpdater::init()
{
	m_versionReply = NULL;
	m_xmlContent.clear();
	m_mapDownloadRequests.clear();
	connect(&manager, SIGNAL(finished(QNetworkReply*)),
		SLOT(downloadFinished(QNetworkReply*)) );
}

void ClientEventDllUpdater::procClientCodeReplyC(const ClientCodeTransferReplyQtModel &qtModel)
{
    if (m_mapDownloadRequests.contains(qtModel.FilePath_))
    {
        if (qtModel.FilePath_.length() == 0)
        {
            m_mapDownloadRequests.remove("");
            // this is the xml content
            m_xmlContent = qtModel.ClientCodeContent_;
            emit xmlFileDownloaded(m_xmlContent);
        } else {
            bool ok = true;
            if (qtModel.ClientCodeContent_.length() == 0)
            {
                //mean no change
            } else {
                DownloadFileInfo downloadInfo = m_mapDownloadRequests[qtModel.FilePath_];
                ok = _handleDllFile(downloadInfo, qtModel.ClientCodeContent_);
            }
            if (!ok)
            {
                m_requestGetError = true;
            }
            m_mapDownloadRequests.remove(qtModel.FilePath_);
            if (m_mapDownloadRequests.size() == 0)
            {
                if (!m_requestGetError)
                {
                    // if we have successfully download all files without error, fire a completion signal now
                    LOG_DEBUG << "Emit client file updated event";
                    emit clientEventDllFileUpdated();
                } else {
                    // if we have encountered error during download, reset everything so that we can update this later
                    LOG_DEBUG << "Got error during download";
                    m_requestGetError = false;
                }
            }
        }
    }
    _clearRequest(qtModel.FilePath_);
}

void ClientEventDllUpdater::_setRequestTimeout(const QString& url, int requestTimeout) 
{
    m_requestTimeMap.insert(url, QDateTime::currentMSecsSinceEpoch() + requestTimeout);
    QTimer::singleShot(requestTimeout, this, SLOT(requestTimeoutSlot()));
}

void ClientEventDllUpdater::requestTimeoutSlot()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    foreach (QString url, m_requestTimeMap.keys())
    {
        qint64 requestTimeOut = m_requestTimeMap[url];
        if (requestTimeOut >= currentTime)
        {
            // timeout
            // will retry later
            if (url.length() == 0)
            {
                LOG_DEBUG << "sync dll content failed";
            } else {
                LOG_DEBUG << "sync dll file failed: " << url.toStdString();
            }
            _clearRequest(url);
        }
    }
}

void ClientEventDllUpdater::_clearRequest(const QString& url)
{
    m_mapDownloadRequests.remove(url);
    m_requestTimeMap.remove(url);
}

void ClientEventDllUpdater::checkUpdate()
{
    if (m_mapDownloadRequests.size() > 0)
    {
        return; // request still pending
    }
    QString url = ""; // default url, mean getting version content
    // get the xml content
    // try to get from local server instead
    ClientCodeTransferRequestQtModel qtModel(
        CConfigManager::clientId(), url, ""
        );
    cs2ssSender::send(cs2ss::ClientCodeTransferRequestCommand(qtModel));
    // insert a dummy request
    m_mapDownloadRequests.insert("", DownloadFileInfo());
    _setRequestTimeout(url, REQUEST_TIMEOUT);
	//// download update config
	//LOG_DEBUG << "check dll ver file (ignore cache)";
 //   QString serverPath = ClientSettingManager::GetInstance()->getClientCodeDllUrlPath();
 //   if (serverPath == "")
 //   {
 //       // if we didnt receive the url from server, dont run the update, ignore it
 //       return;
 //   }
 //   QString urlToDownload = serverPath + STR_DLL_VER_FILE;
 //   QUrl url(urlToDownload);
	//QNetworkRequest req(url);
	//req.setAttribute(
	//	QNetworkRequest::CacheLoadControlAttribute,
	//	QVariant( int(QNetworkRequest::AlwaysNetwork) )
	//	);
	//qint64 timeFromPrevRequest = QDateTime::currentDateTime().toMSecsSinceEpoch() - lastDownloadTime;
	//if (m_versionReply != NULL)
	//{
	//	// an update request is being performed
	//	LOG_DEBUG << "an update xml file request is being performed";
	//	if (timeFromPrevRequest > REQUEST_TIMEOUT) // 5 min
	//	{
	//		LOG_DEBUG << "Previous request is canceled, because request timeout";
	//		LOG_DEBUG << timeFromPrevRequest;
	//		m_versionReply->abort();
	//		m_versionReply->deleteLater();
	//		m_versionReply = NULL;
	//	} else {
	//		return;
	//	}
	//	
	//}
	//if (m_mapDownloadRequests.size() != 0)
	//{
	//	// an batch download request is being performed
	//	LOG_DEBUG << "a batch download request is being performed";
	//	LOG_DEBUG << m_mapDownloadRequests.size();
	//	if (timeFromPrevRequest > REQUEST_TIMEOUT) // 5 min
	//	{
	//		LOG_DEBUG << "Remaining batch requests are canceled, because request timeout";
	//		LOG_DEBUG << timeFromPrevRequest;
	//		for (MapRequestDownloadInfo::iterator it = m_mapDownloadRequests.begin(); it != m_mapDownloadRequests.end();)
	//		{
	//			QNetworkReply* reply = it.key();
	//			reply->abort();
	//			reply->deleteLater();
	//			it = m_mapDownloadRequests.erase(it);				
	//		}
	//	} else {
	//		return;
	//	}
	//}
	//QNetworkReply* reply = manager.get(req);
	//lastDownloadTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	//m_versionReply = reply;
}

void ClientEventDllUpdater::downloadListDllFile(DownloadList& listFileToDownload)
{
	m_requestGetError = false;
	for(DownloadList::iterator it = listFileToDownload.begin(); it != listFileToDownload.end(); it++)
	{
		_downloadDllFile(*it);
	}
}

void ClientEventDllUpdater::_downloadDllFile(DownloadFileInfo& downloadInfo)
{
    QString localHash = Utils::getFileHash(downloadInfo.filePath);
    ClientCodeTransferRequestQtModel qtModel(CConfigManager::clientId(), localHash, downloadInfo.downloadUrl);
    cs2ssSender::send(cs2ss::ClientCodeTransferRequestCommand(qtModel));
    m_mapDownloadRequests.insert(downloadInfo.downloadUrl, downloadInfo);
    _setRequestTimeout(downloadInfo.downloadUrl, REQUEST_TIMEOUT);
    // download from local server instead
	//LOG_DEBUG << "begin to download dll from url (ignore cache): ";
	//// start download new dll file
 //   QString serverPath = ClientSettingManager::GetInstance()->getClientCodeDllUrlPath();
	//LOG_DEBUG << QString(serverPath + downloadInfo.downloadUrl).toStdString();
	//QNetworkRequest req(QUrl(serverPath + downloadInfo.downloadUrl));
	//req.setAttribute(
	//	QNetworkRequest::CacheLoadControlAttribute,
	//	QVariant( int(QNetworkRequest::AlwaysNetwork) )
	//	);
	//QNetworkReply* dllReply = manager.get(req);
	//lastDownloadTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	//m_mapDownloadRequests.insert(dllReply, downloadInfo);
	////m_dllReplyList.append(dllReply);
}

void ClientEventDllUpdater::downloadFinished( QNetworkReply *reply )
{
	//QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//int statusCodeInt = statusCode.toInt();
	//if (statusCodeInt == 200)
	//{
	//	LOG_INFO << "dll http req succeeded";
	//	if (m_versionReply == reply)
	//	{
	//		_handleVersionFile(reply);
	//	}
	//	else if (m_mapDownloadRequests.contains(reply))
	//	{
	//		bool result = _handleDllFile(reply);
	//		if (!result)
	//		{
	//			m_requestGetError = true;
	//		}
	//	}
	//}
	//else
	//{
	//	LOG_INFO << "dll http req failed";
	//	LOG_INFO << statusCodeInt;
	//	if (m_versionReply == reply)
	//	{
	//		LOG_INFO << "sync ver file failed";
	//	}
	//	else if (m_mapDownloadRequests.contains(reply))
	//	{
	//		LOG_INFO << "sync dll content failed";
	//		m_requestGetError = true;
	//	}
	//}

	//if (m_versionReply == reply)
	//{
	//	// invalidate this request, so later we can continue to update
	//	m_versionReply = NULL;
	//}
	//else if (m_mapDownloadRequests.contains(reply))
	//{
	//	LOG_DEBUG << "Receive a file in list download";
	//	// remove this request object, so later we can finish the batch request
	//	m_mapDownloadRequests.remove(reply);
	//	LOG_DEBUG << m_mapDownloadRequests.size();
	//	if (!m_mapDownloadRequests.size())
	//	{
	//		if (!m_requestGetError)
	//		{
	//			// if we have successfully download all files without error, fire a completion signal now
	//			LOG_DEBUG << "Emit client file updated event";
	//			emit clientEventDllFileUpdated();
	//		} else {
	//			// if we have encountered error during download, reset everything so that we can update this later
	//			LOG_DEBUG << "Got error during download";
	//			m_requestGetError = false;
	//		}
	//	}
	//}

	//reply->deleteLater();
}

void ClientEventDllUpdater::_handleVersionFile( QNetworkReply *verReply )
{
	// check version
	m_xmlContent = verReply->readAll();
	emit xmlFileDownloaded(m_xmlContent);
	return;

	//LOG_DEBUG << "dllver content ";
	//while( ! verReply->atEnd())
	//{
	//	QString lineStr = QString(verReply->readLine()).trimmed();
	//	int pos = lineStr.indexOf("=");
	//	QString valueStr = lineStr.right(lineStr.length() - pos - 1);
	//	LOG_DEBUG << qPrintable(valueStr);
	//	if (!lineStr.isEmpty())
	//	{
	//		valueList.append(valueStr);
	//	}
	//}

	//if (valueList.size() != 2)
	//{
	//	LOG_ERROR << "dllVer error. value size: " << valueList.size();
	//	return;
	//}

	//qint64 nVer = valueList[0].toLongLong(&bRet);
	//if (!bRet || nVer == ConfigManager::clientEventDllVersion())
	//{
	//	if (!bRet)
	//	{
	//		LOG_ERROR << "dllVer error: verNum: " << qPrintable(valueList[0]);
	//	}
	//	// invalid version file or same version
	//	LOG_DEBUG << "same version, skip";
	//	return;
	//}

	//if (0 == QString::compare(valueList[1], ConfigManager::clientEventDllHash(), Qt::CaseInsensitive))
	//{
	//	LOG_DEBUG << "same hash, skip";
	//	return;
	//}

	// don't save to DB now, save until dll file download finished
	//m_dllVersion = nVer;
	//m_dllHash = valueList[1];

	//_downloadDllFile();
}

bool ClientEventDllUpdater::_handleDllFile(const DownloadFileInfo& downloadInfo, const QByteArray& content )
{
	// save the dll file
	QString path = downloadInfo.filePath;
	LOG_DEBUG << "Downloaded file from url:";
	LOG_DEBUG << QString(downloadInfo.downloadUrl).toStdString();
	LOG_DEBUG << "Overwrite file at path:";
	LOG_DEBUG << path.toStdString();
	QFile f(path);
	if (f.open(QIODevice::WriteOnly))
	{
		f.write(content);
		f.close();
		LOG_DEBUG << "Overwritten successfully";
	}
	else
	{
		LOG_WARN << "can't open dll file";
	}

	// verify the dll hash fetched from server
	// against the local file hash
	QString dllHash = Utils::getFileHash(path);
	if (0 != QString::compare(dllHash, downloadInfo.hash, Qt::CaseInsensitive))
	{
		LOG_ERROR << "download dll error. expect: " << qPrintable( downloadInfo.hash)
			<< ". after download: " << qPrintable(dllHash);
		
		return false; 		
	}
	return true;
}


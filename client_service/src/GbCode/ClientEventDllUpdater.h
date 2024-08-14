#ifndef DLLUPDATER_H
#define DLLUPDATER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

struct DownloadFileInfo
{
	QString filePath;
	QString downloadUrl;
	QString hash;
	int size;
	bool needUnzip;
};
typedef QList<DownloadFileInfo> DownloadList;
typedef QMap<QString, DownloadFileInfo> MapUrlDownloadInfo;
class ClientCodeTransferReplyQtModel;

class ClientEventDllUpdater : public QObject, public Singleton_T<ClientEventDllUpdater>
{
	Q_OBJECT

public:
	void init();

	void checkUpdate();
	void downloadListDllFile(DownloadList& listFileToDownload);

public:
	QString getLocalDllFilePath() const;
	QString getLocalXmlFilePath() const;

private:
	void _downloadFile(const QUrl & url);

signals:
	void xmlFileDownloaded(QByteArray & xmlContent);
	void clientEventDllFileUpdated();

public slots:
	void downloadFinished(QNetworkReply *reply);
    void procClientCodeReplyC(const ClientCodeTransferReplyQtModel &qtModel);
    void requestTimeoutSlot();
private:
	void _downloadDllFile(DownloadFileInfo& downloadInfo);

private:
	void _handleVersionFile(QNetworkReply *reply);
	bool _handleDllFile(const DownloadFileInfo& downloadInfo, const QByteArray& content);
    void _setRequestTimeout(const QString& url, int requestTimeout);
    void _clearRequest(const QString& url);
private:
	QNetworkAccessManager manager;

	// only one update request is permitted at any time
	QNetworkReply* m_versionReply;
	QByteArray m_xmlContent;
	bool m_requestGetError;
	qint64 lastDownloadTime;
    QMap<QString, qint64> m_requestTimeMap;
    MapUrlDownloadInfo m_mapDownloadRequests;
};

#endif
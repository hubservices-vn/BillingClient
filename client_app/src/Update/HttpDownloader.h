#ifndef UPDATE_HTTPDOWNLOADER_H__
#define UPDATE_HTTPDOWNLOADER_H__

#include <string>
#include <vector>

#include "Common/Singleton_T.h"

#include "SmartPtr/scoped_ptr.h"
//#include "Sigslot/sigslot.h"

#include "WinINet/InternetSession.h"
#include "WinINet/HttpConnection.h"

using SDK::Common::Singleton_T;
using SDK::SmartPtr::scoped_ptr;
using SDK::WinINet::InternetSession;
using SDK::WinINet::HttpConnection;

class HttpDownloader : public Singleton_T<HttpDownloader>
{
public:
	//SDK::Sigslot::signal1<int> SignalPercentage;

public:
	HttpDownloader();
	virtual~ HttpDownloader();

public:
	// Init HTTP downloader
	//@param [in] server: server host 
	//@param [in] path: base path for downloading files
	void	Init(const TCHAR * server, const TCHAR * path);

	// Check file exist on server or not
	//@param [in] filename: file name
	bool	FileExist(const TCHAR * filename);

	// Download file from server
	//@param [in] filename: file name
	//@param [in] data: data buffer to hold the file content
	bool	DownloadFile(const TCHAR * filename, std::vector<char> & data);

private:
	std::wstring server_;
	std::wstring base_path_;

	InternetSession session_;
	scoped_ptr<HttpConnection> connection_;
};

#endif // UPDATE_HTTPDOWNLOADER_H__
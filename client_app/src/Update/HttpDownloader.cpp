#include "HttpDownloader.h"
#include "WinINet/HttpFile.h"

using SDK::WinINet::HttpFile;
using SDK::WinINet::InternetFile;

HttpDownloader::HttpDownloader() : session_(L"WinINet", INTERNET_OPEN_TYPE_DIRECT)
{

}

HttpDownloader::~HttpDownloader()
{

}

void HttpDownloader::Init(const TCHAR * server, const TCHAR * path)
{
	server_ = server;
	base_path_ = path;

	connection_.reset(new HttpConnection(session_, server_.c_str()));
}

bool HttpDownloader::FileExist(const TCHAR * filename)
{
	HttpFile http_file((*connection_.get()), L"HEAD", 
		(base_path_ + filename).c_str(), 
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE);

	if (http_file.operator HINTERNET() == NULL)
	{
		return false;
	}

	DWORD timeout = 10 * 1000;

	http_file.SetOption(INTERNET_OPTION_SEND_TIMEOUT, timeout);
	http_file.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, timeout);

	http_file.SendRequest();

	DWORD status_code;

	if (!http_file.QueryInfo(HTTP_QUERY_STATUS_CODE, status_code))
	{
		return false;
	}

	if (status_code != HTTP_STATUS_OK)
	{
		return false;
	}

	return true;
}

bool HttpDownloader::DownloadFile(const TCHAR * filename, std::vector<char> & data)
{
	HttpFile http_file((*connection_.get()), L"GET", (base_path_ + filename).c_str(), 
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE);

	if (http_file.operator HINTERNET() == NULL)
	{
		return false;
	}

	DWORD timeout = 10 * 1000;

	http_file.SetOption(INTERNET_OPTION_SEND_TIMEOUT, timeout);
	http_file.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, timeout);

	http_file.SendRequest();

	DWORD status_code;

	if (!http_file.QueryInfo(HTTP_QUERY_STATUS_CODE, status_code))
	{
		return false;
	}

	if (status_code != HTTP_STATUS_OK)
	{
		return false;
	}

	InternetFile::Info info(http_file);

	DWORD bytes_read = 0;
	char buffer[4096];

	for (;;)
	{
		if (!http_file.Read(buffer, 4096, bytes_read, info))
		{
			return false;
		}

		if (bytes_read == 0)
		{
			break;
		}

		data.insert(data.end(), buffer, buffer + bytes_read);

		if (info.GetTotalDataLength() > 0)
		{
			//SignalPercentage(
			//	(int)(100ui64 * info.GetTotalRead() / info.GetTotalDataLength()));
		}
	}

	return true;
}
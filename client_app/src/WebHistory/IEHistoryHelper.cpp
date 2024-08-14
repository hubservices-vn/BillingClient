#include "IEHistoryHelper.h"

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <urlhist.h> // Definition of IUrlHistoryStg
#include <comdef.h>  // Definition of smartpointers (we use IUrlHistoryStgPtr) and inclusion of <stdio.h>
#include <shlguid.h> // Definition of SID_SUrlHistory
#include <winbase.h>
#include "WebHistoryQtModel.h"
#include "log.h"


const int QUERY_INTERVAL = 30000; // ms // 1 min, 1 * 60 * 1000

static FILETIME s_lastQueryFT = {ULONG_MAX, ULONG_MAX}; // the value of FILETIME, utc


IEHistoryHelperThread::IEHistoryHelperThread( QObject *parent /* = 0*/ )
{
}

void IEHistoryHelperThread::run()
{
    while(true)
    {
        //LOG_DEBUG << "begin reading history data: ";
        QList<WebHistory_Item> historyList;
        bool succeeded = __getLatestWebHistory(historyList);
        if (succeeded)
        {
            //LOG_DEBUG << "read web history succeeded. web history count: " << historyList.size();
            emit ieHistoryGot(historyList);
        }
        else
        {
            LOG_WARN << "get webhistory failed";
        }

        QThread::msleep(QUERY_INTERVAL);
    }
}

bool IEHistoryHelperThread::__getLatestWebHistory(QList<WebHistory_Item>& historyList)
{
    //LOG_DEBUG4 << "Begin to read webhistory through IE interface";
    HRESULT res;
    CoInitialize(NULL);

    // make sure all COM-related stack variables are freed before CoUninitialize.
    // release them explicitly
    {
        IUrlHistoryStgPtr historyPtr;
        res = historyPtr.CreateInstance(SID_SUrlHistory);
        if (res != S_OK)
        {
            return false;
        }

        IEnumSTATURLPtr enumPtr = NULL;
        if (historyPtr->EnumUrls(&enumPtr) != S_OK)
        {
            return false;
        }

        //LOG_DEBUG4 << "begin to walk through the urls";
        STATURL url;
        ULONG fetched;
        while (enumPtr->Next(1, &url, &fetched) == S_OK)
        {
            //LOG_DEBUG4 << "iterating urls";
            if (fetched == 0)
            {
                //LOG_DEBUG4 << "fetch is 0, quit";
                break;
            }

            //LOG_DEBUG4 << "compare url against user login time";
            // no need to record earlier records
            if (CompareFileTime(&url.ftLastVisited, &s_lastQueryFT) < 0)
            {
                continue;
            }

            // this might be some iframes embeded in the website
            if (url.pwcsTitle == NULL)
            {
                continue;
            }

            WebHistory_Item data;

            // fetch url
            std::wstring urlW(url.pwcsUrl);
            // at most 200 chars
            
            data.Url_ = QString::fromStdWString(urlW);
            if (urlW.length() > 200)
            {
                data.Url_.truncate(200);
            }

            // frech local time
            SYSTEMTIME sysTime, localTime;
            FileTimeToSystemTime(&url.ftLastVisited, &sysTime);
            SystemTimeToTzSpecificLocalTime(NULL, &sysTime, &localTime);
            QDate date = QDate(localTime.wYear, localTime.wMonth, localTime.wDay);
            QTime time = QTime(localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds);
            data.Timestamp_ = QDateTime(date, time, Qt::LocalTime);

            historyList.append(data);

            // release memory
            if (!(url.dwFlags & STATURL_QUERYFLAG_NOURL))
                CoTaskMemFree(url.pwcsUrl);
            if (!(url.dwFlags & STATURL_QUERYFLAG_NOTITLE))
                CoTaskMemFree(url.pwcsTitle);
        }
    }

    // store the current system time (in UTC)
    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &s_lastQueryFT);

    CoUninitialize();
    return true;
}

IEHistoryHelper::IEHistoryHelper()
{
    m_thread.reset(new IEHistoryHelperThread);
    connect(m_thread.data(), SIGNAL(ieHistoryGot(QList<WebHistory_Item>)),
        this, SLOT(ieHistoryGotSlot(QList<WebHistory_Item>)));
}

void IEHistoryHelper::ieHistoryGotSlot( const QList<WebHistory_Item>& historyList )
{
    //LOG_DEBUG << "recved web history records from thread. passing to webHistoryManager";
    emit getIEHistoryDone(historyList);
}

bool IEHistoryHelper::beginFetchHistory()
{
    if (m_thread->isRunning())
    {
        return true;
    }

    // store the current system time (in UTC)
    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &s_lastQueryFT);

    m_thread->start();
    return true;
}

bool IEHistoryHelper::stopFetchHistory()
{
    if (m_thread->isRunning())
    {
        m_thread->quit();
    }

    return true;
}



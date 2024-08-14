#include <Windows.h>
#include <wininet.h>
#include <TlHelp32.h>
#include "UpdateChecker.h"
#include "HttpDownloader.h"
#include "LocalUpdateConfig.h"
#include <atlstr.h>
#include "log.h"
#include <iosfwd>
#include "StringUtils.h"
#include "System/ProcessUtils.h"
using SDK::System::ProcessUtils;


#ifdef RELEASE_PUBLIC
const std::wstring STR_URL_UPDATE = L"http://vnupdate.garenanow.com/gbilling/update/public/client/";
#else
const std::wstring STR_URL_UPDATE = L"http://vnupdate.garenanow.com/gbilling/update/internal/client/";
#endif


const CString UPDATER_APP_NAME_WITH_EXT = L"AutoUpdateH2.exe";
const CString CLIENT_APP_NAME_WITH_EXT = L"gbClientApp.exe|gbClientService";


// shared memory fields
#define MAP_PREFIX              L"Local\\"
#define CLIENT_MAP_NAME         L"GCBCLIENT_7E8412A18DA24dc78AFA515FD44CD0EA"
#define UPDATE_MAP_NAME         L"GCBUPDATER_95D1ACF36EC14201B7174B865F9E9956"
#define FULL_CLIENT_MAP_NAME    MAP_PREFIX CLIENT_MAP_NAME
#define FULL_UPDATER_MAP_NAME   MAP_PREFIX UPDATE_MAP_NAME
#define VIEW_OFFSET         0
#define VIEW_SIZE           256
const DWORD MAP_SIZE    =        2048;
const int TERM_CMD_ID = 1;


namespace
{
    DWORD FindAppProcessID(const CString& program)
    {
        HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

        PROCESSENTRY32 process_entry;
        process_entry.dwSize = sizeof(PROCESSENTRY32);

        if (::Process32First(handle, &process_entry))
        {
            do
            {
                CString process_name = process_entry.szExeFile;
                if (!process_name.CompareNoCase(program))
                {
                    ::CloseHandle(handle);
                    return process_entry.th32ProcessID;
                }
            }
            while (::Process32Next(handle, &process_entry));

            ::CloseHandle(handle);
        }

        return -1;
    }
}

std::string UpdaterStateToStdString( UpdaterState state )
{
    switch (state)
    {
    case UpdaterState_Failed:
        return "UpdaterState_Failed";
    case UpdaterState_Done:
        return "UpdaterState_Done";
    }

    return "Unknown UpdaterState";
}

bool UpdateChecker::HasUpdate()
{
    bool result = LocalUpdateConfig::GetInstance()->Init();
    if (result == false)
    {
        return false;
    }

    float local_version = LocalUpdateConfig::GetInstance()->GetVersion();
    LOG_DEBUG4 << "local version is: " << StringUtils::floatToStdString(local_version, 6);

    float version = local_version;
    version += 0.001f;

    URL_COMPONENTS uc = {0};
    uc.dwStructSize = sizeof(uc);
    TCHAR hostname[256] = {0}, url_path[256] = {0};
    uc.lpszHostName = hostname;
    uc.dwHostNameLength = 256;
    uc.lpszUrlPath = url_path;
    uc.dwUrlPathLength = 256;

    ::InternetCrackUrl(STR_URL_UPDATE.c_str(), 0, 0, &uc);

    HttpDownloader::GetInstance()->Init(hostname, url_path);

    CString update_file;
    update_file.Format(_T("update%.3f.zip"), version);

    LOG_DEBUG4 << "Checking update package: " << StringUtils::cstringToStdString(update_file);

    result = HttpDownloader::GetInstance()->FileExist(update_file);
    return result;
}

UpdaterState UpdateChecker::BeginUpdate()
{
    float local_version = LocalUpdateConfig::GetInstance()->GetVersion();
    LOG_DEBUG << "Client local version: " << StringUtils::floatToStdString(local_version, 6);

    float version = local_version;
    version += 0.001f;

    ///////////////////////////////////////////////////////////////////////////////

    CString update_url;
    update_url.Format(_T("%supdate%.3f.zip"), STR_URL_UPDATE.c_str(), version);
    LOG_DEBUG << "update package url: " << StringUtils::cstringToStdString(update_url);

    ///////////////////////////////////////////////////////////////////////////////
    // prepare the shared memory, as the updater process may tell this app to close itself
    // Try to open the named file mapping identified by the map name.
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // Use paging file - shared memory
        NULL,                   // Default security attributes
        PAGE_READWRITE,         // Allow read and write access
        0,                      // High-order DWORD of file mapping max size
        MAP_SIZE,               // Low-order DWORD of file mapping max size
        FULL_CLIENT_MAP_NAME    // Name of the file mapping object
        );
    if (hMapFile == NULL) 
    {
        LOG_WARN << "Create shared memory: " << FULL_CLIENT_MAP_NAME << " failed, skip updating";
        return UpdaterState_Failed;
    }

    // Map a view of the file mapping into the address space of the current 
    // process.
    PVOID pView = MapViewOfFile(
        hMapFile,               // Handle of the map object
        FILE_MAP_ALL_ACCESS,          // Read access
        0,                      // High-order DWORD of the file offset 
        VIEW_OFFSET,            // Low-order DWORD of the file offset
        VIEW_SIZE               // The number of bytes to map to view
        );
    if (pView == NULL)
    {
        LOG_WARN << "Map file mapping failed, skip updating";
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return UpdaterState_Failed;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // run the updater process
    CString command_line;

    command_line.Format(
        _T("%s %s %f %s"), 
        UPDATER_APP_NAME_WITH_EXT, 
        CLIENT_APP_NAME_WITH_EXT, 
        local_version, 
        update_url
        );
    LOG_DEBUG4 << "Launch updater with cmd line: " << StringUtils::cstringToStdString(command_line);

    ProcessUtils::CreateProcess(NULL, command_line);

    wchar_t termCmdCharArray[255];
    wsprintf(termCmdCharArray, L"%d", TERM_CMD_ID);
    std::wstring termCmdStr(termCmdCharArray);

    LOG_DEBUG << "wait for updater to tell client for termination";
    while (true) 
    {
        // the updater process still running, wait for it to finish
        int pid = FindAppProcessID(UPDATER_APP_NAME_WITH_EXT);

        // the updater still runs, check whether there're any commands
        if (pid != -1)
        {
            bool termCmdRecved = _ReadRpcCommand(termCmdStr);

            if (termCmdRecved == false)
            {
                ::Sleep(100);
                continue;
            }
            // updater asks this app to terminate
            else
            {
                LOG_DEBUG << "updater tells client to exit.";
                // tell updater that we'll terminate self
                std::wstringstream stream;
                stream << TERM_CMD_ID << L"d";
                std::wstring cmdReplyStr = stream.str();
                LOG_DEBUG << " tell updater that client is going to exit";
                _SendReplyToUpdater(cmdReplyStr);

                // release the client shared memory
                // all done. clear the resource
                if (hMapFile)
                {
                    if (pView)
                    {
                        // Unmap the file view.
                        UnmapViewOfFile(pView);
                        pView = NULL;
                    }
                    // Close the file mapping object.
                    CloseHandle(hMapFile);
                    hMapFile = NULL;
                }

                return UpdaterState_Done;
            }
        }
        else
        {
            break;
        }
    } 

    ///////////////////////////////////////////////////////////////////////////////
    // release the shared memory
    // all done. clear the resource
    if (hMapFile)
    {
        if (pView)
        {
            // Unmap the file view.
            UnmapViewOfFile(pView);
            pView = NULL;
        }
        // Close the file mapping object.
        CloseHandle(hMapFile);
        hMapFile = NULL;
    }

    return UpdaterState_Done;
}

bool UpdateChecker::_ReadRpcCommand( const wstring_t& expectedCmd )
{
    bool result = false;

    // Try to open the named file mapping identified by the map name.
    HANDLE hMapFile = OpenFileMapping(
        PAGE_READONLY, FALSE /*inherit the name*/, FULL_CLIENT_MAP_NAME );
    if (hMapFile == NULL) 
    {
        return false;
    }

    // Map a view of the file mapping into the address space of the current 
    // process.
    PVOID pView = MapViewOfFile(
        hMapFile,               // Handle of the map object
        FILE_MAP_ALL_ACCESS,          // Read access
        0,                      // High-order DWORD of the file offset 
        VIEW_OFFSET,            // Low-order DWORD of the file offset
        VIEW_SIZE               // The number of bytes to map to view
        );
    if (pView == NULL)
    {
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return false;
    }

    // try reading from the shared memory, check whether the host
    // process has already written down some response
    PWSTR response = (PWSTR)pView;
    if (_wcsicmp(response, expectedCmd.c_str()) == 0)
    {
        result = true;
    }

    // all done. clear the resource
    if (hMapFile)
    {
        if (pView)
        {
            // Unmap the file view.
            UnmapViewOfFile(pView);
            pView = NULL;
        }
        // Close the file mapping object.
        CloseHandle(hMapFile);
        hMapFile = NULL;
    }

    return result;
}

bool UpdateChecker::_SendReplyToUpdater( const wstring_t& cmdReply )
{
    // Try to open the named file mapping identified by the map name.
    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_WRITE, FALSE /*inherit the name*/, FULL_UPDATER_MAP_NAME );
    if (hMapFile == NULL) 
    {
        return false;
    }

    // Map a view of the file mapping into the address space of the current 
    // process.
    PVOID pView = MapViewOfFile(
        hMapFile,               // Handle of the map object
        FILE_MAP_WRITE,         // Read access
        0,                      // High-order DWORD of the file offset 
        VIEW_OFFSET,            // Low-order DWORD of the file offset
        VIEW_SIZE               // The number of bytes to map to view
        );
    if (pView == NULL)
    {
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return false;
    }

    // write a command into the shared memory
    memcpy_s(pView, VIEW_SIZE, cmdReply.c_str(), (cmdReply.length() + 1) * sizeof(wchar_t));

    // all done. clear the resource
    if (hMapFile)
    {
        if (pView)
        {
            // Unmap the file view.
            UnmapViewOfFile(pView);
            pView = NULL;
        }
        // Close the file mapping object.
        CloseHandle(hMapFile);
        hMapFile = NULL;
    }

    return true;
}

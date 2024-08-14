#pragma once
#include <Windows.h>

class RemoteSubClass
{
public:
	RemoteSubClass();
	~RemoteSubClass();

    // @"enable": when this param is true, then "CAD" is not working
    //            when this param is false, then user can use CAD again
	bool SetSwitch(int enable);
	bool IsEnable();

public:
#include <pshpack1.h>

#define	SIGN_LENGTH 16
#define	CODE_LENGTH 4096
#define	DATA_LENGTH 4096

	struct ParamData
	{
		typedef HWND		(WINAPI *PFNFindWindow)	  (LPCSTR, LPCSTR);
		typedef BOOL		(WINAPI *PFNIsWindowUnicode) (HWND hWnd);
		typedef LONG		(WINAPI *PFNSetWindowLong)	  (HWND, int, LONG); 
		typedef LONG		(WINAPI *PFNGetWindowLong)	  (HWND, int); 
		typedef LRESULT		(WINAPI *PFNCallWindowProc)  (WNDPROC, HWND, UINT, WPARAM, LPARAM);
		typedef LRESULT		(WINAPI *PFNDefWindowProc)(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
		typedef BOOL		(WINAPI *PFNIsBadCodePtr)(void* lpfn);
		typedef BOOL		(WINAPI *PFNFlushInstructionCache)(HANDLE hProcess, LPCVOID lpBaseAddress,SIZE_T dwSize);

		PFNIsBadCodePtr		pfnIsBadCodePtr;	// Addr. of IsBadCodePtr()
		PFNFlushInstructionCache pfnFlushInstructionCache;
		volatile PFNFindWindow		pfnFindWindow;		// Addr. of FindWindow()
		volatile PFNIsWindowUnicode	pfnIsWindowUnicode;		// Addr. of IsWindowUnicode()
		volatile PFNGetWindowLong	pfnGetWindowLong,pfnGetWindowLongW;	// Addr. of GetWindowLong()
		volatile PFNSetWindowLong	pfnSetWindowLong,pfnSetWindowLongW;	// Addr. of SetWindowLong()
		volatile PFNCallWindowProc	pfnCallWindowProc,pfnCallWindowProcW;	// Addr. of CallWindowProc()
		volatile PFNDefWindowProc	pfnDefWindowProc,pfnDefWindowProcW;	// Addr. of DefWindowProc()
		volatile WNDPROC				pfnSASWndProc;		// Addr. of remote SASWindowProc
		volatile WNDPROC 			pfnOldSASWndProc;	// Addr. of old SASWindowProc
		char				szClassName[100];	// Class name = "SAS Window class"
		char				szWindowName[100];	// Window name = "SAS window"
		volatile int					enable_;
	};
	struct InjectData
	{
		char sign_[SIGN_LENGTH];
		char padding[CODE_LENGTH-SIGN_LENGTH];
		char leadcode_[CODE_LENGTH]; // push addtional parameter, and jump to the final function code
		char data_[DATA_LENGTH];
		char function_[CODE_LENGTH]; // the real function code
		char do_hook_[CODE_LENGTH];
	};

#include <poppack.h>

protected:
	DWORD pid_; // target process
	HANDLE process_; // target process handle
	void* lowest_address_,*highest_address_; // the lowest address when open
	InjectData* local_data_,*remote_data_;

protected:
	bool __EnsureEverything();
	DWORD __GetWinLogonPID();
	bool __EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable);
	bool __GetLowestModule();
	void* __RemoteFind(void* base,DWORD size);
	void* __RemoteAlloc(void* base,DWORD size);
	bool __RemoteInit(InjectData* remote_data);
	InjectData* __PrepareLocalData();

protected:
	HANDLE mutex_;
	class AutoLocker
	{
		HANDLE mutex_;
	public:
		AutoLocker(HANDLE mutex):mutex_(mutex)
		{
			WaitForSingleObject(mutex_,INFINITE);
		}
		AutoLocker()
		{
			ReleaseMutex(mutex_);
		}
	};

protected:
	HANDLE WINAPI MyStartRemoteThread(
		HANDLE hProcess,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		SIZE_T dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		DWORD dwCreationFlags,
		LPDWORD lpThreadId
		);
};
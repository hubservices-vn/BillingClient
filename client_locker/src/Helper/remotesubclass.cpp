#include "stdafx.h"
#include "remotesubclass.h"

#pragma optimize( "", off )
#pragma check_stack(off)
#pragma strict_gs_check( off )
#pragma comment(linker, "/INCREMENTAL:NO")

#include <assert.h>

#include <initguid.h>
// {78c3ff48-2129-4f51-a1e7-8eee3155aeed} 
DEFINE_GUID(IID_REMOTE_CLASS, 
0x78c3ff48, 0x2129, 0x4f51, 0xa1, 0xe7, 0x8e, 0xee, 0x31, 0x55, 0xae, 0xed);

#include <tlhelp32.h>

#define EXE_FILE {L'w',L'i',L'n',L'l',L'o',L'g',L'o',L'n',L'.',L'e',L'x',L'e',0}
#define EXE_FILE_CHECK L"winlogon.exe"
#define WINDOW_CLASS {'S','A','S',' ','W','i','n','d','o','w',' ','c','l','a','s','s',0}
#define WINDOW_CLASS_CHECK "SAS Window class"
#define WINDOW_TITLE {'S','A','S',' ','w','i','n','d','o','w',0}
#define WINDOW_TITLE_CHECK "SAS window"

static LRESULT CALLBACK __remote_WndProcAsm(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
static LRESULT CALLBACK __remote_WndProc(RemoteSubClass::ParamData* pd,HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
static DWORD __stdcall __remote_InjectProc(void* base);
static void* __remote_WndProcAsm_endmarker();
static void* __remote_WndProc_endmarker();
static void* __remote_InjectProc_endmarker();

//#define _DEBUG_THIS

RemoteSubClass::RemoteSubClass():
	local_data_(NULL),remote_data_(NULL),
	pid_(0),process_(NULL),lowest_address_(0),highest_address_(0),
	mutex_(CreateMutex(NULL,FALSE,L"RemoteSubClass_Mutex"))
{
	assert(sizeof(ParamData)<=DATA_LENGTH);
}

RemoteSubClass::~RemoteSubClass()
{
	if (local_data_)
		delete local_data_;
	if (process_)
		CloseHandle(process_);
	if (mutex_)
		CloseHandle(mutex_);
}

bool RemoteSubClass::SetSwitch(int enable)
{
	AutoLocker l(mutex_);
	if (!__EnsureEverything())
		return false;

	DWORD ret;
	assert(sizeof(enable)==sizeof(int));
	ParamData *pd = (ParamData*)&remote_data_->data_[0];
	return WriteProcessMemory(process_,(void*)&pd->enable_,&enable,sizeof(int),&ret) && ret==sizeof(int);
}

bool RemoteSubClass::IsEnable()
{
	AutoLocker l(mutex_);
	if (!__EnsureEverything())
		return false;

	DWORD ret;
	ParamData *pd = (ParamData*)&remote_data_->data_[0];
	int enable=false;
	if (ReadProcessMemory(process_,(void*)&pd->enable_,&enable,sizeof(int),&ret) && ret==sizeof(int))
		return enable;
	return false;
}

bool RemoteSubClass::__EnsureEverything()
{
	if (remote_data_==NULL)
	{
		if ((GetVersion()&0xff)!=5)
			return false; // suport windows xp only,so far
		__try
		{
			if (local_data_==NULL)
			{
				local_data_ = __PrepareLocalData();
				if (local_data_==NULL)
					__leave;
			}
			if (!__EnablePrivilege(SE_DEBUG_NAME, TRUE))
				__leave;

			pid_ = __GetWinLogonPID();
			if (pid_==0)
				__leave;

			process_ = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid_);
			if (process_==NULL)
				__leave;

			if (! __GetLowestModule())
				__leave;

			InjectData* remote_data=NULL;
			remote_data = (InjectData*)__RemoteFind(lowest_address_,sizeof(InjectData));
			if (remote_data == NULL)
			{
				remote_data = (InjectData*)__RemoteAlloc(highest_address_,sizeof(InjectData));
				if (remote_data == NULL)
					__leave;

				if (!__RemoteInit(remote_data))
				{
					VirtualFreeEx(process_,(void*)remote_data,0,MEM_RELEASE);
					__leave;
				}
				DWORD thread_id;
				LPTHREAD_START_ROUTINE start_addr = (LPTHREAD_START_ROUTINE)&remote_data->do_hook_[0];
				HANDLE thread_handle = MyStartRemoteThread(process_,NULL,0,
					start_addr,remote_data,/*CREATE_SUSPENDED*/0,&thread_id);
				if (thread_handle==NULL)
				{
					VirtualFreeEx(process_,(void*)remote_data,0,MEM_RELEASE);
					__leave;
				}
				remote_data_ = remote_data;
//				ResumeThread(thread_handle);
				WaitForSingleObject(thread_handle,INFINITE);
				CloseHandle(thread_handle);
			}
			else
			{
				remote_data_ = remote_data;
			}
		}
		__finally
		{
			if (remote_data_==NULL)
			{
				// cleanup
				if (process_)
				{
					CloseHandle(process_);
					process_=NULL;
				}
			}
			__EnablePrivilege(SE_DEBUG_NAME, FALSE);
		}
	}

	if (remote_data_==NULL)
		return false;
	return true;
}

DWORD RemoteSubClass::__GetWinLogonPID()
{
	DWORD ret = 0;
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (handle==INVALID_HANDLE_VALUE) return ret;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	wchar_t exefile[]=EXE_FILE;
	assert(!wcscmp(exefile,EXE_FILE_CHECK));
	if (::Process32First(handle, &pe32))
	{
		do
		{
			if (_wcsicmp(pe32.szExeFile,exefile)==0)
			{
				ret = pe32.th32ProcessID;
				break;
			}
		}
		while (::Process32Next(handle, &pe32));

		::CloseHandle(handle);
	}
	return ret;
}

bool RemoteSubClass::__GetLowestModule()
{
	void* lowest = 0, *highest = 0;
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid_);
	if (handle==INVALID_HANDLE_VALUE) return false;
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	if (::Module32First(handle, &me32))
	{
		do
		{
			if (lowest==0 || me32.modBaseAddr<lowest)
			{
				lowest = me32.modBaseAddr;
			}
			if (highest==0 || me32.modBaseAddr>highest)
			{
				highest = me32.modBaseAddr;
			}
		}
		while (::Module32Next(handle, &me32));

		::CloseHandle(handle);
	}
	if (lowest && highest)
	{
		lowest_address_ = lowest;
		highest_address_ = highest;
		return true;
	}
	return false;
}

void* RemoteSubClass::__RemoteFind(void* _base,DWORD size)
{
	PVOID result = NULL;
	char* base = (char*)_base;
	MEMORY_BASIC_INFORMATION  meminfo = {0};
	while ((UINT_PTR)base < 0x80000000 && result==NULL)
	{
		DWORD dwReturnSize = VirtualQueryEx(process_,(LPVOID)base, &meminfo, sizeof(MEMORY_BASIC_INFORMATION));
		if (dwReturnSize == 0)
			break;

		if ((meminfo.Protect & PAGE_NOACCESS)
			|| (meminfo.Protect & PAGE_NOCACHE)
			|| (meminfo.Protect & PAGE_GUARD)
			|| meminfo.State  == MEM_RESERVE
			|| meminfo.State  == MEM_FREE)
		{
			base = (char*)meminfo.BaseAddress + meminfo.RegionSize;
			continue;
		}

		DWORD dwRet;
		char sign[SIGN_LENGTH]={0};
		if (ReadProcessMemory(process_,meminfo.BaseAddress,sign,SIGN_LENGTH,&dwRet) && 
			dwRet==SIGN_LENGTH &&
			memcmp(sign,&IID_REMOTE_CLASS,SIGN_LENGTH)==0)
		{
			result = meminfo.BaseAddress;
//			printf("found address : %p\n",result);
			break;
		}

		base = (char*)meminfo.BaseAddress + meminfo.RegionSize;
	}
	return result;
}

void* RemoteSubClass::__RemoteAlloc(void* _base,DWORD size)
{
	size = (size/0x100000+1)*0x100000; // at least 0x200000, 2MB
	PVOID p = NULL;
	char* base = (char*)_base;
	do
	{
		p = VirtualAllocEx(process_,base,size,MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (p!=NULL) break;
		base += 0x10000;
	}while (NULL==p);
	return p;
}

bool  RemoteSubClass::__RemoteInit(InjectData* remote_data)
{
	DWORD ret=0;
	if(WriteProcessMemory(process_,remote_data,local_data_,sizeof(InjectData),&ret) && ret==sizeof(InjectData))
	{
		FlushInstructionCache(process_,remote_data,sizeof(InjectData));
		return true;
	}
	return false;
}

RemoteSubClass::InjectData* RemoteSubClass::__PrepareLocalData()
{
	InjectData* id=new InjectData;
	memset(id, 0, sizeof(InjectData));
	ParamData *pd = (ParamData*)&id->data_[0];

	char classname[]=WINDOW_CLASS;
	char windowname[]=WINDOW_TITLE;
	assert(!strcmp(classname,WINDOW_CLASS_CHECK));
	assert(!strcmp(windowname,WINDOW_TITLE_CHECK));

	strcpy(pd->szClassName, classname);
	strcpy(pd->szWindowName, windowname);

#ifdef _DEBUG
	{
		char func[]={'C','r','e','a','t','e','R','e','m','o','t','e','T','h','r','e','a','d',0};//CreateRemoteThread
		assert(!strcmp(func,"CreateRemoteThread"));
	}
#endif

	HINSTANCE hDll = GetModuleHandle(L"user32.dll");
	if (NULL==hDll) hDll = LoadLibrary(L"user32.dll");
	pd->pfnFindWindow = (ParamData::PFNFindWindow)GetProcAddress(hDll, "FindWindowA");
	assert(NULL!=pd->pfnFindWindow);
	pd->pfnIsWindowUnicode = (ParamData::PFNIsWindowUnicode)GetProcAddress(hDll, "IsWindowUnicode");
	assert(NULL!=pd->pfnIsWindowUnicode);
	pd->pfnGetWindowLong = (ParamData::PFNGetWindowLong)GetProcAddress(hDll, "GetWindowLongA");
	assert(NULL!=pd->pfnGetWindowLong);
	pd->pfnGetWindowLongW = (ParamData::PFNGetWindowLong)GetProcAddress(hDll, "GetWindowLongW");
	assert(NULL!=pd->pfnGetWindowLongW);
	pd->pfnSetWindowLong = (ParamData::PFNSetWindowLong)GetProcAddress(hDll, "SetWindowLongA");
	assert(NULL!=pd->pfnSetWindowLong);
	pd->pfnSetWindowLongW = (ParamData::PFNSetWindowLong)GetProcAddress(hDll, "SetWindowLongW");
	assert(NULL!=pd->pfnSetWindowLongW);
	pd->pfnCallWindowProc = (ParamData::PFNCallWindowProc)GetProcAddress(hDll, "CallWindowProcA");
	assert(NULL!=pd->pfnCallWindowProc);
	pd->pfnCallWindowProcW = (ParamData::PFNCallWindowProc)GetProcAddress(hDll, "CallWindowProcW");
	assert(NULL!=pd->pfnCallWindowProcW);
	pd->pfnDefWindowProc = (ParamData::PFNDefWindowProc)GetProcAddress(hDll, "DefWindowProcA");
	assert(NULL!=pd->pfnDefWindowProc);
	pd->pfnDefWindowProcW = (ParamData::PFNDefWindowProc)GetProcAddress(hDll, "DefWindowProcW");
	assert(NULL!=pd->pfnDefWindowProcW);

	hDll = GetModuleHandle(L"kernel32.dll");
	if (NULL==hDll) hDll = LoadLibrary(L"kernel32.dll");
	pd->pfnIsBadCodePtr = (ParamData::PFNIsBadCodePtr)GetProcAddress(hDll, "IsBadCodePtr");
	assert(NULL!=pd->pfnIsBadCodePtr);
	pd->pfnFlushInstructionCache = (ParamData::PFNFlushInstructionCache)GetProcAddress(hDll, "FlushInstructionCache");
	assert(NULL!=pd->pfnFlushInstructionCache);

	{
		char* base=(char*)&__remote_WndProcAsm;
		int size=(char*)&__remote_WndProcAsm_endmarker-base;
		assert(size<RTL_FIELD_SIZE(InjectData,leadcode_));
		memcpy(id->leadcode_,base,size);
	}

	{
		char* base=(char*)&__remote_WndProc;
		int size=(char*)&__remote_WndProc_endmarker-base;
		assert(size<RTL_FIELD_SIZE(InjectData,function_));
		memcpy(id->function_,base,size);
	}

	{
		char* base=(char*)&__remote_InjectProc;
		int size=(char*)&__remote_InjectProc_endmarker-base;
		assert(size<RTL_FIELD_SIZE(InjectData,do_hook_));
		memcpy(id->do_hook_,base,size);
	}

	memcpy(id->sign_,&IID_REMOTE_CLASS,SIGN_LENGTH);

	return id;
}

bool RemoteSubClass::__EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable)
{
	HANDLE				hToken;

	if (!OpenProcessToken(GetCurrentProcess(),	TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,	&hToken))
		return false;

	LUID				luid;
	if (LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
	{
		TOKEN_PRIVILEGES	tp;
		tp.PrivilegeCount           = 1;
		tp.Privileges[0].Luid       = luid;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

		BOOL ret = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

		CloseHandle(hToken);
		return ret;
	}
	return false;
}

HANDLE RemoteSubClass::MyStartRemoteThread( HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, 
										   LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags,
										   LPDWORD lpThreadId)
{
	typedef HANDLE (WINAPI *PFNCreateRemoteThread)(
		HANDLE hProcess,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		SIZE_T dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		DWORD dwCreationFlags,
		LPDWORD lpThreadId
		);
	char func[]={'C','r','e','a','t','e','R','e','m','o','t','e','T','h','r','e','a','d',0};//CreateRemoteThread
	assert(!strcmp(func,"CreateRemoteThread"));
	PFNCreateRemoteThread pfn=(PFNCreateRemoteThread)GetProcAddress(GetModuleHandle(L"kernel32.dll"),func);
	if (pfn)
		return pfn(hProcess,lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);
	return NULL;
}

LRESULT __declspec(naked) CALLBACK __remote_WndProcAsm(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	__asm
	{
 		call get_eip
get_eip:
 		pop eax
 		sub eax,5 // get the beginning of the leadcode_

		push ebp
		mov ebp,esp
		sub esp,8
		mov [ebp-4],eax // save leadcode_

//		int 3 // for test

		pushad
		pushfd
		push [ebp+14h]
		push [ebp+10h]
		push [ebp+0ch]
		push [ebp+8h]
		mov eax,[ebp-4]
		add eax, CODE_LENGTH // point to data_
		push eax
		add eax, DATA_LENGTH // point to function_
		call eax
		mov [ebp-8],eax
		popfd
		popad

		mov eax,[ebp-8]
		mov esp,ebp 
		pop ebp  
		ret 10h
	}
}

void* __remote_WndProcAsm_endmarker(){return (void*)__FUNCTION__;}

LRESULT CALLBACK __remote_WndProc(RemoteSubClass::ParamData* pd,HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
// 	if (pd->pfnIsBadCodePtr(pd->pfnCallWindowProc) ||
// 		pd->pfnIsBadCodePtr(pd->pfnDefWindowProc) ||
// 		pd->pfnIsBadCodePtr(pd->pfnOldSASWndProc))
// 	{
// 		__asm int 3;
// 		return 0;
// 	}

	switch(message)
	{
	case WM_HOTKEY:
		{
			if (pd->enable_)
			{
				// Ctrl+Alt+Del
				if (lparam == MAKELONG(MOD_CONTROL | MOD_ALT, VK_DELETE))
					return 1;

				// Ctrl+Shift+Esc
				if (lparam == MAKELONG(MOD_CONTROL | MOD_SHIFT, VK_ESCAPE))
					return 1;
			}
			break;
		}
	case WM_NCDESTROY:
		{
// 			if (pd->pfnIsBadCodePtr(pd->pfnGetWindowLong) ||
// 				pd->pfnIsBadCodePtr(pd->pfnSetWindowLong) ||
// 				pd->pfnIsBadCodePtr(pd->pfnSASWndProc))
// 			{
// 				__asm int 3;
// 				break;
// 			}

			// try to revert before passthrough this call
			WNDPROC pfnOldSASWndProc = (WNDPROC)pd->pfnGetWindowLong(hwnd, GWL_WNDPROC);
			if (pfnOldSASWndProc==pd->pfnSASWndProc) // it's us
			{
				WNDPROC pfnSASWndProc = (WNDPROC)pd->pfnSetWindowLong(hwnd, GWL_WNDPROC, (LONG_PTR)pd->pfnOldSASWndProc); // unsubclass	
			}

			LRESULT ret = pd->pfnCallWindowProc(pd->pfnOldSASWndProc, hwnd, message, wparam, lparam);

			// try again
			pfnOldSASWndProc = (WNDPROC)pd->pfnGetWindowLong(hwnd, GWL_WNDPROC);
			if (pfnOldSASWndProc==pd->pfnSASWndProc) // it's us
			{
				WNDPROC pfnSASWndProc = (WNDPROC)pd->pfnSetWindowLong(hwnd, GWL_WNDPROC, (LONG_PTR)pd->pfnOldSASWndProc); // unsubclass	
			}

			return ret;
		}
	}
	return pd->pfnCallWindowProc(pd->pfnOldSASWndProc, hwnd, message, wparam, lparam);		
}

void* __remote_WndProc_endmarker(){return (void*)__FUNCTION__;}

DWORD __stdcall __remote_InjectProc(void* base)
{
	RemoteSubClass::InjectData* id=(RemoteSubClass::InjectData*)base;
	RemoteSubClass::ParamData* pd=(RemoteSubClass::ParamData*)&id->data_[0];

	if (pd->pfnIsBadCodePtr(pd->pfnFindWindow) ||
		pd->pfnIsBadCodePtr(pd->pfnFlushInstructionCache))
	{
#ifdef _DEBUG_THIS
		__asm int 3;
#endif
		return 0;
	}

	HWND hwnd = pd->pfnFindWindow(pd->szClassName,pd->szWindowName);
	if (hwnd)
	{
		if (pd->pfnIsBadCodePtr(pd->pfnIsWindowUnicode))
		{
#ifdef _DEBUG_THIS
			__asm int 3;
#endif
			return 0;
		}

		if (pd->pfnIsWindowUnicode(hwnd))
		{
			pd->pfnGetWindowLong = pd->pfnGetWindowLongW;
			pd->pfnSetWindowLong = pd->pfnSetWindowLongW;
			pd->pfnCallWindowProc = pd->pfnCallWindowProcW;
			pd->pfnDefWindowProc = pd->pfnDefWindowProcW;
		}

		if (pd->pfnIsBadCodePtr(pd->pfnGetWindowLong) ||
			pd->pfnIsBadCodePtr(pd->pfnSetWindowLong) ||
			pd->pfnIsBadCodePtr(pd->pfnCallWindowProc) ||
			pd->pfnIsBadCodePtr(pd->pfnDefWindowProc))
		{
#ifdef _DEBUG_THIS
			__asm int 3;
#endif
			return 0;
		}

		pd->pfnSASWndProc = (WNDPROC)&id->leadcode_[0]; // our new windows procedure
		pd->pfnFlushInstructionCache((HANDLE)-1,id,sizeof(RemoteSubClass::InjectData)); // flush
		pd->pfnOldSASWndProc = (WNDPROC)pd->pfnSetWindowLong(hwnd, GWL_WNDPROC, (LONG_PTR)pd->pfnSASWndProc); // subclass	
		pd->pfnFlushInstructionCache((HANDLE)-1,id,sizeof(RemoteSubClass::InjectData)); // flush
		if (pd->pfnOldSASWndProc)
		{
			return 1;
		}
	}
	return 0;
}

void* __remote_InjectProc_endmarker(){return (void*)__FUNCTION__;}

/**************************************************************************************************************
 * Some tips when using CreateRemoteThread()/WriteProcessMemory().                                            *
 *                                                                                                            *
 * 1. Analise generated code.                                                                                 *
 *    Check injected functions for absolute addressing (calls, jumps, data references, ...)                   * 
 *    and other generated code that shouldn't be there.                                                       *
 *    Use:                                                                                                    *
 *    A) Project\Settings\C/C++\Listing Files\Listing file type=Assembly, Machine Code, and Source.           *
 *    B) A disassembler (wdasm32).                                                                            *
 *    C) A debugger (softice).                                                                                *
 *                                                                                                            *
 * 2. Turn off stack probes.                                                                                  *
 *    Check for __chkstk() references in the listing files.                                                   *
 *    A) Use #pragma check_stack(off). [DOESN'T SEEMS TO WORK ?!?]                                            *
 *    B) Use less than 4K of local variables.                                                                 *
 *    C) Augment the stack size: /Gs size (Project\Settings\C/C++\ProjectOptions)                             *
 *                                                                                                            *
 * 3. Remove the /GZ switch in the debug build.                                                               *
 *    Check for __chkesp() references in the listing files.                                                   *
 *    A) Project\Settings\C/C++\Project Options                                                               *
 *                                                                                                            *
 * 4. Disable incremental compilation (/Gi).                                                                  *
 *    A) Use #pragma comment(linker, "/INCREMENTAL:NO")                                                       *
 *    B) Remove the /Gi switch (Project\Settings\C/C++\Customize\Enable incremental compilation=Off           *
 *    C) Declare the functions as static.                                                                     *
 *                                                                                                            *
 * 5. Don't let optimization screw your code.                                                                 *
 *    A) Turn off optimization (Project\Settings\C/C++\General\Optimizations=Disable(Debug)                   *
 *    B) Use #pragma optimize("", off)                                                                        *
 *    C) Don't write functions with the same prototype (e.g. AfterFuncX()). Let them return different values. *
 *                                                                                                            *
 * 6. Split switch() statements in 3 cases maximum, or use if/then/else.                                      *
 *                                                                                                            *
 * 7. Don't call any functions besides those in KERNEL32.LL and USER32.DLL (USER32.DLL isn't garanted to be   *
 *    mapped into every process).                                                                             *
 *    Use LoadLibrary()/GetProcAddress if you need functions from other libraries.                            *
 *                                                                                                            *
 * 8. Don't use any static strings.                                                                           *
 *    Pass them in INJDATA.                                                                                   *
 *                                                                                                            *
 * 9. Don't call any function directly.                                                                       *
 *    Copy each routine to the remote process individually and supply their addresses in INJDATA.             *
 *                                                                                                            *
 * 10. Good luck.                                                                                             *
 *     If you analise the generated code (using a disassembler) you should catch any errors before executing  *
 *     the code (and crashing the process !).                                                                 *
 **************************************************************************************************************/  

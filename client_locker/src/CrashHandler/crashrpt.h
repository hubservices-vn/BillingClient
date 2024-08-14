#pragma once

#include <yvals.h>
#include <exception>
#include <new.h>
#include <signal.h>
#define DBGHELP_TRANSLATE_TCHAR
#include <dbghelp.h>
#pragma comment(lib,"Dbghelp.lib")
#include <tchar.h>

#include "System/ProcessUtils.h"
using SDK::System::ProcessUtils;

extern "C" {
    void * _ReturnAddress(void);
#pragma intrinsic(_ReturnAddress)

#if defined (_M_IX86) || defined (_M_AMD64)
    void * _AddressOfReturnAddress(void);
#pragma intrinsic(_AddressOfReturnAddress)
#endif  /* defined (_X86_) || defined (_AMD64_) */
}

// use constance from crashrpt
#define CR_INST_SEH_EXCEPTION_HANDLER          0x1    //!< Install SEH handler.
#define CR_INST_TERMINATE_HANDLER              0x2    //!< Install terminate handler.
#define CR_INST_UNEXPECTED_HANDLER             0x4    //!< Install unexpected handler.
#define CR_INST_PURE_CALL_HANDLER              0x8    //!< Install pure call handler (VS .NET and later).
#define CR_INST_NEW_OPERATOR_ERROR_HANDLER     0x10   //!< Install new operator error handler (VS .NET and later).
#define CR_INST_SECURITY_ERROR_HANDLER         0x20   //!< Install security error handler (VS .NET and later).
#define CR_INST_INVALID_PARAMETER_HANDLER      0x40   //!< Install invalid parameter handler (VS 2005 and later).
#define CR_INST_SIGABRT_HANDLER                0x80   //!< Install SIGABRT signal handler.
#define CR_INST_SIGFPE_HANDLER                 0x100  //!< Install SIGFPE signal handler.   
#define CR_INST_SIGILL_HANDLER                 0x200  //!< Install SIGILL signal handler.  
#define CR_INST_SIGINT_HANDLER                 0x400  //!< Install SIGINT signal handler.  
#define CR_INST_SIGSEGV_HANDLER                0x800  //!< Install SIGSEGV signal handler.
#define CR_INST_SIGTERM_HANDLER                0x1000 //!< Install SIGTERM signal handler. 
#define CR_INST_VECTORED_EXCEPTION_HANDLER		0x2000 //1< Install Vectored exception handler.

#define CR_INST_ALL_POSSIBLE_HANDLERS          0x3FFF  //!< Install all possible exception handlers.
#define CR_INST_CRT_EXCEPTION_HANDLERS         0x3FFE  //!< Install exception handlers for the linked CRT module.

#define ARRSIZE(x)    (sizeof(x)/sizeof(x[0]))
typedef void (CALLBACK* PFNCRASHHANDLER)(LPCTSTR pszDmpPath,PEXCEPTION_POINTERS ep);
template<bool bCrashHandle,typename SymSup>
class CrashHandler_t:public SymSup
{
    typedef BOOL (WINAPI * MinidumpWriteDumpPtr_t)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
        CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
        CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
        CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);
    MinidumpWriteDumpPtr_t pDumpFunc;
    PVOID m_pHnd;
    LPTOP_LEVEL_EXCEPTION_FILTER m_pre_filter;
    TCHAR m_szFilePath[MAX_PATH+1];
    TCHAR m_szFileName[MAX_PATH+1];

    new_handler m_pre_new;
    terminate_handler m_pre_terminate;
    unexpected_handler m_preunexpect;
    _purecall_handler m_pre_purecall;
    _invalid_parameter_handler m_pre_invalid_param;
    typedef void (__cdecl *PFNSIGNAL)(int);
    PFNSIGNAL m_pre_abort,m_pre_int,m_pre_term;

    BOOL m_bUserFilePath:1;
    BOOL m_bUserDir:1;
    BOOL m_bInited:1;
    PFNCRASHHANDLER m_CrashHander;
    CRITICAL_SECTION m_lock;
    MINIDUMP_TYPE m_MiniDumpType;
    int m_Flag;

private:
    TCHAR program_version_[MAX_PATH];        // Version of host program
    TCHAR report_url_[MAX_PATH];
    TCHAR language_[MAX_PATH];
public:
    CrashHandler_t():
      m_pHnd(NULL),m_pre_filter(NULL),m_pre_new(NULL),m_pre_terminate(NULL),m_preunexpect(NULL),m_pre_purecall(NULL),m_pre_invalid_param(NULL),
          m_pre_abort(NULL),m_pre_int(NULL),m_pre_term(NULL),
          m_bUserFilePath(FALSE),m_bUserDir(FALSE),
          m_CrashHander(NULL),m_bInited(FALSE),m_MiniDumpType(MiniDumpNormal),m_Flag(0)
      {
          pDumpFunc=(MinidumpWriteDumpPtr_t)GetProcAddress(LoadLibrary(_T("DBGHELP.DLL")),"MiniDumpWriteDump");
          __ResetPath();
          InitializeCriticalSection(&m_lock);
      }
      ~CrashHandler_t()
      {
          Deinit();
          DeleteCriticalSection(&m_lock);
      }
public:
    void SetDmpFilePath(LPCTSTR pszPath)
    {
        if (!pszPath ||!*pszPath)
        {
            __ResetPath();
            return;
        }
        m_bUserFilePath = TRUE;
        m_bUserDir = FALSE;
        _tcscpy_s(m_szFilePath,MAX_PATH,pszPath);
    }
    void SetDmpDir(LPCTSTR pszPath)
    {
        if (!pszPath ||!*pszPath)
        {
            __ResetPath();
            return;
        }
        m_bUserFilePath = FALSE;
        m_bUserDir = TRUE;
        _tcscpy_s(m_szFilePath,MAX_PATH,pszPath);
    }
    void SetCrashHandler(PFNCRASHHANDLER CrashHander)
    {
        m_CrashHander = CrashHander;
    }
    void Init(TCHAR* version, TCHAR* url, PFNCRASHHANDLER CrashHander=NULL,int Flag=CR_INST_ALL_POSSIBLE_HANDLERS,MINIDUMP_TYPE MiniDumpType=MiniDumpNormal)
    {
        EnterCriticalSection(&m_lock);
        _tcsncpy_s(program_version_, version, ARRSIZE(program_version_));
        _tcsncpy_s(report_url_, url, ARRSIZE(report_url_));
        // default setting for language
        TCHAR* default_language = L"en";
        _tcsncpy_s(language_, default_language, ARRSIZE(language_));
        LOG_DEBUG << "Update language for crash report: " << default_language;
        if (!m_bInited)
        {
            m_bInited = TRUE;
            m_Flag = Flag;
            m_MiniDumpType = MiniDumpType;
            m_CrashHander = CrashHander;
            if (Flag & CR_INST_SEH_EXCEPTION_HANDLER) m_pre_filter=SetUnhandledExceptionFilter(&SEHandler);
            if (Flag & CR_INST_PURE_CALL_HANDLER) m_pre_purecall = _set_purecall_handler(&PureCallHandler);
            if (Flag & CR_INST_NEW_OPERATOR_ERROR_HANDLER) m_pre_new = set_new_handler(&NewHandler);
            if (Flag & CR_INST_INVALID_PARAMETER_HANDLER) m_pre_invalid_param = _set_invalid_parameter_handler(&InvalidParameterHandler);
            if (Flag & CR_INST_TERMINATE_HANDLER) m_pre_terminate = set_terminate(&TerminateHandler);
            if (Flag & CR_INST_UNEXPECTED_HANDLER) m_preunexpect = set_unexpected(&UnExpectedHandler);
            if (Flag & CR_INST_SIGABRT_HANDLER)
            {
#if _MSC_VER>=1400  
                _set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);
#endif
                m_pre_abort = signal(SIGABRT, &AbortHandler);
            }
            if (Flag & CR_INST_SIGILL_HANDLER) m_pre_int = signal(SIGINT, &IntHandler);
            if (Flag & CR_INST_TERMINATE_HANDLER) m_pre_term = signal(SIGTERM, &TermHandler);
            if (!bCrashHandle)
            {// maybe need to hook RaiseException, because vectored context dosen't display very well.
                if (Flag & CR_INST_VECTORED_EXCEPTION_HANDLER)	m_pHnd=AddVectoredExceptionHandler(0,&VEHandler);
            }
        }
        LeaveCriticalSection(&m_lock);
    }
    void Deinit()
    {
        EnterCriticalSection(&m_lock);
        if (m_bInited)
        {
            m_bInited=FALSE;
            if (m_Flag & CR_INST_NEW_OPERATOR_ERROR_HANDLER)//if (m_pre_new)
            {
                set_new_handler(m_pre_new);
                m_pre_new = NULL;
            }
            if (m_Flag & CR_INST_TERMINATE_HANDLER)//if (m_pre_terminate)
            {
                set_terminate(m_pre_terminate);
                m_pre_terminate= NULL;
            }
            if (m_Flag & CR_INST_UNEXPECTED_HANDLER)//if (m_preunexpect)
            {
                set_unexpected(m_preunexpect);
                m_preunexpect=NULL;
            }
            if (m_Flag & CR_INST_PURE_CALL_HANDLER)//if (m_pre_purecall)
            {
                _set_purecall_handler(m_pre_purecall);
                m_pre_purecall=NULL;
            }
            if (m_Flag & CR_INST_INVALID_PARAMETER_HANDLER)//if (m_pre_invalid_param)
            {
                _set_invalid_parameter_handler(m_pre_invalid_param);
                m_pre_invalid_param=NULL;
            }
            if (m_Flag & CR_INST_SIGABRT_HANDLER)//if (m_pre_abort)
            {
                signal(SIGABRT, m_pre_abort);
                m_pre_abort = NULL;
            }
            if (m_Flag & CR_INST_SIGILL_HANDLER)//if (m_pre_int)
            {
                signal(SIGABRT, m_pre_int);
                m_pre_int = NULL;
            }
            if (m_Flag & CR_INST_SIGTERM_HANDLER)//if (m_pre_term)
            {
                signal(SIGABRT, m_pre_term);
                m_pre_term = NULL;
            }
            if (m_pHnd)
            {
                RemoveVectoredExceptionHandler(m_pHnd);
                m_pHnd=NULL;
            }
            if (m_pre_filter)
            {
                SetUnhandledExceptionFilter(m_pre_filter);
                m_pre_filter=NULL;
            }
        }
        LeaveCriticalSection(&m_lock);
    }
    void UpdateLanguage(TCHAR* language)
    {
        EnterCriticalSection(&m_lock);
        _tcsncpy_s(language_, language, ARRSIZE(language_));
        LeaveCriticalSection(&m_lock);
    }
public:
    static CrashHandler_t<bCrashHandle,SymSup>* GetInstance()
    {
        static CrashHandler_t<bCrashHandle,SymSup> _instance;
        return &_instance;
    }
protected:
#define EXPDMP_MEMFUNC_RET(ret,calltype,n,p1,p2) \
    static ret calltype n p1{return CrashHandler_t<bCrashHandle,SymSup>::GetInstance()->n##_ p2;} \
    ret n##_ p1
#define EXPDMP_MEMFUNC(calltype,n,p1,p2) \
    static void calltype n p1{ CrashHandler_t<bCrashHandle,SymSup>::GetInstance()->n##_ p2;} \
    void n##_ p1

    EXPDMP_MEMFUNC_RET(LONG,CALLBACK,VEHandler,(PEXCEPTION_POINTERS ep),(ep))
    {
        switch(ep->ExceptionRecord->ExceptionCode)
        {
        case 0x40010006:
            return EXCEPTION_CONTINUE_SEARCH;// Known exception: OutputString
        }
        EnterCriticalSection(&m_lock);
#if	defined(_M_IX86)
        //LOG_ERROR(L"VectoredHandler: Eip:0x%p (note: This exception may be expected.)\n",ep->ContextRecord->Eip);
#elif defined (_M_AMD64)
        //LOG_ERROR(L"VectoredHandler: Rip:0x%p (note: This exception may be expected.)\n",ep->ContextRecord->Rip);
#else
#error "unsupport platform"
#endif
        DumpSymbol(ep);
        if (!IsDebuggerPresent())
            __DumpException(ep);
        else
        {
            switch(ep->ExceptionRecord->ExceptionCode)
            {
            case EXCEPTION_ACCESS_VIOLATION:
            case EXCEPTION_DATATYPE_MISALIGNMENT:
            case EXCEPTION_BREAKPOINT:
            case EXCEPTION_SINGLE_STEP:
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            case EXCEPTION_FLT_DENORMAL_OPERAND:
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            case EXCEPTION_FLT_INEXACT_RESULT:
            case EXCEPTION_FLT_INVALID_OPERATION:
            case EXCEPTION_FLT_OVERFLOW:
            case EXCEPTION_FLT_STACK_CHECK:
            case EXCEPTION_FLT_UNDERFLOW:
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
            case EXCEPTION_INT_OVERFLOW:
            case EXCEPTION_PRIV_INSTRUCTION:
            case EXCEPTION_IN_PAGE_ERROR:
            case EXCEPTION_ILLEGAL_INSTRUCTION:
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            case EXCEPTION_STACK_OVERFLOW:
            case EXCEPTION_INVALID_DISPOSITION:
            case EXCEPTION_GUARD_PAGE:
            case EXCEPTION_INVALID_HANDLE:
                //			case EXCEPTION_POSSIBLE_DEADLOCK:
                break;
            default:
                DebugBreak();
                break;
            }
        }
        LeaveCriticalSection(&m_lock);
        return EXCEPTION_CONTINUE_SEARCH;
    }
    EXPDMP_MEMFUNC_RET(LONG,WINAPI,SEHandler,(PEXCEPTION_POINTERS ep),(ep))
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"SehHandler\n");
#ifdef _DEBUG
        DumpSymbol(ep);
#endif
        if (!IsDebuggerPresent())
            __DumpException(ep);
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
        // unreachable
        return EXCEPTION_CONTINUE_SEARCH;
    }
    EXPDMP_MEMFUNC(__cdecl,NewHandler,(),())
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"NewHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,TerminateHandler,(),())
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"TerminateHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,UnExpectedHandler,(),())
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"UnExpectedHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,PureCallHandler,(),())
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"PureCallHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,InvalidParameterHandler,(const wchar_t* expression, 
        const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved),
        (expression,function,file,line,pReserved))
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"file:%s,function:%s,line:%d\n",function?function:L"",file?file:L"",line);
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,AbortHandler,(int n),(n))
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"AbortHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,IntHandler,(int n),(n))
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"IntHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }
    EXPDMP_MEMFUNC(__cdecl,TermHandler,(int n),(n))
    {
        EnterCriticalSection(&m_lock);
        //LOG_ERROR(L"TermHandler\n");
        if (!IsDebuggerPresent())
            __DumpException(GetExceptionPointers(0));
        else
            DebugBreak();
        TerminateProcess(GetCurrentProcess(), 1);    
    }

protected:
    void __DumpException(PEXCEPTION_POINTERS ep)
    {
        if (!pDumpFunc || !ep)
        {
            if (!IsBadCodePtr((PROC)m_CrashHander)) m_CrashHander(NULL,ep);
            return;
        }
        TCHAR szFilePath[MAX_PATH];
        if (!m_bUserFilePath)
        {// there's no need to append a file name if the user provided a full path
            SYSTEMTIME tm;
            GetLocalTime(&tm);
            _stprintf_s(szFilePath,MAX_PATH,_T("%s\\%s-%d-%d-%d-%d-%d-%d.dmp"),m_szFilePath,m_szFileName,(int)tm.wYear,(int)tm.wMonth,(int)tm.wDay,
                (int)tm.wHour,(int)tm.wMinute,(int)tm.wSecond);
        }
        else
        {
            _tcscpy_s(szFilePath,MAX_PATH,m_szFilePath);
        }
        // 		//LOG_ERROR(_T("writing dump to %s"),szFilePath);
        HANDLE hFile=CreateFile(szFilePath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
        if (hFile!=INVALID_HANDLE_VALUE)
        {
            _MINIDUMP_EXCEPTION_INFORMATION ExInfo = {0};
            ExInfo.ThreadId = ::GetCurrentThreadId();
            ExInfo.ExceptionPointers = ep;
            ExInfo.ClientPointers = NULL;
            (*pDumpFunc)(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, m_MiniDumpType, &ExInfo, NULL, NULL);
            CloseHandle(hFile);
            // Launch the CrashSender process
            LaunchCrashSender(szFilePath);
        }
        if (!IsBadCodePtr((PROC)m_CrashHander)) m_CrashHander(szFilePath,ep);
    }

    int LaunchCrashSender(TCHAR * crash_report)
    {
        // Create CrashSender process
        TCHAR filepath[_MAX_PATH] = {0};
        GetModuleFileName(NULL, filepath, ARRSIZE(filepath));

        wstring program_path(filepath);

        size_t position = program_path.rfind('\\');

        program_path = program_path.substr(0, position + 1);
        program_path.append(L"CrashSender.exe");

        LOG_INFO << Utils::UnicodeToUtf8(program_path);

        wstring command(program_path);

        command.append(L" -f");
        command.append(crash_report);
        command.append(L",");

        command.append(L" -v");
        command.append(program_version_);
        command.append(L",");

        command.append(L" -u");
        command.append(report_url_);
        command.append(L",");

        command.append(L" -l");
        command.append(language_);
        command.append(L",");

        LOG_INFO << Utils::UnicodeToUtf8(command);

        ////////////////////////////////////////////////////////////////////////////////////

        if (!ProcessUtils::CreateProcess(NULL, command.c_str()))
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////////////////////////////

        return 0;
    }
protected:
    void __ResetPath()
    {
        m_szFilePath[0]=0;
        m_szFileName[0]=0;
        ::GetModuleFileName(NULL,m_szFilePath,MAX_PATH);
        LPTSTR psz=_tcsrchr(m_szFilePath,'\\');
        if (psz)
        {
            _tcscpy_s(m_szFileName,MAX_PATH,psz+1);
            *psz=0;
        }
        m_bUserFilePath = FALSE;
        m_bUserDir = FALSE;
    }
    // from google crashrpt : CrashHandler.cpp
    EXCEPTION_POINTERS* GetExceptionPointers(DWORD dwExceptionCode)
    {
        // The following code was taken from VC++ 9.0 CRT (invarg.c: line 130)

        EXCEPTION_RECORD ExceptionRecord;
        CONTEXT ContextRecord;
        memset(&ContextRecord, 0, sizeof(CONTEXT));

#ifdef _M_IX86

        __asm {
            mov dword ptr [ContextRecord.Eax], eax
                mov dword ptr [ContextRecord.Ecx], ecx
                mov dword ptr [ContextRecord.Edx], edx
                mov dword ptr [ContextRecord.Ebx], ebx
                mov dword ptr [ContextRecord.Esi], esi
                mov dword ptr [ContextRecord.Edi], edi
                mov word ptr [ContextRecord.SegSs], ss
                mov word ptr [ContextRecord.SegCs], cs
                mov word ptr [ContextRecord.SegDs], ds
                mov word ptr [ContextRecord.SegEs], es
                mov word ptr [ContextRecord.SegFs], fs
                mov word ptr [ContextRecord.SegGs], gs
                pushfd
                pop [ContextRecord.EFlags]
        }

        ContextRecord.ContextFlags = CONTEXT_CONTROL;
#pragma warning(push)
#pragma warning(disable:4311)
        ContextRecord.Eip = (ULONG)_ReturnAddress();
        ContextRecord.Esp = (ULONG)_AddressOfReturnAddress();
#pragma warning(pop)
        ContextRecord.Ebp = *((ULONG *)_AddressOfReturnAddress()-1);

#elif defined (_M_AMD64)

        ULONG64 ControlPc;
        ULONG64 EstablisherFrame;
        PRUNTIME_FUNCTION FunctionEntry;
        PVOID HandlerData;
        ULONG64 ImageBase;

        RtlCaptureContext(&ContextRecord);
        ControlPc = ContextRecord.Rip;
        FunctionEntry = RtlLookupFunctionEntry(ControlPc, &ImageBase, NULL);
        if (FunctionEntry != NULL) {
            RtlVirtualUnwind(UNW_FLAG_NHANDLER,
                ImageBase,
                ControlPc,
                FunctionEntry,
                &ContextRecord,
                &HandlerData,
                &EstablisherFrame,
                NULL);
        } else {
            ContextRecord.Rip = (ULONGLONG) _ReturnAddress();
            ContextRecord.Rsp = (ULONGLONG) _AddressOfReturnAddress();
        }

#elif defined (_IA64_)

        /* Need to fill up the Context in IA64. */
        RtlCaptureContext(&ContextRecord);

#else  /* defined (_IA64_) */

        ZeroMemory(&ContextRecord, sizeof(ContextRecord));

#endif  /* defined (_IA64_) */


        ExceptionRecord.ExceptionCode = dwExceptionCode;
        ExceptionRecord.ExceptionFlags    = EXCEPTION_NONCONTINUABLE;
        ExceptionRecord.ExceptionAddress = _ReturnAddress();

        EXCEPTION_RECORD* pExceptionRecord = new EXCEPTION_RECORD;
        memcpy(pExceptionRecord, &ExceptionRecord, sizeof(EXCEPTION_RECORD));
        CONTEXT* pContextRecord = new CONTEXT;
        memcpy(pContextRecord, &ContextRecord, sizeof(CONTEXT));

        EXCEPTION_POINTERS* pExceptionPointers = new EXCEPTION_POINTERS;
        pExceptionPointers->ExceptionRecord = pExceptionRecord;
        pExceptionPointers->ContextRecord = pContextRecord;  
        return pExceptionPointers;
    }
};

class SymSupport
{
public:
    SymSupport()
    {
        if (!GetEnvironmentVariable(_T("_NT_SYMBOL_PATH"),NULL,0) && GetLastError()==ERROR_ENVVAR_NOT_FOUND)
        {
            LOG_ERROR << "Haven't Set _NT_SYMBOL_PATH,the stack information would be incomplete.";
            SetEnvironmentVariable(_T("_NT_SYMBOL_PATH"),_T("srv*http://msdl.microsoft.com/download/symbols"));
        }
        if(NULL==LoadLibrary(_T("symsrv.dll")))
        {
            LOG_ERROR << "Copy dbghelp.dll/symsrv.dll from VS installation folder.";
        }
        SymSetOptions(/*SYMOPT_DEFERRED_LOADS | */SYMOPT_UNDNAME | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_EXACT_SYMBOLS | SYMOPT_LOAD_LINES | SymGetOptions());
        SymInitialize(GetCurrentProcess(),NULL, TRUE);
    }
    ~SymSupport()
    {
        SymCleanup(GetCurrentProcess());
    }
    void DumpSymbol(DWORD64 Address)
    {
        PSYMBOL_INFO  pSymbol = (PSYMBOL_INFO)new BYTE[sizeof(SYMBOL_INFO) + 1024 * sizeof(TCHAR)];
        ZeroMemory(pSymbol,sizeof(SYMBOL_INFO) + 1024 * sizeof(TCHAR));
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = 1024;

        PIMAGEHLP_LINE64 pLine = (PIMAGEHLP_LINE64)new BYTE[sizeof(IMAGEHLP_LINE64) + 1024*sizeof(TCHAR)];
        ZeroMemory(pLine,sizeof(IMAGEHLP_LINE64) + 1024*sizeof(TCHAR));
        pLine->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        pLine->FileName = (LPTSTR)(pLine+1);

        DWORD dwDisplacement=0;
        BOOL bSymbol = ::SymFromAddr(GetCurrentProcess(),Address, 0, pSymbol );
        // 		LOG_DEBUG("last error:%d",GetLastError());
        BOOL bLines = ::SymGetLineFromAddr64(GetCurrentProcess(),Address,&dwDisplacement,pLine);
        // 		LOG_DEBUG("last error:%d",GetLastError());
        if (bSymbol && bLines)
        {
            // diplay the information to log
            //LOG_ERROR(_T("%s(%d):<%s>\n"),pLine->FileName,pLine->LineNumber,pSymbol->Name);
        }
        else
        {
            MEMORY_BASIC_INFORMATION mbi;
            if(VirtualQuery((VOID*)Address,&mbi,sizeof(mbi)) && GetModuleFileName((HINSTANCE)mbi.AllocationBase,pLine->FileName,1024))
            {
                if (bSymbol)
                {
                    //LOG_ERROR(_T("%s(offset:0x%I64x):<%s>\n"),pLine->FileName,Address-(DWORD64)mbi.AllocationBase,pSymbol->Name);
                }
                else
                {
                    //LOG_ERROR(_T("%s(offset:0x%I64x)\n"),pLine->FileName,Address-(DWORD64)mbi.AllocationBase);
                }
            }
            else if (bSymbol)
            {
                //LOG_ERROR(_T("Location:0x%I64x:<%s>\n"),Address,pSymbol->Name);
            }
            else
            {
                //LOG_ERROR(_T("Location:0x%I64x\n"),Address);
            }
        }

        delete[] (BYTE*)pLine;
        delete[] (BYTE*)pSymbol;
    }
    struct StackList 
    {
        enum
        {
            MAX_FRAME=128,
        };
        UINT nMax;
        UINT nCount;
        DWORD64 Address[1];
    };
    bool CaptureStack(CONTEXT* pContext,StackList* pStacklist)
    {
        STACKFRAME64 StackFrame;
        DWORD MachineType;

        memset(&StackFrame, 0, sizeof(STACKFRAME64));
#ifdef _M_IX86
        MachineType                 = IMAGE_FILE_MACHINE_I386;
        StackFrame.AddrPC.Offset    = pContext->Eip;
        StackFrame.AddrPC.Mode      = AddrModeFlat;
        StackFrame.AddrFrame.Offset = pContext->Ebp;
        StackFrame.AddrFrame.Mode   = AddrModeFlat;
        StackFrame.AddrStack.Offset = pContext->Esp;
        StackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_X64
        MachineType                 = IMAGE_FILE_MACHINE_AMD64;
        StackFrame.AddrPC.Offset    = pContext->Rip;
        StackFrame.AddrPC.Mode      = AddrModeFlat;
        StackFrame.AddrFrame.Offset = pContext->Rsp;
        StackFrame.AddrFrame.Mode   = AddrModeFlat;
        StackFrame.AddrStack.Offset = pContext->Rsp;
        StackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
        MachineType                 = IMAGE_FILE_MACHINE_IA64;
        StackFrame.AddrPC.Offset    = pContext->StIIP;
        StackFrame.AddrPC.Mode      = AddrModeFlat;
        StackFrame.AddrFrame.Offset = pContext->IntSp;
        StackFrame.AddrFrame.Mode   = AddrModeFlat;
        StackFrame.AddrBStore.Offset= pContext->RsBSP;
        StackFrame.AddrBStore.Mode  = AddrModeFlat;
        StackFrame.AddrStack.Offset = pContext->IntSp;
        StackFrame.AddrStack.Mode   = AddrModeFlat;
#else
#error "Unsupported platform"
#endif
        while (pStacklist->nCount<pStacklist->nMax &&
            StackWalk64(
            MachineType,
            GetCurrentProcess(),
            GetCurrentThread(),
            &StackFrame,
            pContext,
            NULL,
            SymFunctionTableAccess64,
            SymGetModuleBase64,
            NULL ) )
        {
            if (StackFrame.AddrPC.Offset)
            {
                pStacklist->Address[pStacklist->nCount++] = StackFrame.AddrPC.Offset;
            }
            else
            {
                break;
            }

        }
        return pStacklist->nCount!=0;
    }
    void DumpSymbol(PEXCEPTION_POINTERS ep)
    {
        CONTEXT Context = *ep->ContextRecord;
        StackList* pStacklist=(StackList*) new BYTE[sizeof(StackList)+sizeof(DWORD64)*StackList::MAX_FRAME];
        pStacklist->nMax=StackList::MAX_FRAME+1;
        pStacklist->nCount=0;
        if(CaptureStack(&Context,pStacklist))
        {
            for (UINT i=0;i<pStacklist->nCount;i++)
            {
                DumpSymbol(pStacklist->Address[i]);
            }
        }
        else
        {
#if	defined(_M_IX86)
            DWORD64 Address=(ep && ep->ContextRecord && ep->ContextRecord->Eip)?ep->ContextRecord->Eip:0;
#elif defined (_M_AMD64)
            DWORD64 Address=(ep && ep->ContextRecord && ep->ContextRecord->Rip)?ep->ContextRecord->Rip:0;
#else
#error "unsupport platform"
#endif
            if (Address) DumpSymbol(Address);
        }
        delete [] (BYTE*)pStacklist;
    }
};

class SymSupportFake
{
public:
    SymSupportFake(){}
    ~SymSupportFake(){}
    void DumpSymbol(DWORD64 Address){} 
    void DumpSymbol(PEXCEPTION_POINTERS ep){} 
};

typedef CrashHandler_t<true,SymSupportFake> CrashHandlerT;
typedef CrashHandler_t<false,SymSupport> ExceptionRecorderT;

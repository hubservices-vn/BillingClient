#pragma once

#define GCACLIENTAPI

// 大厅中的新GCA数据包装格式
#include <PshPack1.h>
typedef struct tagGCA_Data_t 
{
	BYTE cReserved;// fixed 0
	BYTE cVersion;// GCA version
	USHORT wLen;// the whole struct size
#pragma warning(disable:4200)
	BYTE cData[0];// GCA data
#pragma warning(default:4200)
}GCA_Data_t;

#include <PopPack.h>

#define GCA_DATA_LEN_V1	48
#define GCA_DATA_LEN_V2	128

// 客户端必须实现这两个接口
class IGCAClient
{
public:
	virtual void *GetBuffer(size_t dwRequired)=0;
	virtual void ReleaseBuffer(size_t dwWriten)=0;
};

// 客户端调用此函数获取信息
extern "C" GCACLIENTAPI BOOL WINAPI GCA_GetInfo(IGCAClient* pThis);
extern "C" GCACLIENTAPI BOOL WINAPI GCA_GetInfoPlain(IGCAClient* pThis);
// the lastest version support this interface
extern "C" GCACLIENTAPI BOOL WINAPI GCA_GetCafeID(int* pCafeId);
extern "C" GCACLIENTAPI BOOL WINAPI GCA_GetLangID(int* pLangId);

#if !defined(_LIB)

#define _THIS_LIBNAME1_	"gcaclientlib"

#ifdef _DEBUG
	#define _THIS_LIBNAME2_ _THIS_LIBNAME1_## "d"
#else
	#define _THIS_LIBNAME2_ _THIS_LIBNAME1_## ""
#endif

#ifdef _DLL
	#define _THIS_LIBNAME3_ _THIS_LIBNAME2_## "md"
#else
#ifdef _MT
	#define _THIS_LIBNAME3_ _THIS_LIBNAME2_## "mt"
#else
	#define _THIS_LIBNAME3_ _THIS_LIBNAME2_## "ml"
#endif // _MT 
#endif // _DLL

#if _MSC_VER < 1200
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "50.lib"
#elif _MSC_VER == 1200
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "60.lib"
#elif _MSC_VER == 1300
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "70.lib"
#elif _MSC_VER < 1400
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "71.lib"
#elif _MSC_VER < 1500
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "80.lib"
#elif _MSC_VER < 1600
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "90.lib"
#elif _MSC_VER == 1600
	#define _THIS_LIBNAME_	_THIS_LIBNAME3_## "100.lib"
#else
	#error _THIS_LIBNAME1_ "Unsupported VC++ version"
#endif

#pragma comment(lib, _THIS_LIBNAME_)

#undef _THIS_LIBNAME1_
#undef _THIS_LIBNAME2_
#undef _THIS_LIBNAME3_
#undef _THIS_LIBNAME_

#endif
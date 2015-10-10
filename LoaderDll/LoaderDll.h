#ifndef _LOADERDLL_H_586bc656_348b_4b12_ba74_d39366b67f23_
#define _LOADERDLL_H_586bc656_348b_4b12_ba74_d39366b67f23_

#include <Windows.h>

// such stupid link.exe error
extern "C" __declspec(noreturn) void __cdecl __std_terminate() {}

#if !defined(ML_USER_MODE) && !defined(ML_KERNEL_MODE)

#error stupid

#if defined(__cplusplus)
#define DEFAULT_VALUE(type, var, value) type var = value
#define DEF_VAL(var, value)             var = value
#define EXTC extern "C"
#else
#define DEFAULT_VALUE(type, var, value) type var
#define DEF_VAL(var, value)             var
#define EXTC
#endif //CPP_DEFINED

typedef struct ML_PROCESS_INFORMATION : public PROCESS_INFORMATION
{
	PVOID FirstCallLdrLoadDll;

} ML_PROCESS_INFORMATION, *PML_PROCESS_INFORMATION;

typedef struct _TIME_FIELDS {
	SHORT Year;        // range [1601...]
	SHORT Month;       // range [1..12]
	SHORT Day;         // range [1..31]
	SHORT Hour;        // range [0..23]
	SHORT Minute;      // range [0..59]
	SHORT Second;      // range [0..59]
	SHORT Milliseconds;// range [0..999]
	SHORT Weekday;     // range [0..6] == [Sunday..Saturday]
} TIME_FIELDS, *PTIME_FIELDS;

typedef struct _RTL_TIME_ZONE_INFORMATION {
	LONG        Bias;
	WCHAR       StandardName[32];
	TIME_FIELDS StandardStart;
	LONG        StandardBias;
	WCHAR       DaylightName[32];
	TIME_FIELDS DaylightStart;
	LONG        DaylightBias;
} RTL_TIME_ZONE_INFORMATION, *PRTL_TIME_ZONE_INFORMATION;

typedef struct
{
	ULONG AnsiCodePage;
	ULONG OemCodePage;
	ULONG LocaleID;
	ULONG DefaultCharset;

	WCHAR DefaultFaceName[LF_FACESIZE];

	RTL_TIME_ZONE_INFORMATION Timezone;

} LOCALE_ENUMLATOR_ENVIRONMENT_BLOCK, *PLOCALE_ENUMLATOR_ENVIRONMENT_BLOCK, LEB, *PLEB;

#else

#include "../LocaleEmulator/LocaleEmulator.h"
#define LE_API EXTC

#endif // ml

#if !LE_LOADER_DLL
#undef LE_API
#define LE_API EXTC __declspec(dllimport)
#endif

LE_API
NTSTATUS
WINAPI
LeCreateProcess(
	PLEB                    Leb,
	PCWSTR                  ApplicationName,
	PWSTR                   CommandLine = nullptr,
	PCWSTR                  CurrentDirectory = nullptr,
	ULONG                   CreationFlags = 0,
	LPSTARTUPINFOW          StartupInfo = nullptr,
	PML_PROCESS_INFORMATION ProcessInformation = nullptr,
	LPSECURITY_ATTRIBUTES   ProcessAttributes = nullptr,
	LPSECURITY_ATTRIBUTES   ThreadAttributes = nullptr,
	PVOID                   Environment = nullptr,
	HANDLE                  Token = nullptr
	);

LE_API
NTSTATUS
WINAPI
LeCreateProcess2(
	PLEB                    Leb,
	PCWSTR                  ApplicationName,
	PWSTR                   CommandLine = nullptr,
	PCWSTR                  CurrentDirectory = nullptr,
	ULONG                   CreationFlags = 0,
	LPSTARTUPINFOW          StartupInfo = nullptr,
	PML_PROCESS_INFORMATION ProcessInformation = nullptr,
	LPSECURITY_ATTRIBUTES   ProcessAttributes = nullptr,
	LPSECURITY_ATTRIBUTES   ThreadAttributes = nullptr,
	PVOID                   Environment = nullptr,
	HANDLE                  Token = nullptr
	);

#endif // _LOADERDLL_H_586bc656_348b_4b12_ba74_d39366b67f23_
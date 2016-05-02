#ifndef _LOCALEEMULATOR_H_cd444a0d_c7f9_44b2_aac8_8107e9a07ca2_
#define _LOCALEEMULATOR_H_cd444a0d_c7f9_44b2_aac8_8107e9a07ca2_

#include "ml.h"


#define ARCHEAGE_VER 0
#define ARCHEAGE_TX 1


#define ROOTDIR_SYSTEM32            L"\\SystemRoot\\system32\\"

#define REGKEY_ROOT                 HKEY_LOCAL_MACHINE

#define REGPATH_CODEPAGE            L"System\\CurrentControlSet\\Control\\Nls\\CodePage"
#define REGPATH_LANGUAGE            L"System\\CurrentControlSet\\Control\\Nls\\Language"
#define REGKEY_ACP                  L"ACP"
#define REGKEY_OEMCP                L"OEMCP"
#define REGKEY_DEFAULT_LANGUAGE     L"Default"

#define PROP_WINDOW_ANSI_PROC       L"Asuna"

#define FORMAT_LOCALE_ENUMLATOR_PROCESS_ENVIRONMENT_BLOCK   L"Local\\LOCALE_ENUMLATOR_PROCESS_ENVIRONMENT_BLOCK_SECTION_%p"


#define LeHookFromEAT(_Base, _Prefix, _Name)    Mp::FunctionJumpVa(LookupExportTable(_Base, _Prefix##_##_Name), Le##_Name, &HookStub.Stub##_Name)
#define LeHookFromEAT2(_Base, _Prefix, _Name)   Mp::FunctionJumpVa(LookupExportTable(_Base, _Prefix##_##_Name), Le##_Name)
#define LeFunctionJump(_Name)                   Mp::FunctionJumpVa(_Name, Le##_Name, &HookStub.Stub##_Name)
#define LeFunctionCall(_Name)                   Mp::FunctionCallVa(_Name, Le##_Name, &HookStub.Stub##_Name)

class LeGlobalData;
typedef LeGlobalData* PLeGlobalData;

#define THREAD_LOCAL_BUFFER_CONTEXT TAG4('LTLB')
#define LE_LOADER_PROCESS           TAG4('LELP')

inline BOOL IsLeLoader()
{
    return FindThreadFrame(LE_LOADER_PROCESS) != nullptr;
}

typedef struct THREAD_LOCAL_BUFFER : public TEB_ACTIVE_FRAME
{
    BYTE Buffer[MEMORY_PAGE_SIZE * 2];

    THREAD_LOCAL_BUFFER()
    {
        this->Context = THREAD_LOCAL_BUFFER_CONTEXT;
    }

    PVOID GetBuffer()
    {
        return this == nullptr ? nullptr : &Buffer;
    }

    NoInline static THREAD_LOCAL_BUFFER* GetTlb(BOOL Allocate)
    {
        PTHREAD_LOCAL_BUFFER Tlb;

        Tlb = (PTHREAD_LOCAL_BUFFER)FindThreadFrame(THREAD_LOCAL_BUFFER_CONTEXT);

        if (Tlb != nullptr || Allocate == FALSE)
            return Tlb;

        Tlb = new THREAD_LOCAL_BUFFER;
        if (Tlb != nullptr)
        {
            Tlb->Push();
        }

        return Tlb;
    }

    NoInline static VOID ReleaseTlb()
    {
        delete GetTlb(FALSE);
    }

} THREAD_LOCAL_BUFFER, *PTHREAD_LOCAL_BUFFER;

typedef struct
{
    HDC                 DC;
    HFONT               Font;
    HFONT               OldFont;
    ULONG_PTR           FontType;
    LPENUMLOGFONTEXW    EnumLogFontEx;

} ADJUST_FONT_DATA, *PADJUST_FONT_DATA;

typedef struct TEXT_METRIC_INTERNAL
{
    ULONG       Magic;
    BOOL        Filled;
    TEXTMETRICA TextMetricA;
    TEXTMETRICW TextMetricW;

    TEXT_METRIC_INTERNAL()
    {
        this->Magic = TAG4('TMIN');
        this->Filled = FALSE;
    }

    BOOL VerifyMagic()
    {
        return this->Magic == TAG4('TMIN');
    }

} TEXT_METRIC_INTERNAL, *PTEXT_METRIC_INTERNAL;

typedef struct
{
    ULONG64             Root;
    UNICODE_STRING64    SubKey;
    UNICODE_STRING64    ValueName;
    ULONG               DataType;
    PVOID64             Data;
    ULONG64             DataSize;

} REGISTRY_ENTRY64;

typedef struct
{
    REGISTRY_ENTRY64 Original;
    REGISTRY_ENTRY64 Redirected;

} REGISTRY_REDIRECTION_ENTRY64, *PREGISTRY_REDIRECTION_ENTRY64;

typedef struct
{
    ULONG                           AnsiCodePage;
    ULONG                           OemCodePage;
    ULONG                           LocaleID;
    ULONG                           DefaultCharset;
    ULONG                           HookUILanguageApi;
    WCHAR                           DefaultFaceName[LF_FACESIZE];
    RTL_TIME_ZONE_INFORMATION       Timezone;
    ULONG64                         NumberOfRegistryRedirectionEntries;
    REGISTRY_REDIRECTION_ENTRY64    RegistryReplacement[1];

} LOCALE_ENUMLATOR_ENVIRONMENT_BLOCK, *PLOCALE_ENUMLATOR_ENVIRONMENT_BLOCK, LEB, *PLEB;

#define LDR_LOAD_DLL_BACKUP_SIZE 5

#pragma warning(push)
#pragma warning(disable:4324)

typedef struct DECL_ALIGN(16) REGISTRY_ENTRY
{
    HKEY            Root;
    ml::String      SubKey;
    ml::String      ValueName;
    ULONG_PTR       DataType;
    PVOID           Data;
    ULONG_PTR       DataSize;
    ml::String      FullPath;

    REGISTRY_ENTRY()
    {
        Data = nullptr;
    }

    ~REGISTRY_ENTRY()
    {
        FreeMemoryP(this->Data);
        this->Data = nullptr;
    }

private:
    REGISTRY_ENTRY(const REGISTRY_ENTRY&);

} REGISTRY_ENTRY, *PREGISTRY_ENTRY;

#pragma warning(pop)

typedef struct
{
    REGISTRY_ENTRY Original;
    REGISTRY_ENTRY Redirected;

} REGISTRY_REDIRECTION_ENTRY, *PREGISTRY_REDIRECTION_ENTRY;

typedef struct
{
    ULONG_PTR   OriginalCharset;
    ULONG_PTR   OriginalLocaleID;
    CHAR        ScriptNameA[LF_FACESIZE];
    WCHAR       ScriptNameW[LF_FACESIZE];

    HANDLE      Section;
    PVOID       LdrLoadDllAddress;
    ULONG_PTR   LdrLoadDllBackupSize;
    BYTE        LdrLoadDllBackup[16];
    WCHAR       LeDllFullPath[MAX_NTPATH];
    LEB         Leb;

} LOCALE_ENUMLATOR_PROCESS_ENVIRONMENT_BLOCK, *PLOCALE_ENUMLATOR_PROCESS_ENVIRONMENT_BLOCK, LEPEB, *PLEPEB;

inline
ULONG_PTR
GetLePebSectionName(
    PWSTR       Buffer,
    ULONG_PTR   ProcessId
)
{
    return swprintf(Buffer, FORMAT_LOCALE_ENUMLATOR_PROCESS_ENVIRONMENT_BLOCK, ProcessId);
}

inline NTSTATUS CloseLePeb(PLEPEB LePeb)
{
    return LePeb == nullptr ? STATUS_INVALID_PARAMETER : NtUnmapViewOfSection(CurrentProcess, LePeb);
}

inline VOID InitDefaultLeb(PLEB Leb)
{
    static WCHAR StandardName[] = L"@tzres.dll,-632";
    static WCHAR DaylightName[] = L"@tzres.dll,-631";

    ZeroMemory(Leb, sizeof(*Leb));

#if 1

    static WCHAR FaceName[]     = L"MS Gothic";

    Leb->AnsiCodePage      = CP_SHIFTJIS;
    Leb->OemCodePage       = CP_SHIFTJIS;
    Leb->LocaleID          = 0x411;
    Leb->DefaultCharset    = SHIFTJIS_CHARSET;
	Leb->HookUILanguageApi = 0;

#else

    static WCHAR FaceName[]     = L"Microsoft YaHei";

    Leb->AnsiCodePage      = CP_GB2312;
    Leb->OemCodePage       = CP_GB2312;
    Leb->LocaleID          = 0x804;
    Leb->DefaultCharset    = GB2312_CHARSET;
	Leb->HookUILanguageApi = 0;

#endif

    CopyStruct(Leb->DefaultFaceName, FaceName, sizeof(FaceName));

    NtQuerySystemInformation(SystemCurrentTimeZoneInformation, &Leb->Timezone, sizeof(Leb->Timezone), nullptr);

    Leb->Timezone.Bias = -540;
    CopyStruct(Leb->Timezone.StandardName, StandardName, sizeof(StandardName));
    CopyStruct(Leb->Timezone.DaylightName, DaylightName, sizeof(DaylightName));
}

inline
PLEPEB
OpenOrCreateLePeb(
    ULONG_PTR   ProcessId   = CurrentPid(),
    BOOL        Create      = FALSE,
    ULONG_PTR   ExtraSize   = 0
)
{
    NTSTATUS            Status;
    WCHAR               SectionNameBuffer[MAX_NTPATH];
    OBJECT_ATTRIBUTES   ObjectAttributes;
    UNICODE_STRING      SectionName, BaseNamedObjectsName;
    HANDLE              SectionHandle, BaseNamedObjects;
    PLEPEB              LePeb;
    ULONG_PTR           ViewSize, SessionId;
    LARGE_INTEGER       MaximumSize;

    SessionId = GetSessionId(ProcessId);
    if (SessionId == INVALID_SESSION_ID)
        return nullptr;

    BaseNamedObjectsName.Length         = swprintf(SectionNameBuffer, L"\\Sessions\\%d\\BaseNamedObjects", SessionId) * sizeof(WCHAR);
    BaseNamedObjectsName.MaximumLength  = BaseNamedObjectsName.Length;
    BaseNamedObjectsName.Buffer         = SectionNameBuffer;

    InitializeObjectAttributes(&ObjectAttributes, &BaseNamedObjectsName, OBJ_CASE_INSENSITIVE, nullptr, nullptr);
    Status = NtOpenDirectoryObject(&BaseNamedObjects, DIRECTORY_ALL_ACCESS, &ObjectAttributes);
    if (NT_FAILED(Status))
        return nullptr;

    SectionName.Length          = (USHORT)GetLePebSectionName(SectionNameBuffer, ProcessId) * sizeof(WCHAR);
    SectionName.MaximumLength   = SectionName.Length;
    SectionName.Buffer          = SectionNameBuffer;

    InitializeObjectAttributes(&ObjectAttributes, &SectionName, 0, BaseNamedObjects, nullptr);

    Status = NtOpenSection(&SectionHandle, SECTION_ALL_ACCESS, &ObjectAttributes);
    if (NT_FAILED(Status) && Create)
    {
        MaximumSize.QuadPart = sizeof(*LePeb) + ExtraSize;
        Status = NtCreateSection(
                    &SectionHandle,
                    SECTION_ALL_ACCESS,
                    &ObjectAttributes,
                    &MaximumSize,
                    PAGE_READWRITE,
                    SEC_COMMIT,
                    nullptr
                );
    }

    NtClose(BaseNamedObjects);

    if (NT_FAILED(Status))
        return nullptr;

    ViewSize = 0;
    LePeb = nullptr;
    Status = NtMapViewOfSection(
                SectionHandle,
                CurrentProcess,
                (PVOID *)&LePeb,
                0,
                sizeof(*LePeb),
                nullptr,
                &ViewSize,
                ViewShare,
                0,
                PAGE_READWRITE
            );

    if (NT_FAILED(Status))
    {
        NtClose(SectionHandle);
        return nullptr;
    }

    if (Create) LOOP_ONCE
    {
        HANDLE ProcessHandle;

        ZeroMemory(LePeb, ViewSize);

        Status = PidToHandleEx(&ProcessHandle, ProcessId);
        FAIL_BREAK(Status);

        Status = NtDuplicateObject(CurrentProcess, SectionHandle, ProcessHandle, &LePeb->Section, 0, 0, DUPLICATE_SAME_ACCESS);
        NtClose(ProcessHandle);
        FAIL_BREAK(Status);
    }

    NtClose(SectionHandle);
    if (NT_FAILED(Status))
    {
        CloseLePeb(LePeb);
        return nullptr;
    }

    return LePeb;
}

#define ENABLE_LOG 1

#if ENABLE_LOG

inline VOID InitLog(NtFileDisk &LogFile)
{
    WCHAR LogFilePath[MAX_NTPATH];
    UNICODE_STRING SelfPath;
    PLDR_MODULE Self, Target;

    Target = FindLdrModuleByHandle(nullptr);
    Self = FindLdrModuleByHandle(&__ImageBase);

    SelfPath = Self->FullDllName;
    SelfPath.Length -= Self->BaseDllName.Length;

    swprintf(LogFilePath, L"%wZ\\%wZ.%p.log.txt", &SelfPath, &Target->BaseDllName, CurrentPid());

    ULONG BOM = BOM_UTF16_LE;
    LogFile.Create(LogFilePath);
    LogFile.Write(&BOM, 2);

    PROCESS_IMAGE_FILE_NAME2 proc;
    //NtQueryInformationProcess(CurrentProcess, ProcessImageFileName, &proc, sizeof(proc), NULL);
    proc.ImageFileName = Target->FullDllName;
    LogFile.Write(proc.ImageFileName.Buffer, proc.ImageFileName.Length);
    LogFile.Write(L"\r\n", 4);
}

#define WriteLog(...) { if (LeGetGlobalData() != nullptr && LeGetGlobalData()->LogFile) LeGetGlobalData()->LogFile.Print(NULL, __VA_ARGS__), LeGetGlobalData()->LogFile.Print(NULL, L"\r\n"); }

#else

inline VOID InitLog(...) {}

#define WriteLog(...)

#endif // ENABLE_LOG

class LeGlobalData
{
protected:
    BOOLEAN Wow64 : 1;
    BOOLEAN HasWin32U : 1; //windows 10.0.14295 or higher

    LEPEB LePeb;

    ml::GrowableArray<REGISTRY_REDIRECTION_ENTRY> RegistryRedirectionEntry;
    ml::HashTableT<TEXT_METRIC_INTERNAL> TextMetricCache;

    PVOID CodePageMapView;
    ULONG_PTR AnsiCodePageOffset, OemCodePageOffset, UnicodeCaseTableOffset;

    PVOID DllNotificationCookie;

    UNICODE_STRING SystemDirectory;

#if ENABLE_LOG

public:
    NtFileDisk LogFile;

#endif // log

public:

    struct
    {
        API_POINTER(RtlKnownExceptionFilter)    StubRtlKnownExceptionFilter;
        API_POINTER(NtContinue)                 StubLdrInitNtContinue;
        API_POINTER(LdrResSearchResource)       StubLdrResSearchResource;

        API_POINTER(NtUserMessageCall)          StubNtUserMessageCall;
        API_POINTER(NtUserDefSetText)           StubNtUserDefSetText;
        API_POINTER(SetWindowLongA)             StubSetWindowLongA;
        API_POINTER(GetWindowLongA)             StubGetWindowLongA;
        API_POINTER(IsWindowUnicode)            StubIsWindowUnicode;
        API_POINTER(GetClipboardData)           StubGetClipboardData;
        API_POINTER(SetClipboardData)           StubSetClipboardData;
        API_POINTER(GetDC)                      StubGetDC;
        API_POINTER(GetDCEx)                    StubGetDCEx;
        API_POINTER(GetWindowDC)                StubGetWindowDC;
        API_POINTER(BeginPaint)                 StubBeginPaint;

        union
        {
            PVOID                                       StubNtUserCreateWindowEx;
            API_POINTER(::NtUserCreateWindowEx_Win7)    StubNtUserCreateWindowEx_Win7;
            API_POINTER(::NtUserCreateWindowEx_Win8)    StubNtUserCreateWindowEx_Win8;
        };

        API_POINTER(GetStockObject)             StubGetStockObject;
        API_POINTER(DeleteObject)               StubDeleteObject;
        API_POINTER(CreateFontIndirectExW)      StubCreateFontIndirectExW;
        API_POINTER(NtGdiHfontCreate)           StubNtGdiHfontCreate;
        API_POINTER(CreateCompatibleDC)         StubCreateCompatibleDC;
        API_POINTER(EnumFontsA)                 StubEnumFontsA;
        API_POINTER(EnumFontsW)                 StubEnumFontsW;
        API_POINTER(EnumFontFamiliesA)          StubEnumFontFamiliesA;
        API_POINTER(EnumFontFamiliesW)          StubEnumFontFamiliesW;
        API_POINTER(EnumFontFamiliesExA)        StubEnumFontFamiliesExA;
        API_POINTER(EnumFontFamiliesExW)        StubEnumFontFamiliesExW;

    } HookStub;

    ATOM AtomAnsiProc; //, AtomUnicodeProc;

    struct HookRoutineData
    {
        ~HookRoutineData()
        {
            RtlFreeUnicodeString(&Ntdll.CodePageKey);
            RtlFreeUnicodeString(&Ntdll.LanguageKey);
            RtlDeleteCriticalSection(&Gdi32.GdiLock);
        }

        struct
        {
            UNICODE_STRING CodePageKey;
            UNICODE_STRING LanguageKey;

        } Ntdll;

        struct
        {
        } User32;

        struct
        {
            BOOLEAN StockObjectInitialized : 1;

            RTL_CRITICAL_SECTION GdiLock;

            HGDIOBJ StockObject[STOCK_LAST + 1];

        } Gdi32;

    } HookRoutineData;

public:
    LeGlobalData()
    {
        ZeroMemory(this, sizeof(*this));

        new (&this->TextMetricCache) TYPE_OF(this->TextMetricCache);

        IF_EXIST(LeGlobalData::LogFile)
        {
            InitLog(this->LogFile);
        }
    }

    ~LeGlobalData()
    {
        UnInitialize();
    }

    PLEPEB GetLePeb()
    {
        return &LePeb;
    }

    PLEB GetLeb()
    {
        return &LePeb.Leb;
    }

    VOID InitFontCharsetInfo()
    {
        HDC DC;
        LOGFONTW lf;

        DC = HookStub.StubGetDC == nullptr ? ::GetDC(nullptr) : this->GetDC(nullptr);
        GetLePeb()->OriginalCharset = GetTextCharset(DC);

        lf.lfCharSet = GetLeb()->DefaultCharset;
        lf.lfFaceName[0] = 0;

        auto EnumFontCallback = [] (CONST LOGFONTW *lf, CONST TEXTMETRICW *, DWORD, LPARAM Param)
            {
                LeGlobalData *GlobalData = (LeGlobalData *)Param;
                LPENUMLOGFONTEXW elf = (LPENUMLOGFONTEXW)lf;

                CopyStruct(GlobalData->GetLePeb()->ScriptNameW, elf->elfScript, sizeof(elf->elfScript));
                UnicodeToAnsi(GlobalData->GetLePeb()->ScriptNameA, countof(GlobalData->GetLePeb()->ScriptNameA), GlobalData->GetLePeb()->ScriptNameW);

                return FALSE;
            };

        if (HookStub.StubEnumFontFamiliesExW == nullptr)
        {
            ::EnumFontFamiliesExW(DC, &lf, EnumFontCallback, (LPARAM)this, 0);
        }
        else
        {
            EnumFontFamiliesExW(DC, &lf, EnumFontCallback, (LPARAM)this, 0);
        }

        ReleaseDC(nullptr, DC);
    }

    NTSTATUS Initialize();
    NTSTATUS UnInitialize();
    NTSTATUS InitRegistryRedirection(PREGISTRY_REDIRECTION_ENTRY64 Entry64, ULONG_PTR Count, PVOID BaseAddress);

    VOID DllNotification(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData);
    VOID HookModule(PVOID DllBase, PCUNICODE_STRING DllName, BOOL DllLoad);

    NTSTATUS HookUser32Routines(PVOID User32);
    NTSTATUS UnHookUser32Routines();

    NTSTATUS HookGdi32Routines(PVOID Gdi32);
    NTSTATUS UnHookGdi32Routines();

    NTSTATUS HookNtdllRoutines(PVOID Ntdll);
    NTSTATUS UnHookNtdllRoutines();

    NTSTATUS HookKernel32Routines(PVOID Kernel32);
    NTSTATUS UnHookKernel32Routines();


#if ARCHEAGE_VER

    NTSTATUS HookX2GameRoutines(PVOID X2Game);

#endif // ARCHEAGE_VER

    NTSTATUS
    LookupRegistryRedirectionEntry(
        HANDLE                          KeyHandle,
        PUNICODE_STRING                 ValueName,
        PREGISTRY_REDIRECTION_ENTRY*    RedirectionEntry
    );

    NTSTATUS HackUserDefaultLCID(PVOID Kernel32);
    NTSTATUS HackUserDefaultLCID2(PVOID Kernel32);
    NTSTATUS InjectSelfToChildProcess(HANDLE Process, PCLIENT_ID Cid);

    /************************************************************************
      helper func
    ************************************************************************/

    /************************************************************************
      ntdll
    ************************************************************************/

    LONG RtlKnownExceptionFilter(PEXCEPTION_POINTERS ExceptionPointers)
    {
        return HookStub.StubRtlKnownExceptionFilter(ExceptionPointers);
    }

    /************************************************************************
      kernelbase
    ************************************************************************/

    /************************************************************************
      user32
    ************************************************************************/

    PVOID GetWindowDataA(HWND Window)
    {
        return GetPropW(Window, (PCWSTR)AtomAnsiProc);
    }

    BOOL SetWindowDataA(HWND Window, PVOID Data)
    {
        return SetPropW(Window, (PCWSTR)AtomAnsiProc, Data);
    }
/*
    PVOID GetWindowDataW(HWND Window)
    {
        return GetPropW(Window, (PCWSTR)AtomUnicodeProc);
    }

    BOOL SetWindowDataW(HWND Window, PVOID Data)
    {
        return SetPropW(Window, (PCWSTR)AtomUnicodeProc, Data);
    }
*/
    LONG_PTR GetWindowLongA(HWND hWnd, int Index)
    {
        return HookStub.StubGetWindowLongA(hWnd, Index);
    }

    LONG_PTR SetWindowLongA(HWND hWnd, int Index, LONG_PTR NewLong)
    {
        return HookStub.StubSetWindowLongA(hWnd, Index, NewLong);
    }

    BOOL IsWindowUnicode(HWND hWnd)
    {
        return HookStub.StubIsWindowUnicode(hWnd);
    }

    HANDLE SetClipboardData(UINT Format, HANDLE Memory)
    {
        return HookStub.StubSetClipboardData(Format, Memory);
    }

    HANDLE GetClipboardData(UINT Format)
    {
        return HookStub.StubGetClipboardData(Format);
    }

    HDC GetDC(HWND hWnd)
    {
        return HookStub.StubGetDC(hWnd);
    }

    HDC GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
    {
        return HookStub.StubGetDCEx(hWnd, hrgnClip, flags);
    }

    HDC GetWindowDC(HWND hWnd)
    {
        return HookStub.StubGetWindowDC(hWnd);
    }

    HDC BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint)
    {
        return HookStub.StubBeginPaint(hWnd, lpPaint);
    }

    LRESULT NtUserMessageCall(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ULONG_PTR xParam, DWORD xpfnProc, ULONG Flags)
    {
        return HookStub.StubNtUserMessageCall(hWnd, Message, wParam, lParam, xParam, xpfnProc, Flags);
    }

    HWND NtUserCreateWindowEx_Win7(ULONG ExStyle, PLARGE_UNICODE_STRING ClassName, PLARGE_UNICODE_STRING ClassVersion, PLARGE_UNICODE_STRING WindowName, ULONG Style, LONG X, LONG Y, LONG Width, LONG Height, HWND ParentWnd, HMENU Menu, PVOID Instance, LPVOID Param, ULONG ShowMode, ULONG Unknown)
    {
        return HookStub.StubNtUserCreateWindowEx_Win7(ExStyle, ClassName, ClassVersion, WindowName, Style, X, Y, Width, Height, ParentWnd, Menu, Instance, Param, ShowMode, Unknown);
    }

    HWND NtUserCreateWindowEx_Win8(ULONG ExStyle, PLARGE_UNICODE_STRING ClassName, PLARGE_UNICODE_STRING ClassVersion, PLARGE_UNICODE_STRING WindowName, ULONG Style, LONG X, LONG Y, LONG Width, LONG Height, HWND ParentWnd, HMENU Menu, PVOID Instance, LPVOID Param, ULONG ShowMode, ULONG Unknown, ULONG Unknown2)
    {
        return HookStub.StubNtUserCreateWindowEx_Win8(ExStyle, ClassName, ClassVersion, WindowName, Style, X, Y, Width, Height, ParentWnd, Menu, Instance, Param, ShowMode, Unknown, Unknown2);
    }

    BOOL NtUserSetDefText(HWND hWnd, PLARGE_UNICODE_STRING Text)
    {
        return HookStub.StubNtUserDefSetText(hWnd, Text);
    }

    /************************************************************************
      gdi32
    ************************************************************************/

    INT FmsEnumFontFamiliesEx(HDC hDC, PLOGFONTW Logfont, FONTENUMPROCW Proc, LPARAM Parameter, ULONG Flags);

    NTSTATUS AdjustFontData(HDC DC, LPENUMLOGFONTEXW EnumLogFontEx, PTEXT_METRIC_INTERNAL TextMetric, ULONG_PTR FontType);
    NTSTATUS AdjustFontDataInternal(PADJUST_FONT_DATA AdjustData);
    NTSTATUS GetNameRecordFromNameTable(PVOID TableBuffer, ULONG_PTR TableSize, ULONG_PTR NameID, ULONG_PTR LanguageID, PUNICODE_STRING Name);

    VOID GetTextMetricsAFromLogFont(PTEXTMETRICA TextMetricA, CONST LOGFONTW *LogFont);
    VOID GetTextMetricsWFromLogFont(PTEXTMETRICW TextMetricW, CONST LOGFONTW *LogFont);

    PTEXT_METRIC_INTERNAL GetTextMetricFromCache(LPENUMLOGFONTEXW LogFont);
    VOID AddTextMetricToCache(LPENUMLOGFONTEXW LogFont, PTEXT_METRIC_INTERNAL TextMetric);

    HGDIOBJ GetStockObject(LONG Object)
    {
        return HookStub.StubGetStockObject(Object);
    }

    BOOL DeleteObject(HGDIOBJ GdiObject)
    {
        return HookStub.StubDeleteObject(GdiObject);
    }

    HDC CreateCompatibleDC(HDC hDC)
    {
        return HookStub.StubCreateCompatibleDC(hDC);
    }

    int EnumFontsA(HDC hdc, PCSTR lpFaceName, FONTENUMPROCA lpFontFunc, LPARAM lParam)
    {
        return HookStub.StubEnumFontsA(hdc, lpFaceName, lpFontFunc, lParam);
    }

    int EnumFontsW(HDC hdc, PCWSTR lpFaceName, FONTENUMPROCW lpFontFunc, LPARAM lParam)
    {
        return HookStub.StubEnumFontsW(hdc, lpFaceName, lpFontFunc, lParam);
    }

    int EnumFontFamiliesA(HDC hdc, LPCSTR lpFaceName, FONTENUMPROCA lpProc, LPARAM lParam)
    {
        return HookStub.StubEnumFontFamiliesA(hdc, lpFaceName, lpProc, lParam);
    }

    int EnumFontFamiliesW(HDC hdc, LPCWSTR lpFaceName, FONTENUMPROCW lpProc, LPARAM lParam)
    {
        return HookStub.StubEnumFontFamiliesW(hdc, lpFaceName, lpProc, lParam);
    }

    int EnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
    {
        return HookStub.StubEnumFontFamiliesExA(hdc, lpLogfont, lpProc, lParam, dwFlags);
    }

    int EnumFontFamiliesExW(HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags)
    {
        return HookStub.StubEnumFontFamiliesExW(hdc, lpLogfont, lpProc, lParam, dwFlags);
    }
};

ForceInline PLeGlobalData LeGetGlobalData()
{
    extern PLeGlobalData g_GlobalData;
    return g_GlobalData;
}

#endif // _LOCALEEMULATOR_H_cd444a0d_c7f9_44b2_aac8_8107e9a07ca2_

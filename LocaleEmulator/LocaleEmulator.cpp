#include "stdafx.h"

ML_OVERLOAD_NEW

#pragma comment(linker, "/ENTRY:DllMain")
#pragma comment(linker, "/SECTION:.text,ERW /MERGE:.rdata=.text /MERGE:.data=.text")
#pragma comment(linker, "/SECTION:.Asuna,ERW /MERGE:.text=.Asuna")
#pragma comment(linker, "/EXPORT:GetFileAttributesA=_FuckStupid2DJGame@4")

EXTC ULONG NTAPI FuckStupid2DJGame(PCSTR FileName)
{
    return FILE_ATTRIBUTE_NORMAL;
/*
    PVOID Rubbish;
    PLDR_MODULE Garbage;

    Garbage = FindLdrModuleByHandle(nullptr);
    Rubbish = _ReturnAddress();

    if (PtrOffset(Rubbish, Garbage->EntryPoint) < 100 && StrCompareA(FileName, "2djgame.txt") == 0)
        return FILE_ATTRIBUTE_NORMAL;

    return Io::QueryFileAttributes(ml::String::Decode(FileName, StrLengthA(FileName), CP_ACP));
*/
}

PLeGlobalData g_GlobalData;

ForceInline VOID LeSetGlobalData(PLeGlobalData GlobalData)
{
    g_GlobalData = GlobalData;
}

ForceInline VOID LeReleaseGlobalData()
{
    SafeDeleteT(g_GlobalData);
}

BOOL NTAPI DelayInitDllEntry(PVOID BaseAddress, ULONG Reason, PVOID Reserved)
{
    BOOL Success;
    PLDR_MODULE Module = FindLdrModuleByHandle(BaseAddress);
    PIMAGE_NT_HEADERS NtHeaders;

    NtHeaders = PtrAdd((PIMAGE_NT_HEADERS)BaseAddress, ((PIMAGE_DOS_HEADER)BaseAddress)->e_lfanew);
    Module->EntryPoint = PtrAdd(BaseAddress, NtHeaders->OptionalHeader.AddressOfEntryPoint);

    Success = ((API_POINTER(DelayInitDllEntry))Module->EntryPoint)(BaseAddress, Reason, Reserved);

    if (Reason == DLL_PROCESS_ATTACH && !Success)
        return Success;

    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
            if (!Success)
                return Success;
        case DLL_PROCESS_DETACH:
            break;

        default:
            return Success;
    }

    LeGetGlobalData()->HookModule(BaseAddress, &Module->BaseDllName, Reason == DLL_PROCESS_ATTACH);

    return Success;
}

NTSTATUS GetNlsFile(PUNICODE_STRING NlsFile, ULONG NlsIndex, PCWSTR SubKey)
{
    BOOL        Success;
    WCHAR       NlsIndexBuffer[16];
    NTSTATUS    Status;
    PKEY_VALUE_PARTIAL_INFORMATION FileName;

    swprintf(NlsIndexBuffer, L"%d", NlsIndex);

    Status = GetKeyValue(REGKEY_ROOT, SubKey, NlsIndexBuffer, &FileName);
    FAIL_RETURN(Status);

    Success = RtlCreateUnicodeString(NlsFile, (PWSTR)FileName->Data);
    FreeMemoryP(FileName);

    return Success ? STATUS_SUCCESS : STATUS_NO_MEMORY;
}

NTSTATUS GetLangFile(PUNICODE_STRING LangFile, ULONG LangIndex, PCWSTR SubKey)
{
    BOOL        Success;
    WCHAR       LangIndexBuffer[16];
    NTSTATUS    Status;
    PKEY_VALUE_PARTIAL_INFORMATION FileName;

    swprintf(LangIndexBuffer, L"%04x", LangIndex);

    Status = GetKeyValue(REGKEY_ROOT, SubKey, LangIndexBuffer, &FileName);
    FAIL_RETURN(Status);

    Success = RtlCreateUnicodeString(LangFile, (PWSTR)FileName->Data);
    FreeMemoryP(FileName);

    return Success ? STATUS_SUCCESS : STATUS_NO_MEMORY;
}

NTSTATUS ReadFileInSystemDirectory(NtFileMemory &File, PUNICODE_STRING Path)
{
    PWSTR       Buffer;
    ULONG_PTR   Length;
    NTSTATUS    Status;

    Length = sizeof(ROOTDIR_SYSTEM32) + Path->Length + sizeof(WCHAR);
    Buffer = (PWSTR)AllocateMemoryP(Length);
    if (Buffer == nullptr)
        return STATUS_NO_MEMORY;

    Length = CONST_STRLEN(ROOTDIR_SYSTEM32);
    CopyMemory(Buffer, ROOTDIR_SYSTEM32, Length * sizeof(WCHAR));
    CopyMemory(Buffer + Length, Path->Buffer, Path->Length);
    Buffer[Length + Path->Length / sizeof(WCHAR)] = 0;

    Status = File.Open(Buffer, NFD_NOT_RESOLVE_PATH);

    FreeMemoryP(Buffer);

    return Status;
}

NTSTATUS LeGlobalData::Initialize()
{
    BOOL            IsLoader;
    PLEPEB          LePeb;
    PLDR_MODULE     Ntdll;
    PPEB_BASE       Peb;
    NTSTATUS        Status;
    NLSTABLEINFO    NlsTableInfo;
    UNICODE_STRING  SystemDirectory, NlsFileName, OemNlsFileName, LangFileName, Win32U;
    PKEY_VALUE_PARTIAL_INFORMATION IndexValue;

    IsLoader = IsLeLoader();

    Wow64 = Ps::IsWow64Process();
    Ntdll = GetNtdllLdrModule();

    LOOP_ONCE
    {
        LePeb = OpenOrCreateLePeb();
        if (LePeb == nullptr)
        {
            ULONG_PTR       DefaultACPLength, DefaultLCIDLength, DefaultOEMCPLength;
            WCHAR           DefaultACP[0x20], DefaultOEMCP[0x20], DefaultLCID[0x20];
            PVOID           ReloadedNtdll;
            PUNICODE_STRING FullDllName;

            LePeb = GetLePeb();

            InitDefaultLeb(&LePeb->Leb);

            FullDllName = &FindLdrModuleByHandle(&__ImageBase)->FullDllName;
            CopyMemory(LePeb->LeDllFullPath, FullDllName->Buffer, FullDllName->Length + sizeof(WCHAR));

            Status = LoadPeImage(Ntdll->FullDllName.Buffer, &ReloadedNtdll, nullptr, LOAD_PE_IGNORE_RELOC);
            if (NT_SUCCESS(Status))
            {
                PVOID LdrLoadDllAddress;

                LdrLoadDllAddress = LookupExportTable(ReloadedNtdll, NTDLL_LdrLoadDll);
                LePeb->LdrLoadDllAddress = PtrAdd(LdrLoadDllAddress, PtrOffset(Ntdll->DllBase, ReloadedNtdll));
                CopyMemory(LePeb->LdrLoadDllBackup, LdrLoadDllAddress, LDR_LOAD_DLL_BACKUP_SIZE);
                LePeb->LdrLoadDllBackupSize = LDR_LOAD_DLL_BACKUP_SIZE;

                UnloadPeImage(ReloadedNtdll);
            }

            DefaultACPLength    = (swprintf(DefaultACP, L"%d", LePeb->Leb.AnsiCodePage) + 1) * sizeof(WCHAR);
            DefaultOEMCPLength  = (swprintf(DefaultOEMCP, L"%d", LePeb->Leb.OemCodePage) + 1) * sizeof(WCHAR);
            DefaultLCIDLength   = (swprintf(DefaultLCID, L"%d", LePeb->Leb.LocaleID) + 1) * sizeof(WCHAR);

            REGISTRY_REDIRECTION_ENTRY64 *Entry, Entries[] =
            {
                {
                    { (ULONG64)HKEY_LOCAL_MACHINE, USTR64(REGPATH_CODEPAGE), USTR64(REGKEY_ACP), REG_SZ, },
                    { (ULONG64)HKEY_LOCAL_MACHINE, USTR64(REGPATH_CODEPAGE), USTR64(REGKEY_ACP), REG_SZ, DefaultACP, DefaultACPLength },
                },
                {
                    { (ULONG64)HKEY_LOCAL_MACHINE, USTR64(REGPATH_CODEPAGE), USTR64(REGKEY_OEMCP), REG_SZ, },
                    { (ULONG64)HKEY_LOCAL_MACHINE, USTR64(REGPATH_CODEPAGE), USTR64(REGKEY_OEMCP), REG_SZ, DefaultOEMCP, DefaultOEMCPLength },
                },
                {
                    { (ULONG64)HKEY_LOCAL_MACHINE, USTR64(REGPATH_LANGUAGE), USTR64(REGKEY_DEFAULT_LANGUAGE), REG_SZ, },
                    { (ULONG64)HKEY_LOCAL_MACHINE, USTR64(REGPATH_LANGUAGE), USTR64(REGKEY_DEFAULT_LANGUAGE), REG_SZ, DefaultLCID, DefaultLCIDLength },
                },
            };

            Status = this->InitRegistryRedirection(Entries, countof(Entries), nullptr);
        }
        else
        {
            *GetLePeb() = *LePeb;
            Status = this->InitRegistryRedirection(LePeb->Leb.RegistryReplacement, LePeb->Leb.NumberOfRegistryRedirectionEntries, &LePeb->Leb);

            NtClose(LePeb->Section);
            CloseLePeb(LePeb);
        }

        if (IsLoader)
            break;

        Status = this->TextMetricCache.Initialize();
        FAIL_RETURN(Status);

        PVOID           NlsBaseAddress;
        LCID            DefaultLocaleID;
        LARGE_INTEGER   DefaultCasingTableSize;

        Status = NtInitializeNlsFiles(&NlsBaseAddress, &DefaultLocaleID, &DefaultCasingTableSize);
        FAIL_RETURN(Status);

        this->GetLePeb()->OriginalLocaleID = DefaultLocaleID;

        NtUnmapViewOfSection(CurrentProcess, NlsBaseAddress);

        WriteLog(L"init leb %s", GetLePeb()->LeDllFullPath);

        SystemDirectory = Ntdll->FullDllName;
        SystemDirectory.Length -= Ntdll->BaseDllName.Length;

        Status = RtlDuplicateUnicodeString(RTL_DUPSTR_ADD_NULL, &SystemDirectory, &this->SystemDirectory);
        FAIL_RETURN(Status);

        ml::String Win32U_Path(SystemDirectory);
        Win32U_Path += L"\\win32u.dll";
        this->HasWin32U = Nt_GetFileAttributes(Win32U_Path.GetBuffer()) != INVALID_FILE_ATTRIBUTES;
        WriteLog(L"win32u: %s, has:%d", Win32U_Path.GetBuffer(), this->HasWin32U);

        RtlInitEmptyString(&NlsFileName, nullptr, 0);
        RtlInitEmptyString(&OemNlsFileName, nullptr, 0);
        RtlInitEmptyString(&LangFileName, nullptr, 0);

        SCOPE_EXIT
        {
            RtlFreeUnicodeString(&NlsFileName);
            RtlFreeUnicodeString(&OemNlsFileName);
            RtlFreeUnicodeString(&LangFileName);
        }
        SCOPE_EXIT_END;

        Status = GetNlsFile(&NlsFileName, GetLeb()->AnsiCodePage, REGPATH_CODEPAGE);
        FAIL_RETURN(Status);

        Status = GetNlsFile(&OemNlsFileName, GetLeb()->OemCodePage, REGPATH_CODEPAGE);
        FAIL_RETURN(Status);

        Status = GetLangFile(&LangFileName, GetLeb()->LocaleID, REGPATH_LANGUAGE);
        FAIL_RETURN(Status);

        NtFileMemory AnsiFile, OemFile, LangFile;

        Status = ReadFileInSystemDirectory(AnsiFile, &NlsFileName);
        FAIL_RETURN(Status);

        Status = ReadFileInSystemDirectory(OemFile, &OemNlsFileName);
        FAIL_RETURN(Status);

        Status = ReadFileInSystemDirectory(LangFile, &LangFileName);
        FAIL_RETURN(Status);

        AnsiCodePageOffset      = 0;
        OemCodePageOffset       = ROUND_UP(AnsiFile.GetSize32(), 16);
        UnicodeCaseTableOffset  = OemCodePageOffset + ROUND_UP(OemFile.GetSize32(), 16);

        Status = AllocVirtualMemory(&CodePageMapView, UnicodeCaseTableOffset + LangFile.GetSize32(), PAGE_READWRITE, MEM_COMMIT | MEM_TOP_DOWN);
        FAIL_RETURN(Status);

        CopyMemory(PtrAdd(CodePageMapView, AnsiCodePageOffset),     AnsiFile.GetBuffer(),   AnsiFile.GetSize32());
        CopyMemory(PtrAdd(CodePageMapView, OemCodePageOffset),      OemFile.GetBuffer(),    OemFile.GetSize32());
        CopyMemory(PtrAdd(CodePageMapView, UnicodeCaseTableOffset), LangFile.GetBuffer(),   LangFile.GetSize32());

        ProtectVirtualMemory(CodePageMapView, UnicodeCaseTableOffset + LangFile.GetSize32(), PAGE_READONLY);

        RtlInitNlsTables(
            (PUSHORT)PtrAdd(CodePageMapView, AnsiCodePageOffset),
            (PUSHORT)PtrAdd(CodePageMapView, OemCodePageOffset),
            (PUSHORT)PtrAdd(CodePageMapView, UnicodeCaseTableOffset),
            &NlsTableInfo
        );

        RtlResetRtlTranslations(&NlsTableInfo);

        WriteLog(L"reset nls");

        Peb = CurrentPeb();

        Peb->AnsiCodePageData       = (PUSHORT)PtrAdd(CodePageMapView, AnsiCodePageOffset);
        Peb->OemCodePageData        = (PUSHORT)PtrAdd(CodePageMapView, OemCodePageOffset);
        Peb->UnicodeCaseTableData   = (PUSHORT)PtrAdd(CodePageMapView, UnicodeCaseTableOffset);

        // LdrInitShimEngineDynamic(&__ImageBase);

        LdrRegisterDllNotification(0,
            [] (ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData, PVOID Context)
            {
                return ((PLeGlobalData)Context)->DllNotification(NotificationReason, NotificationData);
            },
            this,
            &DllNotificationCookie
        );
    }

    Status = InstallHookPort();
    WriteLog(L"inst hp: %08X", Status);
    FAIL_RETURN(Status);

    HookNtdllRoutines(Ntdll->DllBase);

    WriteLog(L"hook ntdll");

    if (IsLoader)
        return Status;

    PLDR_MODULE Kernel32Ldr;

    Kernel32Ldr = GetKernel32Ldr();
    if (Kernel32Ldr != nullptr)
    {
        Kernel32Ldr->EntryPoint = DelayInitDllEntry;
        // HookKernel32Routines(Kernel32Ldr->DllBase);
    }

    WriteLog(L"init %p", Status);

    return Status;
}

NTSTATUS LeGlobalData::InitRegistryRedirection(PREGISTRY_REDIRECTION_ENTRY64 Entry64, ULONG_PTR Count, PVOID BaseAddress)
{
    NTSTATUS    Status;
    PLEPEB      LePeb;
    PREGISTRY_REDIRECTION_ENTRY Entry;

    if (Count == 0)
        return STATUS_NO_MORE_ENTRIES;

    LePeb = this->GetLePeb();

#pragma push_macro("USTR64ToUSTR")
#undef USTR64ToUSTR
#define USTR64ToUSTR(ustr64) UNICODE_STRING({ ustr64.Length, ustr64.MaximumLength, PtrAdd(ustr64.Buffer, BaseAddress) });

    REGISTRY_REDIRECTION_ENTRY LocalEntry;

    FOR_EACH(Entry64, Entry64, Count)
    {
        ULONG_PTR       LastIndex;
        HANDLE          OriginalKey, RedirectedKey;
        UNICODE_STRING  KeyFullPath;

        OriginalKey     = nullptr;
        RedirectedKey   = nullptr;

        Status = Reg::OpenKey(&OriginalKey, (HANDLE)Entry64->Original.Root, KEY_QUERY_VALUE, PtrAdd(Entry64->Original.SubKey.Buffer, BaseAddress));
        FAIL_CONTINUE(Status);

        if (Entry64->Redirected.Root != NULL)
        {
            Status = Reg::OpenKey(&RedirectedKey, (HANDLE)Entry64->Redirected.Root, KEY_QUERY_VALUE, PtrAdd(Entry64->Redirected.SubKey.Buffer, BaseAddress));
            if (NT_FAILED(Status))
            {
                Reg::CloseKeyHandle(OriginalKey);
                continue;
            }
        }

        this->RegistryRedirectionEntry.Add(LocalEntry);
        LastIndex = this->RegistryRedirectionEntry.GetSize() - 1;
        Entry = &this->RegistryRedirectionEntry[LastIndex];

        Status = QueryRegKeyFullPath(OriginalKey, &KeyFullPath);
        Reg::CloseKeyHandle(OriginalKey);
        if (NT_FAILED(Status))
        {
            Reg::CloseKeyHandle(RedirectedKey);
            this->RegistryRedirectionEntry.Remove(LastIndex);
            continue;
        }

        Entry->Original.FullPath = KeyFullPath;
        RtlFreeUnicodeString(&KeyFullPath);

        if (RedirectedKey != nullptr)
        {
            Status = QueryRegKeyFullPath(RedirectedKey, &KeyFullPath);
            Reg::CloseKeyHandle(RedirectedKey);
            if (NT_FAILED(Status))
            {
                this->RegistryRedirectionEntry.Remove(LastIndex);
                continue;
            }

            Entry->Redirected.FullPath = KeyFullPath;
            RtlFreeUnicodeString(&KeyFullPath);
        }

        Entry->Original.Root        = (HKEY)Entry64->Original.Root;
        Entry->Original.SubKey      = USTR64ToUSTR(Entry64->Original.SubKey);
        Entry->Original.ValueName   = USTR64ToUSTR(Entry64->Original.ValueName);
        Entry->Original.DataType    = Entry64->Original.DataType;
        Entry->Original.Data        = nullptr;
        Entry->Original.DataSize    = 0;

        Entry->Redirected.Root      = (HKEY)Entry64->Redirected.Root;
        Entry->Redirected.SubKey    = USTR64ToUSTR(Entry64->Redirected.SubKey);
        Entry->Redirected.ValueName = USTR64ToUSTR(Entry64->Redirected.ValueName);
        Entry->Redirected.DataType  = Entry64->Redirected.DataType;
        Entry->Redirected.Data      = nullptr;
        Entry->Redirected.DataSize  = 0;

        if (Entry64->Redirected.Data != nullptr && Entry64->Redirected.DataSize != 0)
        {
            Entry->Redirected.DataSize = (ULONG_PTR)Entry64->Redirected.DataSize;
            Entry->Redirected.Data = AllocateMemoryP(Entry->Redirected.DataSize);
            if (Entry->Redirected.Data == nullptr)
            {
                this->RegistryRedirectionEntry.Remove(LastIndex);
                continue;
            }

            CopyMemory(Entry->Redirected.Data, PtrAdd(Entry64->Redirected.Data, BaseAddress), Entry->Redirected.DataSize);
        }
    }

#pragma pop_macro("USTR64ToUSTR")

    return STATUS_SUCCESS;
}

typedef struct
{
    UNICODE_STRING DllName;
    TYPE_OF(&LeGlobalData::HookNtdllRoutines)     HookRoutine;
    TYPE_OF(&LeGlobalData::UnHookNtdllRoutines)   UnHookRoutine;

} DLL_HOOK_ENTRY, *PDLL_HOOK_ENTRY;

PDLL_HOOK_ENTRY LookupDllHookEntry(PCUNICODE_STRING BaseDllName)
{
    static DLL_HOOK_ENTRY DllHookEntries[] =
    {
        { RTL_CONSTANT_STRING(L"USER32.dll"),       &LeGlobalData::HookUser32Routines,      &LeGlobalData::UnHookUser32Routines },
        { RTL_CONSTANT_STRING(L"GDI32.dll"),        &LeGlobalData::HookGdi32Routines,       &LeGlobalData::UnHookGdi32Routines },
        { RTL_CONSTANT_STRING(L"KERNEL32.dll"),     &LeGlobalData::HookKernel32Routines,    &LeGlobalData::UnHookKernel32Routines },
    };

    PDLL_HOOK_ENTRY Entry;

    FOR_EACH_ARRAY(Entry, DllHookEntries)
    {
        if (!RtlEqualUnicodeString(BaseDllName, &Entry->DllName, TRUE))
            continue;

        return Entry;
    }

    return nullptr;
}

VOID LeGlobalData::HookModule(PVOID DllBase, PCUNICODE_STRING DllName, BOOL DllLoad)
{
    PDLL_HOOK_ENTRY Entry;

    Entry = LookupDllHookEntry(DllName);
    if (Entry == nullptr)
        return;

    NTSTATUS Status = DllLoad ? (this->*Entry->HookRoutine)(DllBase) : (this->*Entry->UnHookRoutine)();

    UNREFERENCED_PARAMETER(Status);
    WriteLog(L"hook %s: %p", Entry->DllName.Buffer, Status);
}

VOID LeGlobalData::DllNotification(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData)
{
    NTSTATUS            Status;
    PVOID               DllBase;
    ULONG_PTR           Length;
    PLDR_MODULE         Module;
    UNICODE_STRING      DllPath;
    PCUNICODE_STRING    DllName;

    switch (NotificationReason)
    {
        case LDR_DLL_NOTIFICATION_REASON_LOADED:
            DllName = NotificationData->Loaded.BaseDllName;
            DllBase = NotificationData->Loaded.DllBase;
            DllPath = *NotificationData->Loaded.FullDllName;
            DllPath.Length -= DllName->Length;
            break;

        case LDR_DLL_NOTIFICATION_REASON_UNLOADED:
            DllName = NotificationData->Unloaded.BaseDllName;
            DllBase = NotificationData->Unloaded.DllBase;
            DllPath = *NotificationData->Unloaded.FullDllName;
            DllPath.Length -= DllName->Length;
            break;

        default:
            return;
    }

    //if (!RtlEqualUnicodeString(&SystemDirectory, &DllPath, TRUE))
    //    return;

    if (LookupDllHookEntry(DllName) == nullptr)
        return;

    Module = FindLdrModuleByHandle(DllBase);
    if (!FLAG_ON(Module->Flags, LDRP_PROCESS_ATTACH_CALLED))
    {
        Module->EntryPoint = DelayInitDllEntry;
    }
    else
    {
        HookModule(DllBase, DllName, NotificationReason == LDR_DLL_NOTIFICATION_REASON_LOADED);
    }
}

NTSTATUS LeGlobalData::UnInitialize()
{
    if (DllNotificationCookie != nullptr)
    {
        LdrUnregisterDllNotification(DllNotificationCookie);
        DllNotificationCookie = nullptr;
    }

    UnHookGdi32Routines();
    UnHookUser32Routines();
    UnHookKernel32Routines();
    UnHookNtdllRoutines();

    UnInstallHookPort();

    RtlFreeUnicodeString(&SystemDirectory);

    return 0;
}

inline BOOL UnInitialize(PVOID BaseAddress)
{
    LeReleaseGlobalData();
    ml::MlUnInitialize();
    return FALSE;
}

VOID GenerateModuleList(ml::String &ModuleNames)
{
    PVOID                               Address, BaseAddress, LastAllocationBase;
    ULONG_PTR                           Size, Need, BufferSize;
    NTSTATUS                            Status;
    SYSTEM_BASIC_INFORMATION            SystemBasic;
    MEMORY_BASIC_INFORMATION            MemoryBasic;

    union
    {
        MEMORY_MAPPED_FILENAME_INFORMATION2 MappedFile;
        BYTE MappedFileBuffer[0x2000];
    };

    Status = NtQuerySystemInformation(SystemBasicInformation, &SystemBasic, sizeof(SystemBasic), nullptr);
    if (!NT_SUCCESS(Status))
        return;

    LastAllocationBase = IMAGE_INVALID_VA;
    BaseAddress = (PVOID)SystemBasic.MinimumUserModeAddress;
    BaseAddress = nullptr;

    for (; (ULONG_PTR)BaseAddress < SystemBasic.MaximumUserModeAddress; BaseAddress = PtrAdd(BaseAddress, MemoryBasic.RegionSize))
    {
        MemoryBasic.RegionSize = MEMORY_PAGE_SIZE;
        Status = NtQueryVirtualMemory(CurrentProcess, BaseAddress, MemoryBasicInformation, &MemoryBasic, sizeof(MemoryBasic), nullptr);
        FAIL_CONTINUE(Status);

        BaseAddress = MemoryBasic.BaseAddress;

        if (MemoryBasic.Type != MEM_IMAGE || MemoryBasic.AllocationBase == LastAllocationBase)
            continue;

        LastAllocationBase = MemoryBasic.AllocationBase;

        Status = NtQueryVirtualMemory(CurrentProcess, BaseAddress, MemoryMappedFilenameInformation, &MappedFile, sizeof(MappedFileBuffer), nullptr);
        if (NT_FAILED(Status) || MappedFile.Name.Length == 0)
            continue;

        UNICODE_STRING DosPath;

        Status = Io::QueryDosPathFromNtDeviceName(&DosPath, &MappedFile.Name);

        ModuleNames += ml::String::Format(L"%p: %wZ\n", BaseAddress, NT_SUCCESS(Status) ? &DosPath : &MappedFile.Name);

        RtlFreeUnicodeString(&DosPath);
    }
}

BOOL Initialize(PVOID BaseAddress)
{
    NTSTATUS            Status;
    PLDR_MODULE         Kernel32;
    PLeGlobalData       GlobalData;

    ml::MlInitialize();

    if (FindThreadFrame(LE_LOADER_PROCESS) == nullptr)
    {
        Kernel32 = GetKernel32Ldr();
        if (Kernel32 != nullptr && FLAG_ON(Kernel32->Flags, LDRP_PROCESS_ATTACH_CALLED))
        {
            ml::String ModuleList = L"kernel32 has been loaded before the initialization of LE\n\nModule list:\n\n";
            GenerateModuleList(ModuleList);
            ExceptionBox(ModuleList);
            return FALSE;
        }
    }

    LdrDisableThreadCalloutsForDll(BaseAddress);

    GlobalData = new LeGlobalData;
    if (GlobalData == nullptr)
        return FALSE;

    LeSetGlobalData(GlobalData);

    Status = GlobalData->Initialize();
    if (NT_FAILED(Status))
        return FALSE;

    WriteLog(L"init ret");

    return TRUE;
}

BOOL NTAPI DllMain(PVOID BaseAddress, ULONG Reason, PVOID Reserved)
{
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
            return Initialize(BaseAddress) || UnInitialize(BaseAddress);

        case DLL_PROCESS_DETACH:
            UnInitialize(BaseAddress);
            break;
    }

    return TRUE;
}

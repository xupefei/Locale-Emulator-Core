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
#if ARCHEAGE_VER

        { RTL_CONSTANT_STRING(L"x2game.dll"),       &LeGlobalData::HookX2GameRoutines },

#else

        { RTL_CONSTANT_STRING(L"USER32.dll"),       &LeGlobalData::HookUser32Routines,      &LeGlobalData::UnHookUser32Routines },
        { RTL_CONSTANT_STRING(L"GDI32.dll"),        &LeGlobalData::HookGdi32Routines,       &LeGlobalData::UnHookGdi32Routines },
        { RTL_CONSTANT_STRING(L"KERNEL32.dll"),     &LeGlobalData::HookKernel32Routines,    &LeGlobalData::UnHookKernel32Routines },

#endif // ARCHEAGE_VER
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

#if ARCHEAGE_VER

#pragma comment(linker, "/EXPORT:ShellExecuteA=SHELL32.ShellExecuteA")
#pragma comment(linker, "/EXPORT:ShellExecuteW=SHELL32.ShellExecuteW")

LONG WINAPI GetSQLiteKeyExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
    PDR7_INFO   Dr7;
    PBYTE       SQLiteKeyBuffer;

    switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_SINGLE_STEP:
            SQLiteKeyBuffer = (PBYTE)ExceptionInfo->ContextRecord->Dr3 - 0xC;

            Dr7 = (PDR7_INFO)&ExceptionInfo->ContextRecord->Dr7;
            Dr7->L3 = 0;
            ExceptionInfo->ContextRecord->Dr3 = 0;

            for (ULONG_PTR Count = 0x10; Count; --Count)
            {
                PrintConsoleW(L"0x%02X, ", *SQLiteKeyBuffer++);
                if (Count == 9)
                    PrintConsoleW(L"\n");
            }

            PrintConsoleW(L"\nPress any key to exit...");
            PauseConsole();
            Ps::ExitProcess(0);

            break;

        default:
            return EXCEPTION_CONTINUE_SEARCH;
    }

    // return EXCEPTION_CONTINUE_EXECUTION;
}

PSTR CDECL x2_strstr(PSTR Str, PCSTR SubStr)
{
    PrintConsole(L"%S -> %S\n", Str, SubStr);

    if (SubStr != nullptr && !StrCompareA(SubStr, "+acpxmk"))
    {
        return Str;
    }

    return strstr(Str, SubStr);
}

VOID (*StubX2_FillDecryptTable)();

VOID X2_FillDecryptTable()
{
    PBYTE       KeyBuffer;
    CONTEXT     Context;
    PDR7_INFO   Dr7;

    INLINE_ASM mov KeyBuffer, esi;

    KeyBuffer += 8 + 0xC;

    PrintConsole(L"return address %p\n", _ReturnAddress());

    Context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    NtGetContextThread(CurrentThread, &Context);

    Dr7 = (PDR7_INFO)&Context.Dr7;

    Dr7->L3     = 1;
    Dr7->LEN3   = DR7_LEN_4_BYTE;
    Dr7->RW3    = DR7_RW_WRITE;
    Context.Dr3 = (ULONG_PTR)KeyBuffer;

    NtSetContextThread(CurrentThread, &Context);

    RtlAddVectoredExceptionHandler(TRUE, GetSQLiteKeyExceptionHandler);

    StubX2_FillDecryptTable();
}

VOID FASTCALL PrintKey(PBYTE SQLiteKeyBuffer)
{
    NtFileDisk f;

    f.Create(L"dump.bin");
    f.Write(SQLiteKeyBuffer, 0x100);

    SQLiteKeyBuffer += 8;

    for (ULONG_PTR Count = 0x10; Count; --Count)
    {
        PrintConsoleW(L"0x%02X, ", *SQLiteKeyBuffer++);
        if (Count == 9)
            PrintConsoleW(L"\n");
    }

    PrintConsoleW(L"\nPress any key to exit...");
    PauseConsole();
    Ps::ExitProcess(0);
}

NAKED VOID GetKey()
{
    INLINE_ASM
    {
        mov     ecx, esi;
        call    PrintKey
    }
}

NTSTATUS LeGlobalData::HookX2GameRoutines(PVOID X2Game)
{
    BYTE StubFillDecryptTable[] =
    {
        0x55, 0x8b, 0xec, 0x81, 0xec, 0x08, 0x08, 0x00, 0x00, 0xa1,
    };

    BYTE StubSaveStackCookie[] =
    {
        0x33, 0xC5,
    };

    SEARCH_PATTERN_DATA Pattern[] =
    {
        ADD_PATTERN(StubFillDecryptTable, 0, sizeof(StubFillDecryptTable) + 4),
        ADD_PATTERN(StubSaveStackCookie),
    };

    ULONG_PTR           SearchLength;
    PVOID               StartAddress, FillDecryptTable, FillDecryptTable2;
    PLDR_MODULE         X2GameModule;
    PIMAGE_NT_HEADERS   NtHeaders;

    //MessageBoxW(0, L"??", 0, 0);

    X2GameModule = FindLdrModuleByHandle(X2Game);

    AllocConsole();

    LOOP_ONCE
    {
        NtHeaders   = ImageNtHeaders(X2GameModule->DllBase);
        StartAddress = PtrAdd(X2GameModule->DllBase, IMAGE_FIRST_SECTION(NtHeaders)->VirtualAddress);
        SearchLength = X2GameModule->SizeOfImage - PtrOffset(StartAddress, X2GameModule->DllBase);

        FillDecryptTable = SearchPatternSafe(Pattern, countof(Pattern), StartAddress, SearchLength);

        if (FillDecryptTable == nullptr)
        {
            PrintConsoleW(L"can't find FillDecryptTable\n");
            continue;
        }

        StartAddress = PtrAdd(FillDecryptTable, 1);
        SearchLength = X2GameModule->SizeOfImage - PtrOffset(StartAddress, X2GameModule->DllBase);

        FillDecryptTable2 = SearchPatternSafe(
                                Pattern,
                                countof(Pattern),
                                StartAddress,
                                SearchLength
                            );

        if (FillDecryptTable2 != nullptr)
        {
            PrintConsoleW(L"found multi FillDecryptTable\n");
            break;
        }

        PVOID MSVCR100 = FindLdrModuleByName(&WCS2US(L"MSVCR100.dll"))->DllBase;
        PVOID STRSTR = GetRoutineAddress(MSVCR100, "strstr");

        Mp::PATCH_MEMORY_DATA f[] =
        {
            //INLINE_HOOK_JUMP_NULL(STRSTR, x2_strstr),
            Mp::FunctionJumpVa(FillDecryptTable, X2_FillDecryptTable, StubX2_FillDecryptTable),
        };

        Mp::PatchMemory(f, countof(f), MSVCR100);

        return STATUS_SUCCESS;
    }

    PrintConsoleW(L"Press any key to exit...");
    PauseConsole();
    Ps::ExitProcess(0);
}

#endif // ARCHEAGE_VER

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

#if !ARCHEAGE_VER

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

#endif

    LdrDisableThreadCalloutsForDll(BaseAddress);

    GlobalData = new LeGlobalData;
    if (GlobalData == nullptr)
        return FALSE;

    LeSetGlobalData(GlobalData);

#if ARCHEAGE_VER


/*++

.text:394FF6CB C7 45 EC 04 0D 02 0F                    mov     [ebp+var_14], 0F020D04h
.text:394FF6D2 C7 45 F0 00 05 08 09                    mov     [ebp+var_10], 9080500h
.text:394FF6D9 C7 45 F4 07 0A 0E 06                    mov     [ebp+var_C], 60E0A07h
.text:394FF6E0 C7 45 F8 01 0C 0B 03                    mov     [ebp+var_8], 30B0C01h
.text:394FF6E7 C7 45 D4 02 0E 03 0F                    mov     [ebp+var_2C], 0F030E02h
.text:394FF6EE C7 45 D8 15 0B 0C 06                    mov     [ebp+var_28], 60C0B15h
.text:394FF6F5 C7 45 DC 05 08 04 11                    mov     [ebp+var_24], 11040805h
.text:394FF6FC C7 45 E0 07 0D 17 01                    mov     [ebp+var_20], 1170D07h
.text:394FF703 C7 45 E4 16 13 12 0A                    mov     [ebp+var_1C], 0A121316h
.text:394FF70A C7 45 E8 09 00 14 10                    mov     [ebp+var_18], 10140009h
.text:394FF711 C7 45 B4 11 0B 04 10                    mov     [ebp+var_4C], 10040B11h
.text:394FF718 C7 45 B8 18 09 0F 06                    mov     [ebp+var_48], 60F0918h
.text:394FF71F C7 45 BC 00 1F 12 05                    mov     [ebp+var_44], 5121F00h
.text:394FF726 C7 45 C0 1C 08 15 19                    mov     [ebp+var_40], 1915081Ch
.text:394FF72D C7 45 C4 17 1D 0A 13                    mov     [ebp+var_3C], 130A1D17h
.text:394FF734 C7 45 C8 1E 0E 16 1B                    mov     [ebp+var_38], 1B160E1Eh
.text:394FF73B C7 45 CC 0D 1A 0C 02                    mov     [ebp+var_34], 20C1A0Dh
.text:394FF742 C7 45 D0 14 01 03 07                    mov     [ebp+var_30], 7030114h
.text:394FF749 75 0F                                   jnz     short loc_394FF75A
.text:394FF74B E8 00 FD FF FF                          call    X2_FillDecryptTable
.text:394FF750 C7 05 A8 48 A4 39 01 00+                mov     dword_39A448A8, 1
.text:394FF75A
.text:394FF75A                         loc_394FF75A:                           ; CODE XREF: sub_394FF690+B9j

------------------------------------------------------------------------------------------------------------------

.text:394FF7FA 8B 36                                   mov     esi, [esi]           <---- fuck here

------------------------------------------------------------------------------------------------------------------


.text:394FF7FC 83 EE 0A                                sub     esi, 0Ah
.text:394FF7FF 0F 84 C5 02 00 00                       jz      loc_394FFACA
.text:394FF805 83 EE 02                                sub     esi, 2
.text:394FF808 0F 84 E2 00 00 00                       jz      loc_394FF8F0
.text:394FF80E 83 EE 02                                sub     esi, 2
.text:394FF811 0F 85 98 04 00 00                       jnz     loc_394FFCAF
.text:394FF817 BF 80 26 A4 39                          mov     edi, offset unk_39A42680
.text:394FF81C 8D 64 24 00                             lea     esp, [esp+0]

++*/

    AllocConsole();

    PLDR_MODULE X2Game = FindLdrModuleByName(&USTR(L"x2game.dll"));
    PVOID _getkey;

    BYTE stub[] =
    {
        0x8B, 0x36, 0x83, 0xEE, 0x0A, 0x0F, 0x84, 0xC5, 0x02, 0x00, 0x00, 0x83, 0xEE, 0x02, 0x0F, 0x84, 0xE2, 0x00, 0x00, 0x00, 0x83, 0xEE, 0x02, 0x0F, 0x85, 0x98, 0x04, 0x00, 0x00,
    };

    SEARCH_PATTERN_DATA Pattern[] =
    {
        ADD_PATTERN(stub),
    };

    _getkey = SearchPattern(Pattern, countof(Pattern), X2Game->DllBase, X2Game->SizeOfImage);

    PrintConsole(L"%p\n", _getkey);

    Mp::PATCH_MEMORY_DATA f[] =
    {
        //INLINE_HOOK_JUMP_RVA_NULL(0x4FF7FA, GetKey),
        //Mp::FunctionJumpRva(0x51C8BA, GetKey),
        Mp::FunctionJumpVa(_getkey, GetKey),
    };

    Mp::PatchMemory(f, countof(f), X2Game->DllBase);

    //GlobalData->HookX2GameRoutines(FindLdrModuleByName(&USTR(L"x2game.dll"))->DllBase);

#else

    Status = GlobalData->Initialize();
    if (NT_FAILED(Status))
        return FALSE;

#endif

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

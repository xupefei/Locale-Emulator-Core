#include "stdafx.h"

using namespace Mp;

//
// LocaleName
// sCountry
// sList
// sDecimal
// sThousand
//

LCID NTAPI LeGetUserDefaultLCID()
{
    return LeGetGlobalData()->GetLeb()->LocaleID;
}

NTSTATUS LeGlobalData::HackUserDefaultLCID(PVOID Kernel32)
{
    LCID        Lcid;
    PVOID       gNlsProcessLocalCache;
    PLDR_MODULE Kernel;
    API_POINTER(GetUserDefaultLCID) GetUserDefaultLCID;

    *(PVOID *)&GetUserDefaultLCID = LookupExportTable(Kernel32, KERNEL32_GetUserDefaultLCID);
    Lcid = GetUserDefaultLCID();

    Kernel = FindLdrModuleByName(&USTR(L"KERNELBASE.dll"));
    if (Kernel == nullptr)
        Kernel = FindLdrModuleByHandle(Kernel32);

    gNlsProcessLocalCache = nullptr;

    WalkRelocTableT(Kernel->DllBase,
        WalkRelocCallbackM(ImageBase, RelocationEntry, Offset, Context)
        {
            SEH_TRY
            {
                PULONG_PTR Memory = *(PULONG_PTR *)PtrAdd(ImageBase, RelocationEntry->VirtualAddress + Offset->Offset);
                if (*(PLCID)Memory[2] == Lcid)
                {
                    gNlsProcessLocalCache = Memory;
                    return STATUS_UNSUCCESSFUL;
                }
            }
            SEH_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
            {
            }
            return STATUS_SUCCESS;
        }
    );

    //if (gNlsProcessLocalCache != nullptr)
        //*(PLCID)(((PULONG_PTR)gNlsProcessLocalCache)[2]) = GetLeb()->LocaleID;

    return STATUS_SUCCESS;
}

PVOID (FASTCALL *StubGetNamedLocaleHashNode)(PWSTR LocaleName, LANGID Lcid);

PVOID FASTCALL LeGetNamedLocaleHashNode(PWSTR LocaleName, LANGID LangId)
{
    PLDR_MODULE Kernel;
    API_POINTER(LCIDToLocaleName) LCIDToLocaleName;
    PLeGlobalData GlobalData = LeGetGlobalData();

    Kernel = FindLdrModuleByName(&USTR(L"KERNELBASE.dll"));
    if (Kernel == nullptr)
        Kernel = GetKernel32Ldr();

    if (!IN_RANGE((ULONG_PTR)Kernel->DllBase, (ULONG_PTR)LocaleName, (ULONG_PTR)PtrAdd(Kernel->DllBase, Kernel->SizeOfImage)))
    {
        return StubGetNamedLocaleHashNode(LocaleName, LangId);
    }

    *(PVOID *)&LCIDToLocaleName = LookupExportTable(Kernel->DllBase, KERNEL32_LCIDToLocaleName);

    LocaleName[-1] = (USHORT)(LCIDToLocaleName(GlobalData->GetLeb()->LocaleID, LocaleName, 0xAC, 0) - 1);

    return StubGetNamedLocaleHashNode(LocaleName, LangId);
}

NTSTATUS LeGlobalData::HackUserDefaultLCID2(PVOID Kernel32)
{
    PVOID GetNLSVersionEx, GetNamedLocaleHashNode;
    PLDR_MODULE Kernel;
    API_POINTER(GetUserDefaultLCID) GetUserDefaultLCID;

    Kernel = FindLdrModuleByName(&USTR(L"KERNELBASE.dll"));
    if (Kernel == nullptr)
        Kernel = FindLdrModuleByHandle(Kernel32);

    *(PVOID *)&GetNLSVersionEx = LookupExportTable(Kernel->DllBase, KERNEL32_GetNLSVersionEx);

    GetNamedLocaleHashNode = nullptr;

    WalkOpCodeT(GetNLSVersionEx, 0x20,
        WalkOpCodeM(Buffer, OpLength, Ret)
        {
            if (Buffer[0] != CALL)
                return STATUS_NOT_FOUND;

            *(PVOID *)&GetNamedLocaleHashNode = GetCallDestination(Buffer);
            return STATUS_SUCCESS;
        }
    );

    if (GetNamedLocaleHashNode == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    PATCH_MEMORY_DATA p[] =
    {
        FunctionJumpVa(GetNamedLocaleHashNode, LeGetNamedLocaleHashNode, &StubGetNamedLocaleHashNode),
    };

    PatchMemory(p, countof(p));

    *(PVOID *)&GetUserDefaultLCID = LookupExportTable(Kernel32, KERNEL32_GetUserDefaultLCID);
    GetUserDefaultLCID();

    RestoreMemory(StubGetNamedLocaleHashNode);

    return STATUS_SUCCESS;
}

void* GetFirstCallTarget(void* start_offset, DWORD parse_range, void **next) {
    void* res = nullptr;
    WalkOpCodeT(start_offset, parse_range,
        WalkOpCodeM(Buffer, OpLength, Ret)
    {
        if (Buffer[0] != CALL) {
            return STATUS_NOT_FOUND;
        }

        res = GetCallDestination(Buffer);
        *next = &Buffer[OpLength];
        return STATUS_SUCCESS;
    }
    );
    return res;
}

void* GetKthCallTarget(void* start_offset, DWORD parse_range_each, int K) {
    if (start_offset == nullptr)
        return nullptr;
    void *next, *res;
    while (K >= 1) {
        res = GetFirstCallTarget(start_offset, parse_range_each, &next);
        if (res == nullptr)
            return nullptr;
        start_offset = next;
        --K;
    }
    return res;
}

typedef DWORD(__stdcall* pSetupAnsiOemCodeHashNodes)();

NTSTATUS LeSetupAnsiOemCodeHashNodes() {
    PLDR_MODULE Kernel = FindLdrModuleByName(&USTR(L"KERNELBASE.dll"));
    if (Kernel == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    void* pKernelBaseDllInitialize = GetKthCallTarget(Kernel->EntryPoint, 0x30, 1);
    if (pKernelBaseDllInitialize == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    void* pKernelBaseBaseDllInitialize = GetKthCallTarget(pKernelBaseDllInitialize, 0x20, 2);
    if (pKernelBaseBaseDllInitialize == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    //AllocConsole();
    //PrintConsoleW(L"pKernelBaseDllInitialize = %p\npKernelBaseBaseDllInitialize: %p\n", pKernelBaseDllInitialize, pKernelBaseBaseDllInitialize);

    void* pBaseNlsDllInitialize = nullptr;
    void* _;
    WalkOpCodeT(pKernelBaseBaseDllInitialize, 0x400,
        WalkOpCodeM(Buffer, OpLength, Ret)
    {
        if (Buffer[0] != 0xB8 || *((DWORD*)&Buffer[1]) != 0x190) {
            // locate first `mov eax, 0x190`
            return STATUS_NOT_FOUND;
        }
        _ = Buffer;
        pBaseNlsDllInitialize = GetKthCallTarget(_, 0x20, 1);
        return STATUS_SUCCESS;
    }
    );
    if (pBaseNlsDllInitialize == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    void* pNlsProcessInitialize = GetKthCallTarget(pBaseNlsDllInitialize, 0x30, 1);
    if (pNlsProcessInitialize == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    auto the_func = (pSetupAnsiOemCodeHashNodes)GetKthCallTarget(pNlsProcessInitialize, 0x30, 3);
    if (the_func == nullptr)
        return STATUS_PROCEDURE_NOT_FOUND;

    //AllocConsole();
    //PrintConsoleW(L"SetupAnsiOemCodeHashNodes: %p\n", the_func);

    the_func();
}

NTSTATUS LeGlobalData::HackAnsiOemCodeHashNodes() {
    PLeGlobalData GlobalData = LeGetGlobalData();

    unsigned char* pTeb = (unsigned char*)(__readfsdword(48));
    *(short*)(pTeb + 0x228) = GlobalData->GetLeb()->AnsiCodePage;
    *(short*)(pTeb + 0x22a) = GlobalData->GetLeb()->OemCodePage;

    return LeSetupAnsiOemCodeHashNodes();
}

NTSTATUS LeGlobalData::HookKernel32Routines(PVOID Kernel32)
{
    PVOID GetCurrentNlsCache;
    NTSTATUS Status;

    Status = this->HackUserDefaultLCID2(Kernel32);

    Status = this->HackAnsiOemCodeHashNodes();

    //AllocConsole();
    //PrintConsoleW(L"hook k32: %p", Status);

    return Status;
}

NTSTATUS LeGlobalData::UnHookKernel32Routines()
{
    return 0;
}

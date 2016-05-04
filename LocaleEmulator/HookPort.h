#ifndef _HOOKPORT_H_19ab81ea_402e_4f99_8d93_44baae1cc16b_
#define _HOOKPORT_H_19ab81ea_402e_4f99_8d93_44baae1cc16b_

#include "MyLibrary.h"
#include "HandleTable.h"

_ML_C_HEAD_

#pragma pack(1)

/*++

NTSTATUS
HPCALL
NtCloseFilter(
    HPARGS
    HANDLE Handle
)
{
    if (xxx)
        *Action = SYSTEM_CALL_BLOCK;
    else if (yyy)
        *Action = SYSTEM_CALL_CONTINUE;
    else if (zzz)
        *Action = SYSTEM_CALL_PERMIT;

    return STATUS_SUCCESS;      // return value will be ignored when *Action == ContinueSystemCall
}

--*/

#define HP_MAX_SERVICE_INDEX           0x300
#define HP_MAX_WIN32K_SERVICE_INDEX    0x500

#define HP_MAX_SERVICE_TABLE_COUNT      2
#define HP_NTKRNL_SERVICE_INDEX         0
#define HP_WIN32K_SERVICE_INDEX         1

#define HP_TABLE_INDEX(_ServiceIndex) (((_ServiceIndex) >> 0xC) & 0xF)
#define HP_SERVICE_INDEX(_ServiceIndex) ((_ServiceIndex) & 0xFFF)

#define SYSCALL_SKIP_MAGIC          TAG4('SFSI')
#define HP_INVALID_OFFSET           ULONG_PTR_MAX
#define ALL_SERVICE_INDEX           ULONG_PTR_MAX
#define MAX_FILTER_NUMBER           bitsof(((PSYSCALL_INFO)0)->FilterBitmap)

#define IS_WIN32K_SERVICE(_ServiceIndex) (((_ServiceIndex) & 0x1000) == 0x1000)


#define HOOKPORT_CALLTYPE   FASTCALL
#define HPCALL              HOOKPORT_CALLTYPE


#define HPARG_FLTINFO  FltInfo
#define HPARG_SYSCALL  SysCallInfo

#define HPARG_INVOKE    HPARG_SYSCALL, HPARG_FLTINFO

#define HPARGSN     PSYSCALL_INFO HPARG_SYSCALL, PSYSCALL_FILTER_INFO HPARG_FLTINFO
#define HPARGS      HPARGSN,

#define HpSetFilterAction(_action) HPARG_FLTINFO->Action = _action
#define HpGetFilterContext() HPARG_FLTINFO->FilterContext

typedef struct
{
    USHORT  ServiceIndex        : 12;
    USHORT  ServiceTableIndex   : 4;
    USHORT  ArgumentSize;
    ULONG   Hash;

} HP_SYSTEM_SERVICE_ENTRY, *PHP_SYSTEM_SERVICE_ENTRY;

typedef struct
{
    ULONG_PTR                   Number;
    PHP_SYSTEM_SERVICE_ENTRY    Entries;

} HP_SYSTEM_SERVICE_TABLE, *PHP_SYSTEM_SERVICE_TABLE;

typedef enum
{
    ContinueSystemCall,               // call next filter if exists, default, and must be zero
    BlockSystemCall,                  // Block the system call and return
    PermitSystemCall,                 // Permit the system call
    GlobalFilterHandled,
    GlobalFilterModified,

} SystemCallFilterAction;

typedef struct SYSCALL_FILTER_INFO
{
    SystemCallFilterAction  Action;
    PVOID                   FilterContext;

    union
    {
        ULONG_PTR           Wow64Ecx;
        ULONG_PTR           ArgumentSize;
        PVOID               SsdtRoutine;
    };

    SYSCALL_FILTER_INFO()
    {
        Action = ContinueSystemCall;
    }

} SYSCALL_FILTER_INFO, *PSYSCALL_FILTER_INFO;

enum
{
    SystemCallFilterEnable      = 0x00000001,
    SystemCallFilterUnsupport   = 0x00000002,
};

enum
{
    HookPortStatusFirst = DEFINE_NTSTATUS(STATUS_SEVERITY_ERROR, 0x100),

    STATUS_HOOK_PORT_NOT_INITIALIZED,
    STATUS_HOOK_PORT_UNSUPPORTED_SYSTEM,
    STATUS_HOOK_PORT_ENTRY_NOT_FOUND,
    STATUS_HOOK_PORT_FILTER_NOT_FOUND,
    STATUS_HOOK_PORT_TOO_MANY_FILTERS,
    STATUS_HOOK_PORT_SSDT_SHADOW_NOT_FOUND,

    HookPortStatusLast,
};

typedef struct
{
    PVOID Callback;
    PVOID Context;
} SYSTEM_CALL_FILTER, *PSYSTEM_CALL_FILTER;

typedef struct
{
    USHORT              Flags;
    USHORT              ArgumentSize;

    union
    {
        struct
        {
            USHORT ServiceIndex : 12;
            USHORT ServiceTableIndex : 4;
            USHORT ServiceFlags;
        };

        ULONG ServiceData;
    };

    PVOID               ReturnOpAddress;
    ULONG               FilterBitmap;
    PSYSTEM_CALL_FILTER FilterCallbacks;
    PVOID               FunctionName;
    PVOID               FunctionAddress;
    ULONG               NameHash;

} SYSCALL_INFO, *PSYSCALL_INFO;

typedef struct
{
    ULONG           Hash;
    PSYSCALL_INFO   Entry;

} SYSTEM_CALL_HASH, *PSYSTEM_CALL_HASH;

typedef struct
{
    ULONG_PTR           Count;
    PSYSTEM_CALL_HASH   Entry;

} SYSTEM_CALL_HASH_TABLE, *PSYSTEM_CALL_HASH_TABLE;

struct SYSCALL_FILTER_SKIP_INFO: public TEB_ACTIVE_FRAME
{
    ULONG ServiceIndex;
};

#pragma pack()

typedef NTSTATUS (HPCALL *HpFilterStub)(PSYSCALL_INFO SysCallInfo, PSYSCALL_FILTER_INFO Action);

typedef
NTSTATUS
(HPCALL
 *HookPortGlobalFilterRoutine)(
    HPARGS
    ULONG_PTR               ServiceIndex,
    PVOID*                  ServiceRoutine,
    PULONG_PTR              Arguments,
    PVOID*                  AddressOfReturnAddress
);

#define HpGlobalFilterM(HPARG_INVOKE, ServiceIndex, ServiceRoutine, Arguments) [] (HPARGS ULONG_PTR ServiceIndex, PVOID *ServiceRoutine, PULONG_PTR Arguments) -> NTSTATUS

typedef HookPortGlobalFilterRoutine HpGlobalFilterRoutine;

typedef struct
{
    union
    {
        ULONG ServiceData;
        struct
        {
            ULONG ServiceIndex : 12;
            ULONG TableIndex : 4;
        };
    };

    PVOID           OriginalRoutine;
    PSYSCALL_INFO   SystemCallInfo;
    PVOID*          AddressOfReturnAddress;

} HOOK_PORT_DISPATCHER_INFO, *PHOOK_PORT_DISPATCHER_INFO;

#if ML_AMD64

#define DISPATCHER_INFO_SIZE    0x20

#else

#define DISPATCHER_INFO_SIZE    0x10

#endif // x64

typedef struct
{
    HpGlobalFilterRoutine   Routine;
    PVOID                   Context;

} HP_GLOBAL_FILTER, *PHP_GLOBAL_FILTER;

typedef struct
{
    LONG_PTR                    RefCount;
    SYSTEM_CALL_HASH_TABLE      HashTable;
    HP_GLOBAL_FILTER            GlobalFilter;
    PSYSCALL_INFO               SystemCallInfo[HP_MAX_SERVICE_TABLE_COUNT];
    ULONG_PTR                   SystemCallCount[HP_MAX_SERVICE_TABLE_COUNT];
    ULONG_PTR                   MaxSystemCallCount[HP_MAX_SERVICE_TABLE_COUNT];

    union
    {
        struct
        {
            ULONG   SysEnterSize;
            PVOID   KiFastSystemCall;
            PVOID   Wow64SyscallJumpStub;
            BYTE    Wow64FsC0Backup[0x10];
            BOOLEAN IsWow64 : 1;

        } User;

#if ML_KERNEL_MODE

        struct
        {
            struct
            {
                BOOLEAN SSDTShadowSearched  : 1;
            };

            PLDR_MODULE                     NtKrnlModule;
            PVOID                           ReloadedNtOsKrnlBase;

            PVOID                           HookPortAddress;
            PVOID                           ShadowHookPortReturnAddress;

            PKSERVICE_TABLE_DESCRIPTOR      ServiceDescriptorTable;
            PKSERVICE_TABLE_DESCRIPTOR      ServiceDescriptorTableShadow[2];
            PKSERVICE_TABLE_DESCRIPTOR      ShadowServiceDescriptorTable;

            ULONG_PTR                       ThreadExitStatusOffset;
            ULONG_PTR                       SsdtRoutineOffset;

            struct
            {
                FixedMemoryBlock<HOOK_PORT_DISPATCHER_INFO> Objects;
                ERESOURCE               Lock;
                MlHandleTable*          Table;

            } DispatcherInfo;

        } Kernel;

#endif
    };

} HOOK_PORT_GLOBAL_INFO, *PHOOK_PORT_GLOBAL_INFO;

NTSTATUS
InstallHookPort(
    PLDR_MODULE SysCallModule = NULL,
    ULONG       Flags = 0
);

NTSTATUS
HpInitializeWin32kPort(
    VOID
);

NTSTATUS
HpAddSystemServiceTable(
    PHP_SYSTEM_SERVICE_TABLE ServiceTable
);

PVOID
HpDuplicateHookPort(
    PVOID NtBase
);

NTSTATUS
UnInstallHookPort(
    VOID
);

NTSTATUS
HpAddSystemCallByRoutine(
    PVOID Routine,
    ULONG RoutineHash
);

NTSTATUS
HpAddSystemCallByRoutineRange(
    PVOID*      Routine,
    PULONG      RoutineHash,
    ULONG_PTR   Count
);

NTSTATUS
HpAddSystemCall(
    PSYSCALL_INFO   SystemCall,
    ULONG_PTR       Count = 1
);

NTSTATUS
HpAddSystemCallFilter(
    ULONG RoutineHash,
    PVOID Routine,
    PVOID Context = NULL
);

NTSTATUS
HpRemoveSystemCallFilter(
    ULONG RoutineHash,
    PVOID Routine
);

PSYSCALL_INFO
HpLookupSystemCall(
    ULONG_PTR SystemCallHash
);

NTSTATUS
HpSetGlobalFilter(
    PHP_GLOBAL_FILTER  NewFilter,
    PHP_GLOBAL_FILTER  OldFilter = NULL
);

enum HpValueType
{
    HpvNtKrnlModule,
    HpvShadowNtKrnl,
    HpvShadowSSDT,
    HpvHookPortAddress,
    HpvShadowHookPortReturnAddress,
};

NTSTATUS
HpQueryValue(
    HpValueType Type,
    PVOID*      Value
);

_ML_C_TAIL_

#if CPP_DEFINED

class HookPortBypassFilter
{
public:
    HookPortBypassFilter(ULONG ServiceIndex)
    {
        m_SkipInfo.ServiceIndex = ServiceIndex;
        m_SkipInfo.Context      = SYSCALL_SKIP_MAGIC;

        m_SkipInfo.Push();
    }

    HookPortBypassFilter(PSYSCALL_INFO SystemCall)
    {
        m_SkipInfo.ServiceIndex = SystemCall->ServiceData;
        m_SkipInfo.Context      = SYSCALL_SKIP_MAGIC;

        m_SkipInfo.Push();
    }

    ~HookPortBypassFilter()
    {
        m_SkipInfo.Pop();
    }

protected:
    SYSCALL_FILTER_SKIP_INFO m_SkipInfo;
};

template<class FunctionPointer>
struct HookPortAutoBypassFilter : public HookPortBypassFilter
{
    HookPortAutoBypassFilter(ULONG ServiceIndex) : HookPortBypassFilter(ServiceIndex)
    {
    }

    HookPortAutoBypassFilter(PSYSCALL_INFO SystemCall) : HookPortBypassFilter(SystemCall)
    {
    }

    template<class PointerType>
    ForceInline
    PointerType
    operator()(
        PointerType Pointer
    )
    {
        return Pointer;
    }
};

#endif // CPP_DEFINED

#if SUPPORT_VA_ARGS_MACRO

#if ML_KERNEL_MODE

#define CallSysCall(_Routine, _OriginalRoutine, ...) \
            CallFuncPtr( \
                _Routine, \
                _OriginalRoutine, \
                __VA_ARGS__ \
            )

#define HpCallSysCall(_Routine, ...) CallFuncPtr(_Routine, HPARG_FLTINFO->SsdtRoutine, __VA_ARGS__)

#else // r3

#define CallSysCall(_Routine, _SysCallInfo, ...) \
            CallFuncPtr( \
                _Routine, \
                HookPortAutoBypassFilter<PVOID>(_SysCallInfo)(_SysCallInfo->FunctionAddress), \
                __VA_ARGS__ \
            )

#define HpCallSysCall(_Routine, ...) \
            CallFuncPtr( \
                _Routine, \
                HookPortAutoBypassFilter<PVOID>(HPARG_SYSCALL)(HPARG_SYSCALL->FunctionAddress), \
                __VA_ARGS__ \
            )

#endif // rx

#define ADD_FILTER_(routine, HookRoutine, ...) \
            HpAddSystemCallFilter( \
                NTDLL_##routine, \
                HookRoutine, \
                __VA_ARGS__ \
            )

#define ADD_W32_FILTER_(routine, HookRoutine, ...) \
            HpAddSystemCallFilter( \
                WIN32K_##routine, \
                HookRoutine, \
                __VA_ARGS__ \
            )

#define DEL_FILTER_(routine, HookRoutine, ...) \
            HpRemoveSystemCallFilter( \
                NTDLL_##routine, \
                HookRoutine \
            )

#define DEL_W32_FILTER_(routine, HookRoutine, ...) \
            HpRemoveSystemCallFilter( \
                WIN32K_##routine, \
                HookRoutine \
            )

#define ADD_FILTER(routine, ...) ADD_FILTER_(routine, Hook##routine, __VA_ARGS__)
#define ADD_W32_FILTER(routine, ...) ADD_FILTER_(routine, Hook##routine, __VA_ARGS__)

#define DEL_FILTER(routine) DEL_FILTER_(routine, Hook##routine)
#define DEL_W32_FILTER(routine) DEL_W32_FILTER_(routine, Hook##routine)

#endif // SUPPORT_VA_ARGS_MACRO

#endif // _HOOKPORT_H_19ab81ea_402e_4f99_8d93_44baae1cc16b_

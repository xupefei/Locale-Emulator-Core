#ifndef _STDAFX_H_09d541c2_cb84_4ae2_9d98_febf90394945_
#define _STDAFX_H_09d541c2_cb84_4ae2_9d98_febf90394945_

#include "ml.h"
#include <CommCtrl.h>
#include "LocaleEmulator.h"
#include "Hooks\NtdllHook.h"
#include "Hooks\User32Hook.h"
#include "Hooks\Gdi32Hook.h"
#include "Utility\Utility.h"
#include "SectionProtector.h"

#include "HookPort.h"

#define CALL 0xE8
#define JUMP 0xE9
#define PUSH 0x68

#endif // _STDAFX_H_09d541c2_cb84_4ae2_9d98_febf90394945_

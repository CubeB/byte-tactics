// Decompiled by Haiku, Opus, Sonnet, DeepSeek V4.1 Flash and deepseek-v4.1. Names are provisional.
// The debug library's allocator and page protection: the memfussy, gonzo and
// setmemory command-line switches, the debug fill pattern, and the game's
// malloc wrappers around them.
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

extern char DAT_005289b4;
extern char DAT_005289b8;

// FUNCTION: 0x4d80b0
void SetMemFussyDefault(void)
{
    if (DAT_005289b4 == '\0') {
        DAT_005289b8 = 1;
    }
}

// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings (see 0x4df160 for the "fpufussy" twin).
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

// The original calls this out of line from GetBlockSize, the allocator and
// the page protection.
#pragma auto_inline(off)
// FUNCTION: 0x4d80d0
char IsMemFussy()
{
    DAT_005289b4 = 1;
    static Class_004d9fe0 memFussy("memfussy", 1, DAT_005289b8, "-memfussy", "-memnofussy", "-memfrontalign", 0);
    return memFussy.on;
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8130
void FUN_004d8130(void)
{
}

// The "gonzo" command-line switch, held in a function-local static
// (see 0x4d80d0 for the "memfussy" twin).
// The original calls this out of line from ProtectPages.
#pragma auto_inline(off)
// FUNCTION: 0x4d8140
char IsGonzo()
{
    static Class_004d9fe0 gonzo("gonzo", 1, 1, 0, "-gonzo", 0, 0);
    return gonzo.on;
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8190
void FUN_004d8190(void)
{
}

char FUN_004d8200();
int GetMemSetValue();

// Checks whether a value lies within `range` of the debug fill pattern, read
// at each of the four byte alignments.
// FUNCTION: 0x4d81a0
bool __cdecl IsNearFillPattern(int value, int range)
{
    if (FUN_004d8200() && GetMemSetValue()) {
        int pattern[2];
        pattern[0] = pattern[1] = GetMemSetValue();
        for (unsigned int i = 0; i < 4; i++) {
            int v = *(int*)((char*)pattern + i);
            if (v - range <= value && value <= v + range) {
                return true;
            }
        }
        return false;
    }
    return false;
}

// The "setmemory" command-line switch, held in a function-local static.
// The original calls this out of line from IsNearFillPattern and GameAlloc.
#pragma auto_inline(off)
// FUNCTION: 0x4d8200
char FUN_004d8200()
{
    static Class_004d9fe0 memSet("setmemory", 1, 0, "-memset", "-memnoset", 0, 0);
    return memSet.on;
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8250
void FUN_004d8250(void)
{
}

// The "-memset" fill value, a hex or decimal command-line setting that
// defaults to 0xdeadbeef (see 0x4d8200 for the "setmemory" switch).
class Class_004da040 {
public:
    int value;                         // +0x0
    Class_004da040(char* name, int a, int def, char* sw);
    ~Class_004da040() {}
};

// The original calls this out of line from IsNearFillPattern and GameAlloc.
#pragma auto_inline(off)
// FUNCTION: 0x4d8260
int GetMemSetValue()
{
    static Class_004da040 setValue("setvalue", 0xdeadbeef, 0xdeadbeef, "-memset");
    return setValue.value;
}
#pragma auto_inline(on)

// FUNCTION: 0x4d82b0
void FUN_004d82b0(void)
{
}

// Fills n bytes at p with a repeating 4-byte pattern (a debug-heap fill; its
// checking counterpart follows at 0x4d8310).
// The original calls this out of line from GameAlloc.
#pragma auto_inline(off)
// FUNCTION: 0x4d82c0
void __cdecl FillPattern(void* p, unsigned int pattern, unsigned int n)
{
    unsigned int* d = (unsigned int*)p;
    while (n >= 4) {
        *d++ = pattern;
        n -= 4;
    }
    if (n > 0)
        memcpy(d, &pattern, n);
}
#pragma auto_inline(on)

// CheckFillPattern (0x4d8310) is gap code and stays in
// src/debug/debug_lib_4d8310.cpp: tools/gapcheck.py sizes a region's functions
// by the next annotation in the file, so a file that also holds functions
// outside the region cannot be its source.

unsigned int __cdecl LookupBlockSize(void* p);

// The original calls this out of line from GameRealloc, GameFree and
// ProtectBlock.
#pragma auto_inline(off)
// FUNCTION: 0x4d8360
size_t __cdecl GetBlockSize(void* p)
{
    if (p == 0)
        return 0;

    if (IsMemFussy())
        return LookupBlockSize(p);

    return _msize(p);
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8390
void FUN_004d8390(void)
{
}

// FUNCTION: 0x4d83a0
void __cdecl FUN_004d83a0(int)
{
}

void* __cdecl GameAlloc(unsigned int size);

// FUNCTION: 0x4d83b0
void __cdecl FUN_004d83b0(unsigned int param_1, unsigned int param_2)
{
    GameAlloc(param_2);
}

CRITICAL_SECTION* FUN_004da780();
void* __cdecl AllocDebugBlock(unsigned int size, int flags);
void __cdecl CountAlloc(unsigned int size);
extern void (*DAT_005289bc)();

// The original calls this out of line from FUN_004d83b0, FUN_004d8450,
// GameCalloc, GameStrdup and FUN_004d8660.
#pragma auto_inline(off)
// FUNCTION: 0x4d83c0
void* __cdecl GameAlloc(unsigned int size)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* p;
    do {
        if (IsMemFussy()) {
            p = AllocDebugBlock(size, 0);
        } else {
            p = malloc(size);
            if (p != 0)
                CountAlloc(size);
        }
        if (p == 0 && DAT_005289bc != 0)
            DAT_005289bc();
    } while (p == 0 && DAT_005289bc != 0);
    if (p != 0) {
        if (FUN_004d8200())
            FillPattern(p, GetMemSetValue(), size);
    }
    LeaveCriticalSection(cs);
    return p;
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8450
void __cdecl FUN_004d8450(unsigned int param_1)
{
    GameAlloc(param_1);
}

// calloc-style allocation: count * size bytes from GameAlloc, zeroed.
// FUNCTION: 0x4d8460
void* __cdecl GameCalloc(unsigned int count, unsigned int size)
{
    unsigned int total = size * count;
    void* p = GameAlloc(total);
    if (p != 0) {
        memset(p, 0, total);
    }
    return p;
}

void* __cdecl GameRealloc(void* param_1, unsigned int param_3);

// The original calls this out of line from FUN_004d8580.
#pragma auto_inline(off)
// FUNCTION: 0x4d84a0
void __cdecl FUN_004d84a0(void* param_1, int unused_param_2, unsigned int param_3)
{
    GameRealloc(param_1, param_3);
}
#pragma auto_inline(on)

void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags);
void __cdecl CountFree(int param_1);

// The original calls this out of line from FUN_004d84a0.
#pragma auto_inline(off)
// FUNCTION: 0x4d84c0
void* __cdecl GameRealloc(void* param_1, unsigned int param_2)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* p;
    do {
        if (IsMemFussy()) {
            p = ReallocDebugBlock(param_1, param_2, 0);
        } else {
            size_t old = GetBlockSize(param_1);
            if (param_1 == 0 && param_2 == 0)
                p = 0;
            else
                p = realloc(param_1, param_2);
            if (p != 0 || param_2 == 0) {
                if (param_1 != 0)
                    CountFree(old);
                if (param_2 != 0)
                    CountAlloc(param_2);
            }
        }
        if (p == 0 && param_2 != 0 && DAT_005289bc != 0)
            DAT_005289bc();
    } while (p == 0 && param_2 != 0 && DAT_005289bc != 0);
    LeaveCriticalSection(cs);
    return p;
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8580
void __cdecl FUN_004d8580(int* param_1, int param_2)
{
    FUN_004d84a0(param_1, 0, param_2);
}

void __cdecl GameFree(void* p);

// The original calls this out of line from FUN_004d8670.
#pragma auto_inline(off)
// FUNCTION: 0x4d85a0
void __cdecl FUN_004d85a0(int* param_1)
{
    GameFree(param_1);
}
#pragma auto_inline(on)

void __cdecl FreeDebugBlock(void* p, int flags);

// The game's free(): under the allocator lock, either hands the block to the
// "memfussy" debug heap or updates the allocation counters and frees it.
// The original calls this out of line from FUN_004d85a0.
#pragma auto_inline(off)
// FUNCTION: 0x4d85b0
void __cdecl GameFree(void* p)
{
    if (p) {
        CRITICAL_SECTION* cs = FUN_004da780();
        EnterCriticalSection(cs);
        if (IsMemFussy()) {
            FreeDebugBlock(p, 0);
        } else {
            CountFree(GetBlockSize(p));
            free(p);
        }
        LeaveCriticalSection(cs);
    }
}
#pragma auto_inline(on)

// strdup() through the game's allocator.
// FUNCTION: 0x4d8610
char* __cdecl GameStrdup(char* s)
{
    if (!s)
        return 0;
    char* p = (char*)GameAlloc(strlen(s) + 1);
    if (p)
        strcpy(p, s);
    return p;
}

// FUNCTION: 0x4d8660
void __cdecl FUN_004d8660(unsigned int param_1)
{
    GameAlloc(param_1);
}

// FUNCTION: 0x4d8670
void __cdecl FUN_004d8670(int* param_1)
{
    FUN_004d85a0(param_1);
}

// FUNCTION: 0x4d8680
char __cdecl FUN_004d8680(unsigned long)
{
    return 0;
}

void __cdecl ProtectPages(int addr, int size, int protect);

// FUNCTION: 0x4d8690
void __cdecl ProtectPagesReadOnly(int param_1, int param_2)
{
    ProtectPages(param_1, param_2, 2);
}

// Changes the protection of the whole pages inside [addr, addr + size),
// only when IsGonzo (the "gonzo" switch) is on.
// The original calls this out of line from the page protection wrappers.
#pragma auto_inline(off)
// FUNCTION: 0x4d86b0
void __cdecl ProtectPages(int addr, int size, int protect)
{
    DWORD old;
    if (IsGonzo()) {
        unsigned int start = (addr + 0xfff) & 0xfffff000;
        unsigned int end = (addr + size) & 0xfffff000;
        if (start < end)
            VirtualProtect((void*)start, end - start, protect, &old);
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4d86f0
void __cdecl ProtectPagesReadWrite(int param_1, int param_2)
{
    ProtectPages(param_1, param_2, 4);
}

void __cdecl ProtectBlock(void* p, int protect);

// FUNCTION: 0x4d8710
void __cdecl ProtectBlockReadOnly(void* param_1)
{
    ProtectBlock(param_1, 2);
}

void __cdecl RoundRangeToPages(unsigned int* param_1, unsigned int* param_2);

// Changes the protection of a heap block, widened to whole pages when the
// "memfussy" switch (IsMemFussy) is on.
// The original calls this out of line from ProtectBlockReadOnly and
// ProtectBlockReadWrite.
#pragma auto_inline(off)
// FUNCTION: 0x4d8720
void __cdecl ProtectBlock(void* p, int protect)
{
    unsigned int start = (unsigned int)p;
    size_t size = GetBlockSize(p);
    unsigned int end = start + size;
    if (IsMemFussy())
        RoundRangeToPages(&start, &end);
    ProtectPages(start, end - start, protect);
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8780
void __cdecl ProtectBlockReadWrite(void* param_1)
{
    ProtectBlock(param_1, 4);
}

// Memory block description, 0x30 bytes, passed by value to FormatBlockInfo.
class BlockInfo {
public:
    void* address;                     // +0x00
    long size;                         // +0x04
    int allocNumber;                   // +0x08
    char name[0x24];                   // +0x0c

    BlockInfo(void);
};

// FUNCTION: 0x4d8790
void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused)
{
    if (info.name[0] != 0) {
        sprintf(buf, "\tBlock info: %ld bytes at %08lX - Alloc #%d - '%s'",
                info.size, info.address, info.allocNumber, info.name);
        return;
    }
    sprintf(buf, "\tBlock info: %ld bytes at %08lX - Alloc #%d",
            info.size, info.address, info.allocNumber);
}

struct Class_004d8850 {
    char unknown_0[0xc];
    char field_c[0x20];

    void FUN_004d8850(char* param_1);
};

// FUNCTION: 0x4d87f0
BlockInfo::BlockInfo(void)
{
    address = 0;
    size = 0;
    allocNumber = -1;
    ((Class_004d8850*)this)->FUN_004d8850(0);
}

class Class_004d8820 {
public:
    int field_0;
    int field_4;
    int field_8;
    char unknown_c[0x2c - 0xc];
    int field_2c;

    Class_004d8820(int p1, int p2, int p3, int p4, const char* p5);
};

// FUNCTION: 0x4d8820
Class_004d8820::Class_004d8820(int p1, int p2, int p3, int p4, const char* p5)
{
    field_0 = p1;
    field_4 = p2;
    field_8 = p3;
    ((Class_004d8850*)this)->FUN_004d8850((char*)p5);
    field_2c = p4;
}

// The original calls this out of line from the BlockInfo and Class_004d8820
// constructors.
#pragma auto_inline(off)
// FUNCTION: 0x4d8850
void Class_004d8850::FUN_004d8850(char* param_1)
{
    if (param_1 != 0) {
        lstrcpynA(field_c, param_1, 0x20);
    } else {
        field_c[0] = 0;
    }
}
#pragma auto_inline(on)

// The TraceRecord constructors (0x4d8870 and 0x4d88d0) are gap code and stay
// in src/debug/debug_lib_4d8870.cpp, for the reason above.

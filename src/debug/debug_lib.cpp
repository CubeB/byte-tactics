// Decompiled by Haiku, Opus, Sonnet, DeepSeek V4.1 Flash, deepseek-v4.1, Claude Opus 5.5, claude-opus-5-5, Claude Sonnet 5.5, claude-sonnet-5-5, Sonnet 5.5, deepseek-v4.1-flash, space-bunny-free, Space Bunny Free, muse-spark-1.3-free, mimo-v2.6-pro, GPT-6 and GPT-6.1-sol. Names are provisional.
//
// The debug library (0x4d80b0 to 0x4e42a0) in address order: the
// memfussy, gonzo and setmemory command-line switches, the game's
// allocator and page protection, the debug fill pattern, the
// allocator statistics, the free-block and file-record red-black
// trees, the ErrorLog.txt crash reports with their stack traces and
// symbol lookup (imagehlp, psapi), the unhandled exception filter and
// debug thread, the memory status dialog, the performance status
// dialog and its GDPERF counters, the Cavedog registry key and the
// name table.
//
// The files the gathers left in their own places stay there: 0x4d8310,
// 0x4d8870, 0x4d8d70, 0x4d9ab0, 0x4da120, 0x4da2c0, 0x4e16b0 and
// 0x4e1e50 (with 0x4e20a0) are gap regions; 0x4dacf0, 0x4db610,
// 0x4db7d0, 0x4dfd10, 0x4dfd50, 0x4e1990, 0x4e21f0, 0x4e2580 and
// 0x4e2620 each match only in their own file (see the notes where the
// rest of their part's functions sit).
//
// The join left eight more functions in files of their own, because in
// this file's symbol context their register allocation lands differently
// (docs/c2-regalloc.md): 0x4da3f0 (debug_lib_4da3f0.cpp), 0x4db1c0
// (free_block_map.cpp), 0x4de180 (debug_lib_4de180.cpp), 0x4df280
// (debug_lib_4df280.cpp), 0x4e05f0 (debug_lib_4e05f0.cpp), 0x4e0b90
// (memory_status_dialog.cpp), 0x4e1be0 (debug_lib_4e1be0.cpp) and
// 0x4e3750 (debug_lib_4e3750.cpp).
#define NOMINMAX
// The debug library's allocator and page protection: the memfussy, gonzo and
// setmemory command-line switches, the debug fill pattern, and the game's
// malloc wrappers around them.
// Renames the allocator's _Charalloc so the call carries the symbol name FUN_004e2b60.
#define _Charalloc FUN_004e2b60
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

unsigned int __cdecl LookupBlockSize(unsigned int key);

// The original calls this out of line from GameRealloc, GameFree and
// ProtectBlock.
#pragma auto_inline(off)
// FUNCTION: 0x4d8360
size_t __cdecl GetBlockSize(void* p)
{
    if (p == 0)
        return 0;

    if (IsMemFussy())
        return LookupBlockSize((unsigned int)p);

    return _msize(p);
}
#pragma auto_inline(on)

// The original calls this out of line from AbortProgram.
#pragma auto_inline(off)
// FUNCTION: 0x4d8390
void FUN_004d8390(void)
{
}
#pragma auto_inline(on)

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

class CritSec_004da780;
CritSec_004da780* FUN_004da780();
void* __cdecl AllocDebugBlock(unsigned int size, int flags);
void __cdecl CountAlloc(unsigned int size);
extern void (*DAT_005289bc)();

// The original calls this out of line from FUN_004d83b0, FUN_004d8450,
// GameCalloc, GameStrdup and FUN_004d8660.
#pragma auto_inline(off)
// FUNCTION: 0x4d83c0
void* __cdecl GameAlloc(unsigned int size)
{
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)FUN_004da780();
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
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)FUN_004da780();
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
        CRITICAL_SECTION* cs = (CRITICAL_SECTION*)FUN_004da780();
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

// The original calls this through a cast function pointer from ReportException.
#pragma auto_inline(off)
// FUNCTION: 0x4d8680
char __cdecl FUN_004d8680(unsigned long)
{
    return 0;
}
#pragma auto_inline(on)

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

// The original calls this out of line from the block histories and the block descriptions.
#pragma auto_inline(off)
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
#pragma auto_inline(on)

struct Class_004d8850 {
    char unknown_0[0xc];
    char field_c[0x20];

    void FUN_004d8850(char* param_1);
};

// The original calls this out of line from the block histories and the block descriptions.
#pragma auto_inline(off)
// FUNCTION: 0x4d87f0
BlockInfo::BlockInfo(void)
{
    address = 0;
    size = 0;
    allocNumber = -1;
    ((Class_004d8850*)this)->FUN_004d8850(0);
}
#pragma auto_inline(on)

class Class_004d8820 {
public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    Class_004d8820(int p1, int p2, int p3, int p4, const char* p5);
};
// The original calls this out of line from the block-map lookups and the tree inserts.
#pragma auto_inline(off)
// FUNCTION: 0x4d8820
Class_004d8820::Class_004d8820(int p1, int p2, int p3, int p4, const char* p5)
{
    base = p1;
    size = p2;
    count = p3;
    ((Class_004d8850*)this)->FUN_004d8850((char*)p5);
    tag = p4;
}
#pragma auto_inline(on)

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
// The debug library's crash reporting and command-line settings: the
// ErrorLog.txt reports and stack traces, the fatal error handler, the
// unhandled exception filter, the debug thread, DebugHelper.dll, the dialog
// templates, the allocator statistics and the file-record map singleton.
// The gap functions (0x4d8d70, 0x4d9ab0, 0x4da120, 0x4da2c0) stay in their
// own files.
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <yvals.h>
#include <memory>
// Nothing here uses <io.h>: its symbols put NormalizeLineEndings in the
// symbol-id window it matches in (docs/c2-regalloc.md).
#include <io.h>

struct Stack_004d89b0 {
    unsigned long addrs[14];    // +0x00
    int count;                  // +0x38
};

// The same record as TraceRecord, under the name its formatter has.
class CallSite {
public:
    char name[0x40];            // +0x00
    int field_40;               // +0x40
    Stack_004d89b0 stack;       // +0x44
    void FormatCallSite(char* out, int size);
};

void __cdecl GetSourceFilePath(char* out, char* name);
void __cdecl FormatCallStack(char* dest, int space, int per, int n, unsigned long* addrs);

// Builds a "name(line) : <call stack>" debug string.
// FUNCTION: 0x4d89b0
void CallSite::FormatCallSite(char* out, int size)
{
    char path[1000];

    out[0] = 0;
    strcpy(out, "\n");
    if (name[0]) {
        GetSourceFilePath(path, name);
        char* p = out + strlen(out);
        sprintf(p, "%s(%d)", path, field_40);
        strcat(out, " : ");
        strcat(out, "\n");
    }
    Stack_004d89b0& s = stack;
    // q then space stay locals, in this order: inline expressions change the count load.
    char* q = out + strlen(out);
    int space = size - strlen(out);
    FormatCallStack(q, space, 14, s.count, s.addrs);
    int len = strlen(out);
    if (len != 0 && out[len - 1] == '\n')
        out[len - 1] = 0;
}

// The fields are called min and max.

struct Base_004d8ae0 {
    char unknown_0[0x8c];
};

class RunningStats : public Base_004d8ae0 {
public:
    int count;                         // +0x8c
    unsigned int min;                  // +0x90
    unsigned int max;                  // +0x94
    char name[0x20];                   // +0x98
    unsigned __int64 total;            // +0xb8

    RunningStats(const Base_004d8ae0& src, const char* name);
    void FUN_004d8b30(const char* param_1);
    void FUN_004d8b60(unsigned int value);
};

// FUNCTION: 0x4d8ae0
RunningStats::RunningStats(const Base_004d8ae0& src, const char* name)
    : Base_004d8ae0(src), count(0), min(0), max(0), total(0)
{
    FUN_004d8b30(name);
}

// The original calls this from the constructor rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4d8b30
void RunningStats::FUN_004d8b30(const char* param_1)
{
    if (param_1 != 0) {
        lstrcpynA(name, param_1, 0x20);
    } else {
        name[0] = 0;
    }
}
#pragma auto_inline(on)

// Adds a sample to running statistics: count, minimum, maximum and a 64-bit
// total.
// FUNCTION: 0x4d8b60
void RunningStats::FUN_004d8b60(unsigned int value)
{
    if (count == 0) {
        min = max = value;
    } else {
        if (value < min)
            min = value;
        if (value > max)
            max = value;
    }
    total += value;
    count++;
}

// One allocation or free site, 0x8c bytes.
class TraceRecord {
public:
    char data[0x8c];
    TraceRecord(void);
};

extern void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused);

class BlockHistory : public BlockInfo {
public:
    TraceRecord allocSite;           // +0x30
    TraceRecord freeSite;            // +0xbc
    char field_148;                  // +0x148

    BlockHistory(void);
    void FormatBlockHistory(char* buf, int size, char freed);
    void FUN_004d8d40(char freed);
};

// FUNCTION: 0x4d8bd0
BlockHistory::BlockHistory(void) :
    BlockInfo(),
    allocSite(),
    freeSite()
{
    field_148 = 0;
}

// The original calls this from 0x4d8d40 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4d8c50
void BlockHistory::FormatBlockHistory(char* buf, int size, char freed)
{
    FormatBlockInfo(*this, buf, size);
    strcat(buf, ", allocated from:");
    char* p = buf + strlen(buf);
    ((CallSite*)&allocSite)->FormatCallSite(p, size - strlen(buf));
    if (freed) {
        strcat(buf, "\n\tfreed from:");
        char* q = buf + strlen(buf);
        ((CallSite*)&freeSite)->FormatCallSite(q, size - strlen(buf));
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8d40
void BlockHistory::FUN_004d8d40(char freed)
{
    char buf[2000];
    FormatBlockHistory(buf, 2000, freed);
}

class Class_004d88d0 {
public:
    char unknown_0[0x8c];

    Class_004d88d0(const char* name, int id, int count);
};

class Class_004d8c00 {
public:
    BlockInfo header;                  // +0x0
    Class_004d88d0 field_30;           // +0x30
    TraceRecord field_bc;              // +0xbc
    char field_148;                    // +0x148

    Class_004d8c00(const BlockInfo& h, const char* name, int id, int count);
};

// A constructor: three member objects are constructed, then the first one is
// overwritten with a copy of the header passed in and a flag is cleared.
// FUNCTION: 0x4d8c00
Class_004d8c00::Class_004d8c00(const BlockInfo& h, const char* name, int id, int count)
    : field_30(name, id, count + 1)
{
    header = h;
    field_148 = 0;
}

// IsOutsideStack (0x4d8d70) stays in src/debug/debug_lib_4d8d70.cpp: it is the
// source of a gap region and uses inline assembly. It defines the thread's
// stack variables, read by the two accessors below.

void __cdecl IsOutsideStack(int, int);

// This thread's stack pointer at the last check, which IsOutsideStack (whose
// object, src/debug/debug_lib_4d8d70.cpp, defines the thread's variables) has just
// stored.
extern __declspec(thread) char* g_stackLow;

// The original calls this out of line from FormatSystemInfo.
#pragma auto_inline(off)
// FUNCTION: 0x4d8df0
int __cdecl GetStackLow(void)
{
    IsOutsideStack(0, 0);
    return (int)g_stackLow;
}
#pragma auto_inline(on)

// fs:[0x2c] is the thread-local storage array pointer; the value read back
// after the IsOutsideStack(0, 0) call is the end of this thread's stack, which
// IsOutsideStack looks up the first time (src/debug/debug_lib_4d8d70.cpp defines the
// thread's variables).

extern __declspec(thread) char* g_stackHigh;

// The original calls this out of line from FormatSystemInfo.
#pragma auto_inline(off)
// FUNCTION: 0x4d8e20
void* GetStackHigh()
{
    IsOutsideStack(0, 0);
    return g_stackHigh;
}
#pragma auto_inline(on)

// The out-of-memory handler, seen as a function pointer here and at 0x4da8d0.
extern void (*DAT_005289bc)();

// FUNCTION: 0x4d8e50
int __cdecl SetOutOfMemoryHandler(int param_1)
{
    int temp = (int)DAT_005289bc;
    DAT_005289bc = (void (*)())param_1;
    return temp;
}

// Suspected original bugs: the `i % 3 == 3` test in the parameters loop can never
// be true, so the per-three newline is dead code; CreateFileA's result is tested
// against 0 rather than INVALID_HANDLE_VALUE, so a failed open passes the check;
// and the lstrcpynA dump buffer at obj+0x2084 is copied with
// room = 0x7358 - strlen(log) - 0x3e8 regardless of its own length.
// The EAX register-dump format string really reads "EFLGS" in the exe.

class StackTrace {
public:
    char pad0[0x78];
    int field_78;                      // +0x78
    int field_7c;                      // +0x7c
    char pad1[0x207c - 0x80];
    int field_207c;                    // +0x207c
    int* field_2080;                   // +0x2080
    char dump_text[0xa44c];            // +0x2084

    void CaptureStack(int* param_1, int* param_2, int param_3, int param_4);
};

// The same stack trace object under the name its formatter has.
class Class_004d9ca0 {
public:
    unsigned long ret[0x1e];           // +0x00
    int count;                         // +0x78
    int stack[0x800];                  // +0x7c
    int copied;                        // +0x207c
    unsigned long* pc;                 // +0x2080
    char buf[0xa44c];                  // +0x2084

    void FormatStackReport();
};

extern int DAT_005289c0;

char __cdecl FUN_004d8680();
char* __cdecl GetExceptionName(unsigned long code);
void __cdecl NormalizeLineEndings(char* buf, int size);
void __cdecl FormatSystemInfo(char* dst, int size);
void UnloadImageHelp();

// The game's structured-exception reporter: builds a text dump in a 0x7358-byte
// stack buffer and appends it to ErrorLog.txt next to the executable.
// FUNCTION: 0x4d8e60
int __cdecl ReportException(EXCEPTION_POINTERS* ep, char* handlerName)
{
    HANDLE file;
    char* dot, * base, name[1000], path[1000], log[0x7358], exe[1000];
    StackTrace obj;
    DWORD written;
    char* reason;

    if (DAT_005289c0)
        return 0;
    DAT_005289c0 = 1;
    CONTEXT* ctx = ep->ContextRecord;
    EXCEPTION_RECORD* rec = ep->ExceptionRecord;
    obj.CaptureStack((int*)ctx->Ebp, (int*)ctx->Esp, ctx->Eip, 0);
    ((Class_004d9ca0*)&obj)->FormatStackReport();

    // Each sprintf site keeps its destination form (`d`, `L` or inline strlen): it sets push order.
    // find the executable's directory and open the log there
    char* slash;
    if (0 == GetModuleFileNameA(0, path, 1000) || !(((slash = strrchr(path, '\\')) != 0) != 0)) { strcpy(path, "C:\\"); } else { slash[1] = 0; }
    strcat(path, "ErrorLog.txt");
    file = CreateFileA(path, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file) { SetFilePointer(file, 0, 0, FILE_END); } else {
    }
    log[0] = 0;
    reason = GetExceptionName(rec->ExceptionCode);

    // the "what crashed" header, written out early
    if (((0 < GetModuleFileNameA(0, exe, 1000)) != 0)) {
        base = strrchr(exe, '\\');
        base = base != 0 ? 1 + base : exe;
        strcpy(name, base);
        dot = strrchr(name, '.');
        if (((int)(0 != dot)) != 0) { *dot = 0; }
        sprintf(log + strlen(log), "%s caused an %s in\n", name, reason);
        { char* d = log + strlen(log); sprintf(d, "module %s at %04x:%08lx.\n", base, ctx->SegCs, ctx->Eip); }
        if (file) {
            WriteFile(file, log, strlen(log), &written, 0);
        }
        // Stays inside this block.
        log[0] = 0;
    }

    // the handler name, the module walk and the code/data address the fault
    // happened at
    { size_t L = strlen(log); sprintf(log + L, "Exception handler called in %s. ", handlerName); }
    { char* d = log + strlen(log); FormatSystemInfo(d, 0x7358 - strlen(log)); }
    { char* d = log + strlen(log); sprintf(d, "Instruction pointer is %08lX\n", ctx->Eip); }
    { char* d = log + strlen(log); sprintf(d, "ExceptionCode = %08lX", rec->ExceptionCode); }
    sprintf(log + strlen(log), " - %s\n", reason);
    if (0xc0000005 == rec->ExceptionCode) {
        if (rec->NumberParameters >= 2) {
            if (0 != ((char(__cdecl*)(unsigned long))FUN_004d8680)(rec->ExceptionInformation[1])) {
                char* d = log + strlen(log);
                sprintf(d, "Error: Write to read only memory attempted\n");
            }
            const char* rw = 0 != rec->ExceptionInformation[0] ? "write" : "read";
            char* d = log + strlen(log);
            sprintf(d, "Access violation: Illegal %s, data address 0x%08lX\n",
                    rw, rec->ExceptionInformation[1]);
        }
    }

    // the exception record, then the general registers
    { size_t L = strlen(log); sprintf(log + L, "ExceptionFlags = %08lX\t", rec->ExceptionFlags); }
    { char* d = log + strlen(log); sprintf(d, "ExceptionAddress = %08lX\n", rec->ExceptionAddress); }
    if (rec->NumberParameters > 0) {
        { size_t L = strlen(log); sprintf(log + L, "Parameters = "); }
        // Index ExceptionInformation[i] directly; `c` is its own local before `d`.
        for (unsigned int i = 0; i < rec->NumberParameters; i++) {
            char c = ((((int)i) == rec->NumberParameters - 1) || i % 3 == 3) ? '\n' : '\t';
            char* d = log + strlen(log);
            sprintf(d, "%08lX%c", rec->ExceptionInformation[i], c);
        }
    }
    { size_t L = strlen(log); sprintf(log + L, "\n"); }
    { size_t L = strlen(log); sprintf(log + L, "Registers:\n"); }
    { char* d = log + strlen(log);
    sprintf(d, "EAX=%08lX CS=%04lX EIP=%08lX EFLGS=%08lX\n",
            ctx->Eax, ctx->SegCs, ctx->Eip, ctx->EFlags); }
    { char* d = log + strlen(log);
    sprintf(d, "EBX=%08lX SS=%04lX ESP=%08lX EBP=%08lX\n",
            ctx->Ebx, ctx->SegSs, ctx->Esp, ctx->Ebp); }
    { char* d = log + strlen(log);
    sprintf(d, "ECX=%08lX DS=%04lX ESI=%08lX FS=%08lX\n",
            ctx->Ecx, ctx->SegDs, ctx->Esi, ctx->SegFs); }
    { char* d = log + strlen(log);
    sprintf(d, "EDX=%08lX ES=%04lX EDI=%08lX GS=%08lX\n",
            ctx->Edx, ctx->SegEs, ctx->Edi, ctx->SegGs); }

    // the 16 bytes of code at the faulting address
    { size_t L = strlen(log); sprintf(log + L, "\n"); }
    { size_t L = strlen(log); sprintf(L + log, "Bytes at CS:EIP:\n"); }
    unsigned char* code = *((unsigned char**)&ctx->Eip);
    int i = 0;
    if (i < 0x10) {
        do {
            char* d = log + strlen(log);
            sprintf(d, "%02x%c", code[i], i == 0xf ? '\n' : ' ');
            i = 1 + i;
        } while (0x10 > i);
    }
    { size_t L = strlen(log); sprintf(L + log, "\n"); }

    // the disassembler's own dump, the debug registers and the saved FPU state
    int room = 0x7358 - (int)strlen(log);
    room = room - 0x3e8;
    // The whole tail of the dump stays inside this if.
    if (room > 0) {
        lstrcpynA(log + strlen(log), obj.dump_text, room);
        { size_t L = strlen(log); sprintf(log + L, "\n"); }
        { char* d = log + strlen(log); sprintf(d, "Dr0 = %08lX\t", ctx->Dr0); }
        { char* d = log + strlen(log); sprintf(d, "Dr1 = %08lX\t", ctx->Dr1); }
        { char* d = log + strlen(log); sprintf(d, "Dr2 = %08lX\n", ctx->Dr2); }
        { char* d = log + strlen(log); sprintf(d, "Dr3 = %08lX\t", ctx->Dr3); }
        { char* d = log + strlen(log); sprintf(d, "Dr6 = %08lX\t", ctx->Dr6); }
        { char* d = log + strlen(log); sprintf(d, "Dr7 = %08lX\n", ctx->Dr7); }
        { size_t L = strlen(log); sprintf(L + log, "\n"); }
        { char* d = log + strlen(log); sprintf(d, "ContextFlags = %08lX\n", ctx->ContextFlags); }
        { char* d = log + strlen(log); sprintf(d, "Control Word = %08lX\t\t", ctx->FloatSave.ControlWord); }
        { char* d = log + strlen(log); sprintf(d, "StatusWord = %08lX\n", ctx->FloatSave.StatusWord); }
        { char* d = log + strlen(log); sprintf(d, "TagWord = %08lX\t\t", ctx->FloatSave.TagWord); }
        { char* d = log + strlen(log); sprintf(d, "ErrorOffset = %08lX\n", ctx->FloatSave.ErrorOffset); }
        { char* d = log + strlen(log); sprintf(d, "ErrorSelector = %08lX\t", ctx->FloatSave.ErrorSelector); }
        { char* d = log + strlen(log); sprintf(d, "DataOffset = %08lX\n", ctx->FloatSave.DataOffset); }
        { char* d = log + strlen(log); sprintf(d, "DataSelector = %08lX\t\t", ctx->FloatSave.DataSelector); }
        { char* d = log + strlen(log); sprintf(d, "Cr0NpxState = %08lX\n", ctx->FloatSave.Cr0NpxState); }
        { size_t L = strlen(log); sprintf(log + L, "\n\n\n\n\n"); }
    }

    // flush the buffer, close the log and exit the handler
    NormalizeLineEndings(log, 0x7358);
    if (0 != file) {
        WriteFile(file, log, strlen(log), &written, 0);
        CloseHandle(file);
    }
    UnloadImageHelp();
    return 0;
}

// Turns an exception status code into a printable name. A 24 entry table of
// (code, name) pairs is built on the stack and scanned from the top; the first
// pair whose code matches is returned, and anything else returns "Unknown
// exception type". The codes are the Win32 exception status values from
// windows.h, in ascending order.
struct ExceptionName {
    unsigned long code;
    char* name;
};

// FUNCTION: 0x4d98c0
char* __cdecl GetExceptionName(unsigned long code)
{
    ExceptionName table[24] = {
        {0x40010005, "Control-C"},
        {0x40010008, "Control-Break"},
        {0x80000002, "Datatype Misalignment"},
        {0x80000003, "Breakpoint"},
        {0xc0000005, "Access Violation"},
        {0xc0000006, "In Page Error"},
        {0xc0000017, "No Memory"},
        {0xc000001d, "Illegal Instruction"},
        {0xc0000025, "Noncontinuable Exception"},
        {0xc0000026, "Invalid Disposition"},
        {0xc000008c, "Array Bounds Exceeded"},
        {0xc000008d, "Float Denormal Operand"},
        {0xc000008e, "Float Divide by Zero"},
        {0xc000008f, "Float Inexact Result"},
        {0xc0000090, "Float Invalid Operation"},
        {0xc0000091, "Float Overflow"},
        {0xc0000092, "Float Stack Check"},
        {0xc0000093, "Float Underflow"},
        {0xc0000094, "Integer Divide by Zero"},
        {0xc0000095, "Integer Overflow"},
        {0xc0000096, "Privileged Instruction"},
        {0xc00000fd, "Stack Overflow"},
        {0xc0000142, "DLL Initialization Failed"},
        {0xe06d7363, "Microsoft C++ Exception"},
    };
    unsigned int i;
    for (i = 0; i < 24; i++) {
        if (code == table[i].code) {
            return table[i].name;
        }
    }
    return "Unknown exception type";
}

// FatalError (0x4d9ab0) stays in src/debug/debug_lib_4d9ab0.cpp: it is the
// source of a gap region (its __try/__except and `int 3` have no FPO record).

void __cdecl WalkFrameChain(int* param_1, int* param_2, int param_3, int param_4,
                           void* this_, int flag, int* field_78, int* field_7c,
                           int size, int* field_207c);

// The original calls this out of line from ReportException.
#pragma auto_inline(off)
// FUNCTION: 0x4d9c60
void StackTrace::CaptureStack(int* param_1, int* param_2, int param_3, int param_4)
{
    field_2080 = param_2;
    WalkFrameChain(param_1, param_2, param_3, param_4, this, 0x1e, &field_78, &field_7c, 0x800, &field_207c);
}
#pragma auto_inline(on)

// Formats the saved call stack and stack dump into buf.

// FUNCTION: 0x4d9ca0
void Class_004d9ca0::FormatStackReport()
{
    int m = copied;
    unsigned long* q0 = pc;
    int n = count;
    // One len for both phases, not a second copy.
    unsigned int len = 0xa44c;
    char* p = buf;
    int i;

    if (n > 0) {
        sprintf(p, "Call stack:\n");
        len -= strlen(p);
        p += strlen(p);
        // Both loops index the arrays; walking pointers changes the loop setup.
        for (i = 0; i < n; i++) {
            if (len <= 0x1e) break;
            sprintf(p, "%08lX", ret[i]);
            // A named local set by this if/else, not a ternary: fixes the register split.
            const char* sep;
            if (i == n - 1) {
                sep = "\n";
            } else {
                sep = " ";
                if (i % 8 == 7) sep = "\n";
            }
            strcat(p, sep);
            len -= strlen(p);
            p += strlen(p);
        }
    } else {
        *p = 0;
    }
    if (m > 0 && len > 0x1e) {
        sprintf(p, "Stack dump:\n");
        len -= strlen(p);
        p += strlen(p);
        for (i = 0; i < m; i++) {
            if (len <= 0x1e) break;
            if (i % 8 == 0) {
                sprintf(p, "%08lX: ", (unsigned long)&q0[i]);
                len -= strlen(p);
                p += strlen(p);
            }
            sprintf(p, "%08lX", stack[i]);
            strcat(p, (i == m - 1 || i % 8 == 7) ? "\n" : " ");
            len -= strlen(p);
            p += strlen(p);
        }
    }
}

// A DllMain-shaped entry point: on DLL_PROCESS_ATTACH it saves the module
// handle (read back by 0x4d9f50). `sub eax, 0; je; dec eax; jne` is a switch
// with cases 0 and 1.

extern int DAT_005289c4;

// FUNCTION: 0x4d9f30
BOOL __stdcall FUN_004d9f30(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason) {
    case DLL_PROCESS_DETACH:
        break;
    case DLL_PROCESS_ATTACH:
        DAT_005289c4 = (int)instance;
        break;
    }
    return TRUE;
}

extern int DAT_005289c4;

// The original calls this out of line from ShowDialogBox and
// CreateDialogFromTemplate.
#pragma auto_inline(off)
// FUNCTION: 0x4d9f50
int GetDebugLibInstance(void)
{
    return DAT_005289c4;
}
#pragma auto_inline(on)

// Finds a switch on the command line (case-insensitively) that ends at the
// end of the line, a space or '=', and returns the text just after it, or 0.

// The original calls this out of line from the Class_004d9fe0 and
// Class_004da040 constructors.
#pragma auto_inline(off)
// FUNCTION: 0x4d9f60
char* __cdecl FindCommandLineSwitch(char* name)
{
    if (!name) return 0;
    char* cmd = GetCommandLineA();
    if (!cmd) return 0;
    size_t len = strlen(name);
    if (len == 0) return 0;
    while (*cmd) {
        if (_strnicmp(cmd, name, len) == 0) {
            char c = cmd[len];
            if (c == 0 || isspace(c) || c == '=')
                return cmd + len;
        }
        cmd++;
    }
    return 0;
}
#pragma auto_inline(on)

// A command-line switch: `on` is set from the default, then forced on by
// either on-switch and off by either off-switch (off wins).

char* __cdecl FindCommandLineSwitch(char* name);

// The original calls this out of line from IsMemFussy, IsGonzo and 0x4df160.
#pragma auto_inline(off)
// FUNCTION: 0x4d9fe0
Class_004d9fe0::Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                               char* offSwitch, char* onSwitch2, char* offSwitch2)
{
    on = def;
    if (FindCommandLineSwitch(onSwitch) || FindCommandLineSwitch(onSwitch2))
        on = 1;
    if (FindCommandLineSwitch(offSwitch) || FindCommandLineSwitch(offSwitch2))
        on = 0;
}
#pragma auto_inline(on)

// An integer command-line setting (e.g. "setvalue" / "-memset" in 0x4d8260):
// starts at the default, then parses the text after the switch as hex
// ("0x...") or decimal.

char* __cdecl FindCommandLineSwitch(char* name);

// The original calls this out of line from the setmemory switch and the fpufussy twin.
#pragma auto_inline(off)
// FUNCTION: 0x4da040
Class_004da040::Class_004da040(char* name, int a, int def, char* sw)
{
    value = def;
    char* p = FindCommandLineSwitch(sw);
    if (p) {
        while (*p == ' ' || *p == '\t' || *p == '=')
            p++;
        if (*p == '0' && (p[1] == 'x' || p[1] == 'X')) {
            sscanf(p, "%x", &value);
            return;
        }
        sscanf(p, "%d", &value);
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4da0b0
int FUN_004da0b0(void)
{
    return 0x3e4;
}

// Same shape as the C runtime's abort(): report, raise SIGABRT, exit code 3.

void FUN_004d8390(void);

// FUNCTION: 0x4da0c0
void AbortProgram(void)
{
    FUN_004d8390();
    raise(SIGABRT);
    _exit(3);
}

// Returns whether arg starts (case-insensitively) with one of the 19 option
// names in the table at 0x50c908 ("memfussy" ... "saveresources").

extern char* DAT_0050c908[19];

// FUNCTION: 0x4da0e0
bool __cdecl FUN_004da0e0(const char* arg)
{
    for (char** p = DAT_0050c908; p < DAT_0050c908 + 19; p++) {
        if (_strnicmp(*p, arg, strlen(*p)) == 0)
            return true;
    }
    return false;
}

// The debug helpers ShowDebugMessage, CallDebugHelperDll and InitDebugSupport
// (0x4da120) stay in src/debug/debug_lib_4da120.cpp: they are the source of a
// gap region and are compiled with /Od (see the file's `// FLAGS:` line).

int __cdecl ReportException(EXCEPTION_POINTERS*, char*);
extern const char DAT_0050d2d0[];

// FUNCTION: 0x4da2a0
void __stdcall UnhandledExceptionHandler(int param_1)
{
    ReportException((EXCEPTION_POINTERS*)param_1, (char*)DAT_0050d2d0);
}

// The debug thread DebugThreadProc, RunDebugThread, HandleDialogMessage and
// FUN_004da3e0 (0x4da2c0) stay in src/debug/debug_lib_4da2c0.cpp for the same
// reason (gap region, /Od).

// Normalizes the line endings in `text` in place: counts the '\n's, moves the
// string to the tail of the buffer, then rewrites it from the front converting
// lone '\r' and lone '\n' into "\r\n" and collapsing existing "\r\n" pairs.
// Needed: keeps the destination as `text + (size - len) - 1`.

// 0x4da3f0 NormalizeLineEndings stays in src/debug/debug_lib_4da3f0.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

extern char DAT_0050cd38;
extern char DAT_0050ced8;
extern char DAT_0050c958;

// The original calls this out of line from ShowDialogBox and
// CreateDialogFromTemplate.
#pragma auto_inline(off)
// FUNCTION: 0x4da480
HGLOBAL __cdecl GetDialogTemplate(int id)
{
    char* src = 0;
    int size = 0;
    switch (id) {
    case 0x66:
        src = &DAT_0050cd38;
        size = 0x1a0;
        break;
    case 0x67:
        src = &DAT_0050ced8;
        size = 0x2c0;
        break;
    case 0x81:
        src = &DAT_0050c958;
        size = 0x3e0;
        break;
    }
    if (src != 0 && size != 0) {
        HGLOBAL mem = GlobalAlloc(0x40, size);
        memcpy(mem, src, size);
        return mem;
    }
    return 0;
}
#pragma auto_inline(on)

int GetDebugLibInstance(void);
HGLOBAL __cdecl GetDialogTemplate(int id);

// FUNCTION: 0x4da4f0
int __cdecl ShowDialogBox(int id, HWND parent, DLGPROC proc, LPARAM param)
{
    HGLOBAL mem = GetDialogTemplate(id);
    if (mem == 0)
        return 0;
    int result = DialogBoxIndirectParamA((HINSTANCE)GetDebugLibInstance(), (LPCDLGTEMPLATEA)mem, parent, proc, param);
    GlobalFree(mem);
    return result;
}

int GetDebugLibInstance(void);
HGLOBAL __cdecl GetDialogTemplate(int id);

// The original calls this out of line from the performance and memory dialogs.
#pragma auto_inline(off)
// FUNCTION: 0x4da540
HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param)
{
    HGLOBAL mem = GetDialogTemplate(id);
    if (mem == 0)
        return 0;
    HWND result = CreateDialogIndirectParamA((HINSTANCE)GetDebugLibInstance(), (LPCDLGTEMPLATEA)mem, parent, proc, param);
    GlobalFree(mem);
    return result;
}
#pragma auto_inline(on)

extern char* __cdecl strrchr(const char*, int);

// FUNCTION: 0x4da590
char* __cdecl GetFileNameFromPath(char* param_1)
{
    char* result = strrchr(param_1, 0x5c);
    if (result != 0) {
        return result + 1;
    }
    return param_1;
}

// Opens a URL in the browser. It tries ShellExecuteA on the URL first; when
// that fails (result <= 31) it looks the file association for the extension up
// under HKEY_CLASSES_ROOT, reads the key's default value into buf1, builds
// "<default>\shell\open\command" in the shared `sub` buffer, reads that
// key's default value into buf2, trims the program path out of the command
// line and finally runs that program with the URL as its parameter. If any of
// that fails, the message box at the end reports the last ShellExecute result.
// Callers pass a http:// URL and the extension to look up (".htm").

// FUNCTION: 0x4da5b0
void __cdecl OpenUrl(HWND hwnd, char* url, char* ext)
{
    int res = 0;
    char buf1[500];
    char buf2[1000];
    char sub[1000];
    HKEY key;
    long len;
    char* p;

    if (url) {
        res = (int)ShellExecuteA(hwnd, "open", url, 0, ".", 1);
    }
    if (res > 31) {
        return;
    }
    if (!ext) {
        if (url) {
            ext = strrchr(url, '.');
        }
        if (!ext) {
            goto failed;
        }
    }
    // `sub` is shared by the sub-key path and the error text; key and len are reused across both queries.
    len = 499;
    if (RegOpenKeyA(HKEY_CLASSES_ROOT, ext, &key) == 0) {
        if (RegQueryValueA(key, 0, buf1, &len) == 0) {
            RegCloseKey(key);
            buf1[len] = 0;
            sprintf(sub, "%s\\shell\\open\\command", buf1);
            len = 999;
            if (RegOpenKeyA(HKEY_CLASSES_ROOT, sub, &key) == 0) {
                if (RegQueryValueA(key, 0, buf2, &len) == 0) {
                    RegCloseKey(key);
                    buf2[len] = 0;
                    // `if (p) *p = 0;` stays written out in both arms, not shared.
                    if (buf2[0] == '"') {
                        p = strchr(buf2 + 1, '"');
                        if (p) {
                            *p = 0;
                        }
                    } else {
                        p = strchr(buf2, ' ');
                        if (p) {
                            *p = 0;
                        }
                    }
                    res = (int)ShellExecuteA(hwnd, "open", buf2, url, ".", 1);
                }
            }
        }
    }
failed:
    if (res > 31) {
        return;
    }
    sprintf(sub, "Cannot start %s. ShellExecute result is %d", url, res);
    MessageBoxA(hwnd, sub, "Cavedog", 0);
}

// Returns a function-local static critical section, initialised on first
// use. The empty inline destructor makes MSVC register the empty atexit
// thunk FUN_004da7c0.

class CritSec_004da780 {
public:
    CRITICAL_SECTION cs;

    CritSec_004da780() { InitializeCriticalSection(&cs); }
    ~CritSec_004da780() {}
};

// The original calls this out of line from the allocator, the block map and the trees.
#pragma auto_inline(off)
// FUNCTION: 0x4da780
CritSec_004da780* FUN_004da780()
{
    static CritSec_004da780 lock;
    return &lock;
}
#pragma auto_inline(on)

// FUNCTION: 0x4da7c0
void FUN_004da7c0(void)
{
}

extern unsigned int DAT_00528a04;
extern unsigned int DAT_00528a08;
extern unsigned int DAT_00528a1c;
extern unsigned int DAT_005289dc;
extern unsigned int DAT_005289fc;
extern unsigned int DAT_005289f8;
extern unsigned int DAT_005289d8;
extern unsigned int DAT_005289d0;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_00528a00;

// The original calls this out of line from the allocator and the page protection.
#pragma auto_inline(off)
// FUNCTION: 0x4da7d0
void __cdecl CountAlloc(unsigned int size)
{
    DAT_00528a04++;
    DAT_00528a08++;
    if (DAT_00528a08 > DAT_00528a1c)
        DAT_00528a1c = DAT_00528a08;
    DAT_005289dc += size;
    if (DAT_005289dc >= 1000000000) {
        DAT_005289dc -= 1000000000;
        DAT_005289fc++;
    }
    DAT_005289f8 += size;
    if (DAT_005289f8 > DAT_005289d8)
        DAT_005289d8 = DAT_005289f8;
}
#pragma auto_inline(on)

// The original calls this out of line from the allocator and the page protection.
#pragma auto_inline(off)
// FUNCTION: 0x4da840
void __cdecl CountFree(int param_1) {
    DAT_00528a08--;
    DAT_005289f8 -= param_1;
}
#pragma auto_inline(on)

// FUNCTION: 0x4da860
void ResetAllocStats(void)
{
    DAT_00528a1c = 0;
    DAT_00528a08 = 0;
    DAT_00528a04 = 0;
    DAT_005289d8 = 0;
    DAT_005289f8 = 0;
    DAT_005289dc = 0;
    DAT_005289d0 = 0;
    DAT_005289f0 = 0;
    DAT_005289fc = 0;
    DAT_00528a00 = 0;
}

// FUNCTION: 0x4da8a0
int __cdecl FUN_004da8a0(int param_1)
{
    return (param_1 + 0xfff & ~0xfff) * 2;
}

// FUNCTION: 0x4da8c0
unsigned int __cdecl RoundUpToPage(int param_1)
{
    return (param_1 + 0xfff) & 0xfffff000;
}

// Lazily built singleton for the game's file-record map (the std::_Tree whose
// insert is 0x4dc680 and whose erase is 0x4dc910): allocate the 0x10-byte tree
// object, make the _Nil node DAT_00528a50 (black, self-null children) and the
// head node (red, parent _Nil, both children itself), both carved from the
// pooled free list at DAT_005289e0.

extern void* DAT_005289e0;             // free list of 0x40-byte nodes
extern void (*DAT_005289bc)();         // out-of-memory handler
extern int DAT_00528a4c;               // bumped on every node carved here
extern void* DAT_00528a44;             // the tree singleton

struct Node_004da8d0 {
    Node_004da8d0* left;               // +0x0
    Node_004da8d0* parent;             // +0x4
    Node_004da8d0* right;              // +0x8
    char value[0x30];                  // +0xc
    int color;                         // +0x3c (0 = red)
};

extern void* DAT_00528a50;             // the tree's _Nil node

// The pool allocator that sits at +0 of the tree; its method ignores `this`.
class Class_004dddf0 {
public:
    void* FUN_004dddf0(unsigned int n);
};

class Tree_004da8d0 {
public:
    char field_0;                      // +0x0
    char field_1;                      // +0x1
    Node_004da8d0* head;               // +0x4
    unsigned char multi;               // +0x8
    int size;                          // +0xc
    void* operator new(unsigned int n) { return GlobalAlloc(0, n); }
    Tree_004da8d0(const char& a, const char& b);
    void Init();
};

static inline Node_004da8d0* AllocNode()
{
    if (DAT_005289e0 == 0) {
        Node_004da8d0* block;
        do {
            block = (Node_004da8d0*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004da8d0* head = (Node_004da8d0*)DAT_005289e0;
        for (int i = 0; i < 0x80; i++) {
            block->left = head;
            head = block;
            block++;
        }
        DAT_005289e0 = head;
    }
    Node_004da8d0* node = (Node_004da8d0*)DAT_005289e0;
    DAT_005289e0 = node->left;
    return node;
}

void Tree_004da8d0::Init()
{
    std::_Lockit lock;
    if (DAT_00528a50 == 0) {
        Node_004da8d0* nil =
            (Node_004da8d0*)((Class_004dddf0*)this)->FUN_004dddf0(0x40);
        nil->parent = 0;
        nil->color = 1;
        DAT_00528a50 = nil;
        nil->left = 0;
        ((Node_004da8d0*)DAT_00528a50)->right = 0;
    }
    Node_004da8d0* nil = (Node_004da8d0*)DAT_00528a50;
    DAT_00528a4c++;
    Node_004da8d0* node = AllocNode();
    node->color = 0;
    node->parent = nil;
    head = node;
    size = 0;
    node->left = node;
    head->right = head;
}

inline Tree_004da8d0::Tree_004da8d0(const char& a, const char& b)
{
    field_0 = a;
    field_1 = b;
    multi = 0;
    Init();
}

// The original calls this out of line from the block-map lookups and the trees.
#pragma auto_inline(off)
// FUNCTION: 0x4da8d0
void* GetBlockMap()
{
    if (DAT_00528a44 == 0) {
        char s0, s1;
        DAT_00528a44 = new Tree_004da8d0(s0, s1);
    }
    return DAT_00528a44;
}
#pragma auto_inline(on)

// A lazily created singleton: a vector-like container (an empty allocator,
// copied from an uninitialised default-argument temporary, then four zeroed
// dwords) allocated with a class operator new that calls GlobalAlloc.

template <class T, class A = std::allocator<T> >
class Container_004da9f0 {
public:
    explicit Container_004da9f0(const A& al = A())
        : allocator(al), field_4(0), field_8(0), field_c(0), field_10(0) {}
    static void* operator new(size_t size) { return GlobalAlloc(0, size); }

    A allocator;       // +0x0
    int field_4;       // +0x4
    int field_8;       // +0x8
    int field_c;       // +0xc
    int field_10;      // +0x10
};

extern Container_004da9f0<int>* DAT_00528a48;

// The original calls this out of line from DescribeAddress and the allocator walk.
#pragma auto_inline(off)
// FUNCTION: 0x4da9f0
Container_004da9f0<int>* GetFreedBlockRing()
{
    if (DAT_00528a48 == 0) {
        DAT_00528a48 = new Container_004da9f0<int>;
    }
    return DAT_00528a48;
}
#pragma auto_inline(on)
// The debug library's allocators: the free-block set (std::map<unsigned int,
// unsigned int> as hand-walked red-black trees, GetFreeBlockSet 0x4db610)
// and the game's file-record map (0x4dc680 insert, 0x4dc910 erase, 0x4dce00
// find_, GetBlockMap 0x4da8d0), plus the single copies of the tree helpers
// the linker folded together, the memfussy command-line switches, the debug
// fill pattern and the block descriptions. AllocDebugBlock (0x4dacf0),
// GetFreeBlockSet (0x4db610) and FreeDebugBlock (0x4db7d0) stay in their own
// files: each inlines a tree helper (or constructs the std::map member) from
// that file's own view of the types, which cannot agree with the gathered
// file's one view of each class.
#include <windows.h>
#include <string.h>
#include <set>
#include <yvals.h>
#include <map>
#include <list>
#include <time.h>
// The real <vector> header must supply the body; a hand-rolled copy does not match.
#include <vector>
#include <algorithm>
// ---- the types the gathered files share, unified ---------------------------

// The debug allocator keeps its free blocks and the game its 0x30-byte file
// records in hand-walked red-black trees. Each matched file named the nodes
// and iterators in its own view; a name a symbol pins (data/progress.csv) is
// kept, and the rest of a tree's views share the fullest one.

class FreeBlockMap;
struct Node_004dbec0;
struct Node_004dc680;
struct Node_004dbd00;
struct Pair_004db450;
struct InsertResult_004db450;
struct Pair_004dbec0;
struct Pair_004dc680;
struct Node_004dd150;
struct Node_004dd1f0;
struct Node_004dd710;
struct Node_004dd770;

extern void* DAT_00528a54;             // the free-block tree's _Nil node
extern void* DAT_00528a50;             // the file-record tree's _Nil node
extern void* DAT_00528a10;             // the free-block tree's node free list
extern void* DAT_005289e0;             // the file-record tree's node free list
extern void (*DAT_005289bc)();         // out-of-memory handler
extern unsigned int DAT_005289d4;      // offset the last block was handed out at
extern unsigned int DAT_00528a00;      // how often the search wrapped around
extern unsigned int DAT_005289f0;      // bytes committed
extern unsigned int DAT_005289d0;      // high-water mark of DAT_005289f0
extern unsigned int DAT_00528a04;
extern FreeBlockMap* DAT_00528a40;     // the free-block set singleton

// The file-record tree's node, the fullest of its views.
struct Node_004daa30 {
    Node_004daa30* left;           // +0x0
    Node_004daa30* parent;         // +0x4
    Node_004daa30* right;          // +0x8
    Class_004d8820 value;          // +0xc
    unsigned int color;            // +0x3c
};

// size; each carves nodes from its own free list.
class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

// The node a pool carves, named by the files that only return it.
struct Node_004ddc00;
// A method that ignores `this`: its caller (0x4dbec0, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.
class Class_004ddc00 {
public:
    Node_004ddc00* FUN_004ddc00(int param_1, int param_2);
};
struct Node_004ddc90;
struct Less_004ddc90 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004ddc90 {
public:
    Less_004ddc90 compare;             // +0x0
    Node_004ddc90* head;               // +0x4

    Node_004ddc90* FUN_004ddc90(const unsigned int& key);
};
struct Node_004ddce0;
class Class_004ddce0 {
public:
    Node_004ddce0* FUN_004ddce0(int param_1, int param_2);
};
struct Pair_004db000 {
    unsigned int offset;               // +0x0
    unsigned int length;               // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

struct Node_004db000 {
    Node_004db000* left;               // +0x0
    Node_004db000* parent;             // +0x4
    Node_004db000* right;              // +0x8
    Pair_004db000 value;               // +0xc
    int color;                         // +0x14
};

// The out-of-line _Min, taken on its own opaque node type.
struct Node_004dd1b0 {
    Node_004dd1b0* left;               // +0x0
};

Node_004dd1b0* __cdecl FUN_004dd1b0(Node_004dd1b0* p);

// The other out-of-line moves, on the shared node type.
static inline Node_004db000* Max_004dd2a0(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a54) {
        p = p->right;
    }
    return p;
}

static inline Node_004db000* Min(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54)
        p = p->left;
    return p;
}

// The free-block tree's iterator: one pointer. FUN_004dd2a0 is its _Dec().
class Class_004dd2a0 {
public:
    Node_004db000* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dd2a0& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    Class_004dd2a0& operator++() { Inc(); return *this; }
    Class_004dd2a0 operator++(int) { Class_004dd2a0 tmp = *this; ++*this; return tmp; }
    Class_004dd2a0& operator--() { FUN_004dd2a0(); return *this; }
    Class_004dd2a0 operator--(int) { Class_004dd2a0 tmp = *this; --*this; return tmp; }
    Node_004db000* Mynode() const { return ptr; }
    void FUN_004dd2a0();
    void Inc()
    {
        std::_Lockit lock;
        if (ptr->right != DAT_00528a54)
            ptr = (Node_004db000*)FUN_004dd1b0((Node_004dd1b0*)ptr->right);
        else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->right)
                ptr = p;
            if (ptr->right != p)
                ptr = p;
        }
    }
};

class Class_004dd820 {
public:
    Node_004daa30* ptr;                // +0x0

    Class_004dd820() {}
    Class_004dd820(Node_004daa30* q) : ptr(q) {}
    bool operator==(const Class_004dd820& o) const { return ptr == o.ptr; }
    void FUN_004dd820();
};

// The (iterator, inserted) pair the trees' insert functions return: an
// iterator then a byte. 0x4ddbe0 is its out-of-line constructor.
class Class_004ddbe0 {
public:
    Class_004dd2a0 field_0;            // +0x0
    unsigned char field_4;             // +0x4

    Class_004ddbe0() {}
    Class_004ddbe0(const Class_004dd2a0& i, const unsigned char& b) : field_0(i), field_4(b) {}
    Class_004ddbe0(const Class_004dd820& i, unsigned char b) : field_0((Node_004db000*)i.ptr), field_4(b) {}
    // The byte is copied before the iterator on purpose: 0x4dbbc0 needs the
    // byte in cl and the dword in edx at every return.
    inline Class_004ddbe0(const Class_004ddbe0& o)
    {
        field_4 = o.field_4;
        field_0 = o.field_0;
    }
    Class_004ddbe0* FUN_004ddbe0(int* param_1, unsigned char* param_2);
    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& first, unsigned char& second);
    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& first, const bool& second);
};

// The free-block tree's iterator _Inc, out of line at 0x4dd340.
class Class_004dd340 {
public:
    Node_004db000* ptr;                // +0x0

    void FUN_004dd340();               // _Inc, out of line at 0x4dd340

    Class_004dd340& operator++()
    {
        FUN_004dd340();
        return *this;
    }

    Class_004dd340 operator++(int)
    {
        Class_004dd340 _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    Node_004db000* _Mynode() const { return ptr; }
};

static inline Node_004db000* Min_004dbd80(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54) {
        p = p->left;
    }
    return p;
}

static inline Node_004db000* Max_004dbd80(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a54) {
        p = p->right;
    }
    return p;
}

static inline Node_004daa30* Max_004dd820(Node_004daa30* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a50) {
        p = p->right;
    }
    return p;
}

// std::_Tree<...>::upper_bound: the first node whose key is greater than the
// given key, or the head node; the out-of-line copy is 0x4dbd20.
struct Less_004dbd20 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Node_004dbd20 {
    Node_004dbd20* left;               // +0x0
    Node_004dbd20* parent;             // +0x4
    Node_004dbd20* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Iter_004dbd20 {
public:
    Node_004dbd20* ptr;
    Iter_004dbd20() : ptr(0) {}
    Iter_004dbd20(Node_004dbd20* p) : ptr(p) {}
};

class Class_004dbd20 {
public:
    Less_004dbd20 key_compare;
    Node_004dbd20* head;               // +0x4

    Iter_004dbd20 FUN_004dbd20(const unsigned int& kv);
};

// Inlined std::_Tree<...>::_Ubound(const _K&): its own lock scope.
static inline Node_004dbd20* Ubound_004dbd20(Class_004dbd20* self,
                                             const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dbd20* x = self->head->parent;
    Node_004dbd20* y = self->head;
    while (x != DAT_00528a54)
        if (self->key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}

// The free-block tree's iterator as 0x4dbd80 and 0x4dbe10 move it; the
// callers in 0x4dacf0 and 0x4db000 name the same class.
class Class_004dbe10 {
public:
    Node_004db000* ptr;                // +0x0

    Class_004dbe10() {}
    Class_004dbe10(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dbe10& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    Class_004dbe10 FUN_004dbe10(int); // operator--(int)
    Class_004dbe10 FUN_004dbd80(int); // operator++(int)

    void Inc()
    {
        std::_Lockit lock;
        if (ptr->right != DAT_00528a54) {
            ptr = Min_004dbd80(ptr->right);
        } else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->right) {
                ptr = p;
            }
            if (ptr->right != p) {
                ptr = p;
            }
        }
    }

    void Dec()
    {
        std::_Lockit lock;
        if (ptr->color == 0 && ptr->parent->parent == ptr) {
            ptr = ptr->right;
        } else if (ptr->left != DAT_00528a54) {
            ptr = Max_004dbd80(ptr->left);
        } else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->left) {
                ptr = p;
            }
            ptr = p;
        }
    }
};

class Class_004dbeb0 {
public:
    char unknown_0[4];
    int* field_4;

    Class_004dbe10 FUN_004dbeb0();
    int* FUN_004dbeb0(int* param_1);
};

class Iter_004dbd00 {
public:
    Node_004dbd00* ptr;                // +0x0

    Iter_004dbd00() {}
    Iter_004dbd00(Node_004dbd00* p) : ptr(p) {}
};

class ConstIter_004dbd00 : public Iter_004dbd00 {
public:
    ConstIter_004dbd00() {}
    ConstIter_004dbd00(Node_004dbd00* p) : Iter_004dbd00(p) {}
    ConstIter_004dbd00(const Iter_004dbd00& x) : Iter_004dbd00(x) {}
};

struct Alloc_004dc130 { };             // empty, at the tree's +0
struct Less_004dc130 { };              // empty, at the tree's +1

class Class_004dc130 {
public:
    enum Redbl { _Red, _Black };

    Alloc_004dc130 allocator;          // +0x0
    Less_004dc130 key_compare;         // +0x1
    Node_004db000* head;               // +0x4
    bool multi;                        // +0x8
    unsigned int size;                 // +0xc

    static Node_004db000*& Left(Node_004db000* p) { return p->left; }
    static Node_004db000*& Parent(Node_004db000* p) { return p->parent; }
    static Node_004db000*& Right(Node_004db000* p) { return p->right; }
    static int& Color(Node_004db000* p) { return p->color; }
    static unsigned int* _Value(Node_004db000* p) { return (unsigned int*)&p->value; }

    Node_004db000*& Root() { return Parent(head); }
    Node_004db000*& Lmost() { return Left(head); }
    Node_004db000*& Rmost() { return Right(head); }

    static Node_004db000* Min(Node_004db000* p)
    {
        std::_Lockit _Lk;
        while (Left(p) != DAT_00528a54)
            p = Left(p);
        return p;
    }

    static Node_004db000* Max(Node_004db000* p)
    {
        std::_Lockit _Lk;
        while (Right(p) != DAT_00528a54)
            p = Right(p);
        return p;
    }

    void Lrotate(Node_004db000* x)
    {
        std::_Lockit _Lk;
        Node_004db000* y = Right(x);
        Right(x) = Left(y);
        if (Left(y) != DAT_00528a54)
            Parent(Left(y)) = x;
        Parent(y) = Parent(x);
        if (x == Root())
            Root() = y;
        else if (x == Left(Parent(x)))
            Left(Parent(x)) = y;
        else
            Right(Parent(x)) = y;
        Left(y) = x;
        Parent(x) = y;
    }

    void Rrotate(Node_004db000* x)
    {
        std::_Lockit _Lk;
        Node_004db000* y = Left(x);
        Left(x) = Right(y);
        if (Right(y) != DAT_00528a54)
            Parent(Right(y)) = x;
        Parent(y) = Parent(x);
        if (x == Root())
            Root() = y;
        else if (x == Right(Parent(x)))
            Right(Parent(x)) = y;
        else
            Left(Parent(x)) = y;
        Right(y) = x;
        Parent(x) = y;
    }

    static void Destval(unsigned int*) { }

    static void Freenode(Node_004db000* p)
    {
        if (p != 0) {
            *(void**)p = DAT_00528a10;
            DAT_00528a10 = p;
        }
    }

    Class_004dd340 FUN_004dc130(Class_004dd340 _P);
    Class_004dd2a0 FUN_004dc130(Class_004dd2a0 it);
    void FUN_004dc130(Class_004dd2a0* out, Class_004dd2a0 it);
    Iter_004dbd00 FUN_004dc130(Iter_004dbd00 it);
};

class Class_004dbd00 {
public:
    Class_004dc130 tree;               // +0x0

    Class_004dbe10 FUN_004dbd00(Class_004dbe10 it);
    ConstIter_004dbd00 FUN_004dbd00(ConstIter_004dbd00 it);
};

struct Less_004dd250 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Node_004dd250 {
    Node_004dd250* left;               // +0x0
    Node_004dd250* parent;             // +0x4
    Node_004dd250* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Class_004dd250 {
public:
    Less_004dd250 key_compare;
    Node_004dd250* head;               // +0x4

    Node_004db000* FUN_004dd250(const Pair_004db000& k);
    Node_004dd250* FUN_004dd250(const unsigned int& kv);
};

struct Node_004dd7d0 {
    Node_004dd7d0* left;               // +0x0
    Node_004dd7d0* parent;             // +0x4
    Node_004dd7d0* right;              // +0x8
    unsigned int key;                  // +0xc
    int color;                         // +0x14
};

struct Less_004dd7d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_004dd7d0 {
public:
    Less_004dd7d0 key_compare;         // +0x0
    Node_004dd7d0* head;               // +0x4

    Class_004dd820 End() { return Class_004dd820((Node_004daa30*)head); }
    Class_004dd820 Begin() { return Class_004dd820((Node_004daa30*)head->left); }
    Node_004dd7d0* FUN_004dd7d0(const unsigned int& kv);
};

struct Less_004dd3d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dd3d0 {
public:
    Node_004daa30* ptr;                // +0x0

    Iter_004dd3d0() {}
    Iter_004dd3d0(Node_004daa30* p) : ptr(p) {}
    bool operator==(const Iter_004dd3d0& o) const { return ptr == o.ptr; }
};

class Class_004dd3d0 {
public:
    Less_004dd3d0 compare;             // +0x0
    Node_004daa30* head;               // +0x4

    Iter_004dd3d0 End() { return Iter_004dd3d0(head); }
    Iter_004dd3d0 FUN_004dd3d0(const unsigned int& key);
    Iter_004dd3d0 find(const unsigned int& key)
    {
        Iter_004dd3d0 it = FUN_004dd3d0(key);
        return (it == End() || compare(key, it.ptr->value.base)) ? End() : it;
    }

    Node_004daa30* Lbound(const unsigned int& key) const
    {
        std::_Lockit lock;
        Node_004daa30* x = head->parent;
        Node_004daa30* y = head;
        while (x != DAT_00528a50) {
            if (compare(x->value.base, key))
                x = x->right;
            else
                y = x, x = x->left;
        }
        return y;
    }
};

struct Less_004dc620 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dc620 {
public:
    Node_004db000* ptr;
    Iter_004dc620() : ptr(0) {}
    Iter_004dc620(Node_004db000* p) : ptr(p) {}
};

class Class_004dc620 {
public:
    Less_004dc620 key_compare;
    Node_004db000* head;               // +0x4

    Class_004dbe10 FUN_004dc620(const Pair_004db000& k);
    void FUN_004dc620(Class_004dd2a0* out, const unsigned int& kv);
    Iter_004dc620 FUN_004dc620(const unsigned int& kv);
};

class Class_004dd150 {
public:
    int unknown_0;
    Node_004dd150* head;            // +0x4
    void FUN_004dd150(Node_004dd150* x);
    void FUN_004dd150(Node_004db000* x);
};

struct Node_004dd150 {
    Node_004dd150* left;            // +0x0
    Node_004dd150* parent;          // +0x4
    Node_004dd150* right;           // +0x8
};

class Class_004dd1f0 {
public:
    int unknown_0;
    Node_004dd1f0* head;            // +0x4
    void FUN_004dd1f0(Node_004dd1f0* x);
    void FUN_004dd1f0(Node_004db000* x);
};

struct Node_004dd1f0 {
    Node_004dd1f0* left;            // +0x0
    Node_004dd1f0* parent;          // +0x4
    Node_004dd1f0* right;           // +0x8
};

struct Node_004dd710 {
    Node_004dd710* left;               // +0x0
    Node_004dd710* parent;             // +0x4
    Node_004dd710* right;              // +0x8
};

class Class_004dd710 {
public:
    int unknown_0;
    Node_004dd710* head;               // +0x4
    void FUN_004dd710(Node_004dd710* x);
    void FUN_004dd710(Node_004daa30* x);
};

struct Node_004dd770 {
    Node_004dd770* left;               // +0x0
    Node_004dd770* parent;             // +0x4
    Node_004dd770* right;              // +0x8
};

class Class_004dd770 {
public:
    int unknown_0;
    Node_004dd770* head;               // +0x4
    void FUN_004dd770(Node_004dd770* x);
    void FUN_004dd770(Node_004daa30* x);
};

struct Less_004dd430 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

struct Pair_004dd430 {
    unsigned int key;                 // +0x0
    char unknown_4[44];
};

struct Node_004dd430 {
    Node_004dd430* left;              // +0x0
    Node_004dd430* parent;            // +0x4
    Node_004dd430* right;             // +0x8
    Pair_004dd430 value;              // +0xc
    int color;                        // +0x3c
};

class Class_004dd430 {
public:
    Less_004dd430 key_compare;        // +0x0
    Node_004dd430* head;              // +0x4
    unsigned char unknown_8;          // +0x8
    int size;                         // +0xc

    void Lrotate(Node_004dd430* x)
    {
        std::_Lockit lock;
        Node_004dd430* y = x->right;
        x->right = y->left;
        if (y->left != DAT_00528a50)
            y->left->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void Rrotate(Node_004dd430* x)
    {
        std::_Lockit lock;
        Node_004dd430* y = x->left;
        x->left = y->right;
        if (y->right != DAT_00528a50)
            y->right->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    Class_004dd2a0 FUN_004dd430(Node_004dd430* x, Node_004dd430* y,
                                const Pair_004dd430& v);
    Class_004dd820* FUN_004dd430(Class_004dd820* out, Node_004daa30* x,
                                 Node_004daa30* y, const Pair_004dc680* v);
};

struct Pair_004dc680 {
    unsigned int key;                  // +0x0
    char unknown_4[44];
};

struct Less_004dc680 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

class Class_004dc680 {
public:
    Less_004dc680 key_compare;         // +0x0
    Node_004daa30* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc

    Class_004dd820 Begin() { return Class_004dd820(head->left); }

    Class_004ddbe0 FUN_004dc680(Pair_004dc680* p);
    Class_004ddbe0 FUN_004dc680(const Class_004d8820& v);
};

struct Value_004dc910 { char unknown_0[0x30]; };

struct Node_004dc910 {
    Node_004dc910* _Left;
    Node_004dc910* _Parent;
    Node_004dc910* _Right;
    Value_004dc910 _Value;
    int _Color;
};

struct Node_004dde70;
class Class_004dde70 {
public:
    Node_004dde70* ptr;                // +0x0
    void FUN_004dde70();
};
class Iter_004dc910 {
public:
    Node_004dc910* _Ptr;
    Iter_004dc910() {}
    Iter_004dc910(Node_004dc910* _P) : _Ptr(_P) {}
    // Calls Class_004dde70::FUN_004dde70 (the _Inc at 0x4dde70) by that exact name.
    Iter_004dc910 operator++(int)
        { Iter_004dc910 _Tmp = *this;
          ((Class_004dde70*)this)->FUN_004dde70();
          return (_Tmp); }
    Node_004dc910* _Mynode() const { return _Ptr; }
};

class Class_004dc910 {
public:
    int unknown_0;
    Node_004dc910* _Head;              // +4
    unsigned char _Multi;              // +8
    int _Size;                         // +0xc

    Node_004dc910*& _Root() { return _Head->_Parent; }
    Node_004dc910*& _Lmost() { return _Head->_Left; }
    Node_004dc910*& _Rmost() { return _Head->_Right; }

    static Node_004dc910* _Min(Node_004dc910* _P)
        { std::_Lockit _Lk;
          while (_P->_Left != DAT_00528a50)
              _P = _P->_Left;
          return _P; }

    static Node_004dc910* _Max(Node_004dc910* _P)
        { std::_Lockit _Lk;
          while (_P->_Right != DAT_00528a50)
              _P = _P->_Right;
          return _P; }

    void _Lrotate(Node_004dc910* _X)
        { std::_Lockit _Lk;
          Node_004dc910* _Y = _X->_Right;
          _X->_Right = _Y->_Left;
          if (_Y->_Left != DAT_00528a50)
              _Y->_Left->_Parent = _X;
          _Y->_Parent = _X->_Parent;
          if (_X == _Root())
              _Root() = _Y;
          else if (_X == _X->_Parent->_Left)
              _X->_Parent->_Left = _Y;
          else
              _X->_Parent->_Right = _Y;
          _Y->_Left = _X;
          _X->_Parent = _Y; }

    void _Rrotate(Node_004dc910* _X)
        { std::_Lockit _Lk;
          Node_004dc910* _Y = _X->_Left;
          _X->_Left = _Y->_Right;
          if (_Y->_Right != DAT_00528a50)
              _Y->_Right->_Parent = _X;
          _Y->_Parent = _X->_Parent;
          if (_X == _Root())
              _Root() = _Y;
          else if (_X == _X->_Parent->_Right)
              _X->_Parent->_Right = _Y;
          else
              _X->_Parent->_Left = _Y;
          _Y->_Right = _X;
          _X->_Parent = _Y; }

    Iter_004dc910 erase(Iter_004dc910 _P);
};

struct Less_004dce00 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dce00 {
public:
    Node_004daa30* ptr;

    Iter_004dce00() {}
    Iter_004dce00(Node_004daa30* p) : ptr(p) {}
    bool operator==(const Iter_004dce00& other) const { return ptr == other.ptr; }
    Class_004d8820& operator*() const { return ptr->value; }
    Class_004d8820* operator->() const { return &ptr->value; }
};

class Class_004dce00 {
public:
    Less_004dce00 compare;             // +0x0
    Node_004daa30* head;               // +0x4

    Iter_004dce00 end() { return Iter_004dce00(head); }
    Iter_004dce00 End() { return Iter_004dce00(head); }
    Iter_004dce00 FUN_004dce00(const unsigned int& key);
};

struct Less_004db000 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Less_004dbec0 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

struct Pair_004dbec0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

class Class_004dbec0 {
public:
    Less_004dbec0 key_compare;         // +0x0
    Node_004db000* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc

    Class_004dd2a0 Begin() { return Class_004dd2a0(head->left); }

    Class_004ddbe0 FUN_004dbec0(const Pair_004db000& v);
    void FUN_004dbec0(InsertResult_004db450* it, Pair_004db450* p);
    Class_004ddbe0 FUN_004dbec0(Pair_004dbec0* p);
};

struct Pair_004db450 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct InsertResult_004db450 { Class_004dd2a0 first; int second; };
// The helper's one frame object: the pair, then the value_type.

struct Frame_004db450 { InsertResult_004db450 r; Pair_004db450 p; };

class Class_004db450 {
public:
    char unknown_0[4];
    Node_004db000* head;               // +0x4
    char unknown_8[8];
    int total;                         // +0x10

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }

    inline void Tail(unsigned base, unsigned len, Class_004dd2a0& it) {
    total += len;
    Class_004dd2a0 n;
    // One frame object (pair, then value_type): separate locals move the stack slots.
    Frame_004db450 f;
    f.p.offset = base;
    f.p.length = len;
    ((Class_004dc620*)this)->FUN_004dc620(&n, f.p.offset);
    it = n;
    ((Class_004dbeb0*)this)->FUN_004dbeb0((int*)&f.r);
    if (it == f.r.first)
        it.ptr = head;
    else
        it.FUN_004dd2a0();
    if (Neq(n, Class_004dd2a0(head))) {
        if (n.ptr->value.offset == f.p.offset + f.p.length) {
            f.p.length = f.p.length + n.ptr->value.length;
            ((Class_004dc130*)this)->FUN_004dc130(&f.r.first, n);
        }
    }
    if (Neq(it, Class_004dd2a0(head))) {
        if (it.ptr->value.offset + it.ptr->value.length == f.p.offset) {
            f.p.length = f.p.length + it.ptr->value.length;
            f.p.offset = it.ptr->value.offset;
            ((Class_004dc130*)this)->FUN_004dc130(&f.r.first, it);
        }
    }
    ((Class_004dbec0*)this)->FUN_004dbec0(&f.r, &f.p);
    }
    bool GrowReservation(unsigned int size);
};

struct Pair_004dbbc0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Node_004dbbc0 {
    Node_004dbbc0* left;               // +0x0
    Node_004dbbc0* parent;             // +0x4
    Node_004dbbc0* right;              // +0x8
    Pair_004dbbc0 value;               // +0xc
    int color;                         // +0x14
};

struct Kfn_004dbbc0 {
    const unsigned int& operator()(const Pair_004dbbc0& x) const { return x.offset; }
};

struct Less_004dbbc0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004dce60 {
public:
    unsigned char allocator;           // +0x0
    Less_004dbbc0 key_compare;         // +0x1
    Node_004dbbc0* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc

    static Node_004dbbc0*& Left(Node_004dbbc0* p) { return p->left; }
    static Node_004dbbc0*& Right(Node_004dbbc0* p) { return p->right; }
    static const unsigned int& Key(Node_004dbbc0* p) { return Kfn_004dbbc0()(p->value); }
    Node_004dbbc0*& Root() { return head->parent; }
    Node_004dbbc0*& Lmost() { return Left(head); }
    Class_004dd2a0 begin() { return Class_004dd2a0((Node_004db000*)Lmost()); }

    // Left rotation of x, shaped like std::_Tree<...>::_Lrotate.
    void Lrotate(Node_004dbbc0* x)
    {
        std::_Lockit lock;
        Node_004dbbc0* y = x->right;
        x->right = y->left;
        if (y->left != DAT_00528a54)
            y->left->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // Right rotation of x, shaped like std::_Tree<...>::_Rrotate.
    void Rrotate(Node_004dbbc0* x)
    {
        std::_Lockit lock;
        Node_004dbbc0* y = x->left;
        x->left = y->right;
        if (y->right != DAT_00528a54)
            y->right->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    Class_004dd2a0 FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                const Pair_004dbbc0* v);
    Class_004dd2a0 FUN_004dce60(Node_004db000* x, Node_004db000* y,
                                Pair_004db000* v);
    Class_004dd2a0 FUN_004dce60(Node_004db000* x, Node_004db000* y,
                                const Pair_004dbec0* v);

    Class_004ddbe0 TreeInsert(const Pair_004dbbc0& V);
    Class_004ddbe0 FUN_004dbbc0(const Pair_004dbbc0& V);
    Class_004ddbe0 FUN_004dbbc0(const Pair_004db000& v);
};

class FreeBlockMap {
public:
    Less_004db000 key_compare;         // +0x0
    Node_004db000* head;               // +0x4
    unsigned char rebuild;             // +0x8
    unsigned int count;                // +0xc
    unsigned int total;                // +0x10

    // The constructor (0x4db610) stays in its own file: it is the one function
    // that constructs the std::map member, whose _Init allocates the two
    // sentinel nodes; the walkers below use the same bytes as raw fields.
    Class_004dd2a0 begin() { return Class_004dd2a0(head->left); }
    Class_004dd2a0 end() { return Class_004dd2a0(head); }
    unsigned int size() const { return count; }
    // The original tests this as a value (sete; neg; sbb; inc; test), which
    // MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }
    Class_004dd2a0 upper_bound(const Pair_004db000& k)
    {
        return Class_004dd2a0(((Class_004dd250*)this)->FUN_004dd250(k));
    }

    void AddFreeBlock(Pair_004db000 p);
    unsigned int TakeFreeBlock(unsigned int bytes);

    // 0x4db450: reserve more address space and add it to the free blocks.
    bool Grow(unsigned int size)
    {
        // len declared before base: the other order swaps the operands of base + len.
        unsigned int len = 0x10000000;
        unsigned int base;

        if (2 * size > len && size < 0x40000000u)
            len = ((size + 0x1fff) & 0xffffe000) * 2;
        if (size > len)
            len = (size + 0x1fff) & 0xffffe000;
        base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                          PAGE_READWRITE);
        for (;;) {
            if (base != 0 && base + len <= 0x80000000u)
                break;
            if (base != 0)
                VirtualFree((void*)base, len + 0x2000, MEM_RELEASE);
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000 || len < size)
                return false;
            base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                              PAGE_READWRITE);
        }
        total += len;
        AddFreeBlock(Pair_004db000(base, len));
        return true;
    }

};

FreeBlockMap* GetFreeBlockSet();

// The <vector> the freed-block ring is (the real header supplies insert).
struct Elem_004dd8c0 {
    unsigned int w[0xc];               // +0x0, 0x30 bytes
};

class Alloc_004dd8c0 {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef Elem_004dd8c0* pointer;
    typedef const Elem_004dd8c0* const_pointer;
    typedef Elem_004dd8c0& reference;
    typedef const Elem_004dd8c0& const_reference;
    typedef Elem_004dd8c0 value_type;

    pointer allocate(size_type _N, const void* = 0)
    {
        pointer _P;
        do {
            _P = (pointer)GlobalAlloc(0, _N * sizeof(value_type));
            if (_P == 0 && DAT_005289bc != 0)
                DAT_005289bc();
        } while (_P == 0 && DAT_005289bc != 0);
        return _P;
    }
    void deallocate(pointer _P, size_type)
    {
        // Must test the pointer before freeing.
        if (_P != 0)
            GlobalFree(_P);
    }
    void construct(pointer _P, const value_type& _V)
    {
        std::_Construct(_P, _V);
    }
    void destroy(pointer) {}
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }
};

typedef std::vector<Elem_004dd8c0, Alloc_004dd8c0> Vec_004dd8c0;
typedef void (Vec_004dd8c0::*InsertFn_004dd8c0)(
    Vec_004dd8c0::iterator, Vec_004dd8c0::size_type, const Elem_004dd8c0&);

// The free functions the gathered functions call.
// AllocDebugBlock (0x4dacf0), GetFreeBlockSet (0x4db610) and FreeDebugBlock
// (0x4db7d0) stay in their own files: each inlines a tree helper written
// against that file's own view of the classes (Class_004dbe10's begin/end in
// 0x4dacf0, the std::map member's _Init in 0x4db610, the Class_004dbd20
// upper_bound stub in 0x4db7d0), and the gathered file holds only one view
// of each class.
char IsMemFussy();
char IsBackAlign();
int FUN_004db7c0();
CritSec_004da780* FUN_004da780();
void* GetBlockMap();
Container_004da9f0<int>* GetFreedBlockRing();
unsigned int __cdecl RoundUpToPage(unsigned int size);
unsigned int __cdecl FUN_004da8a0(unsigned int size);
void __cdecl CountAlloc(unsigned int size);
void __cdecl CountFree(int size);
void __cdecl FillPattern(void* at, int value, unsigned int count);
void __cdecl CheckFillPattern(void* at, int value, unsigned int count);
void __cdecl FindBlocksAroundAddress(unsigned int key, BlockInfo* prev, BlockInfo* next);
void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused);
unsigned int __cdecl LookupBlockSize(unsigned int key);
void __cdecl RoundRangeToPages(unsigned int* lo, unsigned int* hi);
void* __cdecl AllocDebugBlock(unsigned int size, int flags);
void __cdecl FreeDebugBlock(void* p, int flags);
void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags);
size_t __cdecl GetBlockSize(void* p);

// ---- from src/debug/debug_lib_4daa30.cpp --------------------

// FUNCTION: 0x4daa30
void __cdecl FindBlocksAroundAddress(unsigned int key, Class_004d8820* prev, Class_004d8820* next)
{
    LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION)FUN_004da780();
    EnterCriticalSection(cs);
    Class_004d8820 rec(key, 0, 0, 0, 0);
    // Named local: chaining the call changes the register allocation.
    Class_004dd7d0* tree = (Class_004dd7d0*)GetBlockMap();
    // Must be Class_004dd820 (the map's _Dec), not an ad hoc type.
    Class_004dd820 it;
    it.ptr = (Node_004daa30*)tree->FUN_004dd7d0(rec.base);
    if (it == ((Class_004dd7d0*)GetBlockMap())->End()) {
        next->base = 0;
        next->size = 0;
    } else {
        *next = it.ptr->value;
    }
    if (it == ((Class_004dd7d0*)GetBlockMap())->Begin()) {
        prev->base = 0;
        prev->size = 0;
    } else {
        it.FUN_004dd820();
        *prev = it.ptr->value;
    }
    LeaveCriticalSection(cs);
}

// ---- from src/debug/debug_lib_4dab10.cpp --------------------

// FUNCTION: 0x4dab10
void __cdecl DescribeAddress(unsigned int address, char* buf, int unused)
{
    BlockInfo local1;
    BlockInfo local2;
    FindBlocksAroundAddress(address, &local1, &local2);
    if (local1.address != 0) {
        bool inRange = address >= (unsigned int)local1.address
                    && address < (unsigned int)local1.size + (unsigned int)local1.address;
        if (inRange) {
            FormatBlockInfo(local1, buf, unused);
            return;
        }
    }
    strcpy(buf, "Address is not within an allocated block.");
}

// ---- from src/debug/debug_lib_4dabb0.cpp --------------------

// FUNCTION: 0x4dabb0
char __cdecl FUN_004dabb0(unsigned int address, char* buf, unsigned int n)
{
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)FUN_004da780();
    EnterCriticalSection(cs);
    *buf = 0;
    if (IsMemFussy()) {
        BlockInfo* p = (BlockInfo*)GetFreedBlockRing()->field_8;
        char found = 0;
        // One `&&` loop condition: a do/while with a break duplicates the size test.
        while (p != (BlockInfo*)GetFreedBlockRing()->field_4 && n > 100) {
            p = (BlockInfo*)((char*)p - 0x30);
            BlockInfo info = *p;
            bool inRange = address >= (unsigned int)info.address
                        && address < (unsigned int)info.size + (unsigned int)info.address;
            if (inRange) {
                if (found)
                    strcat(buf, "\n");
                FormatBlockInfo(info, buf + strlen(buf), n);
                unsigned int len = strlen(buf);
                found = 1;
                buf += len;
                n -= len;
            }
        }
        LeaveCriticalSection(cs);
        return found;
    }
    LeaveCriticalSection(cs);
    return 0;
}

// ---- from src/debug/free_block_map.cpp --------------------

// FUNCTION: 0x4db000
void FreeBlockMap::AddFreeBlock(Pair_004db000 p)
{
    Class_004dd2a0 it;
    Class_004dd2a0 it2;
    // One function-scope object for all three calls: lets their identical endings merge.
    Class_004ddbe0 result;
    unsigned char inserted;

    Class_004dd2a0 n(((Class_004dd250*)this)->FUN_004dd250(p));
    it = n;
    if (n == begin()) {
        it = end();
    } else {
        it.FUN_004dd2a0();
    }
    if (Neq(n, end())) {
        if (n->offset == p.length + p.offset) {
            p.length = p.length + n->length;
            ((Class_004dc130*)this)->FUN_004dc130(n);
        }
    }
    if (Neq(it, end())) {
        Node_004db000* m = it.ptr;
        if (m->value.length + m->value.offset == p.offset) {
            p.length = p.length + m->value.length;
            p.offset = m->value.offset;
            ((Class_004dc130*)this)->FUN_004dc130(it);
        }
    }

    Node_004db000* y = head;
    bool less = true;
    Node_004db000* x = y->parent;
    {
        std::_Lockit lock;
        while (x != DAT_00528a54) {
            y = x;
            less = p.offset < x->value.offset;
            x = less ? x->left : x->right;
        }
    }

    if (rebuild) {
        Class_004dd2a0 t(((Class_004dce60*)this)->FUN_004dce60(x, y, &p));
        return;
    }
    it2.ptr = y;
    if (less) {
        if (Class_004dd2a0(y) == begin()) {
            inserted = 1;
            // Insert call passed straight in, not via a local: fixes the argument push order.
            result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
            goto done;
        }
        it2.FUN_004dd2a0();
    }
    if (key_compare(it2->offset, p.offset)) {
        inserted = 1;
        result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
    } else {
        inserted = 0;
        result.FUN_004ddbe0(it2, inserted);
    }
done: ;
}

// The allocator's alloc(): look for a free block of `bytes` in the free-block
// set, preferring the block the last allocation came from (DAT_005289d4),
// take it out, hand back the leftovers on either side as new free blocks, and
// if two passes over the set find nothing, reserve more address space (Grow,
// the out-of-line copy of which is 0x4db450) and try again. DAT_00528a00
// counts the wraps around the set and DAT_00528a54 is the tree's _Nil node.
// 0x4db1c0 FreeBlockMap::TakeFreeBlock stays in src/debug/free_block_map.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

// ---- from src/debug/debug_lib_4db450.cpp --------------------

// FUNCTION: 0x4db450
bool Class_004db450::GrowReservation(unsigned int size)
{
    unsigned int len = 0x10000000;
    unsigned int base;
    // Once the reservation loop is done the parameter is dead, and the original
    // reuses its stack slot for the map iterator.
    Class_004dd2a0& it = *(Class_004dd2a0*)&size;

    if (2 * size > len && size < 0x40000000u)
        len = ((size + 0x1fff) & 0xffffe000) * 2;
    if (size > len)
        len = (size + 0x1fff) & 0xffffe000;
    base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                      PAGE_READWRITE);
    for (;;) {
        if (base != 0 && base + len <= 0x80000000u)
            break;
        if (base != 0)
            VirtualFree((void*)base, len + 0x2000, MEM_RELEASE);
        len = (len >> 1) & 0x7fffe000;
        if (len < 0x10000 || len < size)
            return false;
        base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                          PAGE_READWRITE);
    }
    Tail(base, len, *(Class_004dd2a0*)&size);
    return true;
}
// ---- from src/debug/debug_lib_4db760.cpp --------------------

// FUNCTION: 0x4db760
char IsBackAlign(void)
{
    static Class_004d9fe0 DAT_00528a20("backalign", 1, 1, 0, "-memfrontalign", 0, 0);
    return DAT_00528a20.on;
}

// ---- from src/debug/debug_lib_4db7b0.cpp --------------------

// FUNCTION: 0x4db7b0
void FUN_004db7b0(void)
{
}

// ---- from src/debug/debug_lib_4db7c0.cpp --------------------

// FUNCTION: 0x4db7c0
int FUN_004db7c0(void)
{
    return 0xcccccccc;
}

// ---- from src/debug/debug_lib_4dba40.cpp --------------------

// The original calls this out of line from GameRealloc.
#pragma auto_inline(off)
// FUNCTION: 0x4dba40
void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags)
{
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)FUN_004da780();
    EnterCriticalSection(cs);
    void* q = 0;
    size_t old = 0;
    if (p)
        old = GetBlockSize(p);
    if (size > 0) {
        q = AllocDebugBlock(size, flags);
        if (!q) {
            LeaveCriticalSection(cs);
            return 0;
        }
        unsigned int n = old >= size ? size : old;
        if (n > 0)
            memcpy(q, p, n);
    }
    if (p)
        FreeDebugBlock(p, flags);
    LeaveCriticalSection(cs);
    return q;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dbae0.cpp --------------------

// The original calls this out of line from GetBlockSize.
#pragma auto_inline(off)
// FUNCTION: 0x4dbae0
unsigned int __cdecl LookupBlockSize(unsigned int key)
{
    LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION)FUN_004da780();
    EnterCriticalSection(cs);
    // Named local fetched before rec is built: GetBlockMap must be called first.
    Class_004dd3d0* tree = (Class_004dd3d0*)GetBlockMap();
    Class_004d8820 rec(key, 0, 0, 0, 0);
    Iter_004dd3d0 it = tree->find(rec.base);
    if (it == ((Class_004dd3d0*)GetBlockMap())->End()) {
        LeaveCriticalSection(cs);
        return 0;
    }
    unsigned int value = it.ptr->value.size;
    LeaveCriticalSection(cs);
    return value;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dbb90.cpp --------------------

// The original calls this out of line from ProtectBlock.
#pragma auto_inline(off)
// FUNCTION: 0x4dbb90
void __cdecl RoundRangeToPages(unsigned int* param_1, unsigned int* param_2)
{
    *param_1 &= 0xfffff000;
    *param_2 = (*param_2 + 0xfff) & 0xfffff000;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dbbc0.cpp --------------------

inline Class_004ddbe0 Class_004dce60::TreeInsert(const Pair_004dbbc0& V)
{
    Node_004dbbc0* X = Root();
    Node_004dbbc0* Y = head;
    bool Ans = true;
    {
        std::_Lockit Lk;
        while (X != DAT_00528a54) {
            Y = X;
            Ans = key_compare(Kfn_004dbbc0()(V), Key(X));
            X = Ans ? Left(X) : Right(X);
        }
    }
    if (multi)
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004dd2a0 P = Class_004dd2a0((Node_004db000*)Y);
    if (!Ans)
        ;
    else if (P == begin())
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    else
        P.FUN_004dd2a0();
    if (key_compare(Key((Node_004dbbc0*)P.Mynode()), Kfn_004dbbc0()(V)))
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004ddbe0 res;
    res.FUN_004ddbe0(P, false);
    return res;
}

// FUNCTION: 0x4dbbc0
Class_004ddbe0 Class_004dce60::FUN_004dbbc0(const Pair_004dbbc0& V)
{
    Class_004ddbe0 ans = TreeInsert(V);
    return Class_004ddbe0(ans.field_0, ans.field_4);
}

// The out-of-line tree insert for the allocator's free-block map, shaped like
// std::_Tree<...>::_Insert from MSVC 5's <xtree> but written out by hand.
//
// Under the outer std::_Lockit a 0x18-byte node is carved from the pool
// (0x4ddd70), the pair is placement-new'd into it, the map's size is bumped and
// the node is linked in. Then the red-black fixup walks up from the new node
// with a cursor z, colouring and rotating until z's parent is black or the root
// is reached, and finally blackens the root.
// FUNCTION: 0x4dce60
Class_004dd2a0 Class_004dce60::FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                            const Pair_004dbbc0* v)
{
    std::_Lockit lock;
    Node_004dbbc0* p = (Node_004dbbc0*)((Class_004ddd70*)this)->FUN_004ddd70(0x18);
    p->parent = y;
    p->color = 0;                      // red
    p->left = (Node_004dbbc0*)DAT_00528a54;
    p->right = (Node_004dbbc0*)DAT_00528a54;
    new ((void*)&p->value) Pair_004dbbc0(*v);
    ++size;

    // A positive disjunction through the bool comparator: keeps the left-child block as the then-part.
    if (y == head || x != DAT_00528a54 || key_compare(v->offset, y->value.offset)) {
        y->left = p;
        // The empty-tree case comes first and updates head->right, not head->left.
        if (y == head) {
            head->parent = p;          // the root
            head->right = p;           // the rightmost node
        } else if (y == head->left) {
            head->left = p;            // the leftmost node
        }
    } else {
        y->right = p;
        if (y == head->right)
            head->right = p;
    }

    Node_004dbbc0* z = p;
    // Explicit break, and z->parent->... re-read from the cursor: parent/grandparent locals cost a register.
    while (z != head->parent) {
        if (z->parent->color != 0)
            break;

        if (z->parent == z->parent->parent->left) {
            Node_004dbbc0* u = z->parent->parent->right;
            if (u->color == 0) {
                // Red uncle: recolour and carry on two levels up.
                z->parent->color = 1;
                u->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    Lrotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                Rrotate(z->parent->parent);
            }
        } else {
            Node_004dbbc0* u = z->parent->parent->left;
            if (u->color == 0) {
                z->parent->color = 1;
                u->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    Rrotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                Lrotate(z->parent->parent);
            }
        }
    }
    head->parent->color = 1;
    return Class_004dd2a0((Node_004db000*)p);
}

// ---- from src/debug/debug_lib_4dbd00.cpp --------------------

// FUNCTION: 0x4dbd00
ConstIter_004dbd00 Class_004dbd00::FUN_004dbd00(ConstIter_004dbd00 it)
{
    return tree.FUN_004dc130((Iter_004dbd00&)it);
}

// ---- from src/debug/debug_lib_4dbd20.cpp --------------------

// FUNCTION: 0x4dbd20
Iter_004dbd20 Class_004dbd20::FUN_004dbd20(const unsigned int& kv)
{
    return Iter_004dbd20(Ubound_004dbd20(this, kv));
}

// ---- from src/debug/debug_lib_4dbd80.cpp --------------------

// FUNCTION: 0x4dbd80
Class_004dbe10 Class_004dbe10::FUN_004dbd80(int)
{
    Class_004dbe10 tmp = *this;
    Inc();
    return tmp;
}

// Tree iterator operator--(int): copy the iterator, step it to the in-order
// predecessor and return the copy. DAT_00528a54 is the tree's _Nil node.
// FUNCTION: 0x4dbe10
Class_004dbe10 Class_004dbe10::FUN_004dbe10(int)
{
    Class_004dbe10 tmp = *this;
    Dec();
    return tmp;
}

// ---- from src/debug/debug_lib_4dbeb0.cpp --------------------

// 0x4db450's inlined Tail calls this out of line in the original.
#pragma auto_inline(off)
// FUNCTION: 0x4dbeb0
int* Class_004dbeb0::FUN_004dbeb0(int* param_1)
{
    *param_1 = *field_4;
    return param_1;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dbec0.cpp --------------------

// FUNCTION: 0x4dbec0
Class_004ddbe0 Class_004dbec0::FUN_004dbec0(Pair_004dbec0* p)
{
    // The pair is returned by value (no out parameter) and _Insert returns its
    // iterator by value too; a &p result slot would make p address-taken.
    Node_004db000* y = head;
    bool less = true;
    Node_004db000* x = y->parent;
    Class_004dd2a0 it2;
    Class_004dd2a0 it;
    {
        std::_Lockit lock;
        while (x != DAT_00528a54) {
            y = x;
            less = p->offset < x->value.offset;
            x = less ? x->left : x->right;
        }
    }
    if (rebuild) {
        {
            std::_Lockit lock;
            it = Class_004dd2a0((Node_004db000*)((Class_004ddc00*)this)->FUN_004ddc00((int)y, 0));
            Node_004db000* z = it.ptr;
            z->left = (Node_004db000*)DAT_00528a54;
            z->right = (Node_004db000*)DAT_00528a54;
            new ((void*)&z->value) Pair_004dbec0(*p);
            size++;
            if (y == head || x != DAT_00528a54 || key_compare(p->offset, y->value.offset)) {
                y->left = z;
                if (y == head) {
                    head->parent = z;
                    head->right = z;
                } else if (y == head->left) {
                    head->left = z;
                }
            } else {
                y->right = z;
                if (y == head->right) {
                    head->right = z;
                }
            }
            for (Node_004db000* q = z; q != head->parent && q->parent->color == 0; ) {
                if (q->parent == q->parent->parent->left) {
                    Node_004db000* w = q->parent->parent->right;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->right) {
                            q = q->parent;
                            ((Class_004dd150*)this)->FUN_004dd150(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd1f0*)this)->FUN_004dd1f0(q->parent->parent);
                    }
                } else {
                    Node_004db000* w = q->parent->parent->left;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->left) {
                            q = q->parent;
                            ((Class_004dd1f0*)this)->FUN_004dd1f0(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd150*)this)->FUN_004dd150(q->parent->parent);
                    }
                }
            }
            head->parent->color = 1;
        }
        return Class_004ddbe0(it, 1);
    }
    it2 = Class_004dd2a0(y);
    if (less) {
        if (Class_004dd2a0(y) == Begin())
            return Class_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, p), 1);
        it2.FUN_004dd2a0();
    }
    if (key_compare(it2.ptr->value.offset, p->offset))
        return Class_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, p), 1);
    return Class_004ddbe0(it2, 0);
}

// ---- from src/debug/debug_lib_4dc130.cpp --------------------

// FUNCTION: 0x4dc130 ?FUN_004dc130@Class_004dc130@@QAE?AVClass_004dd340@@V2@@Z
Class_004dd340 Class_004dc130::FUN_004dc130(Class_004dd340 _P)
{
    Node_004db000* _X;
    Node_004db000* _Y = (_P++)._Mynode();
    Node_004db000* _Z = _Y;
    std::_Lockit _Lk;
    if (Left(_Y) == DAT_00528a54)
        _X = Right(_Y);
    else if (Right(_Y) == DAT_00528a54)
        _X = Left(_Y);
    else
        _Y = Min(Right(_Y)), _X = Right(_Y);
    if (_Y != _Z) {
        Parent(Left(_Z)) = _Y;
        Left(_Y) = Left(_Z);
        if (_Y == Right(_Z))
            Parent(_X) = _Y;
        else {
            Parent(_X) = Parent(_Y);
            Left(Parent(_Y)) = _X;
            Right(_Y) = Right(_Z);
            Parent(Right(_Z)) = _Y;
        }
        if (Root() == _Z)
            Root() = _Y;
        else if (Left(Parent(_Z)) == _Z)
            Left(Parent(_Z)) = _Y;
        else
            Right(Parent(_Z)) = _Y;
        Parent(_Y) = Parent(_Z);
        std::swap(Color(_Y), Color(_Z));
        _Y = _Z;
    } else {
        Parent(_X) = Parent(_Y);
        if (Root() == _Z)
            Root() = _X;
        else if (Left(Parent(_Z)) == _Z)
            Left(Parent(_Z)) = _X;
        else
            Right(Parent(_Z)) = _X;
        if (Lmost() != _Z)
            ;
        else if (Right(_Z) == DAT_00528a54)
            Lmost() = Parent(_Z);
        else
            Lmost() = Min(_X);
        if (Rmost() != _Z)
            ;
        else if (Left(_Z) == DAT_00528a54)
            Rmost() = Parent(_Z);
        else
            Rmost() = Max(_X);
    }
    if (Color(_Y) == _Black) {
        while (_X != Root() && Color(_X) == _Black)
            if (_X == Left(Parent(_X))) {
                Node_004db000* _W = Right(Parent(_X));
                if (Color(_W) == _Red) {
                    Color(_W) = _Black;
                    Color(Parent(_X)) = _Red;
                    Lrotate(Parent(_X));
                    _W = Right(Parent(_X));
                }
                if (Color(Left(_W)) == _Black && Color(Right(_W)) == _Black) {
                    Color(_W) = _Red;
                    _X = Parent(_X);
                } else {
                    if (Color(Right(_W)) == _Black) {
                        Color(Left(_W)) = _Black;
                        Color(_W) = _Red;
                        Rrotate(_W);
                        _W = Right(Parent(_X));
                    }
                    Color(_W) = Color(Parent(_X));
                    Color(Parent(_X)) = _Black;
                    Color(Right(_W)) = _Black;
                    Lrotate(Parent(_X));
                    break;
                }
            } else {
                Node_004db000* _W = Left(Parent(_X));
                if (Color(_W) == _Red) {
                    Color(_W) = _Black;
                    Color(Parent(_X)) = _Red;
                    Rrotate(Parent(_X));
                    _W = Left(Parent(_X));
                }
                if (Color(Right(_W)) == _Black && Color(Left(_W)) == _Black) {
                    Color(_W) = _Red;
                    _X = Parent(_X);
                } else {
                    if (Color(Left(_W)) == _Black) {
                        Color(Right(_W)) = _Black;
                        Color(_W) = _Red;
                        Lrotate(_W);
                        _W = Left(Parent(_X));
                    }
                    Color(_W) = Color(Parent(_X));
                    Color(Parent(_X)) = _Black;
                    Color(Left(_W)) = _Black;
                    Rrotate(Parent(_X));
                    break;
                }
            }
        Color(_X) = _Black;
    }
    Destval(_Value(_Y));
    Freenode(_Y);
    --size;
    return (_P);
}

// ---- from src/debug/debug_lib_4dc620.cpp --------------------

// FUNCTION: 0x4dc620
Iter_004dc620 Class_004dc620::FUN_004dc620(const unsigned int& kv)
{
    Iter_004dc620 y;
    {
        std::_Lockit lock;
        Node_004db000* x = head->parent;
        y.ptr = head;
        while (x != DAT_00528a54)
            if (key_compare(kv, x->value.offset))
                y.ptr = x, x = x->left;
            else
                x = x->right;
    }
    return y;
}

// ---- from src/debug/debug_lib_4dc680.cpp --------------------

// FUNCTION: 0x4dc680
Class_004ddbe0 Class_004dc680::FUN_004dc680(Pair_004dc680* p)
{
    Node_004daa30* y = head;
    bool less = true;
    // Read before the first _Lockit.
    Node_004daa30* x = y->parent;
    Class_004dd820 it2;
    Class_004dd820 it;
    {
        std::_Lockit lock;
        while (x != DAT_00528a50) {
            y = x;
            less = p->key < x->value.base;
            x = less ? x->left : x->right;
        }
    }
    if (rebuild) {
        {
            std::_Lockit lock;
            it = Class_004dd820((Node_004daa30*)((Class_004ddce0*)this)->FUN_004ddce0((int)y, 0));
            Node_004daa30* z = it.ptr;
            z->left = (Node_004daa30*)DAT_00528a50;
            z->right = (Node_004daa30*)DAT_00528a50;
            new ((void*)&z->value) Pair_004dc680(*p);
            size++;
            if (y == head || x != DAT_00528a50 || key_compare(p->key, y->value.base)) {
                y->left = z;
                if (y == head) {
                    head->parent = z;
                    head->right = z;
                } else if (y == head->left) {
                    head->left = z;
                }
            } else {
                y->right = z;
                if (y == head->right) {
                    head->right = z;
                }
            }
            for (Node_004daa30* q = z; q != head->parent && q->parent->color == 0; ) {
                if (q->parent == q->parent->parent->left) {
                    Node_004daa30* w = q->parent->parent->right;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->right) {
                            q = q->parent;
                            ((Class_004dd710*)this)->FUN_004dd710(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd770*)this)->FUN_004dd770(q->parent->parent);
                    }
                } else {
                    Node_004daa30* w = q->parent->parent->left;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->left) {
                            q = q->parent;
                            ((Class_004dd770*)this)->FUN_004dd770(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd710*)this)->FUN_004dd710(q->parent->parent);
                    }
                }
            }
            head->parent->color = 1;
        }
        // Outside the _Lockit scope: the pair is built after ~_Lockit.
        return Class_004ddbe0(it, 1);
    }
    it2 = Class_004dd820(y);
    if (less) {
        if (Class_004dd820(y) == Begin())
            return Class_004ddbe0(*((Class_004dd430*)this)->FUN_004dd430((Class_004dd820*)&p, x, y, p), 1);
        it2.FUN_004dd820();
    }
    if (key_compare(it2.ptr->value.base, p->key))
        return Class_004ddbe0(*((Class_004dd430*)this)->FUN_004dd430((Class_004dd820*)&p, x, y, p), 1);
    return Class_004ddbe0(it2, 0);
}

// ---- from src/debug/debug_lib_4dc910.cpp --------------------

// FUNCTION: 0x4dc910
Iter_004dc910 Class_004dc910::erase(Iter_004dc910 _P)
{
    Node_004dc910* _X;
    Node_004dc910* _Y = (_P++)._Mynode();
    Node_004dc910* _Z = _Y;
    std::_Lockit _Lk;
    if (_Y->_Left == DAT_00528a50)
        _X = _Y->_Right;
    else if (_Y->_Right == DAT_00528a50)
        _X = _Y->_Left;
    else
        _Y = _Min(_Y->_Right), _X = _Y->_Right;
    if (_Y != _Z)
        {
        _Z->_Left->_Parent = _Y;
        _Y->_Left = _Z->_Left;
        if (_Y == _Z->_Right)
            _X->_Parent = _Y;
        else
            {
            _X->_Parent = _Y->_Parent;
            _Y->_Parent->_Left = _X;
            _Y->_Right = _Z->_Right;
            _Z->_Right->_Parent = _Y;
            }
        if (_Root() == _Z)
            _Root() = _Y;
        else if (_Z->_Parent->_Left == _Z)
            _Z->_Parent->_Left = _Y;
        else
            _Z->_Parent->_Right = _Y;
        _Y->_Parent = _Z->_Parent;
        std::swap(_Y->_Color, _Z->_Color);
        _Y = _Z;
        }
    else
        {
        _X->_Parent = _Y->_Parent;
        if (_Root() == _Z)
            _Root() = _X;
        else if (_Z->_Parent->_Left == _Z)
            _Z->_Parent->_Left = _X;
        else
            _Z->_Parent->_Right = _X;
        if (_Lmost() != _Z)
            ;
        else if (_Z->_Right == DAT_00528a50)
            _Lmost() = _Z->_Parent;
        else
            _Lmost() = _Min(_X);
        if (_Rmost() != _Z)
            ;
        else if (_Z->_Left == DAT_00528a50)
            _Rmost() = _Z->_Parent;
        else
            _Rmost() = _Max(_X);
        }
    if (_Y->_Color == 1)
        {
        while (_X != _Root() && _X->_Color == 1)
            if (_X == _X->_Parent->_Left)
                {
                Node_004dc910* _W = _X->_Parent->_Right;
                if (_W->_Color == 0)
                    {
                    _W->_Color = 1;
                    _X->_Parent->_Color = 0;
                    _Lrotate(_X->_Parent);
                    _W = _X->_Parent->_Right;
                    }
                if (_W->_Left->_Color == 1 && _W->_Right->_Color == 1)
                    {
                    _W->_Color = 0;
                    _X = _X->_Parent;
                    }
                else
                    {
                    if (_W->_Right->_Color == 1)
                        {
                        _W->_Left->_Color = 1;
                        _W->_Color = 0;
                        _Rrotate(_W);
                        _W = _X->_Parent->_Right;
                        }
                    _W->_Color = _X->_Parent->_Color;
                    _X->_Parent->_Color = 1;
                    _W->_Right->_Color = 1;
                    _Lrotate(_X->_Parent);
                    break;
                    }
                }
            else
                {
                Node_004dc910* _W = _X->_Parent->_Left;
                if (_W->_Color == 0)
                    {
                    _W->_Color = 1;
                    _X->_Parent->_Color = 0;
                    _Rrotate(_X->_Parent);
                    _W = _X->_Parent->_Left;
                    }
                if (_W->_Right->_Color == 1 && _W->_Left->_Color == 1)
                    {
                    _W->_Color = 0;
                    _X = _X->_Parent;
                    }
                else
                    {
                    if (_W->_Left->_Color == 1)
                        {
                        _W->_Right->_Color = 1;
                        _W->_Color = 0;
                        _Lrotate(_W);
                        _W = _X->_Parent->_Left;
                        }
                    _W->_Color = _X->_Parent->_Color;
                    _X->_Parent->_Color = 1;
                    _W->_Left->_Color = 1;
                    _Rrotate(_X->_Parent);
                    break;
                    }
                }
        _X->_Color = 1;
        }
    if (_Y != 0)
        {
        *(void**)_Y = DAT_005289e0;
        DAT_005289e0 = _Y;
        }
    --_Size;
    return (_P);
}

// ---- from src/debug/debug_lib_4dce00.cpp --------------------

// FUNCTION: 0x4dce00
Iter_004dce00 Class_004dce00::FUN_004dce00(const unsigned int& key)
{
    Iter_004dce00 p = Iter_004dce00((Node_004daa30*)((Class_004ddc90*)this)->FUN_004ddc90(key));
    return (p == End() || compare(key, p.ptr->value.base)) ? End() : p;
}

// ---- from src/debug/debug_lib_4dd150.cpp --------------------

// FUNCTION: 0x4dd150
void Class_004dd150::FUN_004dd150(Node_004dd150* x)
{
    std::_Lockit lock;
    Node_004dd150* y = x->right;
    x->right = y->left;
    if (y->left != DAT_00528a54)
        y->left->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// ---- from src/debug/debug_lib_4dd1b0.cpp --------------------

// 0x4db1c0 calls this out of line in the original (Inc repeats the loop
// inline only where the original did).
#pragma auto_inline(off)
// FUNCTION: 0x4dd1b0
Node_004dd1b0* __cdecl FUN_004dd1b0(Node_004dd1b0* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54)
        p = p->left;
    return p;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd1f0.cpp --------------------

// FUNCTION: 0x4dd1f0
void Class_004dd1f0::FUN_004dd1f0(Node_004dd1f0* x)
{
    std::_Lockit lock;
    Node_004dd1f0* y = x->left;
    x->left = y->right;
    if (y->right != DAT_00528a54)
        y->right->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// ---- from src/debug/debug_lib_4dd250.cpp --------------------

// FUNCTION: 0x4dd250
Node_004dd250* Class_004dd250::FUN_004dd250(const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dd250* x = head->parent;
    Node_004dd250* y = head;
    while (x != DAT_00528a54)
        if (key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}

// ---- from src/debug/debug_lib_4dd2a0.cpp --------------------

// The original calls this out of line from 0x4daa30 and 0x4db1c0;
// their files had no definition to inline.
#pragma auto_inline(off)
// FUNCTION: 0x4dd2a0
void Class_004dd2a0::FUN_004dd2a0()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_00528a54) {
        ptr = Max_004dd2a0(ptr->left);
    } else {
        Node_004db000* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd340.cpp --------------------

// FUNCTION: 0x4dd340
void Class_004dd340::FUN_004dd340()
{
    std::_Lockit lock;
    if (ptr->right != DAT_00528a54)
        ptr = Min(ptr->right);
    else {
        Node_004db000* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}

// ---- from src/debug/debug_lib_4dd3d0.cpp --------------------

// 0x4dbae0's inlined find calls this out of line in the original; its
// file had no definition to inline.
#pragma auto_inline(off)
// FUNCTION: 0x4dd3d0
Iter_004dd3d0 Class_004dd3d0::FUN_004dd3d0(const unsigned int& key)
{
    return Iter_004dd3d0(Lbound(key));
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd430.cpp --------------------

// FUNCTION: 0x4dd430
Class_004dd2a0 Class_004dd430::FUN_004dd430(Node_004dd430* x, Node_004dd430* y,
                                            const Pair_004dd430& v)
{
    std::_Lockit lock;
    Node_004dd430* z = (Node_004dd430*)
                       ((Class_004dddf0*)this)->FUN_004dddf0(0x40);
    z->parent = y;
    z->color = 0;
    z->left = (Node_004dd430*)DAT_00528a50;
    z->right = (Node_004dd430*)DAT_00528a50;
    new ((void*)&z->value) Pair_004dd430(v);
    size++;
    if (y == head || x != DAT_00528a50 || key_compare(v.key, y->value.key)) {
        y->left = z;
        if (y == head) {
            head->parent = z;
            head->right = z;
        } else if (y == head->left) {
            head->left = z;
        }
    } else {
        y->right = z;
        if (y == head->right) {
            head->right = z;
        }
    }
    for (x = z; x != head->parent && x->parent->color == 0; ) {
        if (x->parent == x->parent->parent->left) {
            Node_004dd430* w = x->parent->parent->right;
            if (w->color == 0) {
                x->parent->color = 1;
                w->color = 1;
                x->parent->parent->color = 0;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    Lrotate(x);
                }
                x->parent->color = 1;
                x->parent->parent->color = 0;
                Rrotate(x->parent->parent);
            }
        } else {
            Node_004dd430* w = x->parent->parent->left;
            if (w->color == 0) {
                x->parent->color = 1;
                w->color = 1;
                x->parent->parent->color = 0;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    Rrotate(x);
                }
                x->parent->color = 1;
                x->parent->parent->color = 0;
                Lrotate(x->parent->parent);
            }
        }
    }
    head->parent->color = 1;
    return Class_004dd2a0((Node_004db000*)z);
}

// ---- from src/debug/debug_lib_4dd710.cpp --------------------

// FUNCTION: 0x4dd710
void Class_004dd710::FUN_004dd710(Node_004dd710* x)
{
    std::_Lockit lock;
    Node_004dd710* y = x->right;
    x->right = y->left;
    if (y->left != DAT_00528a50)
        y->left->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// ---- from src/debug/debug_lib_4dd770.cpp --------------------

// FUNCTION: 0x4dd770
void Class_004dd770::FUN_004dd770(Node_004dd770* x)
{
    std::_Lockit lock;
    Node_004dd770* y = x->left;
    x->left = y->right;
    if (y->right != DAT_00528a50)
        y->right->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// ---- from src/debug/debug_lib_4dd7d0.cpp --------------------

// The original calls this out of line from 0x4daa30; its file had no
// definition to inline.
#pragma auto_inline(off)
// FUNCTION: 0x4dd7d0
Node_004dd7d0* Class_004dd7d0::FUN_004dd7d0(const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dd7d0* x = head->parent;
    Node_004dd7d0* y = head;
    while (x != DAT_00528a50)
        if (key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd820.cpp --------------------

// FUNCTION: 0x4dd820
void Class_004dd820::FUN_004dd820()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_00528a50) {
        ptr = Max_004dd820(ptr->left);
    } else {
        Node_004daa30* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}

// ---- from src/debug/debug_lib_4dd8c0.cpp --------------------

// FUNCTION: 0x4dd8c0 ?insert@?$vector@UElem_004dd8c0@@VAlloc_004dd8c0@@@std@@QAEXPAUElem_004dd8c0@@IABU3@@Z
InsertFn_004dd8c0 g_insert_004dd8c0 = &Vec_004dd8c0::insert;

// ---- from src/debug/debug_lib_4ddbe0.cpp --------------------

// FUNCTION: 0x4ddbe0
Class_004ddbe0* Class_004ddbe0::FUN_004ddbe0(int* param_1, unsigned char* param_2)
{
    Class_004ddbe0* eax = this;
    int* ecx = param_1;
    int edx = *ecx;
    unsigned char* ecx2 = (unsigned char*)param_2;
    eax->field_0.ptr = (Node_004db000*)edx;
    unsigned char dl = *ecx2;
    eax->field_4 = dl;
    return eax;
}
// The debug library's image and symbol handling, and the performance status
// dialog: the pooled tree-node allocators, the loaded-image reader (the FPO
// records and the imagehlp symbol handler), the stack walkers and the
// call-stack formatter, the system information dump, and the performance
// window's settings and dialog procedure. The files of the module's fourth
// part, gathered in address order.
// Included only for its symbol count: the functions below match at this count.
#include <math.h>
#include <windows.h>
#include <yvals.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

// A 0x18-byte pooled node, allocated 0x155 at a time (one 0x2000 block).
struct Node_004ddc00 {
    Node_004ddc00* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14
};

extern void* DAT_00528a10;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

static inline Node_004ddc00* AllocNode_004ddc00()
{
    if (DAT_00528a10 == 0) {
        Node_004ddc00* block;
        do {
            block = (Node_004ddc00*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004ddc00* head = (Node_004ddc00*)DAT_00528a10;
        for (int i = 0; i < 0x155; i++) {
            block->next = head;
            head = block;
            block++;
        }
        DAT_00528a10 = head;
    }
    Node_004ddc00* node = (Node_004ddc00*)DAT_00528a10;
    DAT_00528a10 = node->next;
    return node;
}

// A method that ignores `this`: its caller (0x4dbec0, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.

// The original calls this out of line from the file-record tree insert.
#pragma auto_inline(off)
// FUNCTION: 0x4ddc00
Node_004ddc00* Class_004ddc00::FUN_004ddc00(int param_1, int param_2)
{
    Node_004ddc00* node = AllocNode_004ddc00();
    node->field_4 = param_1;
    node->field_14 = param_2;
    return node;
}
#pragma auto_inline(on)

// std::_Tree<unsigned int, ...>::_Lbound(const key&) from MSVC 5's <xtree>,
// written out by hand: DAT_00528a50 is the tree's _Nil node, keys are
// unsigned ints compared with less<>.

struct Node_004ddc90 {
    Node_004ddc90* left;               // +0x0
    Node_004ddc90* parent;             // +0x4
    Node_004ddc90* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern void* DAT_00528a50;             // the tree's _Nil node

// The original calls this out of line from Class_004dce00::FUN_004dce00.
#pragma auto_inline(off)
// FUNCTION: 0x4ddc90
Node_004ddc90* Class_004ddc90::FUN_004ddc90(const unsigned int& key)
{
    std::_Lockit lock;
    Node_004ddc90* x = head->parent;
    Node_004ddc90* y = head;
    while (x != DAT_00528a50) {
        if (compare(x->key, key))
            x = x->right;
        else
            y = x, x = x->left;
    }
    return y;
}
#pragma auto_inline(on)

// A 0x40-byte pooled node, allocated 0x80 at a time.
struct Node_004ddce0 {
    Node_004ddce0* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x3c - 0x8];
    int field_3c;                      // +0x3c
};

extern void* DAT_005289e0;             // free list

static inline Node_004ddce0* AllocNode_004ddce0()
{
    if (DAT_005289e0 == 0) {
        Node_004ddce0* block;
        do {
            block = (Node_004ddce0*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004ddce0* head = (Node_004ddce0*)DAT_005289e0;
        for (int i = 0; i < 0x80; i++) {
            block->next = head;
            head = block;
            block++;
        }
        DAT_005289e0 = head;
    }
    Node_004ddce0* node = (Node_004ddce0*)DAT_005289e0;
    DAT_005289e0 = node->next;
    return node;
}

// A method that ignores `this`: its caller (0x4dc680, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.

// The original calls this out of line from the file-record tree insert.
#pragma auto_inline(off)
// FUNCTION: 0x4ddce0
Node_004ddce0* Class_004ddce0::FUN_004ddce0(int param_1, int param_2)
{
    Node_004ddce0* node = AllocNode_004ddce0();
    node->field_4 = param_1;
    node->field_3c = param_2;
    return node;
}
#pragma auto_inline(on)

// Pool allocator for the DAT_00528a10 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. Identical to 0x4dddf0 apart from the
// free list.

// A method that ignores `this`: its callers (0x4db610, 0x4dce60) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x18).
// The original calls this out of line from the free-block tree.
#pragma auto_inline(off)
// FUNCTION: 0x4ddd70
void* Class_004ddd70::FUN_004ddd70(unsigned int n)
{
    if (DAT_00528a10 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_00528a10;
            DAT_00528a10 = block;
            block += n;
        }
    }
    void* p = DAT_00528a10;
    DAT_00528a10 = *(void**)DAT_00528a10;
    return p;
}
#pragma auto_inline(on)

// Pool allocator for the DAT_005289e0 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. Same shape as 0x4e2b60; 0x4ddce0 has
// an inlined copy for 0x40-byte nodes.

// A method that ignores `this`: its callers (0x4da8d0, 0x4dd430) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x40).
// The original calls this out of line from the file-record tree.
#pragma auto_inline(off)
// FUNCTION: 0x4dddf0
void* Class_004dddf0::FUN_004dddf0(unsigned int n)
{
    if (DAT_005289e0 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_005289e0;
            DAT_005289e0 = block;
            block += n;
        }
    }
    void* p = DAT_005289e0;
    DAT_005289e0 = *(void**)DAT_005289e0;
    return p;
}
#pragma auto_inline(on)

// Shaped like std::_Tree<...>::const_iterator::_Inc() from MSVC 5's <xtree>
// (step an iterator to the in-order successor) under a lock object;
// DAT_00528a50 is the tree's _Nil node. Compare 0x4dd710 and 0x4ddc90.

struct Node_004dde70 {
    Node_004dde70* left;               // +0x0
    Node_004dde70* parent;             // +0x4
    Node_004dde70* right;              // +0x8
};

static inline Node_004dde70* Min_004dde70(Node_004dde70* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a50)
        p = p->left;
    return p;
}

// The original calls this out of line from the const_iterator increment.
#pragma auto_inline(off)
// FUNCTION: 0x4dde70
void Class_004dde70::FUN_004dde70()
{
    std::_Lockit lock;
    if (ptr->right != DAT_00528a50)
        ptr = Min_004dde70(ptr->right);
    else {
        Node_004dde70* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
#pragma auto_inline(on)

class Class_004e1590 {
public:
    HANDLE hFile;      // +0x0
    HANDLE hMapping;   // +0x4
    void* view;        // +0x8
    DWORD size;        // +0xc
    int state;         // +0x10

    void OpenMappedFile(const char* fileName);
};

class MappedFile {
public:
    HANDLE hFile;     // +0x0
    HANDLE hMapping;  // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    MappedFile(const char* fileName);
    void CloseMappedFile();
};

// One FPO_DATA record (see 0x4de020).
struct Fpo_004de020 {
    unsigned int offStart;             // +0x0
    unsigned int procSize;             // +0x4
    unsigned int locals;               // +0x8
    unsigned short params;             // +0xc
    unsigned short flags;              // +0xe
};

class LoadedImage : public MappedFile {
public:
    HMODULE module;                        // +0x14
    unsigned int imageBase;                // +0x18
    IMAGE_DOS_HEADER* dosHeader;           // +0x1c
    IMAGE_NT_HEADERS* ntHeaders;           // +0x20
    IMAGE_DEBUG_DIRECTORY* debugDirs;      // +0x24
    int numDebugDirs;                      // +0x28

    LoadedImage(HMODULE m);
    ~LoadedImage();
    Fpo_004de020* GetFpoRecords();
};

// The loaded-image reader of the debug library; 0x4de0a0's function-local
// static builds the one at 0x528a78 lazily.
extern LoadedImage DAT_00528a78;

struct DebugDir_004ddfa0 {
    unsigned int characteristics;      // +0x00
    unsigned int timeDateStamp;        // +0x04
    unsigned short majorVersion;       // +0x08
    unsigned short minorVersion;       // +0x0a
    unsigned int type;                 // +0x0c
    unsigned int sizeOfData;           // +0x10
    unsigned int addressOfRawData;     // +0x14
    unsigned int pointerToRawData;     // +0x18
};

class Class_004ddfe0 {
public:
    char unknown_0[0x8];
    char* base;                        // +0x08, the mapped file
    char unknown_c[0x24 - 0xc];
    DebugDir_004ddfa0* debugDirs;      // +0x24
    int numDebugDirs;                  // +0x28

    unsigned int GetFpoRecordCount();
};

class Class_004de020 {
public:
    char unknown_0[0x18];
    unsigned int imageBase;            // +0x18
    Fpo_004de020* FindFpoRecord(unsigned int address);
};

// FUNCTION: 0x4ddf00
LoadedImage::LoadedImage(HMODULE m) : MappedFile(0)
{
    char path[1000];
    module = m;
    imageBase = (unsigned int)m;
    dosHeader = (IMAGE_DOS_HEADER*)m;
    if (GetModuleFileNameA(m, path, sizeof(path)))
        path[sizeof(path) - 1] = 0;
    else
        path[0] = 0;
    ((Class_004e1590*)this)->OpenMappedFile(path);
    ntHeaders = (IMAGE_NT_HEADERS*)((char*)dosHeader + dosHeader->e_lfanew);
    numDebugDirs = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size / sizeof(IMAGE_DEBUG_DIRECTORY);
    // Count stored before debugDirs is cleared: ntHeaders is reloaded for the sum.
    debugDirs = 0;
    if (numDebugDirs)
    {
        // base local and the second test pick the registers of the sum.
        char* base = (char*)imageBase;
        if (numDebugDirs)
            debugDirs = (IMAGE_DEBUG_DIRECTORY*)((char*)base + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
    }
}

// Returns the image's FPO records (debug directory type 3,
// IMAGE_DEBUG_TYPE_FPO), or 0 if it has none.

// The original calls this out of line from 0x4de020.
#pragma auto_inline(off)
// FUNCTION: 0x4ddfa0
Fpo_004de020* LoadedImage::GetFpoRecords()
{
    if (debugDirs == 0)
        return 0;
    for (int i = 0; i < numDebugDirs; i++) {
        if (debugDirs[i].Type == IMAGE_DEBUG_TYPE_FPO)
            return (Fpo_004de020*)((char*)view + debugDirs[i].PointerToRawData);
    }
    return 0;
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4de020.
#pragma auto_inline(off)
// FUNCTION: 0x4ddfe0
unsigned int Class_004ddfe0::GetFpoRecordCount()
{
    if (debugDirs == 0)
        return 0;
    for (int i = 0; i < numDebugDirs; i++) {
        if (debugDirs[i].type == 3)
            return debugDirs[i].sizeOfData / sizeof(Fpo_004de020);
    }
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4de020
Fpo_004de020* Class_004de020::FindFpoRecord(unsigned int address)
{
    Fpo_004de020* first = ((LoadedImage*)this)->GetFpoRecords();
    if (first == 0)
        return 0;
    int n = ((Class_004ddfe0*)this)->GetFpoRecordCount();
    if (n == 0)
        return 0;
    Fpo_004de020* last = first + n;
    Fpo_004de020* mid = first + n / 2;
    unsigned int rva = address - imageBase;
    while (first + 1 != last) {
        if (rva < mid->offStart)
            last = mid;
        else
            first = mid;
        mid = first + (last - first) / 2;
    }
    if (rva >= first->offStart && rva < first->offStart + first->procSize)
        return first;
    return 0;
}

// FUNCTION: 0x4de0a0
void __stdcall FUN_004de0a0(int unused, unsigned int address)
{
    static LoadedImage table(GetModuleHandleA(0));
    ((Class_004de020*)&table)->FindFpoRecord(address);
}

// FUNCTION: 0x4de0f0
void FUN_004de0f0()
{
    DAT_00528a78.CloseMappedFile();
}

extern int GetDebugLibInstance();

// FUNCTION: 0x4de100
void __stdcall FUN_004de100(int arg1, int arg2)
{
    GetDebugLibInstance();
}

// The imagehlp entry points, resolved by LoadImageHelp (0x4de180): the
// loader, the stack walker and the call-stack formatter each use a different
// signature of the same pointer.
typedef DWORD (__stdcall *SymSetOptions_004de180)(DWORD);
typedef BOOL (__stdcall *SymInitialize_004de180)(HANDLE, char*, DWORD);
typedef void (__stdcall *SymProc_004de180)(void);
typedef BOOL (__stdcall *StackWalk_004de700)(DWORD, HANDLE, HANDLE, void*, void*, void*, void*, void*, void*);

extern char DAT_00528ad8;
extern char DAT_00528adc;
extern HMODULE DAT_00528ae0;
extern SymSetOptions_004de180 DAT_00528ad0;
extern SymInitialize_004de180 DAT_00528ab8;
extern BOOL (__stdcall* DAT_00528abc)(HANDLE);
extern StackWalk_004de700 DAT_00528ac0;
extern void* DAT_00528ac4;
extern void* DAT_00528ac8;
extern SymProc_004de180 DAT_00528acc;
extern SymProc_004de180 DAT_00528ab4;
extern SymProc_004de180 DAT_00528ad4;

// The original calls this out of line from 0x4de180.
#pragma auto_inline(off)
// FUNCTION: 0x4de110
void UnloadImageHelp()
{
    DAT_00528ad8 = 0;
    if (DAT_00528ae0) {
        DAT_00528abc(GetCurrentProcess());
        FreeLibrary(DAT_00528ae0);
    }
    DAT_00528ae0 = 0;
    DAT_00528ad0 = 0;
    DAT_00528ab8 = 0;
    DAT_00528abc = 0;
    DAT_00528ac0 = 0;
    DAT_00528ac4 = 0;
    DAT_00528ac8 = 0;
    DAT_00528acc = 0;
    DAT_00528ab4 = 0;
    DAT_00528ad4 = 0;
}
#pragma auto_inline(on)

// 0x4de180 LoadImageHelp stays in src/debug/debug_lib_4de180.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).
char __cdecl LoadImageHelp(char param);

// FUNCTION: 0x4de4c0
void FUN_004de4c0(void)
{
}

// The original calls this out of line from 0x4dea00.
#pragma auto_inline(off)
// FUNCTION: 0x4de4d0
char FUN_004de4d0()
{
    static Class_004d9fe0 lines("imagehlplines", 1, 1, "-enableimagehlplines",
                                "-disableimagehlplines", 0, 0);
    if (lines.on && LoadImageHelp(1) && (DAT_00528ab4 || DAT_00528acc))
        return 1;
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4de540
void FUN_004de540(void)
{
}

// The stack line the imagehlp lookup fills in.
struct Line_004de550 {
    DWORD SizeOfStruct;
    DWORD Key;
    DWORD LineNumber;
    DWORD FileName;
    DWORD Address;
};

typedef BOOL (__stdcall *SymGetLineFromAddr_004de550)(HANDLE, DWORD, DWORD*, Line_004de550*);

extern char DAT_00528aac;
extern HANDLE DAT_00528aa4;

// The original calls this out of line from 0x4dea00.
#pragma auto_inline(off)
// FUNCTION: 0x4de550
char __cdecl GetLineFromAddress(DWORD addr, Line_004de550* out, DWORD* err)
{
    if (DAT_00528ab4) {
        if (!(DAT_00528aac & 1)) {
            DAT_00528aac |= 1;
            DAT_00528aa4 = GetCurrentProcess();
        }
        // Aggregate-initialised inside this block, not hoisted: keeps the zero stores here.
        Line_004de550 line = { 0x14 };
        // No initialiser, zeroed after the Address store: fixes where the store lands.
        DWORD disp;
        line.Address = addr;
        disp = 0;
        if (((SymGetLineFromAddr_004de550)DAT_00528ab4)(DAT_00528aa4, addr, &disp, &line)) {
            *out = line;
            return 1;
        }
        *err = GetLastError();
        return 0;
    }
    *err = 0;
    return 0;
}
#pragma auto_inline(on)

// A chain of stack frames (each frame holds the next frame pointer and a
// return address), walked without imagehlp when a stack symbol lookup is
// unavailable.
char __cdecl IsOutsideStack(void* p, int size);
void __cdecl WalkStack(int param1, int param2, int param3, int param4,
                       int* param5, int param6, int* param7);

// FUNCTION: 0x4de600
void __cdecl WalkFrameChain(int* frame, int* stack, int eip, int skip, int* out, int max, int* count,
                          int* copy, int copyMax, int* copied)
{
    *count = 0;
    if (skip == 0) {
        out[*count] = eip;
        (*count)++;
    } else {
        skip--;
    }
    if (LoadImageHelp(0)) {
        WalkStack((int)frame, (int)stack, eip, skip + 1, out, max, count);
    } else {
        for (int* fp = frame; *count < max; fp = (int*)*fp) {
            if (IsOutsideStack(fp, 8))
                break;
            if (fp < stack)
                break;
            if (*fp <= (int)fp)
                break;
            if (fp[1] == 0)
                break;
            if (skip == 0) {
                out[*count] = fp[1];
                (*count)++;
            } else {
                skip--;
            }
        }
    }
    int i = 0;
    if (stack != 0) {
        for (i = 0; i < copyMax; i++) {
            if (IsOutsideStack(&stack[i], 4))
                break;
            copy[i] = stack[i];
        }
    }
    *copied = i;
    for (; i < copyMax; i++)
        copy[i] = 0;
}

// The STACKFRAME-ish record StackWalk fills in.
struct Frame_004de700 {
    int addrFrame;                             // +0x00
    int unknown_04;
    int flags0;                                // +0x08
    char unknown_0c[0x0c];
    int addrStack;                             // +0x18
    char unknown_1c[4];
    int flags1;                                // +0x20
    int handler;                               // +0x24
    char unknown_28[4];
    int flags2;                                // +0x2c
    char unknown_30[0x40];
};

extern char DAT_00528aa8;                     // "process handle read" flag
extern HANDLE DAT_00528ab0;                   // cached process handle

// FUNCTION: 0x4de700
void __cdecl WalkStack(int param1, int param2, int param3, int param4, int* param5, int param6, int* param7)
{
    Frame_004de700 frame;
    memset(&frame, 0, sizeof(frame));
    frame.addrFrame = param3;
    frame.flags0 = 3;
    frame.flags2 = 3;
    frame.flags1 = 3;
    frame.handler = param2;
    frame.addrStack = param1;
    if (!(DAT_00528aa8 & 1)) {
        DAT_00528aa8 |= 1;
        DAT_00528ab0 = GetCurrentProcess();
    }
    HANDLE thread = GetCurrentThread();
    while (*param7 < param6) {
        if (!DAT_00528ac0(0x14c, DAT_00528ab0, thread, &frame, 0, 0, DAT_00528ac4, DAT_00528ac8, 0)) {
            if (*param7 == 0) {
                *param7 = 1;
                param5[0] = param3;
            }
            return;
        }
        if (frame.addrStack == 0)
            return;
        if (frame.addrFrame == 0)
            return;
        if (param4 == 0) {
            param5[*param7] = frame.addrFrame;
            ++(*param7);
        } else {
            --param4;
        }
    }
}

// The original calls this out of line from 0x4de8a0.
#pragma auto_inline(off)
// FUNCTION: 0x4de810
bool __cdecl FileExists(char* dir, char* name)
{
    struct _stat st;
    char path[1000];
    strcpy(path, dir);
    strcat(path, name);
    return _stat(path, &st) == 0 ? true : false;
}
#pragma auto_inline(on)

// Builds the full path of a file that lives in the Visual C++ source tree.
// If the name has no directory part, the Visual Studio install directory is
// found once from the MSDevDir environment variable, cut down to its drive
// (everything from the first backslash on, and any ';' comment, is dropped),
// and the two usual source roots are tried in turn, MFC first, then the CRT.
// DAT_00528ae4 is the "MSDevDir already read" flag, DAT_00528ae8 the CRT root,
// DAT_00528ed0 the MFC root, DAT_005119b8 the empty default prefix.
extern char DAT_005119b8[];
extern char DAT_00528ae4;
extern char DAT_00528ae8[0x1e8];
extern char DAT_00528ed0[0x1e8];

// The original calls this out of line from 0x4dea00.
#pragma auto_inline(off)
// FUNCTION: 0x4de8a0
void __cdecl GetSourceFilePath(char* out, char* name)
{
    char* dir = DAT_005119b8;
    if (strchr(name, '\\') == 0) {
        if (!DAT_00528ae4) {
            DAT_00528ae4 = 1;
            char* msdev = getenv("MSDevDir");
            if (msdev) {
                strcpy(DAT_00528ae8, msdev);
                char* p = strchr(DAT_00528ae8, ';');
                if (p)
                    *p = 0;
                p = strrchr(DAT_00528ae8, '\\');
                if (p)
                    *p = 0;
                strcpy(DAT_00528ed0, DAT_00528ae8);
                strcat(DAT_00528ae8, "\\vc\\crt\\src\\");
                strcat(DAT_00528ed0, "\\vc\\mfc\\src\\");
            }
        }
        if (FileExists(DAT_00528ed0, name)) {
            dir = DAT_00528ed0;
        } else if (FileExists(DAT_00528ae8, name)) {
            dir = DAT_00528ae8;
        }
    }
    sprintf(out, "%s%s", dir, name);
}
#pragma auto_inline(on)

// The imagehlp symbol record FormatCallStack fills in.
struct Sym_004dea00 {
    unsigned long SizeOfStruct;
    unsigned long Address;
    unsigned long Size;
    unsigned long Flags;
    unsigned long MaxNameLength;
    char Name[0x204];
};

typedef BOOL (__stdcall *SymFn_004dea00)(HANDLE, unsigned long, int*, void*);
typedef DWORD (__stdcall *UnDecFn_004dea00)(char*, char*, DWORD, DWORD);

// FUNCTION: 0x4dea00
void __cdecl FormatCallStack(char* dest, int space, int per, int n, unsigned long* addrs)
{
    int i = 0;
    space--;
    char lines = FUN_004de4d0();
    int width;
    int disp;
    DWORD err;
    Line_004de550 line;
    Sym_004dea00 sym;
    char path[1000];
    char undec[2000];

    strcpy(dest, lines ? "Call stack:\n" : "Call stack: ");
    width = 15;
    space -= strlen(dest);
    dest += strlen(dest);
    if (lines)
        width = 800;

    char found = 0;
    for (; i < n; i++) {
        if (space <= width)
            break;
        if (lines) {
            if (GetLineFromAddress(addrs[i], &line, &err)) {
                GetSourceFilePath(path, (char*)line.FileName);
                sprintf(dest, "%s(%d) : %08lX", path, line.LineNumber, addrs[i]);
                found = 1;
            } else {
                sprintf(dest, "%08lX", addrs[i]);
            }
            sym.SizeOfStruct = 0x218;
            sym.MaxNameLength = 0x200;
            disp = 0;
            if (((SymFn_004dea00)DAT_00528acc)(GetCurrentProcess(), addrs[i], &disp, &sym)) {
                char* str;
                if (DAT_00528ad4 && ((UnDecFn_004dea00)DAT_00528ad4)(sym.Name, undec, 0x7d0, 0) > 0) {
                    str = undec;
                } else {
                    if (strcmp(sym.Name, "??2@YAPAXI@Z") == 0 ||
                        strcmp(sym.Name, "??2@YAPAXIPBDH@Z") == 0)
                        strcat(sym.Name, " - operator new");
                    str = sym.Name;
                }
                char* end = dest + strlen(dest);
                sprintf(end, " - %s + %d", str, disp);
                found = 1;
            }
            // One strcat per branch, no shared `sep` variable: moves the temp register rotation.
            if (found)
                strcat(dest, "\n");
            else if (i == n - 1 || i % per == per - 1)
                strcat(dest, "\n");
            else
                strcat(dest, " ");
        } else {
            sprintf(dest, "%08lX", addrs[i]);
            if (i == n - 1 || i % per == per - 1)
                strcat(dest, "\n");
            else
                strcat(dest, " ");
        }
        space -= strlen(dest);
        dest += strlen(dest);
    }
}

extern char* DAT_0050d4d0;
int __cdecl GetStackLow(void);
void* GetStackHigh(void);

// FUNCTION: 0x4ded60
void __cdecl FormatSystemInfo(char* dest, int destLen)
{
    WORD fatDate;
    time_t now;
    WORD fatTime;
    DWORD userNameSize;
    FILETIME ft;
    MEMORYSTATUS memStatus;
    SYSTEM_INFO sysInfo;
    struct tm gmTimeCopy;
    struct tm localTimeCopy;
    char userName[300];
    char buf[2000];
    char exeName[1000];
    char* p;

    buf[0] = DAT_005119b8[0];
    memset(buf + 1, 0, 1999);

    now = time(NULL);
    localTimeCopy = *localtime(&now);
    p = buf + strlen(buf);
    sprintf(p, "Time: %s", asctime(&localTimeCopy));

    userNameSize = 300;
    if (GetUserNameA(userName, &userNameSize) == 0) {
        strcpy(userName, "unknown user");
    }

    char* machine = getenv("computername");
    if (machine == NULL) {
        machine = "unknown machine";
    }

    if (GetModuleFileNameA(NULL, exeName, 1000) == 0) {
        strcpy(exeName, "Unknown");
    }

    sprintf(buf + strlen(buf), "%s, run by %s on %s\n", exeName, userName, machine);

    HANDLE hFile = CreateFileA(exeName, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != 0) {
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (GetFileTime(hFile, NULL, NULL, &ft)) {
            if (FileTimeToLocalFileTime(&ft, &ft)) {
                if (FileTimeToDosDateTime(&ft, &fatDate, &fatTime)) {
                    p = buf + strlen(buf);
                    sprintf(p,
                        "Executable is %d bytes long and dated %d/%d/%d %02d:%02d:%02d\n",
                        fileSize, (fatDate >> 5) & 0xf, fatDate & 0x1f, (fatDate >> 9) + 1980,
                        fatTime >> 11, (fatTime >> 5) & 0x3f, (fatTime & 0x1f) * 2);
                }
            }
        }
        CloseHandle(hFile);
    }

    HANDLE hMod = GetModuleHandleA(NULL);
    // NT header named and used twice: forces it into a register.
    IMAGE_NT_HEADERS* pNT = (IMAGE_NT_HEADERS*)((char*)hMod + ((IMAGE_DOS_HEADER*)hMod)->e_lfanew);
    gmTimeCopy = *gmtime((time_t*)&pNT->FileHeader.TimeDateStamp);
    p = buf + strlen(buf);
    sprintf(p, "UTC link time: %08lx - %s", pNT->FileHeader.TimeDateStamp, asctime(&gmTimeCopy));

    p = buf + strlen(buf);
    sprintf(p, "Library version %d. Library date %s\n",
            996, DAT_0050d4d0 + strlen("Library date stamp: "));

    GetSystemInfo(&sysInfo);
    if (sysInfo.dwNumberOfProcessors > 1) {
        // Then arm keeps the p temporary, else arm calls sprintf directly: not interchangeable.
        p = buf + strlen(buf);
    sprintf(p, "%d processors\n", sysInfo.dwNumberOfProcessors);
    } else {
        sprintf(buf + strlen(buf), "1 processor\n");
    }

    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatus(&memStatus);
    p = buf + strlen(buf);
    sprintf(p, "%d MBytes physical memory\n",
            (memStatus.dwTotalPhys + 900000) >> 20);

    p = buf + strlen(buf);
    sprintf(p, "Stack goes from %08lX to %08lX\n",
            GetStackLow(), GetStackHigh());

    strncpy(dest, buf, destLen);
    dest[destLen - 1] = '\0';
}

// FUNCTION: 0x4df160
void FUN_004df160()
{
    static Class_004d9fe0 fussy("fpufussy", 1, 0, "-fpufussy", "-fpunofussy", 0, 0);
    if (fussy.on)
        _controlfp(0, _EM_ZERODIVIDE | _EM_INVALID);
    else
        _controlfp(_EM_ZERODIVIDE | _EM_INVALID, _EM_ZERODIVIDE | _EM_INVALID);
}

// FUNCTION: 0x4df1d0
void FUN_004df1d0(void)
{
}

// The performance status window and the name table it shows. The singleton at
// 0x5292d0 is the Class_004df1e0 0x4df1e0 builds and 0x4dfd10 hands out; its
// name map at +0x21c is the same tree the global name table uses, so the two
// share the node and iterator types below. 0x4dfd10 and 0x4dfd50 stay in
// their own files: the singleton's atexit term function is that file's first
// static, and 0x4dfd50 is the name (_$E2) the placement build knows.

extern int DAT_00529dcc;
struct Entry_004df590;
extern Entry_004df590* DAT_00529df8;

extern double __cdecl GetTimeSeconds();
void InitPerformanceEvents();

// The map value: the name key and its 500-byte text.
struct Value_004df590 {
    const char* name;                  // +0x00
    char text[500];                    // +0x04
};

struct Node_004df590 {
    Node_004df590* left;               // +0x0
    Node_004df590* parent;             // +0x4
    Node_004df590* right;              // +0x8
    Value_004df590 value;              // +0xc
};

// The map's node, as the erase machinery sees it: the 0x1f8-byte value and
// the red/black colour at +0x204.
struct Node_004dfea0 {
    Node_004dfea0* _Left;              // +0x0
    Node_004dfea0* _Parent;            // +0x4
    Node_004dfea0* _Right;             // +0x8
    char _Value[0x1f8];                // +0xc
    int _Color;                        // +0x204  (0 = _Red, 1 = _Black)
};

extern void* DAT_005292c4;             // tree _Nil
extern void* DAT_00529e58;             // node free list
extern unsigned int DAT_00529500;      // tree _Nilrefs

// The tree's in-order successor, as 0x4df590 walks the name map.
struct Iterator_004df590 {
    Node_004df590* ptr;
    Iterator_004df590(Node_004df590* p) : ptr(p) {}
    bool operator==(const Iterator_004df590& other) const { return ptr == other.ptr; }
    bool operator!=(const Iterator_004df590& other) const { return !(*this == other); }
};

// The tree iterator; passed by value and returned by value, so the caller
// supplies a hidden return buffer.
struct Node_004e0450;
class Class_004e0450 {
public:
    Node_004e0450* ptr;                // +0x0

    void FUN_004e0450();               // _Inc
    Class_004e0450& operator++() { FUN_004e0450(); return *this; }
    Class_004e0450 operator++(int)
    {
        Class_004e0450 t = *this;
        ++*this;
        return t;
    }
    Node_004dfea0* _Mynode() const { return (Node_004dfea0*)ptr; }
    bool operator==(const Class_004e0450& x) const { return ptr == x.ptr; }
    bool operator!=(const Class_004e0450& x) const { return !(*this == x); }
};

// std::_Tree<...>::_Erase(_Nodeptr): frees a whole subtree.
struct Node_004e03f0;
struct Node_004e18c0;
class Class_004e03f0 {
public:
    void FUN_004e03f0(Node_004e03f0* x);
    void FUN_004e03f0(Node_004e18c0* x);   // 4e04e0's view
};

// std::_Tree<...>::erase(iterator): erases one node, returns the next.
class Iter_004e18c0;
class Class_004dfea0 {
public:
    char _Alnod[4];                    // +0x0
    Node_004dfea0* _Head;              // +0x4
    char _Multi;                       // +0x8
    char pad_9[3];
    unsigned int _Size;                // +0xc

    static int& _Color(Node_004dfea0* _P) { return _P->_Color; }
    static Node_004dfea0*& _Left(Node_004dfea0* _P) { return _P->_Left; }
    static Node_004dfea0*& _Parent(Node_004dfea0* _P) { return _P->_Parent; }
    static Node_004dfea0*& _Right(Node_004dfea0* _P) { return _P->_Right; }
    Node_004dfea0*& _Root() const { return _Head->_Parent; }
    Node_004dfea0*& _Lmost() const { return _Head->_Left; }
    Node_004dfea0*& _Rmost() const { return _Head->_Right; }

    static Node_004dfea0* _Min(Node_004dfea0* _P)
    {
        std::_Lockit _Lk;
        while (_Left(_P) != DAT_005292c4)
            _P = _Left(_P);
        return (_P);
    }
    static Node_004dfea0* _Max(Node_004dfea0* _P)
    {
        std::_Lockit _Lk;
        while (_Right(_P) != DAT_005292c4)
            _P = _Right(_P);
        return (_P);
    }
    void _Lrotate(Node_004dfea0* _X)
    {
        std::_Lockit _Lk;
        Node_004dfea0* _Y = _Right(_X);
        _Right(_X) = _Left(_Y);
        if (_Left(_Y) != DAT_005292c4)
            _Parent(_Left(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Left(_Parent(_X)))
            _Left(_Parent(_X)) = _Y;
        else
            _Right(_Parent(_X)) = _Y;
        _Left(_Y) = _X;
        _Parent(_X) = _Y;
    }
    void _Rrotate(Node_004dfea0* _X)
    {
        std::_Lockit _Lk;
        Node_004dfea0* _Y = _Left(_X);
        _Left(_X) = _Right(_Y);
        if (_Right(_Y) != DAT_005292c4)
            _Parent(_Right(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Right(_Parent(_X)))
            _Right(_Parent(_X)) = _Y;
        else
            _Left(_Parent(_X)) = _Y;
        _Right(_Y) = _X;
        _Parent(_X) = _Y;
    }
    static void _Freenode(Node_004dfea0* _P)
    {
        if (_P != 0) {
            *(void**)_P = DAT_00529e58;
            DAT_00529e58 = _P;
        }
    }

    Class_004e0450 FUN_004dfea0(Class_004e0450 _P);
    Iter_004e18c0 FUN_004dfea0(Iter_004e18c0 it);   // 4e04e0's view
};

// The name map: comparator and allocator bytes, _Head, _Multi, _Size and the
// "changed" flag at +0x10. It is the map member of the global NameTable and
// of the performance singleton; its destructor is the pooled tree teardown
// the atexit term function 0x4dfd50 runs.
struct Map_004df590 {
    char compare;                      // +0x0
    char allocator;                    // +0x1
    Node_004df590* head;               // +0x4
    char multi;                        // +0x8
    unsigned int size;                 // +0xc
    char changed;                      // +0x10

    Node_004df590*& _Root() const { return head->parent; }
    Node_004df590*& _Lmost() const { return head->left; }
    Node_004df590*& _Rmost() const { return head->right; }
    unsigned int Size() const { return size; }
    Class_004e0450 begin() const { Class_004e0450 i; i.ptr = (Node_004e0450*)head->left; return i; }
    Class_004e0450 end() const { Class_004e0450 i; i.ptr = (Node_004e0450*)head; return i; }

    // Shaped exactly like the MSVC 5 STL, including the dead `_F != begin()` test.
    Class_004e0450 erase(Class_004e0450 _F, Class_004e0450 _L)
    {
        if (Size() == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                ((Class_004dfea0*)this)->FUN_004dfea0(_F++);
            return _F;
        } else {
            std::_Lockit Lk;
            ((Class_004e03f0*)this)->FUN_004e03f0((Node_004e03f0*)_Root());
            _Root() = (Node_004df590*)DAT_005292c4;
            size = 0;
            _Lmost() = head;
            _Rmost() = head;
            return begin();
        }
    }

    ~Map_004df590()
    {
        erase(begin(), end());
        // Loaded nodes kept in locals (h, n) for the free-list push: re-reading shifts registers.
        Node_004df590* h = head;
        if (h != 0) {
            *(void**)h = DAT_00529e58;
            DAT_00529e58 = h;
        }
        head = 0, size = 0;
        {
            std::_Lockit Lk;
            if (--DAT_00529500 == 0) {
                Node_004df590* n = (Node_004df590*)DAT_005292c4;
                if (n != 0) {
                    *(void**)n = DAT_00529e58;
                    DAT_00529e58 = n;
                }
                DAT_005292c4 = 0;
            }
        }
    }
};

// The global name table (0x4e17c0 builds it, 0x4e1a90 hands it out).
extern void* DAT_00529e58;             // node free list
extern unsigned int DAT_00529500;      // tree _Nilrefs
extern void (*DAT_005289bc)();         // out-of-memory handler

struct Less_004e17c0 {
    bool operator()(const char* a, const char* b) const
    {
        return a != b && strcmp(a, b) < 0;
    }
};

struct Value_004e17c0 {
    char text[500];
};

typedef std::pair<const char*, Value_004e17c0> ValueType_004e17c0;

// The pooled allocator; its _Charalloc is the out-of-line 0x4e2b60.
class Class_004e2b60 {
public:
    typedef ValueType_004e17c0 value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type n, const void* = 0)
    {
        return (pointer)FUN_004e2b60(n * sizeof(value_type));
    }
    void deallocate(void* p, size_type)
    {
        if (p != 0) {
            *(void**)p = DAT_00529e58;
            DAT_00529e58 = p;
        }
    }
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }

    void* FUN_004e2b60(size_type n);   // out of line at 0x4e2b60
};

typedef std::map<const char*, Value_004e17c0, Less_004e17c0, Class_004e2b60>
    Map_004e17c0;

class NameTable {
public:
    Map_004e17c0 names;                // +0x0
    bool changed;                      // +0x10

    NameTable();
};

NameTable* GetNameTable();

class CriticalSection {
public:
    CRITICAL_SECTION cs;
};

class CritSec_004e1ac0;
CritSec_004e1ac0* FUN_004e1ac0();

struct Node_004e18c0 {
    Node_004e18c0* left;               // +0x0
    Node_004e18c0* parent;             // +0x4
    Node_004e18c0* right;              // +0x8
};

class Iter_004e18c0 {
public:
    Node_004e18c0* ptr;

    Iter_004e18c0() {}
    Iter_004e18c0(Node_004e18c0* p) : ptr(p) {}
    bool operator==(const Iter_004e18c0& x) const { return ptr == x.ptr; }
    bool operator!=(const Iter_004e18c0& x) const { return !(*this == x); }
    Iter_004e18c0& operator++()
    {
        ((Class_004e0450*)&ptr)->FUN_004e0450();
        return *this;
    }
    Iter_004e18c0 operator++(int)
    {
        Iter_004e18c0 tmp = *this;
        ((Class_004e0450*)&ptr)->FUN_004e0450();
        return tmp;
    }
};

class Class_004e2240 {
public:
    char unknown_0[4];                 // +0x0
    Node_004e18c0* head;               // +0x4

    Iter_004e18c0 FUN_004e2240();
};

class Class_004e18c0 {
public:
    char unknown_0[4];                 // +0x0
    Node_004e18c0* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc
    bool changed;                      // +0x10

    Iter_004e18c0 begin() { return head->left; }
    Iter_004e18c0 end() { return head; }

    Iter_004e18c0 erase(Iter_004e18c0 _F, Iter_004e18c0 _L)
    {
        if (size == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                ((Class_004dfea0*)this)->FUN_004dfea0(_F++);
            return _F;
        }
        // Early return above, not an else: keeps the return slot apart from the lock.
        std::_Lockit Lk;
        ((Class_004e03f0*)this)->FUN_004e03f0(head->parent);
        head->parent = (Node_004e18c0*)DAT_005292c4;
        size = 0;
        head->left = head;
        head->right = head;
        return ((Class_004e2240*)this)->FUN_004e2240();
    }

    void FUN_004e18c0();
};

class Class_004e1990 {
public:
    void FUN_004e1990(void* key);
};

struct Entry_004df590 {
    int field_0;                       // +0x0
    LPARAM text;                       // +0x4
    int flags_8;                       // +0x8
    char* name;                        // +0xc
};

extern Entry_004df590 DAT_00529e00[];
extern char* DAT_0050d660;
extern unsigned char DAT_00529dd8;
extern unsigned char DAT_00529dd4;
extern unsigned char DAT_00529ddc;
extern unsigned char DAT_00529e64;
extern unsigned char DAT_00529dc8;

void __cdecl SyncPerformanceSettings(int flag);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void __cdecl OpenUrl(HWND hwnd, const char* url, const char* ext);
void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);

class Class_004df280 {
public:
    HWND hwnd;                         // +0x00
    char unknown_4[0x1c];
    unsigned char flag_20;             // +0x20

    void SetPerformanceWindowVisible(char show);
};

class Class_004df380 {
public:
    HWND hwnd;                          // +0x00
    char unknown_4[0x20];
    const char* name;                   // +0x24

    // The key test, with the pointer compare the original does first: the
    // key is loaded before this->name, which is the order the code needs.
    bool Same(const char* key)
    {
        return key == name || strcmp(key, name) == 0;
    }
    void FUN_004df380();
};

class Class_004df4e0 {
public:
    HWND hwnd;                          // +0x00
    void FUN_004df4e0();
};

class Id_004df1e0 {
public:
    const char* id;                    // +0x0
    char text[0x1f4];                  // +0x4
    Id_004df1e0(const char* s)
    {
        id = s;
        if (id == 0)
            id = DAT_005119b8;
        text[0] = 0;
    }
};

class Class_004df1e0 {
public:
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int unknown_c;                     // +0xc
    double time;                       // +0x10
    int count;                         // +0x18
    void* table;                       // +0x1c
    char flag_20;                      // +0x20
    Id_004df1e0 ident;                 // +0x24
    NameTable map;                     // +0x21c

    Class_004df1e0();
    ~Class_004df1e0() {}
};

class PerformanceDialog {
public:
    HWND hwnd;                         // +0x00
    int left;                          // +0x04
    int top;                           // +0x08
    char unknown_0c[0xc];
    int count;                         // +0x18
    Entry_004df590* entries;           // +0x1c
    char flag_20;                      // +0x20
    char unknown_21[3];
    Value_004df590 selected;           // +0x24
    Map_004df590 set;                  // +0x21c

    BOOL HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void CreatePerformanceDialog(void);
};

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK PerformanceDlgProc(HWND, UINT, WPARAM, LPARAM);

static inline Node_004df590* Min_004df590(Node_004df590* p)
{
    std::_Lockit lock;
    while (p->left != (Node_004df590*)DAT_005292c4)
        p = p->left;
    return p;
}

static inline bool NamesEqual_004df590(const char* a, const char* b) { return a == b || strcmp(a,b) == 0; }

// The original calls this out of line from 0x4dfd10.
#pragma auto_inline(off)
// FUNCTION: 0x4df1e0
Class_004df1e0::Class_004df1e0()
    : ident("This is a unique identifier, isn't it - tell me the truth!")
{
    unknown_0 = 0;
    unknown_4 = -1;
    unknown_8 = -1;
    flag_20 = 0;
    InitPerformanceEvents();
    count = DAT_00529dcc;
    table = DAT_00529df8;
    time = GetTimeSeconds();
    ((PerformanceDialog*)this)->CreatePerformanceDialog();
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4df1e0.
#pragma auto_inline(off)
// FUNCTION: 0x4df250
void PerformanceDialog::CreatePerformanceDialog(void)
{
    if (CreateDialogFromTemplate(0x67, GetDesktopWindow(), (DLGPROC)PerformanceDlgProc, (LPARAM)this) == 0) {
        MessageBoxA(0, "Performance dialog failed to open", "Cavedog", 0);
    }
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4df590 and 0x4dfd00.
// 0x4df280 Class_004df280::SetPerformanceWindowVisible stays in src/debug/debug_lib_4df280.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

// FUNCTION: 0x4df330
BOOL __stdcall PerformanceDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((PerformanceDialog*)lParam)->hwnd = hwnd;
    }
    PerformanceDialog* obj = (PerformanceDialog*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->HandlePerformanceMessage(msg, wParam, lParam);
    return 0;
}

// The name table's tree nodes as 0x4df380 walks them: the same bytes as
// Node_004df590 (key at +0xc, 500-byte text at +0x10, colour at +0x204).
struct Value_004df380 {
    char text[500];                     // +0x00
};

struct Node_004df380 {
    Node_004df380* left;               // +0x0
    Node_004df380* parent;             // +0x4
    Node_004df380* right;              // +0x8
    const char* first;                 // +0xc
    Value_004df380 second;             // +0x10
    int color;                         // +0x204
};

Node_004df380* __cdecl FUN_004e04e0(Node_004df380* p);

class Iter_004df380 {
public:
    Node_004df380* ptr;

    Iter_004df380() {}
    Iter_004df380(Node_004df380* p) : ptr(p) {}
    Iter_004df380& operator++() { _Inc(); return *this; }
    bool operator==(const Iter_004df380& x) const { return ptr == x.ptr; }
    bool operator!=(const Iter_004df380& x) const { return !(*this == x); }
    // The tree's iterator increment, out of <xtree>. The while loop is
    // unrolled twice and the trailing "if" is the tree's own test, which
    // keeps the parent from being stored when the node is a left child.
    void _Inc()
    {
        std::_Lockit lk;
        if (ptr->right != (Node_004df380*)DAT_005292c4)
            ptr = FUN_004e04e0(ptr->right);
        else {
            Node_004df380* p;
            while (ptr == (p = ptr->parent)->right)
                ptr = p;
            if (ptr->right != p)
                ptr = p;
        }
    }
};

class Map_004df380 {
public:
    char compare;                      // +0x0
    char allocator;                    // +0x1
    Node_004df380* head;               // +0x4
    char multi;                        // +0x8
    int size;                          // +0xc
    char changed;                      // +0x10

    Iter_004df380 begin() { return Iter_004df380(head->left); }
    Iter_004df380 end() { return Iter_004df380(head); }
};

// FUNCTION: 0x4df380
void Class_004df380::FUN_004df380()
{
    CriticalSection* lock = (CriticalSection*)FUN_004e1ac0();
    EnterCriticalSection(&lock->cs);
    Map_004df380* map = (Map_004df380*)&GetNameTable()->names;
    for (Iter_004df380 it = map->begin(); it != map->end(); ++it) {
        if (Same(it.ptr->first)) {
            char buf[500];
            if (GetDlgItemTextA(hwnd, 0x3f0, buf, 500) == 0
                || strcmp(buf, it.ptr->second.text) != 0)
                SetDlgItemTextA(hwnd, 0x3f0, it.ptr->second.text);
            break;
        }
    }
    LeaveCriticalSection(&lock->cs);
}

unsigned char __cdecl HasPerfCounters(void);

// FUNCTION: 0x4df4e0
void Class_004df4e0::FUN_004df4e0()
{
    unsigned char b = HasPerfCounters() && DAT_00529dd8;
    EnableWindow(GetDlgItem(hwnd, 0x3f5), b);
    EnableWindow(GetDlgItem(hwnd, 0x3ed), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f3), b);
    EnableWindow(GetDlgItem(hwnd, 0x3ee), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f1), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f6), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f7), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f8), b);
}

// FUNCTION: 0x4df590
BOOL PerformanceDialog::HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Case order WM_COMMAND, WM_TIMER, WM_INITDIALOG: decides register ids and frame slots.
    switch (msg) {
    case 0x111: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDOK:
        case IDCANCEL:
            ((Class_004df280*)this)->SetPerformanceWindowVisible(0);
            return 0;

        case 0x3ed:
        case 0x3ee: {
                // HIWORD(wParam) for the notification tests: keeps the shr.
                if (HIWORD(wParam) != CBN_SELCHANGE)
                    return 0;
                int b = 0;
                if (LOWORD(wParam) == 0x3ee) b = 1;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x147, 0, 0);
                int mask = 1 << b;
                int i = 0;
                for (int j = 0; j < count; j++) {
                    if (entries[j].flags_8 & mask) {
                        if (i == sel) {
                            DAT_00529e00[b] = entries[j];
                            if (DAT_00529dc8 && entries[j].name != 0) {
                                int nmask = ~mask;
                                int n = 0;
                                for (int k = 0; k < count; k++) {
                                    if (entries[k].flags_8 & nmask) {
                                        if (entries[k].field_0 == (int)entries[j].name) {
                                            DAT_00529e00[1 - b] = entries[k];
                                            SendDlgItemMessageA(hwnd,
                                                (LOWORD(wParam) == 0x3ed) ? 0x3ee : 0x3ed,
                                                0x14e, n, 0);
                                        }
                                        n++;
                                    }
                                }
                            }
                            sel = -1;
                        }
                        i++;
                    }
                }
                SyncPerformanceSettings(0);
                return 0;
            }

            case 0x3fa:
                OpenUrl(hwnd,
                    "http://10.0.150.18/programming/library/extras/performancestatusdialog.html",
                    ".htm");
                return 0;

            case 0x3ef:
                DAT_00529dd8 = (DAT_00529dd8 == 0);
                SyncPerformanceSettings(0);
                ((Class_004df4e0*)this)->FUN_004df4e0();
                return 0;

            case 0x3f1:
                DAT_00529dd4 = (DAT_00529dd4 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f6:
                DAT_00529ddc = (DAT_00529ddc == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f7:
                DAT_00529e64 = (DAT_00529e64 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f8:
                DAT_00529dc8 = (DAT_00529dc8 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f4: {
                if (HIWORD(wParam) != LBN_SELCHANGE)
                    return 0;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x188, 0, 0);
                int j = 0;
                Node_004df590* node = set.head->left;
                while (Iterator_004df590(node) != Iterator_004df590(set.head)) {
                    if (j == sel) {
                        selected = *(Value_004df590*)((char*)node + 0xc);
                    }
                    j++;
                    {
                        // Min_004df590 inlined with node->right as argument: loads it before the lock.
                        std::_Lockit lock;
                        if (node->right != (Node_004df590*)DAT_005292c4) {
                            node = Min_004df590(node->right);
                        } else {
                            Node_004df590* p;
                            while (node == (p = node->parent)->right)
                                node = p;
                            if (node->right != p)
                                node = p;
                        }
                    }
                }
                ((Class_004df380*)this)->FUN_004df380();
                return 0;
            }
        }
        return 0;
    }

    case 0x113: {
        if (!IsWindowVisible(hwnd))
            return 0;
        RECT rect;
        GetWindowRect(hwnd, &rect);
        if (rect.left != left || rect.top != top) {
            left = rect.left;
            top = rect.top;
            SaveWindowPosition(hwnd, DAT_0050d660);
        }
        CriticalSection* cs = (CriticalSection*)FUN_004e1ac0();
        EnterCriticalSection(&cs->cs);
        NameTable* info = GetNameTable();
        if (info->changed) {
            int sel = -1;
            int n = 0;
            Node_004df590* node = ((Map_004df590*)&info->names)->head->left;
            SendDlgItemMessageA(hwnd, 0x3f4, 0x184, 0, 0);
            ((Class_004e18c0*)&set)->FUN_004e18c0();
            // Guarded do-while: a while or for loop moves the loop registers.
            if (Iterator_004df590(node) != Iterator_004df590(((Map_004df590*)&info->names)->head)) {
                do {
                    ((Class_004e1990*)&set)->FUN_004e1990(&node->value);
                    SendDlgItemMessageA(hwnd, 0x3f4, 0x180, 0, (LPARAM)node->value.name);
                    if (NamesEqual_004df590(node->value.name, selected.name))
                        sel = n;
                    n++;
                    ((Class_004e0450*)&node)->FUN_004e0450();
                } while (Iterator_004df590(node) != Iterator_004df590(((Map_004df590*)&info->names)->head));
            }
            if (sel >= 0)
                SendDlgItemMessageA(hwnd, 0x3f4, 0x186, sel, 0);
            info->changed = 0;
        }
        ((Class_004df380*)this)->FUN_004df380();
        LeaveCriticalSection(&cs->cs);
        return 0;
    }

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        for (int i = 0; i < 2; i++) {
            int mask = 1 << i;
            int id = 0x3ed;
            // Branch, not arithmetic: stops strength reduction of DAT_00529e00[i].
            if (i == 1)
                id = 0x3ee;
            int sel = 0;
            int n = 0;
            for (int j = 0; j < count; j++) {
                Entry_004df590* e = &entries[j];
                if (entries[j].flags_8 & mask) {
                    if (e->field_0 == DAT_00529e00[i].field_0)
                        sel = n;
                    n++;
                    SendDlgItemMessageA(hwnd, id, 0x143, 0, e->text);
                }
            }
            SendDlgItemMessageA(hwnd, id, 0x14e, sel, 0);
        }
        RegisterHotKey(hwnd, 10, 1, 0x24);
        CheckDlgButton(hwnd, 0x3ef, DAT_00529dd8);
        CheckDlgButton(hwnd, 0x3f1, DAT_00529dd4);
        CheckDlgButton(hwnd, 0x3f6, DAT_00529ddc);
        CheckDlgButton(hwnd, 0x3f7, DAT_00529e64);
        CheckDlgButton(hwnd, 0x3f8, DAT_00529dc8);
        ((Class_004df4e0*)this)->FUN_004df4e0();
        if (flag_20)
            ((Class_004df280*)this)->SetPerformanceWindowVisible(1);
        return 1;
    }

    case 0x312:
        if (wParam == 10) {
            ((Class_004df280*)this)->SetPerformanceWindowVisible(IsWindowVisible(hwnd) == 0);
        }
        return 0;

    }
    return 0;
}

Class_004df1e0* GetPerformanceWindow(void);

// The original calls this out of line from 0x4dfe80.
#pragma auto_inline(off)
// FUNCTION: 0x4dfd00
void ShowPerformanceStatus()
{
    ((Class_004df280*)GetPerformanceWindow())->SetPerformanceWindowVisible(1);
}
#pragma auto_inline(on)

extern const char* __cdecl FindCommandLineSwitch(const char*);

// FUNCTION: 0x4dfe80
void StartPerformanceStatus() {
    GetPerformanceWindow();
    const char* result = FindCommandLineSwitch("-performancestatus");
    if (result != 0) {
        ShowPerformanceStatus();
    }
}

// FUNCTION: 0x4dfea0
Class_004e0450 Class_004dfea0::FUN_004dfea0(Class_004e0450 _P)
{
    Node_004dfea0* _X;
    Node_004dfea0* _Y = (_P++)._Mynode();
    Node_004dfea0* _Z = _Y;
    std::_Lockit _Lk;
    if (_Left(_Y) == DAT_005292c4)
        _X = _Right(_Y);
    else if (_Right(_Y) == DAT_005292c4)
        _X = _Left(_Y);
    else
        _Y = _Min(_Right(_Y)), _X = _Right(_Y);
    if (_Y != _Z) {
        _Parent(_Left(_Z)) = _Y;
        _Left(_Y) = _Left(_Z);
        if (_Y == _Right(_Z))
            _Parent(_X) = _Y;
        else {
            _Parent(_X) = _Parent(_Y);
            _Left(_Parent(_Y)) = _X;
            _Right(_Y) = _Right(_Z);
            _Parent(_Right(_Z)) = _Y;
        }
        if (_Root() == _Z)
            _Root() = _Y;
        else if (_Left(_Parent(_Z)) == _Z)
            _Left(_Parent(_Z)) = _Y;
        else
            _Right(_Parent(_Z)) = _Y;
        _Parent(_Y) = _Parent(_Z);
        std::swap(_Color(_Y), _Color(_Z));
        _Y = _Z;
    } else {
        _Parent(_X) = _Parent(_Y);
        if (_Root() == _Z)
            _Root() = _X;
        else if (_Left(_Parent(_Z)) == _Z)
            _Left(_Parent(_Z)) = _X;
        else
            _Right(_Parent(_Z)) = _X;
        if (_Lmost() != _Z)
            ;
        else if (_Right(_Z) == DAT_005292c4)
            _Lmost() = _Parent(_Z);
        else
            _Lmost() = _Min(_X);
        if (_Rmost() != _Z)
            ;
        else if (_Left(_Z) == DAT_005292c4)
            _Rmost() = _Parent(_Z);
        else
            _Rmost() = _Max(_X);
    }
    if (_Color(_Y) == 1) {
        while (_X != _Root() && _Color(_X) == 1)
            if (_X == _Left(_Parent(_X))) {
                Node_004dfea0* _W = _Right(_Parent(_X));
                if (_Color(_W) == 0) {
                    _Color(_W) = 1;
                    _Color(_Parent(_X)) = 0;
                    _Lrotate(_Parent(_X));
                    _W = _Right(_Parent(_X));
                }
                if (_Color(_Left(_W)) == 1
                    && _Color(_Right(_W)) == 1) {
                    _Color(_W) = 0;
                    _X = _Parent(_X);
                } else {
                    if (_Color(_Right(_W)) == 1) {
                        _Color(_Left(_W)) = 1;
                        _Color(_W) = 0;
                        _Rrotate(_W);
                        _W = _Right(_Parent(_X));
                    }
                    _Color(_W) = _Color(_Parent(_X));
                    _Color(_Parent(_X)) = 1;
                    _Color(_Right(_W)) = 1;
                    _Lrotate(_Parent(_X));
                    break;
                }
            } else {
                Node_004dfea0* _W = _Left(_Parent(_X));
                if (_Color(_W) == 0) {
                    _Color(_W) = 1;
                    _Color(_Parent(_X)) = 0;
                    _Rrotate(_Parent(_X));
                    _W = _Left(_Parent(_X));
                }
                if (_Color(_Right(_W)) == 1
                    && _Color(_Left(_W)) == 1) {
                    _Color(_W) = 0;
                    _X = _Parent(_X);
                } else {
                    if (_Color(_Left(_W)) == 1) {
                        _Color(_Right(_W)) = 1;
                        _Color(_W) = 0;
                        _Lrotate(_W);
                        _W = _Left(_Parent(_X));
                    }
                    _Color(_W) = _Color(_Parent(_X));
                    _Color(_Parent(_X)) = 1;
                    _Color(_Left(_W)) = 1;
                    _Rrotate(_Parent(_X));
                    break;
                }
            }
        _Color(_X) = 1;
    }
    _Freenode(_Y);
    --_Size;
    return (_P);
}

// Shaped like std::_Tree<...>::_Erase(_Nodeptr) from MSVC 5's <xtree>
// (recursively frees a subtree) under a lock object; DAT_005292c4 is the
// tree's _Nil node. Nodes are returned to a free list (DAT_00529e58) linked
// through their first dword instead of being deleted.
struct Node_004e03f0 {
    Node_004e03f0* left;               // +0x0
    Node_004e03f0* parent;             // +0x4
    Node_004e03f0* right;              // +0x8
};

static inline void FreeNode(Node_004e03f0* p)
{
    if (p != 0) {
        p->left = (Node_004e03f0*)DAT_00529e58;
        DAT_00529e58 = p;
    }
}

// FUNCTION: 0x4e03f0
void Class_004e03f0::FUN_004e03f0(Node_004e03f0* x)
{
    std::_Lockit lock;
    for (Node_004e03f0* y = x; y != (Node_004e03f0*)DAT_005292c4; x = y) {
        FUN_004e03f0(y->right);
        y = y->left;
        FreeNode(x);
    }
}

// Shaped like std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree>: moves
// the iterator (a node pointer at +0) to the next node in order, under a lock.
// DAT_005292c4 is the tree's _Nil node; the inlined _Min (0x4e04e0) takes its
// own lock.
struct Node_004e0450 {
    Node_004e0450* left;               // +0x0
    Node_004e0450* parent;             // +0x4
    Node_004e0450* right;              // +0x8
};

static inline Node_004e0450* Min_004e0450(Node_004e0450* p)
{
    std::_Lockit lock;
    while (p->left != (Node_004e0450*)DAT_005292c4)
        p = p->left;
    return p;
}

// The original calls this out of line from 0x4df590.
#pragma auto_inline(off)
// FUNCTION: 0x4e0450
void Class_004e0450::FUN_004e0450()
{
    std::_Lockit lock;
    if (ptr->right != (Node_004e0450*)DAT_005292c4)
        ptr = Min_004e0450(ptr->right);
    else {
        Node_004e0450* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
#pragma auto_inline(on)
// The debug library's memory status dialog, performance settings, timer and
// name table: the working-set report and its dialog, the Cavedog registry key,
// the performance counters and events, the timer, the mapped-file wrapper and
// the name table's tree methods.
//
// 0x4e16b0 (src/debug/debug_lib_4e16b0.cpp) and 0x4e1e50 with 0x4e20a0
// (src/debug/debug_lib_4e1e50.cpp) stay in their own files: each is the source
// of a `gap` region of data/functions.csv and uses inline assembly.
// 0x4e1990 (src/debug/debug_lib_4e1990.cpp) stays in its own file: /Ob2
// inlines NameKey::FUN_004e1a30 at its call site here, where the original
// calls it, while 0x4e2250 needs the same definition inlinable.
// 0x4e21f0 (src/debug/debug_lib_4e21f0.cpp) stays in its own file: its 0.0
// and 5.0 constants sit in a different constant pool from the memory status
// dialog's 0.0, so the original had it in another translation unit.
// 0x4e2580 (src/debug/debug_lib_4e2580.cpp) and 0x4e2620 (src/debug/debug_lib_4e2620.cpp)
// stay in their own files: their views of Class_004e2580 and Class_004e2620
// disagree with the views the tree methods here compile from.
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <yvals.h>
#include <map>

// The trees' shared _Nil node (0x5292c4). Each tree's methods below name it
// under their own node type, so it is declared as the untyped node pointer.
extern void* DAT_005292c4;

struct Node_004e04e0 {
    Node_004e04e0* left;               // +0x0
    Node_004e04e0* parent;             // +0x4
    Node_004e04e0* right;              // +0x8
};

// Shaped like std::_Tree<...>::_Min(_Nodeptr) from MSVC 5's <xtree>: follows
// left links under a lock until the tree's _Nil node (DAT_005292c4). Its
// caller (0x4df380) uses it for an inlined iterator increment.
// FUNCTION: 0x4e04e0
Node_004e04e0* __cdecl FUN_004e04e0(Node_004e04e0* p)
{
    std::_Lockit lock;
    while (p->left != DAT_005292c4)
        p = p->left;
    return p;
}

// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// Note: 0x4e2cb0 is this class's (empty, out-of-line) destructor; it is
// called with ecx = the local key object at the end of its scope.
class CavedogRegistryKey {
public:
    HKEY key;                          // +0x00
    unsigned char readOnly;            // +0x04, set: read the values; clear: write them

    CavedogRegistryKey(char readOnly, char* app, char* section);
    ~CavedogRegistryKey();
    DWORD ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue);
    void FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue);
    void FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue);
    void FUN_004e2ee0(char* name, short* value, short minValue, short maxValue, short defaultValue);
    void FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue);
    void FUN_004e2f90(char* name, char* value, char minValue, char maxValue, char defaultValue);
    void FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue);
};


class Class_004e2fe0 {
public:
    HKEY key;                        // +0x0
    char reading;                    // +0x4
    void FUN_004e2fe0(char* name, bool* value, bool defaultValue);
};
extern char* DAT_0050d72c;

class Class_004e0520 {
public:
    char unknown_0[0x79];
    unsigned char workingSet;          // +0x79
    void FUN_004e0520(int readOnly);
};

// The original calls this from 0x4e0570 and HandleMemoryStatusMessage rather
// than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e0520
void Class_004e0520::FUN_004e0520(int readOnly)
{
    CavedogRegistryKey key(readOnly, DAT_0050d72c, "CavedogLibrary");
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("WorkingSet", (bool*)&workingSet, 0);
}
#pragma auto_inline(on)

double __cdecl GetTimeSeconds();

struct Triple_004e0570 {
    int a;
    int b;
    int c;
    Triple_004e0570() { a = 0; b = 0; c = 0; }
};

struct Sub_004e0570 {
    int table[20];                     // +0x00
    Triple_004e0570 triple;            // +0x50
    Sub_004e0570() { memset(table, 0, sizeof(table)); }
};

class Class_004e0570 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    double time;                       // +0x10
    Sub_004e0570 sub;                  // +0x18
    int unknown_74;                    // +0x74
    unsigned char flag_78;             // +0x78
    Class_004e0570();
    // The empty inline destructor makes MSVC register the empty atexit thunk
    // FUN_004e1450.
    ~Class_004e0570() {}
};

class Class_004e05f0 {
public:
    HWND hwnd;                         // +0x00
    char unknown_4[0x74];
    char flag_78;                      // +0x78
    void SetMemoryStatusWindowVisible(char on);
};

struct Rate_004e0b90 {
    double table[10]; double total; int index; int unknown_5c;
};

class MemoryStatusDialog {
public:
    HWND hwnd;                         // +0x00
    int field_4;                       // +0x04
    int left;                          // +0x08
    int top;                           // +0x0c
    double time;                       // +0x10
    Rate_004e0b90 rates;              // +0x18
    char flag_78;                      // +0x78
    unsigned char workingSet;          // +0x79

    int HandleMemoryStatusMessage(unsigned int msg, int wParam, int lParam);
    void CreateMemoryStatusDialog();
};

// The original calls the constructor from FUN_004e1410 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e0570
Class_004e0570::Class_004e0570()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = -1;
    field_c = -1;
    flag_78 = 0;
    time = GetTimeSeconds();
    ((Class_004e0520*)this)->FUN_004e0520(1);
    ((MemoryStatusDialog*)this)->CreateMemoryStatusDialog();
}
#pragma auto_inline(on)

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK MemoryStatusDlgProc(HWND, UINT, WPARAM, LPARAM);

extern char DAT_0050d6b4[]; // "Performance dialog failed to open"
extern char DAT_0050c8ac[]; // "Cavedog"

// The original calls this from the timer constructor rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e05c0
void MemoryStatusDialog::CreateMemoryStatusDialog()
{
    HWND result = CreateDialogFromTemplate(0x66, GetDesktopWindow(), (DLGPROC)MemoryStatusDlgProc, (LPARAM)this);
    if (result == 0)
        MessageBoxA(0, DAT_0050d6b4, DAT_0050c8ac, 0);
}
#pragma auto_inline(on)

void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void FUN_004e0790(void);

// The original calls this from HandleMemoryStatusMessage and ShowMemoryStatus
// rather than inlining it.
// 0x4e05f0 Class_004e05f0::SetMemoryStatusWindowVisible stays in src/debug/debug_lib_4e05f0.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

// Dialog procedure: on WM_INITDIALOG it stores the object passed as lParam in
// the window's user data (and the window handle in the object's first field),
// then forwards every message to that object's handler. Same shape as 0x4df330.
// FUNCTION: 0x4e06a0
BOOL __stdcall MemoryStatusDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((MemoryStatusDialog*)lParam)->hwnd = hwnd;
    }
    MemoryStatusDialog* obj = (MemoryStatusDialog*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->HandleMemoryStatusMessage(msg, wParam, lParam);
    return 0;
}

// Returns a critical section that is initialised on first use (a
// function-local static; its atexit destructor 0x4e0730 is empty).
class CritSec_004e06f0 {
public:
    CRITICAL_SECTION cs;

    CritSec_004e06f0() { InitializeCriticalSection(&cs); }
    ~CritSec_004e06f0() {}
};

// The original calls this from 0x4e0740, 0x4e0790 and FormatWorkingSet
// rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e06f0
LPCRITICAL_SECTION FUN_004e06f0(void)
{
    static CritSec_004e06f0 lock;
    return &lock.cs;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e0730
void FUN_004e0730(void)
{
}

extern HMODULE DAT_005295bc;
extern int DAT_00529508;
extern char DAT_005295cc;

// FUNCTION: 0x4e0740
void FUN_004e0740(void)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    EnterCriticalSection(cs);
    if (DAT_005295bc != 0) {
        FreeLibrary(DAT_005295bc);
        DAT_005295bc = 0;
        DAT_00529508 = 0;
    }
    DAT_005295cc = 1;
    LeaveCriticalSection(cs);
}

// The original calls this from 0x4e05f0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e0790
void FUN_004e0790(void)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    EnterCriticalSection(cs);
    if (DAT_005295bc != 0) {
        FreeLibrary(DAT_005295bc);
        DAT_005295bc = 0;
        DAT_00529508 = 0;
    }
    DAT_005295cc = 0;
    LeaveCriticalSection(cs);
}
#pragma auto_inline(on)

extern HANDLE DAT_00529530;
extern int DAT_00529528;
extern int DAT_005295b8;
extern int DAT_005295c0;
extern int DAT_005295c8;
extern int DAT_005295d0;
extern int DAT_005295d4;

typedef struct {
    DWORD NumberOfPages;
    unsigned int WorkingSetInfo[99999];
} WSInfo_004e07e0;

typedef BOOL (WINAPI *QueryWorkingSet_004e07e0)(HANDLE, PVOID, DWORD);

// Writes n as a decimal with thousands separators, then reverses it.
static void __inline fmt_004e07e0(char *buf, unsigned int n)
{
    char *f;
    char *w;
    int digits;

    *buf = 0;
    f = buf;
    w = buf;
    digits = 0;
    do {
        *w = (char)('0' + n % 10);
        w++;
        n /= 10;
        digits++;
        if (digits % 3 == 0) {
            if (n != 0) {
                *w = ',';
                w++;
            }
        }
    } while (n);
    *w = 0;
    w--;
    for (; f < w;) {
        char a = *w;
        char b = *f;
        *f++ = a;
        *w-- = b;
    }
}

// Reports the process working set into a caller-supplied buffer, with a
// psapi.dll QueryWorkingSet refresh at most once every ten calls.
// FUNCTION: 0x4e07e0
char __cdecl FormatWorkingSet(char *dest)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    WSInfo_004e07e0 ws;
    char priv[20];
    char max[20];
    char shared[20];
    char total[20];
    char pt[20];
    DWORD *p;
    DWORD w;
    DWORD lo;
    DWORD hi;
    DWORD n;

    EnterCriticalSection(cs);
    if (DAT_005295d0 < 0 || !(--DAT_005295d0 > 0)) {
        if (DAT_005295cc == 0) {
            DAT_005295bc = LoadLibraryA("psapi.dll");
            if (DAT_005295bc != 0) {
                DAT_00529508 = (int)GetProcAddress(DAT_005295bc, "QueryWorkingSet");
            }
            DAT_00529530 = GetCurrentProcess();
            DAT_005295cc = 1;
        }
        if (DAT_00529508 == 0) {
            LeaveCriticalSection(cs);
            return 0;
        }
        if (!((QueryWorkingSet_004e07e0)DAT_00529508)(DAT_00529530, &ws, 0x61a80)) {
            LeaveCriticalSection(cs);
            return 0;
        }
        n = ws.NumberOfPages;
        DAT_005295c8 = n;
        if (n > DAT_005295d4) {
            DAT_005295d4 = n;
        }
        // Chained, not separate statements: decouples register order from store order.
        DAT_00529528 = DAT_005295b8 = DAT_005295c0 = 0;
        // Counters stay globals, incremented in the loop: the formatters reload them.
        if (n > 0) {
            p = (DWORD *)&ws.WorkingSetInfo[0];
            // Separate copy of n, assigned after p: fixes the instruction order.
            DWORD cnt = n;
            do {
                w = *p;
                lo = w & 0xfff;
                hi = w & 0xfffff000;
                if (hi >= 0xc0000000 && hi <= 0xe0000000) {
                    DAT_005295c0++;
                } else if (lo & 0x100) {
                    DAT_005295b8++;
                } else {
                    DAT_00529528++;
                }
                p++;
            } while (--cnt);
        }
        DAT_005295d0 = 10;
    }
    fmt_004e07e0(pt, DAT_005295c0 << 12);
    fmt_004e07e0(shared, DAT_005295b8 << 12);
    fmt_004e07e0(priv, DAT_00529528 << 12);
    fmt_004e07e0(max, DAT_005295d4 << 12);
    fmt_004e07e0(total, DAT_005295c8 << 12);
    sprintf(dest, "\r\nTotal Working set:   %13s\r\nMaximum Working set: %13s\r\nPrivate:             %13s\r\nShared:              %13s\r\nPage Tables:         %13s\r\n", total, max, priv, shared, pt);
    LeaveCriticalSection(cs);
    return 1;
}

extern unsigned int DAT_005289d0;
extern unsigned int DAT_005289d4;
extern unsigned int DAT_005289d8;
extern unsigned int DAT_005289dc;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_005289f8;
extern unsigned int DAT_005289fc;
extern unsigned int DAT_00528a00;
extern unsigned int DAT_00528a04;
extern unsigned int DAT_00528a08;
extern unsigned int DAT_00528a1c;
extern char DAT_005119b8[];
extern char DAT_005295d8[];

void __cdecl OpenUrl(HWND hwnd, char* url, char* ext);
void ResetAllocStats(void);
char __cdecl FormatWorkingSet(char* dest);

// Free function shape: the quotient is computed inside the helper, so the
// divisor stays on the x87 stack and the divide becomes three fxch plus
// fdiv st(2), as in the original.
static void __inline rate_add(Rate_004e0b90* r, int delta, double dt)
{
    double speed = (double)delta / dt;
    r->total = speed + r->total - r->table[r->index];
    r->table[r->index] = speed;
    if (++r->index == 10)
        r->index = 0;
}

// Same routine as fmt_004e07e0: decimal with thousands
// separators, then reversed in place.
static void __inline fmt_004e0b90(char* buf, unsigned int n)
{
    char* f;
    char* w;
    int digits;

    *buf = 0;
    f = buf;
    w = buf;
    digits = 0;
    do {
        *w = (char)('0' + n % 10);
        w++;
        n /= 10;
        digits++;
        if (digits % 3 == 0) {
            if (n != 0) {
                *w = ',';
                w++;
            }
        }
    } while (n);
    *w = 0;
    w--;
    for (; f < w;) {
        char a = *w;
        char b = *f;
        *f++ = a;
        *w-- = b;
    }
}

// 0x4e0b90 MemoryStatusDialog::HandleMemoryStatusMessage stays in src/debug/memory_status_dialog.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

Class_004e0570* FUN_004e1410();

// The original calls this from StartMemoryStatus rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1400
void ShowMemoryStatus()
{
    ((Class_004e05f0*)FUN_004e1410())->SetMemoryStatusWindowVisible(1);
}
#pragma auto_inline(on)

// Returns a function-local static Class_004e0570; the empty inline destructor
// makes MSVC register the empty atexit thunk FUN_004e1450.
// The original calls this from 0x4e1400 and 0x4e1460 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1410
Class_004e0570* FUN_004e1410()
{
    static Class_004e0570 timer;
    return &timer;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e1450
void FUN_004e1450(void)
{
}

extern char* __cdecl FindCommandLineSwitch(char*);
void ShowMemoryStatus();

extern char DAT_0050d220[];

// FUNCTION: 0x4e1460
void __cdecl StartMemoryStatus()
{
    FUN_004e1410();
    char* result = FindCommandLineSwitch(DAT_0050d220);
    if (result != 0) {
        ShowMemoryStatus();
    }
}

// Constructor of a memory-mapped-file wrapper: initialises the handle/state
// fields, then opens the file if a name was given (see OpenMappedFile).
// The original calls this out of line from the LoadedImage constructor.
#pragma auto_inline(off)
// FUNCTION: 0x4e1560
MappedFile::MappedFile(const char* fileName)
{
    hFile = (void*)-1;
    hMapping = 0;
    view = 0;
    size = 0;
    state = 0;
    if (fileName != 0)
        ((Class_004e1590*)this)->OpenMappedFile(fileName);
}
#pragma auto_inline(on)

// Opens a memory-mapped file: create the file, map it read-only, then map a
// view. On each failure the handles are closed and a state code is stored
// (1 = file open failed, 2 = mapping failed, 3 = view failed).
// The original calls this out of line from the mapped-file users.
#pragma auto_inline(off)
// FUNCTION: 0x4e1590
void Class_004e1590::OpenMappedFile(const char* fileName)
{
    hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        state = 1;
        return;
    }
    size = GetFileSize(hFile, NULL);
    hMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == NULL) {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        state = 2;
        return;
    }
    view = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (view == NULL) {
        CloseHandle(hMapping);
        hMapping = NULL;
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        state = 3;
    }
}
#pragma auto_inline(on)

// Closes a memory-mapped file (the wrapper built by 0x4e1560): unmaps the
// view, then closes the mapping and file handles.
// The original calls this out of line from FUN_004de0f0.
#pragma auto_inline(off)
// FUNCTION: 0x4e1650
void MappedFile::CloseMappedFile()
{
    if (view != 0) {
        UnmapViewOfFile(view);
    }
    if (hMapping != 0) {
        CloseHandle(hMapping);
    }
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
    }
}
#pragma auto_inline(on)

unsigned char __cdecl OpenGdperf(void);

extern unsigned char DAT_00529e6c;
extern unsigned char DAT_00529e70;

// The original calls this from InitPerformanceEvents rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1680
unsigned char __cdecl HasPerfCounters(void)
{
    if (DAT_00529e6c == 0) {
        DAT_00529e6c = 1;
        unsigned char result = OpenGdperf();
        if (result != 0) {
            DAT_00529e70 = 1;
        }
    }
    return DAT_00529e70;
}
#pragma auto_inline(on)

// 0x4e16b0 (src/debug/debug_lib_4e16b0.cpp) stays in its own file: it is the
// source of a gap region and uses inline cpuid.

// FUNCTION: 0x4e1700
char IsPentiumOrBetter(void)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    if (info.dwProcessorType == 0x182 || info.dwProcessorType == 0x1e6)
        return 0;
    return 1;
}

// Returns the current time in seconds: from the performance counter when it
// is available, otherwise from the system time (100 ns file-time units).
// The original calls this from the timer and the dialog rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1730
double __cdecl GetTimeSeconds()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    BOOL haveFrequency = QueryPerformanceFrequency(&frequency);
    BOOL haveCounter = QueryPerformanceCounter(&counter);
    if (haveFrequency == TRUE && haveCounter == TRUE)
        return (double)counter.QuadPart / (double)frequency.QuadPart;

    SYSTEMTIME systemTime;
    FILETIME fileTime;
    GetSystemTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &fileTime);
    return ((double)fileTime.dwLowDateTime + (double)fileTime.dwHighDateTime * 4294967296.0) * 1e-7;
}
#pragma auto_inline(on)

// Constructor of the global name-table singleton (allocated by GetNameTable).
// The tree's static _Nil / _Nilrefs are DAT_005292c4 / DAT_00529500 and the
// pooled node free list is DAT_00529e58, exactly as in <xtree>'s _Init with a
// pooled allocator. The map member sits at +0, the "changed" flag at +0x10.
//
// The map's allocator pools 0x208-byte nodes.
// The original calls the constructor from GetNameTable rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e17c0
NameTable::NameTable() : changed(0)
{
}
#pragma auto_inline(on)

// The wrapper around a std::_Tree whose other methods are 0x4e1990 (insert),
// 0x4dfea0 (erase one node) and 0x4e03f0 (_Erase a subtree). The fast path
// erases the whole tree under a std::_Lockit and returns begin(), which the
// caller discards; the slow path walks the nodes with operator++(int).
// DAT_005292c4 is the tree's shared _Nil node.
// The original calls this out of line from the performance dialog.
#pragma auto_inline(off)
// FUNCTION: 0x4e18c0
void Class_004e18c0::FUN_004e18c0()
{
    erase(begin(), end());
    changed = 1;
}
#pragma auto_inline(on)

// The key: a C string ordered by strcmp.
class NameKey {
public:
    char* name;                        // +0x0
    bool FUN_004e1a30(const NameKey& other) const;
};

struct Value_004e2250 {
    char text[500];
};

struct Data1;
struct Data2;
typedef std::pair<const NameKey, Value_004e2250> Pair_004e2250;

// std::less<key>.
struct Less_004e2250 : public std::binary_function<NameKey, NameKey, bool> {
    bool operator()(const NameKey& _X, const NameKey& _Y) const
    {
        return (_X.FUN_004e1a30(_Y));
    }
};

// std::map<...>::_Kfn.
struct Kfn_004e2250 : public std::unary_function<Pair_004e2250, NameKey> {
    const NameKey& operator()(const Pair_004e2250& _X) const
    {
        return (_X.first);
    }
};

class Alloc_004e2b60 {
public:
    char* FUN_004e2b60(unsigned int n);
};

enum _Redbl_004e2250 { _Red, _Black };

struct Node_004e2250 {
    void* _Left;                       // +0x0
    void* _Parent;                     // +0x4
    void* _Right;                      // +0x8
    Pair_004e2250 _Value;              // +0xc
    _Redbl_004e2250 _Color;            // +0x204
};

typedef Node_004e2250* _Nodeptr;

// The members and static accessors of std::_Tree.
// Kept as the XTREE bodies and accessors: the inline budget follows their shape.
class Tree_004e2250 {
public:
    static _Redbl_004e2250& _Color(_Nodeptr _P)
        {return ((_Redbl_004e2250&)(*_P)._Color); }
    static const NameKey& _Key(_Nodeptr _P)
        {return (Kfn_004e2250()(_Value(_P))); }
    static _Nodeptr& _Left(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Right); }
    static Pair_004e2250& _Value(_Nodeptr _P)
        {return ((Pair_004e2250&)(*_P)._Value); }
    static _Nodeptr _Max(_Nodeptr _P)
        {std::_Lockit _Lk;
        while (_Right(_P) != DAT_005292c4)
            _P = _Right(_P);
        return (_P); }
    _Nodeptr& _Lmost()
        {return (_Left(_Head)); }
    _Nodeptr& _Rmost()
        {return (_Right(_Head)); }
    _Nodeptr& _Root()
        {return (_Parent(_Head)); }

    Alloc_004e2b60 allocator;          // +0x0
    Less_004e2250 key_compare;         // +0x1
    _Nodeptr _Head;                    // +0x4
    bool _Multi;                       // +0x8
    unsigned int _Size;                // +0xc
};

// std::_Tree<...>::iterator; _Dec is 0x4e2ab0.
// std::_Tree<...>::iterator; _Dec is 0x4e2ab0.
class Class_004e2ab0 : public std::_Bidit<Pair_004e2250, int> {
public:
    Class_004e2ab0()
        {}
    Class_004e2ab0(_Nodeptr _P)
        : _Ptr(_P) {}
    Class_004e2ab0& operator--()
        {FUN_004e2ab0();
        return (*this); }
    bool operator==(const Class_004e2ab0& _X) const
        {return (_Ptr == _X._Ptr); }
    void FUN_004e2ab0();
    _Nodeptr _Mynode() const
        {return (_Ptr); }
protected:
    _Nodeptr _Ptr;
};
// std::pair<iterator, bool>; its constructor is 0x4e2a10.
// std::pair<iterator, bool>; its constructor is 0x4e2a10.
class Class_004e2a10 {
public:
    Class_004e2a10(const Class_004e2ab0& _V1, const bool& _V2)
        : first(_V1), second(_V2) {}
    Class_004e2a10* FUN_004e2a10(const Data1* param_1, const Data2* param_2);
    Class_004e2ab0 first;
    bool second;
};
// _Rrotate (0x4e29b0).
// _Rrotate (0x4e29b0).
class Class_004e29b0 : public Tree_004e2250 {
public:
    void FUN_004e29b0(_Nodeptr _X);
};
// _Lrotate (0x4e2950).
// _Lrotate (0x4e2950).
class Class_004e2950 : public Class_004e29b0 {
public:
    void FUN_004e2950(_Nodeptr _X);
};
// _Buynode (0x4e2a30).
// _Buynode (0x4e2a30).
class Class_004e2a30 : public Class_004e2950 {
public:
    _Nodeptr FUN_004e2a30(_Nodeptr _Parg, _Redbl_004e2250 _Carg);
    void _Consval(Pair_004e2250* _P, const Pair_004e2250& _V)
        {std::_Construct(&*_P, _V); }
};
// _Insert (0x4e2620).
class Class_004e2620 : public Class_004e2a30 {
public:
    Class_004e2ab0 FUN_004e2620(_Nodeptr _X, _Nodeptr _Y, const Pair_004e2250& _V)
        {std::_Lockit _Lk;
        _Nodeptr _Z = FUN_004e2a30(_Y, _Red);
        _Left(_Z) = (_Nodeptr)DAT_005292c4, _Right(_Z) = (_Nodeptr)DAT_005292c4;
        _Consval(&_Value(_Z), _V);
        ++_Size;
        if (_Y == _Head || _X != DAT_005292c4
            || key_compare(Kfn_004e2250()(_V), _Key(_Y)))
            {_Left(_Y) = _Z;
            if (_Y == _Head)
                {_Root() = _Z;
                _Rmost() = _Z; }
            else if (_Y == _Lmost())
                _Lmost() = _Z; }
        else
            {_Right(_Y) = _Z;
            if (_Y == _Rmost())
                _Rmost() = _Z; }
        for (_X = _Z; _X != _Root()
            && _Color(_Parent(_X)) == _Red; )
            if (_Parent(_X) == _Left(_Parent(_Parent(_X))))
                {_Y = _Right(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                    {_Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X)); }
                else
                    {if (_X == _Right(_Parent(_X)))
                        {_X = _Parent(_X);
                        FUN_004e2950(_X); }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    FUN_004e29b0(_Parent(_Parent(_X))); }}
            else
                {_Y = _Left(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                    {_Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X)); }
                else
                    {if (_X == _Left(_Parent(_X)))
                        {_X = _Parent(_X);
                        FUN_004e29b0(_X); }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    FUN_004e2950(_Parent(_Parent(_X))); }}
        _Color(_Root()) = _Black;
        return (Class_004e2ab0(_Z)); }
};

// std::_Tree<...>::insert(const value_type&).
class Class_004e2250 : public Class_004e2620 {
public:
    Class_004e2ab0 begin()
        {return (Class_004e2ab0(_Lmost())); }
    Class_004e2a10 FUN_004e2250(const Pair_004e2250& _V);
};

class CritSec_004e1ac0 {
public:
    CRITICAL_SECTION cs;

    CritSec_004e1ac0() { InitializeCriticalSection(&cs); }
    ~CritSec_004e1ac0() {}
};

CritSec_004e1ac0* FUN_004e1ac0();

// 0x4e1990 (src/debug/debug_lib_4e1990.cpp) stays in its own file: this merge
// would make NameKey::FUN_004e1a30 (0x4e1a30, below) inlinable at its call
// site, and /Ob2 inlines it there, where the original calls it; 0x4e2250
// needs the same definition inlinable, so the two cannot share one file.

// FUNCTION: 0x4e1a30
bool NameKey::FUN_004e1a30(const NameKey& other) const
{
    return name != other.name && strcmp(name, other.name) < 0;
}

// The original calls this from GetNameTable rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1a80
void* __cdecl FUN_004e1a80(unsigned int size)
{
    return GlobalAlloc(0, size);
}
#pragma auto_inline(on)

// Lazily creates the global NameTable object, allocated with
// FUN_004e1a80 (a GlobalAlloc wrapper), and returns it. It is a placement
// new into that block: the constructor is 0x4e17c0.
extern NameTable* DAT_00529e7c;

// The original calls this out of line from the name-table users.
#pragma auto_inline(off)
// FUNCTION: 0x4e1a90
NameTable* GetNameTable()
{
    if (DAT_00529e7c == 0)
        DAT_00529e7c = new (FUN_004e1a80(sizeof(NameTable))) NameTable;
    return DAT_00529e7c;
}
#pragma auto_inline(on)

// Returns a function-local static critical section, initialised on first
// use (same shape as 0x4da780). The empty inline destructor makes MSVC
// register the empty atexit thunk FUN_004e1b00.
// The original calls this from 0x4e1990 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1ac0
CritSec_004e1ac0* FUN_004e1ac0()
{
    static CritSec_004e1ac0 lock;
    return &lock;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e1b00
void FUN_004e1b00(void)
{
}

typedef Entry_004df590 EventEntry;

extern EventEntry* DAT_00529df8;
extern int DAT_00529dcc;
extern EventEntry DAT_0050da00[];
extern EventEntry DAT_0050d980[];
extern EventEntry DAT_00529e10;        // "Event1"

class Class_004e2e20 {
public:
    HKEY key;                        // +0x0
    char reading;                    // +0x4
    void FUN_004e2e20(LPCSTR name, DWORD* value, DWORD minValue, DWORD maxValue, DWORD defaultValue);
};
extern unsigned char DAT_00529dd8;
extern unsigned char DAT_00529dd4;
extern unsigned char DAT_00529ddc;
extern unsigned char DAT_00529e64;
extern unsigned char DAT_00529dc8;

// Loads (or saves) the PerformanceSettings block of the Cavedog registry key.
// The original calls this from InitPerformanceEvents rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1b10
void __cdecl SyncPerformanceSettings(int readOnly)
{
    CavedogRegistryKey key(readOnly, "PerformanceSettings", "CavedogLibrary");
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("EnabledInRelease", (bool*)&DAT_00529dd8, 0);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("RaisePriority", (bool*)&DAT_00529dd4, 1);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("DisplayInDebugger", (bool*)&DAT_00529ddc, 0);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("DisplayInWindow", (bool*)&DAT_00529e64, 1);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("AutoPairing", (bool*)&DAT_00529dc8, 1);
    ((Class_004e2e20*)&key)->FUN_004e2e20("Event0", (DWORD*)&DAT_00529e00, 0, -1, 0);
    ((Class_004e2e20*)&key)->FUN_004e2e20("Event1", (DWORD*)&DAT_00529e10, 0, -1, 0);
}
#pragma auto_inline(on)

void __cdecl SyncPerformanceSettings(int arg);
int GetCpuFamily(void);

// 0x4e1be0 InitPerformanceEvents stays in src/debug/debug_lib_4e1be0.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

class Class_004e1d60 {
public:
    char unknown_0[0x40];
    int field_40;                      // +0x40
    int field_44;                      // +0x44
    char field_48;                     // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    void FUN_004e1d60(int a, int b);
};

class Class_004e20a0 {
public:
    double RestartTimer();
};

class Class_004e1e50 {
public:
    void ReportElapsedTime(char* text);
};

extern int DAT_00529dd0;
extern char DAT_00529e20[];

// While stopped, `time` holds the elapsed time; while running, it holds the
// start time.
class Timer {
public:
    double time;                       // +0x00
    char unknown_8[0x18 - 0x8];
    double history[5];                 // +0x18, the last five sample times
    char unknown_40[0x44 - 0x40];
    char* name;                        // +0x44
    char stopped;                      // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    ~Timer();
    Timer(int param_1);
    Timer(int a, int b);
    double GetElapsedSeconds();
    void ResumeTimer();
    void ResetTimer();
    void FUN_004e21a0(double delta);
    void FUN_004e21c0(double elapsed);
    double FUN_004e21f0();
};

// A timer object: starts timing (0x4e1d60, which saves and raises the thread
// priority) and discards a first elapsed-time reading (0x4e20a0).
// FUNCTION: 0x4e1d20 ??0Timer@@QAE@H@Z
Timer::Timer(int param_1)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(0, param_1);
    ((Class_004e20a0*)this)->RestartTimer();
}

// A second constructor of the timer object of 0x4e1d20: starts timing
// (0x4e1d60) with both values given, without the first reading.
// FUNCTION: 0x4e1d40 ??0Timer@@QAE@HH@Z
Timer::Timer(int a, int b)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(a, b);
}

// The original calls this from the two Timer constructors rather than
// inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1d60
void Class_004e1d60::FUN_004e1d60(int a, int b)
{
    field_40 = b;
    field_44 = a;
    DAT_00529e20[DAT_00529dd0++] = 9;
    boosted = DAT_00529dd4;
    if (boosted) {
        HANDLE thread = GetCurrentThread();
        oldThreadPriority = GetThreadPriority(thread);
        SetThreadPriority(thread, THREAD_PRIORITY_ABOVE_NORMAL);
        HANDLE process = GetCurrentProcess();
        oldPriorityClass = GetPriorityClass(process);
        SetPriorityClass(process, HIGH_PRIORITY_CLASS);
    }
    ((Class_004e20a0*)this)->RestartTimer();
}
#pragma auto_inline(on)

// The timer's counterpart to 0x4e1d60: pops the profiling nesting level,
// reports the elapsed time when the timer has a name (the hand-written
// routine at 0x4e1e50), and restores the process and thread priorities that
// 0x4e1d60 saved before raising them.
// FUNCTION: 0x4e1de0
Timer::~Timer()
{
    DAT_00529e20[--DAT_00529dd0] = 0;
    if (name) {
        ((Class_004e1e50*)this)->ReportElapsedTime(0);
    }
    if (boosted) {
        HANDLE process = GetCurrentProcess();
        SetPriorityClass(process, oldPriorityClass);
        HANDLE thread = GetCurrentThread();
        SetThreadPriority(thread, oldThreadPriority);
    }
}

// Timer: while stopped (flag at +0x48) the double at +0 holds the elapsed
// time; while running it holds the start time and the elapsed time is the
// current time (GetTimeSeconds) minus it. Callers set ecx to the timer
// (0x4e1eda, 0x4e2150), so this is a method.
// The original calls this from 0x4e2150 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1e30
double Timer::GetElapsedSeconds()
{
    if (stopped) {
        return time;
    }
    return GetTimeSeconds() - time;
}
#pragma auto_inline(on)

// 0x4e1e50 and 0x4e20a0 (src/debug/debug_lib_4e1e50.cpp) stay in their own
// file: it is the source of a gap region and uses inline rdpmc.

// The timer's elapsed-time getter (0x4e1e30), a method on the same object.
struct Class_004e2150 {
public:
    double field_0;
    char unknown_8[0x40];
    unsigned char field_48;
    
    void StopTimer();
};

// FUNCTION: 0x4e2150
void Class_004e2150::StopTimer()
{
    field_0 = ((Timer*)this)->GetElapsedSeconds();
    field_48 = 1;
}

// FUNCTION: 0x4e2160
void Timer::ResumeTimer()
{
    if (stopped != 0) {
        double now = GetTimeSeconds();
        time = now - time;
        stopped = 0;
    }
}

// FUNCTION: 0x4e2180
void Timer::ResetTimer()
{
    time = 0;
    stopped = 1;
}

// FUNCTION: 0x4e21a0
void Timer::FUN_004e21a0(double delta)
{
    if (stopped) {
        time -= delta;
    } else {
        // Without /Op the float conversion emits nothing, but it makes MSVC
        // load delta first (fld delta; fadd time) instead of hoisting a shared
        // `fld time` above the branch.
        time += (float)delta;
    }
}

// FUNCTION: 0x4e21c0
void Timer::FUN_004e21c0(double elapsed)
{
    if (stopped) {
        time = elapsed;
    } else {
        time = GetTimeSeconds() - elapsed;
    }
}

// 0x4e21f0 (src/debug/debug_lib_4e21f0.cpp) stays in its own file: its 0.0
// and 5.0 constants sit in a different constant pool from the memory status
// dialog's 0.0, so the original had it in another translation unit.


// The original calls this from 0x4e18c0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e2240
Iter_004e18c0 Class_004e2240::FUN_004e2240()
{
    return head->left;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e2250
Class_004e2a10 Class_004e2250::FUN_004e2250(const Pair_004e2250& _V)
{
    _Nodeptr _X = _Root();
    _Nodeptr _Y = _Head;
    bool _Ans = true;
    {
        std::_Lockit Lk;
        while (_X != DAT_005292c4) {
            _Y = _X;
            _Ans = key_compare(Kfn_004e2250()(_V), _Key(_X));
            _X = _Ans ? _Left(_X) : _Right(_X);
        }
    }
    if (_Multi)
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    Class_004e2ab0 _P = Class_004e2ab0(_Y);
    if (!_Ans)
        ;
    else if (_P == begin())
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    else
        --_P;
    if (key_compare(_Key(_P._Mynode()), Kfn_004e2250()(_V)))
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    return (Class_004e2a10(_P, false));
}

// 0x4e2580 (src/debug/debug_lib_4e2580.cpp) stays in its own file: its view
// of Class_004e2580 is keyed by const char* and returns Iter_004e2580, which
// cannot be one class with the NameKey-keyed view the tree methods above use.
// 0x4e2620 (src/debug/debug_lib_4e2620.cpp) stays in its own file: its
// Class_004e2620 carries the tree's fields and its own _Lrotate/_Rrotate,
// while 0x4e2250's Class_004e2620 inherits them from the XTREE chain above.
// The std::_Tree rotations and node allocators, the Cavedog registry key
// helper with the window position save and restore, and the GDPERF driver
// calls. 0x4e35b0 (OpenGdperf) is in a gap region and stays in its own file.
#include <yvals.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef _Redbl_004e2250 Redbl;

// The tree's _Nil node.
extern void* DAT_005292c4;
extern void* DAT_00529e58;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler
extern char* DAT_00529e80;
extern HANDLE DAT_00529e98;            // the GDPERF driver
extern bool DAT_00529e9c;              // the driver is open
extern int DAT_00529ea0;               // the CPU family

// FUNCTION: 0x4e2950
void Class_004e2950::FUN_004e2950(_Nodeptr _X)
{
    std::_Lockit _Lk;
    _Nodeptr _Y = _Right(_X);
    _Right(_X) = _Left(_Y);
    if (_Left(_Y) != DAT_005292c4)
        _Parent(_Left(_Y)) = _X;
    _Parent(_Y) = _Parent(_X);
    if (_X == _Root())
        _Root() = _Y;
    else if (_X == _Left(_Parent(_X)))
        _Left(_Parent(_X)) = _Y;
    else
        _Right(_Parent(_X)) = _Y;
    _Left(_Y) = _X;
    _Parent(_X) = _Y;
}

// FUNCTION: 0x4e29b0
void Class_004e29b0::FUN_004e29b0(_Nodeptr _X)
{
    std::_Lockit _Lk;
    _Nodeptr _Y = _Left(_X);
    _Left(_X) = _Right(_Y);
    if (_Right(_Y) != DAT_005292c4)
        _Parent(_Right(_Y)) = _X;
    _Parent(_Y) = _Parent(_X);
    if (_X == _Root())
        _Root() = _Y;
    else if (_X == _Right(_Parent(_X)))
        _Right(_Parent(_X)) = _Y;
    else
        _Left(_Parent(_X)) = _Y;
    _Right(_Y) = _X;
    _Parent(_X) = _Y;
}

struct Data1 {
    int field_0;
};

struct Data2 {
    char field_0;
};

// FUNCTION: 0x4e2a10
Class_004e2a10* Class_004e2a10::FUN_004e2a10(const Data1* param_1, const Data2* param_2)
{
    *(int*)&first = param_1->field_0;
    *(char*)&second = param_2->field_0;
    return this;
}

// The pooled allocator inlined into _Buynode: the free list is refilled
// 0x2000 bytes at a time with the generic "carve n-byte pieces" loop (the
// out-of-line copy is 0x4e2b60).
static inline void* PoolAlloc(unsigned int n)
{
    if (DAT_00529e58 == 0) {
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (unsigned int rem = 0x2000; rem >= n; rem -= n) {
            *(void**)block = DAT_00529e58;
            DAT_00529e58 = block;
            block += n;
        }
    }
    void* p = DAT_00529e58;
    DAT_00529e58 = *(void**)p;
    return p;
}

// FUNCTION: 0x4e2a30
_Nodeptr Class_004e2a30::FUN_004e2a30(_Nodeptr _Parg, _Redbl_004e2250 _Carg)
{
    // The original inlines the pool allocator into this definition (0x4e2b60
    // is its out-of-line copy).
    _Nodeptr _S = (_Nodeptr)PoolAlloc(sizeof(Node_004e2250));
    _Parent(_S) = _Parg;
    _Color(_S) = _Carg;
    return (_S);
}

// FUNCTION: 0x4e2ab0
void Class_004e2ab0::FUN_004e2ab0()
{
    std::_Lockit _Lk;
    if (Tree_004e2250::_Color(_Ptr) == _Red
        && Tree_004e2250::_Parent(Tree_004e2250::_Parent(_Ptr)) == _Ptr)
        _Ptr = Tree_004e2250::_Right(_Ptr);
    else if (Tree_004e2250::_Left(_Ptr) != DAT_005292c4)
        _Ptr = Tree_004e2250::_Max(Tree_004e2250::_Left(_Ptr));
    else
        {_Nodeptr _P;
        while (_Ptr == Tree_004e2250::_Left(_P = Tree_004e2250::_Parent(_Ptr)))
            _Ptr = _P;
        _Ptr = _P; }
}

// FUNCTION: 0x4e2b60
void* Class_004e2b60::FUN_004e2b60(unsigned int n)
{
    if (DAT_00529e58 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_00529e58;
            DAT_00529e58 = block;
            block += n;
        }
    }
    void* p = DAT_00529e58;
    DAT_00529e58 = *(void**)DAT_00529e58;
    return p;
}

// Reads a REG_DWORD value clamped to [minValue, maxValue], or returns
// defaultValue (signed counterpart of 0x4e2d90).
class Class_004e2d00 {
public:
    HKEY key;                        // +0x00
    int ReadInt(LPCSTR name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    HKEY field_0;

    void WriteDword(LPCSTR param_1, DWORD param_2);
};

class Class_004e2cc0 {
public:
    bool FUN_004e2cc0(const char* param1, unsigned int param2);
};

class Class_004e2ce0 {
public:
    void FUN_004e2ce0(LPCSTR param1, DWORD param2);
};

class Class_004e2e00 {
public:
    HKEY key;                          // +0x0

    void WriteInt(LPCSTR name, int value);
};

// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// `readOnly` selects the family: nonzero only opens, zero creates as well.
// A null `section` falls back to the DAT_00529e80 default section name.
// The original calls this out of line from the registry users.
#pragma auto_inline(off)
// FUNCTION: 0x4e2be0
CavedogRegistryKey::CavedogRegistryKey(char readOnly, char* app, char* section)
{
    HKEY k;
    if (section == 0) {
        section = DAT_00529e80;
    }
    key = 0;
    if (readOnly) {
        if (RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegOpenKeyA(k, section, &k) == ERROR_SUCCESS
            && RegOpenKeyA(k, app, &k) == ERROR_SUCCESS) {
            this->readOnly = readOnly;
            key = k;
            return;
        }
    } else {
        if (RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, section, &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, app, &k) == ERROR_SUCCESS) {
            key = k;
        }
    }
    this->readOnly = readOnly;
}
#pragma auto_inline(on)

// The original calls this empty destructor out of line.
#pragma auto_inline(off)
// FUNCTION: 0x4e2cb0
CavedogRegistryKey::~CavedogRegistryKey()
{
}
#pragma auto_inline(on)

// The original calls this out of line from RestoreWindowPosition.
#pragma auto_inline(off)
// FUNCTION: 0x4e2cc0
bool Class_004e2cc0::FUN_004e2cc0(const char* param1, unsigned int param2)
{
    int r = ((Class_004e2d00*)this)->ReadInt(param1, 0, 1, param2 & 0xff);
    return r != 0 ? true : false;
}
#pragma auto_inline(on)

// The original calls this out of line from SaveWindowPosition.
#pragma auto_inline(off)
// FUNCTION: 0x4e2ce0
void Class_004e2ce0::FUN_004e2ce0(LPCSTR param1, DWORD param2)
{
    ((Class_004e2d70*)this)->WriteDword(param1, param2 & 0xff);
}
#pragma auto_inline(on)

// The original calls this out of line from its callers.
#pragma auto_inline(off)
// FUNCTION: 0x4e2d00
int Class_004e2d00::ReadInt(LPCSTR name, int minValue, int maxValue, int defaultValue)
{
    int value;
    DWORD size = 4;
    DWORD type = REG_DWORD;
    if (RegQueryValueExA(key, name, 0, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS
        && size == 4 && type == REG_DWORD) {
        if (value < minValue) {
            value = minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }
    return defaultValue;
}
#pragma auto_inline(on)

// The original calls this out of line from its callers.
#pragma auto_inline(off)
// FUNCTION: 0x4e2d70
void Class_004e2d70::WriteDword(LPCSTR param_1, DWORD param_2) {
    RegSetValueExA(field_0, param_1, 0, REG_DWORD, (LPBYTE)&param_2, 4);
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4e2e20.
#pragma auto_inline(off)
// FUNCTION: 0x4e2d90
DWORD CavedogRegistryKey::ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue)
{
    DWORD value;
    DWORD size = 4;
    DWORD type = REG_DWORD;
    if (RegQueryValueExA(key, name, 0, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS
        && size == 4 && type == REG_DWORD) {
        if (value < minValue) {
            value = minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }
    return defaultValue;
}
#pragma auto_inline(on)

// Writes an integer registry value (same code as 0x4e2d70).
// The original calls this out of line from 0x4e2e20.
#pragma auto_inline(off)
// FUNCTION: 0x4e2e00
void Class_004e2e00::WriteInt(LPCSTR name, int value)
{
    RegSetValueExA(key, name, 0, REG_DWORD, (LPBYTE)&value, 4);
}
#pragma auto_inline(on)

// Reads or writes a DWORD registry value depending on the mode flag
// (compare 0x4e2ee0, the short version).

// The original calls this out of line from the performance settings.
#pragma auto_inline(off)
// FUNCTION: 0x4e2e20
void Class_004e2e20::FUN_004e2e20(LPCSTR name, DWORD* value, DWORD minValue, DWORD maxValue, DWORD defaultValue)
{
    if (reading) {
        *value = ((CavedogRegistryKey*)this)->ReadDword(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2e00*)this)->WriteInt(name, *value);
    }
}
#pragma auto_inline(on)

// Reads or writes an int registry value depending on the mode flag
// (compare 0x4e2e20 and 0x4e2ee0).
// FUNCTION: 0x4e2e60
void CavedogRegistryKey::FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// The int version of 0x4e2ee0: reads a registry value into *value (clamped
// by ReadInt) when reading (readOnly), otherwise writes *value.
// FUNCTION: 0x4e2ea0
void CavedogRegistryKey::FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2ee0
void CavedogRegistryKey::FUN_004e2ee0(char* name, short* value, short minValue, short maxValue, short defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2f30
void CavedogRegistryKey::FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2f90
void CavedogRegistryKey::FUN_004e2f90(char* name, char* value, char minValue, char maxValue, char defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// Reads or writes a bool registry value, like the char/short versions at
// 0x4e2f90 and 0x4e2ee0.

// The original calls this out of line from the memory status dialog.
#pragma auto_inline(off)
// FUNCTION: 0x4e2fe0
void Class_004e2fe0::FUN_004e2fe0(char* name, bool* value, bool defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, 0, 1, defaultValue) ? true : false;
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4e3030
void CavedogRegistryKey::FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e3080
// Restores where a window sits from
// HKCU\Software\Cavedog Entertainment\Cavedog library\WindowPositions\<name>.
// Edges of -500 are the "no saved value" sentinel; zoomX/zoomY of 1.0 with no
// saved size resizes the window instead of moving it.
unsigned char __cdecl RestoreWindowPosition(HWND hwnd, char* name, double zoomX, double zoomY, char doSize)
{
    char buf[200];
    WINDOWPLACEMENT placement;
    RECT cur;
    RECT full;
    RECT work;
    int y;
    int flags;
    bool resizable;
    bool zoomed;
    int x;
    int w;
    int h;

    // The screen metrics are only a seed: SPI_GETWORKAREA overwrites all four
    // words, so left and top are dead and right and bottom never survive.
    work.left = 0;
    work.top = 0;
    work.right = GetSystemMetrics(SM_CXSCREEN);
    work.bottom = GetSystemMetrics(SM_CYSCREEN);
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);

    placement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &placement);

    strcpy(buf, "WindowPositions\\");
    strncat(buf, name, sizeof(buf) - 1 - strlen(buf));

    CavedogRegistryKey key(1, buf, "Cavedog library");
    x = ((Class_004e2d00*)&key)->ReadInt("LeftEdge", -500, 50000, -500);
    y = ((Class_004e2d00*)&key)->ReadInt("TopEdge", -500, 50000, -500);

    long style = GetWindowLongA(hwnd, GWL_STYLE);
    resizable = false;
    GetWindowRect(hwnd, &cur);
    if ((style & WS_THICKFRAME) == WS_THICKFRAME) {
        resizable = true;
        w = ((Class_004e2d00*)&key)->ReadInt("Width", 0, work.right - work.left, 0);
        h = ((Class_004e2d00*)&key)->ReadInt("Height", 0, work.bottom - work.top, 0);
        zoomed = ((Class_004e2cc0*)&key)->FUN_004e2cc0("Zoomed", 0);
        if (w < 100) {
            w = 100;
        }
        if (h < 50) {
            h = 50;
        }
    } else {
        w = cur.right - cur.left;
        h = cur.bottom - cur.top;
        zoomed = false;
    }

    flags = SWP_NOZORDER;
    if (doSize) {
        flags = SWP_NOZORDER | SWP_SHOWWINDOW;
    }

    if (x == -500 || y == -500 || w <= 0 || h <= 0) {
        if (zoomX == 1.0 && zoomY == 1.0) {
            if (doSize) {
                ShowWindow(hwnd, SW_SHOWNORMAL);
            }
            return 0;
        }
        GetWindowRect(hwnd, &full);
        SetWindowPos(hwnd, 0, 0, 0,
                     (int)((full.right - full.left) * zoomX),
                     (int)((full.bottom - full.top) * zoomY),
                     flags | SWP_NOMOVE);
        return 1;
    }

    if (x < work.left - w / 2) {
        x = work.left - w / 2;
    }
    if (y < work.top) {
        y = work.top;
    }
    if (x + w / 2 > work.right) {
        x = work.right - w / 2;
    }
    if (y + h / 2 > work.bottom) {
        y = work.bottom - h / 2;
    }
    SetWindowPos(hwnd, 0, x, y, w, h, flags);
    if (resizable && zoomed) {
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4e33d0
void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b)
{
    RestoreWindowPosition(hwnd, name, a, b, 1);
}

// FUNCTION: 0x4e3400
// Saves where a resizable window sits, under
// HKCU\Software\Cavedog Entertainment\Cavedog library\WindowPositions\<name>.
void __cdecl SaveWindowPosition(HWND hwnd, char* name)
{
    if (IsIconic(hwnd)) {
        return;
    }
    char buf[200];
    strcpy(buf, "WindowPositions\\");
    strncat(buf, name, sizeof(buf) - 1 - strlen(buf));
    CavedogRegistryKey key(0, buf, "Cavedog library");
    WINDOWPLACEMENT placement;
    RECT r;
    placement.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(hwnd, &placement)) {
        r = placement.rcNormalPosition;
        if (!IsZoomed(hwnd)) {
            if (!IsIconic(hwnd)) {
                GetWindowRect(hwnd, &r);
            }
        }
        r.right = r.right - r.left;
        r.bottom = r.bottom - r.top;
        // Both edges are clamped to -499 when they are 500 or further out.
        if (r.left <= -500) {
            r.left = -499;
        }
        if (r.top <= -500) {
            r.top = -499;
        }
        ((Class_004e2d70*)&key)->WriteDword("LeftEdge", r.left);
        ((Class_004e2d70*)&key)->WriteDword("TopEdge", r.top);
        if ((GetWindowLongA(hwnd, GWL_STYLE) & WS_THICKFRAME) == WS_THICKFRAME) {
            ((Class_004e2d70*)&key)->WriteDword("Width", r.right);
            ((Class_004e2d70*)&key)->WriteDword("Height", r.bottom);
            ((Class_004e2ce0*)&key)->FUN_004e2ce0("Zoomed", IsZoomed(hwnd) != 0);
        }
    }
}

// FUNCTION: 0x4e3710
bool CloseGdperf()
{
    if (DAT_00529e9c) {
        DAT_00529e9c = 0;
        if (DAT_00529e98) {
            BOOL ok = CloseHandle(DAT_00529e98);
            DAT_00529e98 = 0;
            if (ok)
                return true;
        }
    }
    return false;
}

// Unused here: declared early for the symbol ids (see 0x4e39a0).
int GetCpuFamily(void);
extern bool __cdecl ReadGdperf(DWORD a, void* out);
extern bool __cdecl WriteGdperf(DWORD a, DWORD b, DWORD c);
// The same function (0x4e3930), seen by ProgramPerfEvent with the 64-bit
// register value as one argument.
extern bool __cdecl WriteGdperf(DWORD a, __int64 b);

// Sends one effect-descriptor dword to the "Microsoft Game Device" driver that
// 0x4e38e0 opened (\\.\GDPERF, see the caller at 0x4e36c0). Two device families
// are handled, selected by DAT_00529ea0, which holds the CPU family nibble:
// 5 gets a two-call sequence, 6 a single call.
// 0x4e3750 ProgramPerfEvent stays in src/debug/debug_lib_4e3750.cpp: joined into
// this file its register allocation lands differently
// (docs/c2-regalloc.md).

// Sends one dword to the driver opened in DAT_00529e98 and reads back
// 8 bytes; succeeds only when exactly 8 bytes were returned.
// The original calls this out of line from ProgramPerfEvent.
#pragma auto_inline(off)
// FUNCTION: 0x4e38e0
bool __cdecl ReadGdperf(DWORD a, void* out)
{
    DWORD in = a;
    if (!DAT_00529e9c) {
        return false;
    }
    DWORD returned;
    BOOL ok = DeviceIoControl(DAT_00529e98, 0x9c406404, &in, sizeof(in), out, 8, &returned, 0);
    if (returned != 8) {
        ok = 0;
    }
    return ok != 0;
}
#pragma auto_inline(on)

// The original calls this out of line from ProgramPerfEvent.
#pragma auto_inline(off)
// FUNCTION: 0x4e3930
bool __cdecl WriteGdperf(DWORD a, DWORD b, DWORD c)
{
    if (!DAT_00529e9c) {
        return false;
    }
    DWORD in[3];
    in[0] = a;
    in[1] = b;
    in[2] = c;
    DWORD returned;
    BOOL ok = DeviceIoControl(DAT_00529e98, 0x9c406400, in, sizeof(in), 0, 0, &returned, 0);
    if (returned != 0) {
        ok = 0;
    }
    return ok != 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e39a0
int GetCpuFamily(void)
{
    return DAT_00529ea0;
}

// FUNCTION: 0x4e3e10
int FUN_004e3e10(void)
{
    return 0xfffffffd;
}

extern void __cdecl EmptyAtexitHandler();
void __cdecl FUN_004e4250(void);
void __cdecl FUN_004e4260(void);
void __cdecl FUN_004e4270(void);
void __cdecl FUN_004e4280(void);
void __cdecl FUN_004e4290(void);
void __cdecl FUN_004e42a0(void);

// FUNCTION: 0x4e41e0
void FUN_004e41e0(void)
{
    atexit(FUN_004e42a0);
}

// FUNCTION: 0x4e41f0
void FUN_004e41f0()
{
    atexit(FUN_004e4290);
}

// FUNCTION: 0x4e4200
void FUN_004e4200()
{
    atexit(EmptyAtexitHandler);
}

// FUNCTION: 0x4e4210
void FUN_004e4210()
{
    atexit(FUN_004e4280);
}

// FUNCTION: 0x4e4220
void FUN_004e4220()
{
    atexit(FUN_004e4270);
}

// FUNCTION: 0x4e4230
void FUN_004e4230()
{
    atexit(FUN_004e4260);
}

// FUNCTION: 0x4e4240
void FUN_004e4240()
{
    atexit(FUN_004e4250);
}

// FUNCTION: 0x4e4250
void __cdecl FUN_004e4250(void)
{
}

// FUNCTION: 0x4e4260
void __cdecl FUN_004e4260(void)
{
}

// FUNCTION: 0x4e4270
void __cdecl FUN_004e4270(void)
{
}

// FUNCTION: 0x4e4280
void __cdecl FUN_004e4280(void)
{
}

// FUNCTION: 0x4e4290
void __cdecl FUN_004e4290(void)
{
}

// FUNCTION: 0x4e42a0
void __cdecl FUN_004e42a0(void)
{
}

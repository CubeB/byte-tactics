// Decompiled by Haiku, Opus, Sonnet, Claude Opus 5.5, claude-opus-5-5, Claude Sonnet 5.5, claude-sonnet-5-5, Sonnet 5.5, deepseek-v4.1, deepseek-v4.1-flash, DeepSeek V4.1 Flash, space-bunny-free, Space Bunny Free, muse-spark-1.3-free, mimo-v2.6-pro, GPT-6 and GPT-6.1-sol. Names are provisional.
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

// Memory block description, 0x30 bytes, passed by value to FormatBlockInfo.
class BlockInfo {
public:
    int address;                     // +0x00
    int size;                        // +0x04
    int allocNumber;                 // +0x08
    char name[0x24];                 // +0x0c

    BlockInfo(void);
};

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

// FUNCTION: 0x4d8df0
int __cdecl GetStackLow(void)
{
    IsOutsideStack(0, 0);
    return (int)g_stackLow;
}

// fs:[0x2c] is the thread-local storage array pointer; the value read back
// after the IsOutsideStack(0, 0) call is the end of this thread's stack, which
// IsOutsideStack looks up the first time (src/debug/debug_lib_4d8d70.cpp defines the
// thread's variables).

extern __declspec(thread) char* g_stackHigh;

// FUNCTION: 0x4d8e20
void* GetStackHigh()
{
    IsOutsideStack(0, 0);
    return g_stackHigh;
}

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
void __cdecl UnloadImageHelp();

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

class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
};

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

// An integer command-line setting (e.g. "setvalue" / "-memset" in 0x4d8260):
// starts at the default, then parses the text after the switch as hex
// ("0x...") or decimal.

char* __cdecl FindCommandLineSwitch(char* name);

class Class_004da040 {
public:
    int value;                         // +0x0
    Class_004da040(char* name, int a, int def, char* sw);
};

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

// FUNCTION: 0x4da3f0
void __cdecl NormalizeLineEndings(char *text, int size)
{
    int len = strlen(text);
    char *dst = text + (size - len) - 1;
    int room = dst - text;

    // Index text[i]; a pointer copy is set up after the loop guard.
    for (int i = 0; text[i] != 0; i++)
        if (text[i] == '\n')
            room--;
    if (room <= 0)
        return;

    // memmove, not memcpy: the callee is 0x4e84e0.
    memmove(dst, text, len + 1);

    while (*dst != '\0') {
        char c = *dst;
        if (c == '\r') {
            *text++ = c;
            *text++ = '\n';
            if (dst[1] == '\n')
                dst++;
        } else if (c == '\n') {
            *text++ = '\r';
            *text++ = '\n';
        } else {
            *text++ = c;
        }
        dst++;
    }
    *text = '\0';
}

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

// FUNCTION: 0x4da780
CritSec_004da780* FUN_004da780()
{
    static CritSec_004da780 lock;
    return &lock;
}

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

// FUNCTION: 0x4da840
void __cdecl CountFree(int param_1) {
    DAT_00528a08--;
    DAT_005289f8 -= param_1;
}

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

extern Node_004da8d0* DAT_00528a50;    // the tree's _Nil node

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
        DAT_00528a50->right = 0;
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

// FUNCTION: 0x4da8d0
void* GetBlockMap()
{
    if (DAT_00528a44 == 0) {
        char s0, s1;
        DAT_00528a44 = new Tree_004da8d0(s0, s1);
    }
    return DAT_00528a44;
}

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

// FUNCTION: 0x4da9f0
Container_004da9f0<int>* GetFreedBlockRing()
{
    if (DAT_00528a48 == 0) {
        DAT_00528a48 = new Container_004da9f0<int>;
    }
    return DAT_00528a48;
}

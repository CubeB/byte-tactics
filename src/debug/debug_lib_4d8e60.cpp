// Decompiled by Claude Sonnet 5.5 and deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-sonnet-5-5. Names are provisional.
// MATCH. Notes on what made it match (from 92.9%):
//  - sprintf destination form matters at every site. `char* d = log + strlen(log);
//    sprintf(d, ...)` computes the pointer before any argument is pushed (the
//    lea displacement has no pending pushes), `size_t L = strlen(log);
//    sprintf(log + L, ...)` pushes the arguments first and does `lea eax,[esp+ecx+..]`
//    last, and a plain `sprintf(log + strlen(log), ...)` pushes the last argument
//    before running the strlen. Each site uses the form the disassembly shows.
//  - the parameter loop is `for (i = 0; i < rec->NumberParameters; i++)` indexing
//    rec->ExceptionInformation[i] directly (MSVC strength-reduces it to a pointer
//    in the preheader) with the separator char computed in its own local before
//    the destination pointer.
//  - the whole tail of the dump (lstrcpynA, the Dr/FPU lines, Cr0NpxState and the
//    trailing newlines) is inside `if (room > 0)`; the original's jle jumps
//    straight to the NormalizeLineEndings call.
//  - `log[0] = 0` after the first WriteFile is inside the GetModuleFileNameA
//    success block, so the failure path skips it.
//  - the EAX register-dump format string really reads "EFLGS" in the exe.
// Suspected original bugs: the `i % 3 == 3` test in the parameters loop can never
// be true, so the per-three newline is dead code; CreateFileA's result is tested
// against 0 rather than INVALID_HANDLE_VALUE, so a failed open passes the check;
// and the lstrcpynA dump buffer at obj+0x2084 is copied with
// room = 0x7358 - strlen(log) - 0x3e8 regardless of its own length.
#include <windows.h>
#include <stdio.h>
#include <string.h>

class StackTrace {
public:
    char unknown_0[0x2084];
    char dump_text[0xa44c];
    void CaptureStack(unsigned long ebp, unsigned long esp, unsigned long eip, int zero);
};

class Class_004d9ca0 {
public:
    void FormatStackReport();
};

extern int DAT_005289c0;

char __cdecl FUN_004d8680();
char* __cdecl GetExceptionName(int code);
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
    obj.CaptureStack(ctx->Ebp, ctx->Esp, ctx->Eip, 0);
    ((Class_004d9ca0*)&obj)->FormatStackReport();

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
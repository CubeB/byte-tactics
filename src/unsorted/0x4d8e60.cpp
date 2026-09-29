// Decompiled by Claude Sonnet 5.5 (skeleton and integration), regions by
// deepseek-v4.1-flash. Names are provisional.
// Crash handler: appends a report (exception record, registers, code bytes at
// EIP, debug and floating point registers) to ErrorLog.txt next to the exe.
// Runs once (guard flag), the report is built in one big stack buffer with
// `sprintf(log + strlen(log), ...)`.
//
// STATUS: not a match. `uv run tools/check.py 0x4d8e60` gives 2644 of 2644
// bytes (exact size), 50.6%. The callee conventions, every string, call and
// constant match; what is left is stack slot order, register choices and a
// handful of evaluation-order differences.
//  - The second argument matters: the signature is
//    `int __cdecl(EXCEPTION_POINTERS* ep, char* handlerName)`. Callers push
//    "fatal error handler" (0x4d9c07) and "Global Exception Handler"
//    (0x4da2aa); the original prints the second stack dword in
//    "Exception handler called in %s. " (0x4d90c4 reads [esp+0x14408], i.e.
//    arg1, not arg0). Adding it fixed the byte count to exact (48.8 -> 50.6).
//  - After that change our _chkstk size is 0x143f4, the original's 0x143f0:
//    `Class_004d9c60 obj` here is declared with unknown_0[0xc4d0] and likely
//    needs to be 0x10 smaller (real size <= 0xc4c0) so the frame lands on
//    0x143f0 again.
//  - The original keeps the small locals at [esp+0x10] = reason (returned by
//    FUN_004d98c0), [esp+0x14] = base (exe name after the last backslash),
//    [esp+0x18] = file. Ours orders them base 0x10, file 0x14, reason 0x18.
//    Declaration order made no difference (12 orders tried), so the ordering
//    comes from use/liveness; unresolved.
//  - The original does `call CreateFileA; test eax, eax; mov [esp+0x18], eax`
//    and pushes eax again for SetFilePointer, so the handle is never held in
//    a register; ours does `mov esi, eax` first.
//  - `FUN_004ded60(log+strlen(log), 0x7358-strlen(log))` and each sprintf
//    compute strlen after the other arguments are pushed in the original;
//    ours hoists some of those loads. r6 (`room` maths, lstrcpynA source) is
//    the weakest region.
//
// Pilot notes: docs/splitting-huge-functions.md and docs/splitting-pilot2-results.md.

// SHARED begin
#include <windows.h>
#include <stdio.h>
#include <string.h>

class Class_004d9c60 {
public:
    char unknown_0[0xc4d0];
    void FUN_004d9c60(unsigned long ebp, unsigned long esp, unsigned long eip, int zero);
};

class Class_004d9ca0 {
public:
    void FUN_004d9ca0();
};

extern int DAT_005289c0;

char __cdecl FUN_004d8680();
char* __cdecl FUN_004d98c0(int code);
void __cdecl FUN_004da3f0(char* buf, int size);
void __cdecl FUN_004ded60(char* dst, int size);
void __cdecl FUN_004de110();
// SHARED end

// FUNCTION: 0x4d8e60
int __cdecl FUN_004d8e60(EXCEPTION_POINTERS* ep, char* handlerName)
{
    char path[1000];
    char name[1000];
    char log[0x7358];
    char exe[1000];
    Class_004d9c60 obj;
    HANDLE file;
    DWORD written;
    char* reason;

    if (DAT_005289c0)
        return 0;
    DAT_005289c0 = 1;
    CONTEXT* ctx = ep->ContextRecord;
    EXCEPTION_RECORD* rec = ep->ExceptionRecord;
    obj.FUN_004d9c60(ctx->Ebp, ctx->Esp, ctx->Eip, 0);
    ((Class_004d9ca0*)&obj)->FUN_004d9ca0();

    // REGION r1 begin
    char* slash;
    if (GetModuleFileNameA(0, path, 1000) == 0 || (slash = strrchr(path, '\\')) == 0)
        strcpy(path, "C:\\");
    else
        slash[1] = 0;
    strcat(path, "ErrorLog.txt");
    file = CreateFileA(path, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file != 0)
        SetFilePointer(file, 0, 0, FILE_END);
    log[0] = 0;
    reason = FUN_004d98c0(rec->ExceptionCode);
    // REGION r1 end

    // REGION r2 begin
    if (GetModuleFileNameA(0, exe, 1000) > 0) {
        char* base = strrchr(exe, '\\');
        base = base ? base + 1 : exe;
        strcpy(name, base);
        char* dot = strrchr(name, '.');
        if (dot)
            *dot = 0;
        sprintf(log + strlen(log), "%s caused an %s in\n", name, reason);
        sprintf(log + strlen(log), "module %s at %04x:%08lx.\n", base, ctx->SegCs, ctx->Eip);
        if (file != 0)
            WriteFile(file, log, strlen(log), &written, 0);
    }
    // REGION r2 end

    // REGION r3 begin
    log[0] = 0;
    sprintf(log + strlen(log), "Exception handler called in %s. ", handlerName);
    FUN_004ded60(log + strlen(log), 0x7358 - strlen(log));
    sprintf(log + strlen(log), "Instruction pointer is %08lX\n", ctx->Eip);
    sprintf(log + strlen(log), "ExceptionCode = %08lX", rec->ExceptionCode);
    sprintf(log + strlen(log), " - %s\n", (char*)file);
    if (rec->ExceptionCode == 0xc0000005 && rec->NumberParameters >= 2) {
        if (((char(__cdecl*)(unsigned long))FUN_004d8680)(rec->ExceptionInformation[1]))
            sprintf(log + strlen(log), "Error: Write to read only memory attempted\n");
        sprintf(log + strlen(log), "Access violation: Illegal %s, data address 0x%08lX\n",
                rec->ExceptionInformation[0] ? "write" : "read", rec->ExceptionInformation[1]);
    }
    // REGION r3 end

    // REGION r4 begin
    sprintf(log + strlen(log), "ExceptionFlags = %08lX\t", rec->ExceptionFlags);
    sprintf(log + strlen(log), "ExceptionAddress = %08lX\n", rec->ExceptionAddress);
    if (rec->NumberParameters != 0) {
        sprintf(log + strlen(log), "Parameters = ");
        for (unsigned int i = 0; i < rec->NumberParameters; i++)
            // The i % 3 == 3 test is always false (i % 3 is 0..2); kept as-is.
            sprintf(log + strlen(log), "%08lX%c", rec->ExceptionInformation[i],
                    (char)(i == rec->NumberParameters - 1 ? '\n'
                                                          : i % 3 == 3 ? '\n' : '\t'));
    }
    sprintf(log + strlen(log), "\n");
    sprintf(log + strlen(log), "Registers:\n");
    sprintf(log + strlen(log), "EAX=%08lX CS=%04lX EIP=%08lX EFLGS=%08lX\n",
            ctx->Eax, ctx->SegCs, ctx->Eip, ctx->EFlags);
    sprintf(log + strlen(log), "EBX=%08lX SS=%04lX ESP=%08lX EBP=%08lX\n",
            ctx->Ebx, ctx->SegSs, ctx->Esp, ctx->Ebp);
    sprintf(log + strlen(log), "ECX=%08lX DS=%04lX ESI=%08lX FS=%08lX\n",
            ctx->Ecx, ctx->SegDs, ctx->Esi, ctx->SegFs);
    sprintf(log + strlen(log), "EDX=%08lX ES=%04lX EDI=%08lX GS=%08lX\n",
            ctx->Edx, ctx->SegEs, ctx->Edi, ctx->SegGs);
    // REGION r4 end

    // REGION r5 begin
    sprintf(log + strlen(log), "\n");
    sprintf(log + strlen(log), "Bytes at CS:EIP:\n");
    for (int i = 0; i < 0x10; i++)
        sprintf(log + strlen(log), "%02x%c", (*((unsigned char**)&ctx->Eip))[i],
                i == 0xf ? '\n' : ' ');
    sprintf(log + strlen(log), "\n");
    // REGION r5 end

    // REGION r6 begin
    int room = 0x7358 - (int)strlen(log) - 0x3e8;
    if (0 < room) {
        lstrcpynA(log + strlen(log), (char*)&obj, room);
        sprintf(log + strlen(log), "\n");
        sprintf(log + strlen(log), "Dr0 = %08lX\t", ctx->Dr0);
        sprintf(log + strlen(log), "Dr1 = %08lX\t", ctx->Dr1);
        sprintf(log + strlen(log), "Dr2 = %08lX\n", ctx->Dr2);
        sprintf(log + strlen(log), "Dr3 = %08lX\t", ctx->Dr3);
        sprintf(log + strlen(log), "Dr6 = %08lX\t", ctx->Dr6);
        sprintf(log + strlen(log), "Dr7 = %08lX\n", ctx->Dr7);
        sprintf(log + strlen(log), "\n");
        sprintf(log + strlen(log), "ContextFlags = %08lX\n", ctx->ContextFlags);
        sprintf(log + strlen(log), "Control Word = %08lX\t\t", ctx->FloatSave.ControlWord);
        sprintf(log + strlen(log), "StatusWord = %08lX\n", ctx->FloatSave.StatusWord);
        sprintf(log + strlen(log), "TagWord = %08lX\t\t", ctx->FloatSave.TagWord);
        sprintf(log + strlen(log), "ErrorOffset = %08lX\n", ctx->FloatSave.ErrorOffset);
        sprintf(log + strlen(log), "ErrorSelector = %08lX\t", ctx->FloatSave.ErrorSelector);
        sprintf(log + strlen(log), "DataOffset = %08lX\n", ctx->FloatSave.DataOffset);
        sprintf(log + strlen(log), "DataSelector = %08lX\t\t", ctx->FloatSave.DataSelector);
        // REGION r6 end

        // REGION r7 begin
        sprintf(log + strlen(log), "Cr0NpxState = %08lX\n", ctx->FloatSave.Cr0NpxState);
        sprintf(log + strlen(log), "\n\n\n\n\n");
    }
    FUN_004da3f0(log, 0x7358);
    if (file != 0) {
        WriteFile(file, log, strlen(log), &written, 0);
        CloseHandle(file);
    }
    FUN_004de110();
    return 0;
    // REGION r7 end
}

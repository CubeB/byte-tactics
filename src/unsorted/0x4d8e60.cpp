// Decompiled by Claude Sonnet 5.5 and deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free pass (issue #4435): BEST 92.9% (2647 of 2644 bytes), up from
// the 81.1% the earlier passes reached. The one lever that moved this far is
// the FORM of the `sprintf` destination, not its value. The earlier passes
// settled on `{ size_t L = strlen(log); sprintf(log + L, FMT, ...); }` and
// measured every alternative as worse; that is true of the sites they tried,
// but it is not the best form everywhere. The original computes the
// destination pointer into a register BEFORE it pushes any argument, and the
// pending-push count in the original's `lea` displacement (0x7f0 with none
// pending, 0x7f8 with two) is the giveaway: the pointer is a value MSVC
// materialises first, which is what a `char* d = log + strlen(log);` local in
// front of the call produces. Adopting that form at the right sites was worth
// 81.1 -> 92.0 in five steps, each measured with check.py on a scratch copy:
//   (1) the four `EAX=`/`EBX=`/`ECX=`/`EDX=` register-dump lines and the six
//       `Dr0..Dr7` lines: 81.1 -> 85.0;
//   (2) the `ContextFlags`/`Control Word`/`StatusWord`/`TagWord`/
//       `ErrorOffset`/`ErrorSelector`/`DataOffset`/`DataSelector` FPU-save
//       lines and `Cr0NpxState`: 85.0 -> 87.4, and 87.9 with Cr0NpxState;
//   (3) the r6 `room` computation and its `lstrcpynA`: the whole guarded block
//       as one expression, `if (0x7358 - (int)strlen(log) - 0x3e8 > 0) {`,
//       with the same expression repeated as the third argument, scores
//       87.9 -> 90.3; hoisting it into an `int room` local is 88.1, and a
//       single `int room` local shared by the test and the call is 90.8;
//   (4) the r3 block (`Exception handler called in`, `FUN_004ded60`,
//       `Instruction pointer`, `ExceptionCode`): 90.3 -> 90.4, and 90.9 once
//       `FUN_004ded60` also takes a `char* d` destination;
//   (5) the `%02x%c` byte-dump loop body: 90.9 -> 91.5, and the r6 room
//       expression split into two statements (`room = 0x7358 - strlen(log);`
//       then `room = room - 0x3e8;`), the only form that keeps the original's
//       unfused `mov eax,0x7358 / sub eax,ecx / sub eax,0x3e8`: 91.5 -> 92.0.
// A sixth, unrelated one: the `" - %s\n"` line of r3 prints `reason`, not the
// file handle, worth 92.0 -> 92.9. The earlier passes reached the same reading
// of the disassembly (0x4d9171's `mov eax,[esp+0x1c]`, with three pushes still
// pending, is F+0x10, the `reason` slot) but kept `(char*)file` because on
// THEIR skeleton passing `reason` scored lower. On the fixed skeleton it wins,
// so that earlier measurement was an artefact of the surrounding form, not
// evidence against the source. The `L + log` and `log + L` spellings of that
// line are identical, as are the `char*` and `size_t*` casts on it.
// Still differs, and what I tried: the CreateFileA handle is copied to `esi`
// where the original keeps it in `eax` and reloads it from F+0x18 (four
// spellings of the assignment-in-condition all score identically, so this is a
// register-allocation tie, not a source problem); the parameters loop still
// uses a cached `n` with a `char`-cast ternary, where the original re-reads
// `rec->NumberParameters` from `[ebx+0x10]` every iteration, spills `info` to
// F+0x10 and builds the separator branchily (`mov dl,9 / jne / mov dl,0xa`) in
// `dl`, and every redesign of that loop I tried scored 88.5-91.9 against the
// 92.0 this form holds; the Dr0..Dr7 and the FPU-save lines alternate between
// two argument-scheduling shapes in the original (destination `lea` first for
// Dr0/Dr2/Dr6, argument load first for Dr1/Dr3/Dr7) and the alternation did
// not follow the source form in any of the twelve mixes I scored, so it looks
// like allocator state. Two tools/permute.py runs (16 min from 90.4%, and one
// from 92.0%) logged no improvement. An `inl1(p)` static inline helper that
// returns `p + strlen(p)` is WORSE than the plain local (79.6% across all 27
// destinations), so the win is the named local, not the extra function. The
// header set is not a lever here: adding <math.h>, or dropping <string.h>,
// both score identically, as do all four spellings of the "Access violation"
// write/read select and all four of the "module %s at" argument forms.
// Suspected original bugs, unchanged: the `i % 3 == 3` test in the parameters
// loop can never be true, so the per-three newline is dead code; CreateFileA's
// result is tested against 0 rather than INVALID_HANDLE_VALUE, so a failed
// open passes the check; and the lstrcpynA dump buffer at obj+0x2084 is copied
// with room = 0x7358 - strlen(log) - 0x3e8 regardless of its own length.
#include <windows.h>
#include <stdio.h>
#include <string.h>

class Class_004d9c60 {
public:
    char unknown_0[0x2084];
    char dump_text[0xa44c];
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

// The address of the byte count WriteFile fills in. Taking it in a helper is
// what makes the call sites compile to the original's argument sequence.
static inline DWORD* inl0(DWORD written) { return &written; }

// The game's structured-exception reporter: builds a text dump in a 0x7358-byte
// stack buffer and appends it to ErrorLog.txt next to the executable.
// FUNCTION: 0x4d8e60
int __cdecl FUN_004d8e60(EXCEPTION_POINTERS* ep, char* handlerName)
{
    int tmp7;
    unsigned int tmp0;
    size_t tmp4;
    HANDLE file;
    unsigned long* info;
    char* dot, * base, * tmp1, name[1000], path[1000], log[0x7358], exe[1000];
    Class_004d9c60 obj;
    DWORD written;
    char* reason;

    if (DAT_005289c0)
        return 0;
    DAT_005289c0 = 1;
    CONTEXT* ctx = ep->ContextRecord;
    ctx = (CONTEXT*)ctx;
    ctx = ctx;
    EXCEPTION_RECORD* rec;
    rec = ep->ExceptionRecord;
    obj.FUN_004d9c60(ctx->Ebp, ctx->Esp, ctx->Eip, 0);
    ((Class_004d9ca0*)&obj)->FUN_004d9ca0();

    // REGION r1 begin: find the executable's directory and open the log there
    char* slash;
    if (0 == GetModuleFileNameA(0, path, 1000) || !(((slash = strrchr(path, '\\')) != 0) != 0)) { strcpy(path, "C:\\"); } else { slash[1] = 0; }
    strcat(path, "ErrorLog.txt");
    file = CreateFileA(path, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file) { SetFilePointer(file, 0, 0, FILE_END); } else {
    }
    log[0] = 0;
    char* tmp5;
    tmp5 = FUN_004d98c0(rec->ExceptionCode);
    reason = tmp5;
    reason = reason;
    reason = reason;
    // REGION r1 end

    // REGION r2 begin: the "what crashed" header, written out early
    if (((0 < GetModuleFileNameA(0, exe, 1000)) != 0)) {
        base = strrchr(exe, '\\');
        base = base != 0 ? 1 + base : exe;
        strcpy(name, ((char*)base));
        dot = strrchr(name, '.');
        if (((int)(0 != dot)) != 0) { *((char*)dot) = 0; }
        do {
            sprintf(log + strlen(log), "%s caused an %s in\n", name, ((char*)reason));
            { {
                    size_t L;
                    L = strlen(log);
                    sprintf((((size_t)L)) + log, "module %s at %04x:%08lx.\n", ((base)), ctx->SegCs, ctx->Eip);
                } }
            if (0 == file) goto skip0;
            WriteFile(file, log, strlen(log), inl0(written), 0);
    skip0:;
        } while (0);
    }
    // REGION r2 end

    // REGION r3 begin: the handler name, the module walk and the code/data
    // address the fault happened at
    log[0] = 0;
    { char* d = log + strlen(log); sprintf(d, "Exception handler called in %s. ", ((char*)handlerName)); }
    { char* d = log + strlen(log); FUN_004ded60(d, 0x7358 - strlen(log)); }
    { char* d = log + strlen(log); sprintf(d, "Instruction pointer is %08lX\n", ctx->Eip); }
    { char* d = log + strlen(log); sprintf(d, "ExceptionCode = %08lX", rec->ExceptionCode); }
    { size_t L = strlen(log);
    L = strlen(log); do sprintf(L + log, " - %s\n", reason); while (0); }
    if (0xc0000005 == rec->ExceptionCode) {
        if (rec->NumberParameters >= 2) goto skip12;
        goto skip6;
    skip12:;
        if (0 != ((char(__cdecl*)(unsigned long))FUN_004d8680)(rec->ExceptionInformation[1])) { char* d = log + strlen(log);
                                                                    do sprintf(d, "Error: Write to read only memory attempted\n"); while (0); }
                                                                { size_t tmp6, L = strlen(log);
                                                                tmp6 = (size_t)L;
                                                                sprintf((((size_t)tmp6)) + log, "Access violation: Illegal %s, data address 0x%08lX\n",
                                                                        0 != rec->ExceptionInformation[0] ? "write" : "read", rec->ExceptionInformation[1]); }
    skip6:;
    }
    // REGION r3 end

    // REGION r4 begin: the exception record, then the general registers
    { size_t L = strlen(log), same0 = L, same3;
    L = same0; sprintf(((size_t)L) + log, "ExceptionFlags = %08lX\t", rec->ExceptionFlags); }
    { size_t L;
    L = strlen(log); sprintf(log + ((size_t)L), "ExceptionAddress = %08lX\n", rec->ExceptionAddress); }
    if (rec->NumberParameters > 0) {
        unsigned int n;
        { size_t L = strlen(log);
        L = L; sprintf(log + L, "Parameters = "); }
        n = rec->NumberParameters;
        info = rec->ExceptionInformation;
        unsigned int i = 0;
        tmp0 = (unsigned int)rec->NumberParameters;
        tmp7 = (int)(((int)i) >= (tmp0));
        if (!((int)((tmp7)))) { goto skip9; }
        goto skip4;
    skip9:;
        while (1) {
        sprintf(log + strlen(log), "%08lX%c", *info,
                    ((char)(((((int)i) == (n) - 1) || i % 3 == 3) ? '\n' : '\t'))); i++, info++;
            if (((int)i) >= ((unsigned int)n)) { break; } else {
            }
        }
    skip4:;
    }
    { size_t L;
    L = strlen(log); sprintf(log + L, "\n"); }
    { size_t L = strlen(log); sprintf(log + L, "Registers:\n"); }
    { char* d = log + strlen(log);
    sprintf(d, "EAX=%08lX CS=%04lX EIP=%08lX EFLAGS=%08lX\n",
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
    // REGION r4 end

    // REGION r5 begin: the 16 bytes of code at the faulting address
    { size_t L = strlen(log); sprintf(log + L, "\n"); }
    { {
        size_t L = strlen(log); sprintf(L + log, "Bytes at CS:EIP:\n");
    } }
    unsigned char* code;
    code = *((unsigned char**)&ctx->Eip);
    int i = 0;
    if (i < 0x10) goto skip7;
    goto skip5;
skip7:;
    do { char* d = log + strlen(log);
    sprintf(d, "%02x%c", code[((int)i)],
                ((int)i) == 0xf ? '\n' : ' ');
        i = 1 + ((int)i);
    } while (0x10 > i);
skip5:;
    { size_t L = strlen(log);
    sprintf((((size_t)L)) + log, "\n"); }
    // REGION r5 end

    // REGION r6 begin: the disassembler's own dump, the debug registers and
    // the saved FPU state
    { int room = 0x7358 - (int)strlen(log);
    room = room - 0x3e8;
    if (room > 0) {
        lstrcpynA(log + strlen(log), obj.dump_text, room);
    } }
    { size_t L = strlen(log);
    L = ((L)); sprintf(log + ((size_t)L), "\n"); }
    { char* d = log + strlen(log); sprintf(d, "Dr0 = %08lX\t", ctx->Dr0); }
    { char* d = log + strlen(log); sprintf(d, "Dr1 = %08lX\t", ctx->Dr1); }
    { char* d = log + strlen(log); sprintf(d, "Dr2 = %08lX\n", ctx->Dr2); }
    { char* d = log + strlen(log); sprintf(d, "Dr3 = %08lX\t", ctx->Dr3); }
    { char* d = log + strlen(log); sprintf(d, "Dr6 = %08lX\t", ctx->Dr6); }
    { char* d = log + strlen(log); sprintf(d, "Dr7 = %08lX\n", ctx->Dr7); }    { {
        size_t tmp2;
        size_t L = strlen(log); tmp2 = ((size_t)L);
        sprintf((((size_t)tmp2)) + log, "\n");
    } }
    { char* d = log + strlen(log); sprintf(d, "ContextFlags = %08lX\n", ctx->ContextFlags); }
    { char* d = log + strlen(log); sprintf(d, "Control Word = %08lX\t\t", ctx->FloatSave.ControlWord); }
    { char* d = log + strlen(log); sprintf(d, "StatusWord = %08lX\n", ctx->FloatSave.StatusWord); }
    { char* d = log + strlen(log); sprintf(d, "TagWord = %08lX\t\t", ctx->FloatSave.TagWord); }
    { char* d = log + strlen(log); sprintf(d, "ErrorOffset = %08lX\n", ctx->FloatSave.ErrorOffset); }
    { char* d = log + strlen(log); sprintf(d, "ErrorSelector = %08lX\t", ctx->FloatSave.ErrorSelector); }
    { char* d = log + strlen(log); sprintf(d, "DataOffset = %08lX\n", ctx->FloatSave.DataOffset); }
    { char* d = log + strlen(log); sprintf(d, "DataSelector = %08lX\t\t", ctx->FloatSave.DataSelector); }    // REGION r6 end

    // REGION r7 begin: flush the buffer, close the log and exit the handler
    { char* d = log + strlen(log); sprintf(d, "Cr0NpxState = %08lX\n", ctx->FloatSave.Cr0NpxState); }
    { size_t L;
    L = strlen(log); sprintf(log + ((size_t)L), "\n\n\n\n\n"); }
skip8:;
    FUN_004da3f0(log, 0x7358);
    if (0 != file) goto skip2;
    goto skip3;
skip2:;
    WriteFile(file, log, strlen(log), &written, 0);
    CloseHandle(file);
skip3:;
    FUN_004de110();
    return 0;
    // REGION r7 end
}

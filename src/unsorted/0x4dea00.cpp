// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry in #3226: eight checker invocations, best remains 97.3%; no MATCH. A single-use helper and De Morgan predicate tied; split last condition, shared boolean, and local last variants regressed. Existing source still has the two branch-local register/order differences.
// #3031 retry by GPT-6.1-sol: five checks retained 97.3%; a ternary source
// form did not alter the two branch-local register/order differences.
// Retry (deepseek-v4.1-flash, issue 2857): re-confirmed 97.3%. The two
// inversions are the guide's scheduler tie-break class (deferred `add esp,0xc`
// around a call's pushes); 11 new source shapes (sep scope, addrs[i] temporaries,
// casts, deref form, while-loop, split found chain, ternary sep, `int k=i`,
// `(void)n;`) plus headers.py all stay at 97.3, so it is not source-reachable.
// BUG: `found` is declared before the loop and only ever set to 1, never reset
// per iteration, so once any address resolves a symbol every later separator
// becomes "\n" (cmp bl,bl/jne at 0x4dec73 and the mov bl,1 stores at
// 0x4deb29/0x4dec6f, with no zeroing store in the loop). It looks like it should
// be reset each iteration, but the shipping code keeps it sticky.
// NOT MATCHING yet (97.3%, 850 bytes both). Two codegen differences remain, both the
// same MSVC 5 allocation decision applied in opposite directions:
//  1. lines branch, `i == n - 1`: original emits `mov eax,n; lea ecx,[eax-1]; mov eax,i;
//     cmp eax,ecx`; ours emits `mov ecx,n; mov eax,i; dec ecx; cmp eax,ecx` (one byte
//     shorter, which also moves the else-branch target from 0x4dec9d to 0x4dec9b).
//  2. non-lines branch, same source `i == n - 1`: original emits `mov ecx,n; mov eax,i;
//     dec ecx; cmp eax,ecx`; ours emits `mov eax,n; lea ecx,[eax-1]; mov eax,i; cmp`.
//     The sprintf value is loaded into eax in the original, edx in ours.
// Everything else is byte-exact, including the frame (0xdfc), the loop preheader, all
// calls and the strcat/sprintf tail.
// What fixed the preheader and the a=addrs sink: writing the element accesses as
// addrs[i] (no explicit `unsigned long* a` in the loop). MSVC strength-reduces it and
// emits `a = addrs` inside the n > 0 guard block, matching the original.
// deepseek-v4.1-flash retries (scratch --sym only, all below the 97.3 best): swapping
// `i == n-1` to `n-1 == i` in either or both branches (96.9/96.9/96.6), swapping the ||
// operands (91.4/96.1), adding `found ||` to the else (93.8), ternary sep (78.5),
// `for (i = 0; ...)` (85.5), unsigned i (89.7), a temp local for addrs[i] (97.3),
// default-then-if sep (93.6), an extra pointer induction variable (97.3).
// tools/headers.py tried all 128 header sets, closest 97.3.
// The remaining flip is one allocator decision per branch; no source spelling tried
// reproduces it while keeping i at 0x14 and width at 0x20.
//
// deepseek-v4.1-flash retry #2 (10 minute box, scratch --sym only, all 97.3 or worse):
// the two branch flips are truly the allocator's register order for the same source.
// In the original lines branch n is loaded into eax and n-1 goes to a fresh ecx
// (lea), while the else branch loads n straight into ecx and dec's it. Both end with
// eax=i, ecx=n-1, cmp eax,ecx, so only the materialisation ordering differs.
// Tried and flat at 97.3: every equivalent spelling of `i == n - 1` (n-1==i changes
// cmp order to cmp ecx,eax and drops to 96.6/96.9; i+1==n, i-n==-1, n-i==1,
// i==n+(n-n)-1, i==n-(0*n)-1, i==n-1L and friends all canonicalise to identical
// bytes); `int nn = n;` and `int last = n - 1;` locals; `bool`/`unsigned char`/`BOOL`
// found; `!` inversion of either branch; parenthesising the || chain; moving the
// `found = 0` declaration (before the arrays drops to 85, so it must stay after the
// big buffers); reordering the sym.SizeOfStruct/MaxNameLength/disp stores (96.9).
// Likely a C2 scheduler/allocator tick that needs a source shape change elsewhere,
// not a spelling change in the condition. Leave the rest of the file as above.
#include <stdio.h>
#include <string.h>
#include <windows.h>

extern char FUN_004de4d0();
extern char __cdecl FUN_004de550(unsigned long addr, void* line, int* err);
extern void __cdecl FUN_004de8a0(char* out, const char* name);

struct Line_004dea00 {
    unsigned long SizeOfStruct;
    void* Key;
    unsigned long LineNumber;
    char* FileName;
    unsigned long Address;
};

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

extern SymFn_004dea00 DAT_00528acc;
extern UnDecFn_004dea00 DAT_00528ad4;

// FUNCTION: 0x4dea00
void __cdecl FUN_004dea00(char* dest, int space, int per, int n, unsigned long* addrs)
{
    int i = 0;
    space--;
    char lines = FUN_004de4d0();
    int width;
    int disp;
    int err;
    Line_004dea00 line;
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
        const char* sep;
        if (space <= width)
            break;
        if (lines) {
            if (FUN_004de550(addrs[i], &line, &err)) {
                FUN_004de8a0(path, line.FileName);
                sprintf(dest, "%s(%d) : %08lX", path, line.LineNumber, addrs[i]);
                found = 1;
            } else {
                sprintf(dest, "%08lX", addrs[i]);
            }
            sym.SizeOfStruct = 0x218;
            sym.MaxNameLength = 0x200;
            disp = 0;
            if (DAT_00528acc(GetCurrentProcess(), addrs[i], &disp, &sym)) {
                char* str;
                if (DAT_00528ad4 && DAT_00528ad4(sym.Name, undec, 0x7d0, 0) > 0) {
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
            if (found || i == n - 1 || i % per == per - 1)
                sep = "\n";
            else
                sep = " ";
        } else {
            sprintf(dest, "%08lX", addrs[i]);
            if (i == n - 1 || i % per == per - 1)
                sep = "\n";
            else
                sep = " ";
        }
        strcat(dest, sep);
        space -= strlen(dest);
        dest += strlen(dest);
    }
}

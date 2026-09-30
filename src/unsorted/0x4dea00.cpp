// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
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

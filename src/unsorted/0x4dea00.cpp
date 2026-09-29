// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// NOT MATCHING yet (96.6%, 850 bytes both). Only two small codegen differences remain:
//  1. Loop preheader: the original tests `n` BEFORE storing `a = addrs` (the store is
//     sunk past the guard); mine stores a = addrs first, then tests n. Two instructions
//     transposed.
//  2. `i == n - 1` in the lines branch: the original emits
//     `mov eax,n; lea ecx,[eax-1]; mov eax,i; cmp eax,ecx`; mine emits
//     `mov ecx,n; mov eax,i; dec ecx; cmp eax,ecx`. The non-lines branch has the
//     opposite lea/dec choice in both versions, so this is one allocation decision.
// Everything else (frame 0xdfc, all local offsets, every call sequence, the merged
// `sprintf` tail through a `char* str` local) is byte-exact.
#include <stdio.h>
#include <string.h>
#include <windows.h>

extern char FUN_004de4d0();
extern char FUN_004de550(unsigned long addr, void* line, int* err);
extern void FUN_004de8a0(char* out, const char* name);

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
    unsigned long* a;
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
    a = addrs;
    for (; i < n; i++, a++) {
        const char* sep;
        if (space <= width)
            break;
        if (lines) {
            if (FUN_004de550(*a, &line, &err)) {
                FUN_004de8a0(path, line.FileName);
                sprintf(dest, "%s(%d) : %08lX", path, line.LineNumber, *a);
                found = 1;
            } else {
                sprintf(dest, "%08lX", *a);
            }
            sym.SizeOfStruct = 0x218;
            sym.MaxNameLength = 0x200;
            disp = 0;
            if (DAT_00528acc(GetCurrentProcess(), *a, &disp, &sym)) {
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
            sprintf(dest, "%08lX", *a);
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

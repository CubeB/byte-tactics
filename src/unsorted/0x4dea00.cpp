// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Claude Opus 5.5. Names are provisional.
// MATCH. Formats a call stack into dest: "Call stack:" then one entry per
// address, with file/line and symbol information when imagehlp lines are on.
//
// What took it from 97.3% to MATCH: each separator is its own strcat call in
// its own branch, and the found case has a strcat of its own:
//     if (found) strcat(dest, "\n");
//     else if (i == n - 1 || i % per == per - 1) strcat(dest, "\n");
//     else strcat(dest, " ");
// MSVC cross-jumps the inlined strcats into the original's single
// `mov edi, sep; jmp tail`, so a `sep` variable with one shared strcat
// compiles to the same instructions, but the extra strcat copies are still
// code-generated first and each one moves C2's eax/ecx/edx rotation for
// expression temporaries (c2prio.py --rotation). The found branch's copy is
// generated before the `i == n - 1` test and leaves the rotation on eax, which
// gives the original's `mov eax,n; lea ecx,[eax-1]` there; the lines arm's
// other copies leave it on eax again for the plain arm, which gives
// `mov eax,[esi]` and `mov ecx,n; dec ecx`. Every `sep` spelling (ternary,
// helpers, De Morgan, locals, headers, the permuter) stayed at 97.3%.
//
// `found` is set before the loop and never reset, so once any address
// resolves, every later separator is "\n" (no zeroing store in the loop; the
// mov bl,1 stores at 0x4deb29 and 0x4dec6f). Probably intended: once there is
// symbol information, one entry per line.
#include <stdio.h>
#include <string.h>
#include <windows.h>

struct Line_004de550 {
    DWORD SizeOfStruct;
    DWORD Key;
    DWORD LineNumber;
    DWORD FileName;
    DWORD Address;
};

extern char FUN_004de4d0();
extern char __cdecl FUN_004de550(DWORD addr, Line_004de550* out, DWORD* err);
extern void __cdecl FUN_004de8a0(char* out, char* name);

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
            if (FUN_004de550(addrs[i], &line, &err)) {
                FUN_004de8a0(path, (char*)line.FileName);
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

// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Claude Opus 5.5. Names are provisional.
// Formats a call stack into dest: "Call stack:" then one entry per
// address, with file/line and symbol information when imagehlp lines are on.
//
// `found` is set before the loop and never reset, so once any address
// resolves, every later separator is "\n". Probably intended: once there is
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
extern char __cdecl GetLineFromAddress(DWORD addr, Line_004de550* out, DWORD* err);
extern void __cdecl GetSourceFilePath(char* out, char* name);

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

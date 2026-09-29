// Decompiled by space-bunny-free. Names are provisional.
// NOT MATCHING yet (64.6%). The whole body and every call sequence are right,
// but MSVC keeps more of the scalar locals in registers than the original:
// the original has i, addrs, disp, width, err and space all in the frame
// (i at esp+0x14, addrs at +0x18, disp +0x1c, width +0x20, err +0x24, and
// space in arg2's home slot), which makes the frame 0xdfc instead of my 0xdf0.
// That single allocation difference shifts every lea/mov displacement in the
// body, so the remaining diff is mostly offsets.  One more shape diff: the
// original computes `n - 1` with `lea ecx, [eax-1]` and compares i against it,
// mine uses `dec`.
// Appends a symbolised call stack to a text buffer: one entry per address,
// with source file and line from imagehelp when "imagehlplines" is on.
// DATABASE: 0x528acc is SymGetSymFromAddr, 0x528ad4 UnDecorateSymbolName.
#include <stdio.h>
#include <string.h>
#include <windows.h>

extern char FUN_004de4d0();
extern char FUN_004de550(int addr, void* sym, int* err);
extern void FUN_004de8a0(char* out, char* name);

// The five dwords FUN_004de550 copies out of its own SYMBOL.
struct Sym20_004dea00 {
    int size;
    int typeIndex;
    int reserved;
    char* name;
    int line;
};

// The symbol buffer handed to SymGetSymFromAddr, plus the two name buffers.
struct Sym_004dea00 {
    int size;                        // +0x000
    int unknown_04;                  // +0x004
    int unknown_08;                  // +0x008
    int unknown_0c;                  // +0x00c
    int nameSize;                    // +0x010
    char name[0x204];                // +0x014
    char path[1000];                 // +0x218
    char undecorated[0x7c0];         // +0x600
};

typedef BOOL (__stdcall *SymGetSymFromAddr_t)(HANDLE, unsigned long, int*, Sym_004dea00*);
typedef DWORD (__stdcall *UnDecorateSymbolName_t)(char*, char*, DWORD, DWORD);

extern SymGetSymFromAddr_t DAT_00528acc;
extern UnDecorateSymbolName_t DAT_00528ad4;

// FUNCTION: 0x4dea00
void __cdecl FUN_004dea00(char* dest, int len, int per, int n, unsigned long* addrs)
{
    int i = 0;
    char lines = FUN_004de4d0();
    int space = len - 1;
    unsigned long* a = addrs;
    int disp;
    int width = 15;
    int err;
    char found = 0;
    Sym20_004dea00 sym20;
    Sym_004dea00 sym;

    strcpy(dest, lines ? "Call stack:\n" : "Call stack: ");
    space -= strlen(dest);
    dest += strlen(dest);
    if (lines)
        width = 800;

    for (; i < n; i++, a++) {
        if (space <= width)
            break;
        const char* sep;
        if (lines) {
            if (FUN_004de550(*a, &sym20, &err)) {
                FUN_004de8a0(sym.path, sym20.name);
                sprintf(dest, "%s(%d) : %08lX", sym.path, sym20.line, *a);
                found = 1;
            } else {
                sprintf(dest, "%08lX", *a);
            }
            sym.size = 0x218;
            sym.nameSize = 0x200;
            disp = 0;
            if (DAT_00528acc(GetCurrentProcess(), *a, &disp, &sym)) {
                if (DAT_00528ad4 && DAT_00528ad4(sym.name, sym.undecorated, 0x7d0, 0) > 0) {
                    sprintf(dest + strlen(dest), " - %s + %d", sym.undecorated, disp);
                } else {
                    if (strcmp(sym.name, "??2@YAPAXI@Z") == 0 ||
                        strcmp(sym.name, "??2@YAPAXIPBDH@Z") == 0)
                        strcat(sym.name, " - operator new");
                    sprintf(dest + strlen(dest), " - %s + %d", sym.name, disp);
                }
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

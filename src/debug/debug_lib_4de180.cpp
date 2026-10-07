// Decompiled by space-bunny-free. Names are provisional.
// Loads IMAGEHLP.DLL, resolves the symbol functions the crash reporter and the
// stack walker need, then SymInitialize()s the symbol handler with a search
// path of "<windir>;<directory of this exe>".
#include <windows.h>
#include <string.h>
#include <stdlib.h>

class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

void UnloadImageHelp(void);
void __stdcall FUN_004de0a0(int unused, unsigned int address);
void __stdcall FUN_004de100(int arg1, int arg2);

typedef DWORD (__stdcall *SymSetOptions_004de180)(DWORD);
typedef BOOL (__stdcall *SymInitialize_004de180)(HANDLE, char*, DWORD);
typedef void (__stdcall *SymProc_004de180)(void);

extern char DAT_00528ad8;
extern char DAT_00528adc;
extern HMODULE DAT_00528ae0;
extern SymSetOptions_004de180 DAT_00528ad0;
extern SymInitialize_004de180 DAT_00528ab8;
extern SymProc_004de180 DAT_00528abc;
extern SymProc_004de180 DAT_00528ac0;
extern SymProc_004de180 DAT_00528ac4;
extern SymProc_004de180 DAT_00528ac8;
extern SymProc_004de180 DAT_00528acc;
extern SymProc_004de180 DAT_00528ab4;
extern SymProc_004de180 DAT_00528ad4;

// FUNCTION: 0x4de180
char __cdecl LoadImageHelp(char param)
{
    static Class_004d9fe0 imagehlp("imagehlp", 0, 1, "-enableimagehlp",
                                   "-disableimagehlp", 0, 0);
    char path[0x100];
    char symPath[0x3e8];
    // Declared bare and assigned 0 as a statement just before GetModuleFileNameA.
    char* searchPath;
    char* windir;
    char* slash;
    typedef int (__stdcall *GetProcAddress_004de180)(HMODULE, char*);
    GetProcAddress_004de180 getProcAddress = (GetProcAddress_004de180)GetProcAddress;
    HANDLE (__stdcall *getCurrentProcess)(void) = GetCurrentProcess;

    if (!param && !imagehlp.on)
        return 0;
    if (DAT_00528ad8)
        return DAT_00528adc;
    DAT_00528ad8 = 1;
    if (!(DAT_00528ae0 = LoadLibraryA("IMAGEHLP.DLL")))
        return 0;
    if (!(DAT_00528ad0 = (SymSetOptions_004de180)getProcAddress(DAT_00528ae0, "SymSetOptions")))
        return 0;
    if (!(DAT_00528ab8 = (SymInitialize_004de180)getProcAddress(DAT_00528ae0, "SymInitialize")))
        return 0;
    if (!(DAT_00528abc = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymCleanup")))
        return 0;
    if (!(DAT_00528ac0 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "StackWalk")))
        return 0;
    if (!(DAT_00528ac4 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymFunctionTableAccess")))
        return 0;
    if (!(DAT_00528ac8 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymGetModuleBase")))
        return 0;
    DAT_00528acc = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymGetSymFromAddr");
    DAT_00528ab4 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymGetLineFromAddr");
    DAT_00528ad4 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "UnDecorateSymbolName");
    DWORD symOpts = 4;
    if (DAT_00528ab4)
        symOpts = 0x14;
    DAT_00528ad0(symOpts);
    searchPath = 0;
    if (GetModuleFileNameA((HMODULE)searchPath, path, sizeof(path))) {
        windir = getenv("windir");
        if (windir) {
            if (strlen(path) + strlen(windir) < 0x3e8) {
                strcpy(symPath, windir);
                slash = strrchr(path, '\\');
                if (slash) {
                    *slash = 0;
                    strcat(symPath, ";");
                    strcat(symPath, path);
                }
                searchPath = symPath;
            }
        }
    }
    // The second SymInitialize passes this result as its last argument, not a literal.
    BOOL inited = DAT_00528ab8(getCurrentProcess(), searchPath, 1);
    if (!inited) {
        DAT_00528ac4 = (SymProc_004de180)FUN_004de0a0;
        DAT_00528ac8 = (SymProc_004de180)FUN_004de100;
        DAT_00528acc = 0;
        DAT_00528ab4 = 0;
        if (!DAT_00528ab8(getCurrentProcess(), searchPath, inited)) {
            GetLastError();
            UnloadImageHelp();
            DAT_00528ad8 = 1;
            return 0;
        }
    }
    DAT_00528adc = 1;
    return 1;
}

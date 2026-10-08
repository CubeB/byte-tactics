// Decompiled by space-bunny-free. Names are provisional.
// Loads IMAGEHLP.DLL, resolves the symbol functions the crash reporter and the
// stack walker need, then SymInitialize()s the symbol handler with a search
// path of "<windir>;<directory of this exe>".
#include <windows.h>
#include <string.h>
#include <stdlib.h>

class CommandLineSwitch {
public:
    char on;                           // +0x0
    CommandLineSwitch(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~CommandLineSwitch() {}
};

void UnloadImageHelp(void);
void __stdcall FunctionTableAccess(int unused, unsigned int address);
void __stdcall GetModuleBase(int arg1, int arg2);

typedef DWORD (__stdcall *SymSetOptions_004de180)(DWORD);
typedef BOOL (__stdcall *SymInitialize_004de180)(HANDLE, char*, DWORD);
typedef void (__stdcall *SymProc_004de180)(void);

extern char g_imageHelpLoaded;
extern char g_imageHelpInited;
extern HMODULE g_imageHelpModule;
extern SymSetOptions_004de180 g_pfnSymSetOptions;
extern SymInitialize_004de180 g_pfnSymInitialize;
extern SymProc_004de180 g_pfnSymCleanup;
extern SymProc_004de180 g_pfnStackWalk;
extern SymProc_004de180 g_pfnSymFunctionTableAccess;
extern SymProc_004de180 g_pfnSymGetModuleBase;
extern SymProc_004de180 g_pfnSymGetSymFromAddr;
extern SymProc_004de180 g_pfnSymGetLineFromAddr;
extern SymProc_004de180 g_pfnUnDecorateSymbolName;

// FUNCTION: 0x4de180
char __cdecl LoadImageHelp(char param)
{
    static CommandLineSwitch imagehlp("imagehlp", 0, 1, "-enableimagehlp",
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
    if (g_imageHelpLoaded)
        return g_imageHelpInited;
    g_imageHelpLoaded = 1;
    if (!(g_imageHelpModule = LoadLibraryA("IMAGEHLP.DLL")))
        return 0;
    if (!(g_pfnSymSetOptions = (SymSetOptions_004de180)getProcAddress(g_imageHelpModule, "SymSetOptions")))
        return 0;
    if (!(g_pfnSymInitialize = (SymInitialize_004de180)getProcAddress(g_imageHelpModule, "SymInitialize")))
        return 0;
    if (!(g_pfnSymCleanup = (SymProc_004de180)getProcAddress(g_imageHelpModule, "SymCleanup")))
        return 0;
    if (!(g_pfnStackWalk = (SymProc_004de180)getProcAddress(g_imageHelpModule, "StackWalk")))
        return 0;
    if (!(g_pfnSymFunctionTableAccess = (SymProc_004de180)getProcAddress(g_imageHelpModule, "SymFunctionTableAccess")))
        return 0;
    if (!(g_pfnSymGetModuleBase = (SymProc_004de180)getProcAddress(g_imageHelpModule, "SymGetModuleBase")))
        return 0;
    g_pfnSymGetSymFromAddr = (SymProc_004de180)getProcAddress(g_imageHelpModule, "SymGetSymFromAddr");
    g_pfnSymGetLineFromAddr = (SymProc_004de180)getProcAddress(g_imageHelpModule, "SymGetLineFromAddr");
    g_pfnUnDecorateSymbolName = (SymProc_004de180)getProcAddress(g_imageHelpModule, "UnDecorateSymbolName");
    DWORD symOpts = 4;
    if (g_pfnSymGetLineFromAddr)
        symOpts = 0x14;
    g_pfnSymSetOptions(symOpts);
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
    BOOL inited = g_pfnSymInitialize(getCurrentProcess(), searchPath, 1);
    if (!inited) {
        g_pfnSymFunctionTableAccess = (SymProc_004de180)FunctionTableAccess;
        g_pfnSymGetModuleBase = (SymProc_004de180)GetModuleBase;
        g_pfnSymGetSymFromAddr = 0;
        g_pfnSymGetLineFromAddr = 0;
        if (!g_pfnSymInitialize(getCurrentProcess(), searchPath, inited)) {
            GetLastError();
            UnloadImageHelp();
            g_imageHelpLoaded = 1;
            return 0;
        }
    }
    g_imageHelpInited = 1;
    return 1;
}

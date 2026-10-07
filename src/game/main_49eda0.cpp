// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

#pragma pack(push, 1)
struct Game_0049ee30 {
    char unknown_0[0x2c74];
    unsigned short field_2c74;               // +0x2c74
    char unknown_2c76[0x37f31 - 0x2c76];
    int field_37f31;                         // +0x37f31
    int field_37f35;                         // +0x37f35
    char unknown_37f39[0x39245 - 0x37f39];
    int field_39245;                         // +0x39245
};
#pragma pack(pop)

extern Game_0049ee30* g_game;
extern int DAT_0051fb48;
extern char DAT_0051fb50[];
extern GUID DAT_004fcfb8;

// dsetup.h's DIRECTXREGISTERAPPA.
struct DirectXRegisterApp {
    DWORD dwSize;
    DWORD dwFlags;
    LPSTR lpszApplicationName;
    LPGUID lpGUID;
    LPSTR lpszFilename;
    LPSTR lpszCommandLine;
    LPSTR lpszPath;
    LPSTR lpszCurrentDirectory;
};
typedef int (__stdcall* DirectXRegisterApplicationProc)(HWND, DirectXRegisterApp*);

int __stdcall GameMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                           LPSTR lpCmdLine, int nCmdShow);
int __stdcall ExceptionFilter(EXCEPTION_POINTERS* exception, const char* thread);
bool __cdecl FUN_004da0e0(const char* arg);
void __stdcall FUN_0045b670(char* param_1);
void __stdcall SetBypassDriveScan(int val);
void __stdcall SetCommandLineUnusedFlagL(int val);
void __stdcall FUN_0045b820(int flag, char* text);
void __stdcall SetDirectConnectAddress(char* param_1);
void __stdcall FUN_0045b860(int param_1);
void __stdcall SetPacketRate(int param_1);
char* __stdcall Translate(char* text);
void SetNoDirectSound();
void SetUseWindowsSound();

// The game's entry point: an exception in the main thread is reported,
// with the thread's name, before the process dies.
// FUNCTION: 0x49eda0
extern "C" int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                 LPSTR lpCmdLine, int nCmdShow)
{
    __try {
        return GameMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    } __except (ExceptionFilter(GetExceptionInformation(), "Main Thread")) {
    }
    return 0;
}

// Parses the command line. Returns 0 when the game should quit at once
// (after -r registered it with DirectPlay), 1 otherwise.
// FUNCTION: 0x49ee30
int __stdcall ParseCommandLine(char* cmdLine, char* appName)
{
    char* copy = (char*)_alloca(strlen(cmdLine) + 1);
    strcpy(copy, cmdLine);
    g_game->field_37f31 = 30;
    g_game->field_37f35 = 0;
    g_game->field_39245 = 0;

    char* p = strtok(copy, " \t");
    while (p) {
        if (*p != '-' && *p != '/') {
            strcpy(DAT_0051fb50, p);
        } else if (!FUN_004da0e0(p)) {
            switch (p[1]) {
            case 'B':
            case 'b':
                if (!*p)
                    p = strtok(NULL, " \t");
                if (p && *p) {
                    if (!_strcmpi(p, "lock"))
                        g_game->field_2c74 |= 1;
                    else if (!_strcmpi(p, "deathends"))
                        ;
                    else if (!_strcmpi(p, "deathplays"))
                        ;
                    else if (!_strcmpi(p, "deathmatch"))
                        ;
                    else if (!_strcmpi(p, "fixedloc"))
                        ;
                    else if (!_strcmpi(p, "mapping"))
                        ;
                    else if (!_strcmpi(p, "circlos"))
                        ;
                    else if (!_strcmpi(p, "truelos"))
                        ;
                    else if (!_strcmpi(p, "permlos"))
                        ;
                    else if (!_strcmpi(p, "cheating"))
                        ;
                    else if (!_strcmpi(p, "watching"))
                        ;
                }
                break;
            case 'C':
            case 'c':
                p += 2;
                if (!*p)
                    p = strtok(NULL, " \t");
                if (p && *p) {
                    try {
                        FUN_0045b670(p);
                    } catch (...) {
                    }
                }
                g_game->field_39245 = 1;
                break;
            case 'D':
            case 'd':
            {
                char mode = p[2];
                DAT_0051fb48 = 2;
                if (mode != 'f' && mode != 'F')
                    DAT_0051fb48 = 3;
                break;
            }
            case 'E':
            case 'e': {
                p += 2;
                if (!*p)
                    p = strtok(NULL, " \t");
                if (p && *p) {
                    int v = -1;
                    if (*p != '-')
                        v = atoi(p);
                    if (v < 0 || v > 100)
                        v = 0;
                    g_game->field_37f35 = v;
                }
                break;
            }
            case 'F':
            case 'f':
                SetBypassDriveScan(1);
                break;
            case 'H':
            case 'h':
                p += 2;
                if (*p) {
                    FUN_0045b820(1, p);
                } else {
                    char* q = strtok(NULL, " \t");
                    if (q && *q && *q != '-')
                        FUN_0045b820(1, q);
                }
                break;
            case 'L':
            case 'l':
                SetCommandLineUnusedFlagL(0);
                break;
            case 'N':
            case 'n':
                p += 2;
                if (!*p)
                    p = strtok(NULL, " \t");
                if (p && *p && *p != '-') {
                    char* colon = strchr(p, ':');
                    int n = atoi(p);
                    if (n == 1 && colon)
                        SetDirectConnectAddress(colon + 1);
                    FUN_0045b860(n);
                }
                g_game->field_39245 = 1;
                break;
            case 'P':
            case 'p':
                p += 2;
                if (!*p)
                    p = strtok(NULL, " \t");
                if (p && *p) {
                    int v = -1;
                    if (*p != '-')
                        v = atoi(p);
                    SetPacketRate(v);
                }
                break;
            case 'R':
            case 'r': {
                // Register with DirectPlay as a lobbyable application and quit.
                char* args = cmdLine + (p - copy) + 2;
                while (isspace(*args))
                    args++;
                // Local names are chosen for the frame slot order, and the message
                // reuses p: one more named local would add a slot.
                char moduleName[MAX_PATH];
                HMODULE setupDll;
                DWORD len = GetModuleFileNameA(NULL, moduleName, MAX_PATH);
                int registered = 0;
                if (len > 0) {
                    char drive[_MAX_DRIVE];
                    char ext[_MAX_EXT];
                    char folder[_MAX_DIR + 2];
                    char fname[_MAX_FNAME + _MAX_EXT];
                    char* lastChar;
                    DirectXRegisterApp dxra;
                    DirectXRegisterApplicationProc func;
                    fname[0] = 0;
                    ext[0] = 0;
                    _splitpath(moduleName, drive, folder + 2, fname, ext);
                    strcat(fname, ext);
                    folder[0] = drive[0];
                    folder[1] = drive[1];
                    lastChar = &folder[strlen(folder) - 1];
                    if (strchr("/\\", *lastChar))
                        *lastChar = 0;
                    memset(&dxra, 0, sizeof(dxra));
                    dxra.dwSize = sizeof(dxra);
                    dxra.lpszApplicationName = appName;
                    dxra.lpGUID = &DAT_004fcfb8;
                    dxra.lpszFilename = fname;
                    dxra.lpszCommandLine = args;
                    dxra.lpszPath = folder;
                    dxra.lpszCurrentDirectory = folder;
                    setupDll = LoadLibraryA("dsetup.dll");
                    if (setupDll) {
                        func = (DirectXRegisterApplicationProc)
                            GetProcAddress(setupDll, "DirectXRegisterApplicationA");
                        if (func)
                            registered = func(NULL, &dxra);
                    }
                    FreeLibrary(setupDll);
                }
                if (!registered) {
                    MessageBeep(MB_ICONHAND);
                    p = Translate("DirectPlay registration failed.");
                    MessageBoxA(NULL, p, appName, MB_ICONHAND);
                }
                return 0;
            }
            case 'S':
            case 's':
                SetNoDirectSound();
                break;
            case 'T':
            case 't':
                p += 2;
                if (!*p)
                    p = strtok(NULL, " \t");
                if (p && *p) {
                    int v = -1;
                    if (*p != '-')
                        v = atoi(p);
                    if (v < 30 || v > 300)
                        v = 30;
                    g_game->field_37f31 = v;
                }
                break;
            case 'W':
            case 'w':
                SetUseWindowsSound();
                break;
            }
        }
        p = strtok(NULL, " \t");
    }
    return 1;
}

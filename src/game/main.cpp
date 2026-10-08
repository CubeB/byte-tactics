// Decompiled by Opus, Haiku, space-bunny-free, deepseek-v4.1-flash, deepseek-v4.1 and Sonnet 5.5. Names are provisional.
// WinMain of Total Annihilation.
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <new>

// Appends a string (with its terminating zero) to DEBUG.FIL, creating the
// file if it cannot be opened for appending.
struct FileHandle;

FileHandle* __stdcall HAPI_OpenFileAppend(char* path);
FileHandle* __stdcall HAPI_CreateFile(char* path);
unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);
int __stdcall HAPI_CloseFile(FileHandle* file);

// FUNCTION: 0x49e640
void __stdcall AppendToDebugFile(char* text)
{
    FileHandle* file = HAPI_OpenFileAppend("DEBUG.FIL");
    if (!file)
        file = HAPI_CreateFile("DEBUG.FIL");
    HAPI_WriteFile(file, text, strlen(text) + 1);
    HAPI_CloseFile(file);
}

void OutOfMemoryHandler();
void __cdecl SetOutOfMemoryHandler(void (*param_1)());

// FUNCTION: 0x49e6f0
void InstallOutOfMemoryHandler()
{
    SetOutOfMemoryHandler(OutOfMemoryHandler);
}

// Out of memory handler: appends a note to ErrorLog.txt beside the
// executable, then reports and aborts.
void __stdcall ReportViaException(char* text);
void FUN_004d8390(void);

// FUNCTION: 0x49e700
void OutOfMemoryHandler()
{
    char path[1000];
    DWORD written;
    HANDLE file;
    char* slash;

    GetModuleFileNameA(NULL, path, 1000);
    slash = strrchr(path, '\\');
    if (slash)
        slash[1] = 0;
    else
        strcpy(path, "C:\\");
    strcat(path, "ErrorLog.txt");
    file = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file) {
        SetFilePointer(file, 0, NULL, FILE_END);
        WriteFile(file, "Out of memory!\r\nYour hard disk may be full\r\n",
                  strlen("Out of memory!\r\nYour hard disk may be full\r\n"), &written, NULL);
        CloseHandle(file);
    }
    ReportViaException("Out of memory handler");
    MessageBoxA(NULL, "Out of memory!\r\nYour hard disk may be full\r\n", "Total Annihilation", 0x41010);
    FUN_004d8390();
    raise(SIGABRT);
    _exit(3);
}

// space-bunny-free retry: MATCH (1365 of 1365 bytes), from 89.2%. Three
// changes, all in the g_displayContext initialisation block, none of them a new
// construct: the or chain, the b0 read-modify-write register and the whole
// eax/ecx/edx rotation fall out of them.
//  1. The field stores are in plain program order (startWidth, b0, b1, b8,
//     b4, b9, b5, b6, b7, startHeight, hInstance, nCmdShow, className,
//     title, menuId). The previous version interleaved the hInstance and
//     nCmdShow stores between the single-bit sets to hold the or chain apart;
//     that is not needed (MSVC 5 keeps the individual `or al/ah` ops anyway)
//     and it cost 1 byte and the store order. Only the order of the last four
//     stores matters for the bytes: hInstance BEFORE nCmdShow is the original
//     (className, hInstance, nCmdShow in that order scores 99.5%, nCmdShow,
//     className, hInstance 99.3%).
//  2. The bit-0 write is a bare `video.bits.b0 = ~g_cmdlineDisplaySeed;`. A named
//     `int notFlags` local compiles to 1367 bytes and a different schedule.
//     `b0 = b0 ^ notFlags`, `b0 ^= notFlags`, `b0 = (int)(~g_cmdlineDisplaySeed) & 1`
//     and an explicit `video.value ^ ((video.value ^ ~g_cmdlineDisplaySeed) & 1)` all
//     give the same 1365 bytes, so the lowering (byte load of the old b0, xor
//     into DL, and 1, xor into the word) is the only one available and the
//     delta register is not a source-level choice.
//  3. `#include <stdio.h>` is load bearing, and nothing in this file uses it,
//     exactly as docs/agent-guide.md warns: without it the function compiles
//     to 93.1% with the b0 delta in CL instead of DL, the single-bit sets in
//     the order b1, b4, b8, b5, b9, b6, b7, and the g_game and inlined-strcpy
//     register rotations one step out. tools/headers.py reaches 99.3% on the
//     93.1% body with <windows.h> <stdio.h> and with <windows.h>
//     <string.h> <stdio.h>; every other header set scores below that.
class Sound {
public:
    char pad[0x294];
    Sound();
    void SetTrackCategory(int param_1);
    void ReapFinishedBuffers();
};

class Class_004ce680 {
public:
    int GetTrackCategory();
};

class Class_004ce410 {
public:
    void CloseCdAudio();
};

class Class_004ce260 {
public:
    void OpenCdAudio();
};

class Class_004cd9d0 {
public:
    void SetCdCallback(void (*param_1)());
};

class Class_004cedc0 {
public:
    void EnableCdAudio(int param_1);
};

class Class_004ce7a0 {
public:
    void SetPlaybackOrder(int param_1);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[1];                       // +0
    unsigned char field_1;                   // +1
    unsigned char field_2;                   // +2
    unsigned char field_3;                   // +3
    char unknown_4[0xc - 4];                 // +4
    void* field_c;                           // +0xc
    void* field_10;                          // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned short field_2a44;               // +0x2a44
    char unknown_2a46[0x37f14 - 0x2a46];
    unsigned char field_37f14;               // +0x37f14
    unsigned char field_37f15;               // +0x37f15
    unsigned char field_37f16;               // +0x37f16
};
#pragma pack(pop)

extern Game* g_game;

extern unsigned char g_atexitRegistered;
extern char* g_appName;
extern char* g_windowClassName;
extern int g_cmdlineDisplaySeed;
extern char g_preferredLanguage[];
extern char DAT_005119b8[];
extern int g_cdTrackCategory;
extern int g_cdNeedsReopenAfterFocus;
extern DWORD g_lastSoundReapTick;

union Word_0051f522 {
    unsigned short value;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short b2 : 1;
        unsigned short b3 : 1;
        unsigned short b4 : 1;
        unsigned short b5 : 1;
        unsigned short b6 : 1;
        unsigned short b7 : 1;
        unsigned short b8 : 1;
        unsigned short b9 : 1;
        unsigned short spare : 6;
    } bits;
};
union Dword_0051f410 {
    int value;
    struct {
        unsigned b0 : 1;
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
        unsigned b9 : 1;
        unsigned b10 : 1;
        unsigned b11 : 1;
        unsigned spare : 20;
    } bits;
};

#pragma pack(push, 2)
struct App_0049e830 {
    int hInstance;                     // +0x00
    int nCmdShow;                      // +0x04
    int className;                     // +0x08
    int title;                         // +0x0c
    int unknown_10;                    // +0x10
    int menuId;                        // +0x14
    char unknown_18[0xe0 - 0x18];
    int field_e0;                      // +0xe0
    char unknown_e4[0xf0 - 0xe4];
    Dword_0051f410 flags;              // +0xf0
    char unknown_f4[0x1fa - 0xf4];
    int startWidth;                    // +0x1fa
    int startHeight;                   // +0x1fe
    Word_0051f522 video;               // +0x202
};
#pragma pack(pop)
extern App_0049e830 g_displayContext;


extern const char DAT_005097f4[];
extern const char g_languageValueName[];
extern const char g_translationFile[];
extern const char g_audioCdShellKey[];
extern const char g_cdShellValueName[];
extern const char g_defaultLanguage[];

void AtexitNoOp();
void __cdecl InitDebugSupport(int param_1);
void CreateGameObject();
void RegisterDataArchives();
void EmptyPreFrontendInitHook();
void InitGame();
void __stdcall InitDisplayDefaults(void* param_1);
int __stdcall InitEnvironment(void* param_1);
void __stdcall InitTimers(int param_1);
void __stdcall EmptyPostArchiveMountHook(const char* param_1);
void __stdcall ShutdownEnvironment(void* param_1);
void SaveCdLists();
void ReopenCdAudio();
void MainFrameTick();
void ShutdownMouse();
void ShutdownGame();
int __stdcall ParseCommandLine(char* param_1, char* param_2);
void __stdcall ReadGameRegistryValue(const char* param_1, void* param_2, int* param_3);
void __stdcall WriteGameRegistryValue(const char* param_1, void* param_2, int param_3);
void __stdcall LoadTranslations(const char* param_1, char* param_2);

// FUNCTION: 0x49e830
int __stdcall GameMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                           LPSTR lpCmdLine, int nCmdShow)
{
    HKEY hKey;
    DWORD size14;
    DWORD type;
    DWORD size1c;
    DWORD size20;
    MSG msg;
    char buf40[0x34];
    char buf74[0x32];
    int lzero = 0;

    (void)hPrevInstance;

    InitDebugSupport(8);
    SetOutOfMemoryHandler(OutOfMemoryHandler);
    if ((g_atexitRegistered & 1) == 0) {
        g_atexitRegistered |= 1;
        // atexit wants a __cdecl handler; /Gz makes AtexitNoOp __stdcall.
        atexit((void (__cdecl *)(void))AtexitNoOp);
    }
    CreateGameObject();
    HANDLE hSem = OpenSemaphoreA(0x1f0003, lzero, g_appName);
    if (hSem != (HANDLE)lzero)
        return -1;
    CreateSemaphoreA(NULL, 1, 1, g_appName);
    srand(time(0));
    if (ParseCommandLine(lpCmdLine, g_appName) == 0)
        return 1;
    InitDisplayDefaults(&g_displayContext);
    g_displayContext.startWidth = 0x280;
    g_displayContext.video.bits.b0 = ~g_cmdlineDisplaySeed;
    g_displayContext.video.bits.b1 = 1;
    g_displayContext.video.bits.b8 = 1;
    g_displayContext.video.bits.b4 = 1;
    g_displayContext.video.bits.b9 = 1;
    g_displayContext.video.bits.b5 = 1;
    g_displayContext.video.bits.b6 = 1;
    g_displayContext.video.bits.b7 = 1;
    g_displayContext.startHeight = 0x1e0;
    g_displayContext.hInstance = (int)hInstance;
    g_displayContext.nCmdShow = nCmdShow;
    g_displayContext.className = (int)g_windowClassName;
    g_displayContext.title = (int)g_appName;
    g_displayContext.menuId = 0;
    int bGameOk = InitEnvironment(&g_displayContext);
    if (bGameOk == lzero)
        return 0;
    InitTimers(0x1e);
    RegisterDataArchives();
    EmptyPostArchiveMountHook(DAT_005097f4);
    g_game->field_c = &g_displayContext;
    g_game->field_1 = 3;
    g_game->field_2 = 1;
    g_game->field_3 = 1;
    if (strlen(g_preferredLanguage) == 0) {
        size1c = 0x40;
        ReadGameRegistryValue(g_languageValueName, g_preferredLanguage, (int*)&size1c);
        if (g_preferredLanguage[0] == 0)
            strcpy(g_preferredLanguage, g_defaultLanguage);
    }
    LoadTranslations(g_translationFile, g_preferredLanguage);
    g_game->field_10 = new Sound;
    EmptyPreFrontendInitHook();
    InitGame();

    size20 = 0x32;
    hKey = NULL;
    size14 = 0x32;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_audioCdShellKey, 0, 0xf003f, &hKey) == 0) {
        if (RegQueryValueExA(hKey, NULL, NULL, &type, (LPBYTE)buf40, &size20) == 0) {
            RegSetValueExA(hKey, NULL, 0, type, (const BYTE*)DAT_005119b8, 1);
        } else {
            strcpy(buf40, DAT_005119b8);
        }
        size14 = 0x32;
        ReadGameRegistryValue(g_cdShellValueName, buf74, (int*)&size14);
        if (strlen(buf74) != 0)
            strcpy(buf40, buf74);
        else
            WriteGameRegistryValue(g_cdShellValueName, buf40, 0x32);
        RegFlushKey(hKey);
        RegCloseKey(hKey);
    }

    for (;;) {
        for (;;) {
            if (g_displayContext.field_e0 == lzero && *(int*)g_game->field_10 != 0) {
                SaveCdLists();
                g_cdTrackCategory = ((Class_004ce680*)g_game->field_10)->GetTrackCategory();
                ((Class_004ce410*)g_game->field_10)->CloseCdAudio();
                g_cdNeedsReopenAfterFocus = 1;
            } else if (g_displayContext.field_e0 != lzero && *(int*)g_game->field_10 == 0
                       && g_cdNeedsReopenAfterFocus != 0) {
                ((Class_004ce260*)g_game->field_10)->OpenCdAudio();
                ((Class_004cd9d0*)g_game->field_10)->SetCdCallback(ReopenCdAudio);
                ((Class_004cedc0*)g_game->field_10)->EnableCdAudio(g_game->field_37f14 & 1);
                ((Class_004ce7a0*)g_game->field_10)->SetPlaybackOrder(g_game->field_37f16);
                ((Sound*)g_game->field_10)->SetTrackCategory(g_cdTrackCategory);
                ReopenCdAudio();
                g_cdNeedsReopenAfterFocus = 0;
            }
            if (PeekMessageA(&msg, NULL, 0, 0, 0) != 0)
                break;
            if (g_displayContext.field_e0 == 0 && (g_game->field_2a44 & 1) == 0)
                break;
            MainFrameTick();
            {
                DWORD tick = GetTickCount();
                if ((int)(tick - g_lastSoundReapTick) >= 100) {
                    ((Sound*)g_game->field_10)->ReapFinishedBuffers();
                    g_lastSoundReapTick = tick;
                }
            }
        }
        if (GetMessageA(&msg, NULL, 0, 0) == 0)
            break;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (g_displayContext.flags.bits.b11) {
        ShutdownMouse();
        ShutdownGame();
    }
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_audioCdShellKey, 0, 0xf003f, &hKey) == 0) {
        RegSetValueExA(hKey, NULL, 0, type, (const BYTE*)buf40,
                       (DWORD)strlen(buf40) + 1);
        RegFlushKey(hKey);
        RegCloseKey(hKey);
        strcpy(buf40, DAT_005119b8);
        WriteGameRegistryValue(g_cdShellValueName, buf40, 0x32);
    }
    ShutdownEnvironment(&g_displayContext);
    return msg.wParam;
}

// FUNCTION: 0x49ed90
void AtexitNoOp(void)
{
}

// Changes the current directory to the one holding the executable.
char* __stdcall StripFileName(char* path);

// FUNCTION: 0x49f540
void ChdirToExeDirectory()
{
    char path[256];

    GetModuleFileNameA(NULL, path, 256);
    StripFileName(path);
    SetCurrentDirectoryA(path);
}

// FUNCTION: 0x49f580
int GetPreferredLanguage(void)
{
    return 0 < strlen(g_preferredLanguage) ? (int)(const void*)g_preferredLanguage : 0;
}

// FUNCTION: 0x49f5a0
UINT __stdcall GetPreferenceInt(char* key, int defaultValue)
{
    char exePath[256];
    char iniPath[256];

    GetModuleFileNameA(NULL, exePath, 256);
    StripFileName(exePath);
    sprintf(iniPath, "%s\\totala.ini", exePath);
    return GetPrivateProfileIntA("Preferences", key, defaultValue, iniPath);
}

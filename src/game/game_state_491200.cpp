// Decompiled by deepseek-v4.1-flash, finished by deepseek-v4.1, finished by
// deepseek-v4.1-flash, finished by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// MATCH, 1174 of 1174 bytes.
//
// The whole function was byte-identical except the second GlobalMemoryStatus,
// where the original fills the call's delay slot
//     lea edx, [esp+0x10]; push edx; mov dword ptr [esp+0x14], 0x20; call esi
// and this file emitted
//     lea edx, [esp+0x10]; mov dword ptr [esp+0x10], 0x20; push edx; call esi
// Five earlier passes (about 200 spellings of the second call) all left that
// tie alone, and concluded it was unreachable. It is reachable, and the lever
// is the SCOPE of the local, not its spelling:
//
//     { MEMORYSTATUS mem;  mem.dwLength = 0x20; GlobalMemoryStatus(&mem);  }
//     ...
//     { MEMORYSTATUS mem2; mem2.dwLength = 0x20; GlobalMemoryStatus(&mem2); }
//
// Each call needs its own block because that is what puts the dwLength store
// into the call's delay slot, and the two blocks are what makes both locals
// share one stack slot (the first is dead when the second block opens, so
// MSVC reuses [esp+0x10] and the frame stays 0x24 dwords). Both blocks must
// be nested: nesting only the second one grows the frame to 0x44 and moves
// the struct, nesting only the first does the same.
//
// How it was found (see build/scratch/0x491200/): a miniature of the tail
// (two `p->dwLength = 0x20; GlobalMemoryStatus(&p);` around a call) compiles
// in about 0.15 s, and shows that with one MEMORYSTATUS local shared by both
// calls the FIRST call gets the delay slot and the second never does, with
// the same statement lists either side. Giving the second call a block of its
// own with its own MEMORYSTATUS flips it to match. The mini also shows what
// else moves that store: three or more stores to globals between the two
// calls, and any call between them (which is why every spelling tried before
// could not reach it: SetCloseHandler sits between the two sites here).
//
// Scratch used: build/scratch/0x491200/{probe,mini,orc}.py and s1-s5.py.
#include <string.h>
#include <windows.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_004cedc0 {
public:
    void EnableCdAudio(int param_1);
};

class Class_004ce7a0 {
public:
    void SetPlaybackOrder(int param_1);
};

class Class_004cd9d0 {
public:
    void SetCdCallback(void (*param_1)());
};

class Class_004ce690 {
public:
    void SetTrackCategory(int param_1);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* field_10;                          // +0x10
    char unknown_14[0x519 - 0x14];
    char field_519[8];                       // +0x519
    int field_521;                           // +0x521
    char unknown_525[0x52d - 0x525];
    int field_52d;                           // +0x52d
    char unknown_531[0x589 - 0x531];
    int field_589;                           // +0x589
    char unknown_58d[0x12ef - 0x58d];
    char field_12ef[4];                      // +0x12ef
    char unknown_12f3[0x29a0 - 0x12f3];
    void* field_29a0;                        // +0x29a0
    char unknown_29a4[0x14280 - 0x29a4];
    unsigned char field_14280;               // +0x14280
    char unknown_14281[0x143a7 - 0x14281];
    char field_143a7[4];                     // +0x143a7
    char unknown_143ab[0x148cb - 0x143ab];
    unsigned short* field_148cb;             // +0x148cb
    char unknown_148cf[0x37e1b - 0x148cf];
    int field_37e1b;                         // +0x37e1b
    int field_37e1f;                         // +0x37e1f
    int field_37e23;                         // +0x37e23
    char unknown_37e27[0x37ee6 - 0x37e27];
    short field_37ee6;                       // +0x37ee6
    char unknown_37ee8[0x37eea - 0x37ee8];
    short field_37eea;                       // +0x37eea
    short field_37eec;                       // +0x37eec
    char unknown_37eee[0x37efe - 0x37eee];
    int field_37efe;                         // +0x37efe
    char unknown_37f02[0x37f08 - 0x37f02];
    int field_37f08;                         // +0x37f08
    char unknown_37f0c[0x37f14 - 0x37f0c];
    unsigned char field_37f14;               // +0x37f14
    char unknown_37f15[0x37f16 - 0x37f15];
    unsigned char field_37f16;               // +0x37f16
    char unknown_37f17[0x37f2f - 0x37f17];
    unsigned short field_37f2f;              // +0x37f2f
    char unknown_37f31[0x38a43 - 0x37f31];
    int field_38a43;                         // +0x38a43
    char unknown_38a47[0x38a4b - 0x38a47];
    short field_38a4b;                       // +0x38a4b
    short field_38a4d;                       // +0x38a4d
    char unknown_38a4f[0x38c53 - 0x38a4f];
    int field_38c53;                         // +0x38c53
    char unknown_38c57[0x38c5f - 0x38c57];
    int field_38c5f;                         // +0x38c5f
    int field_38c63;                         // +0x38c63
    int field_38c67;                         // +0x38c67
    char unknown_38c6b[0x38d6b - 0x38c6b];
    int field_38d6b;                         // +0x38d6b
    char unknown_38d6f[0x38d7b - 0x38d6f];
    int field_38d7b;                         // +0x38d7b
    char unknown_38d7f[0x391e9 - 0x38d7f];
    Class_00435100* field_391e9;             // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                         // +0x391f1
    void (*field_391f5)();                   // +0x391f5
    int field_391f9;                         // +0x391f9
    char unknown_391fd[0x3923b - 0x391fd];
    unsigned short field_3923b;              // +0x3923b
    char unknown_3923d[0x39249 - 0x3923d];
    int field_39249;                         // +0x39249
};
#pragma pack(pop)

extern Game* g_game;

extern const char DAT_005091d4[];          // "OFFSCREEN"
extern const char DAT_00509268[];          // "SkirmishInfo"
extern const char DAT_00509200[];          // "CDLISTS"
extern const char DAT_00502820[];          // "guis"
extern const char DAT_00502e30[];          // "anims"
extern const char DAT_0050338c[];          // "fonts"
extern const char DAT_0050925c[];          // "commongui"
extern const char DAT_00509250[];          // "hattfont12"
extern const char DAT_00509244[];          // "hattfont11"
extern const char DAT_00509238[];          // "UnitLimit"
extern int DAT_0051e828[];

int GetScreenWidth();
int GetScreenHeight();
int __stdcall AllocSurface(const char* name, int width, int height);
void __stdcall SetRestoreSurface(int param_1);
void __stdcall SetPageFlipping(int param_1);
void __stdcall FUN_00434ab0(int param_1);
void LoadGameResources();
void InitSound();
void ResetFrontendState();
void __stdcall InitPacketTables(void* param_1);
void RegisterAllOrderTypes();
void LoadGameFonts();
void FUN_0042a400();
void LoadAllSound();
void __stdcall LoadAlphaTable(void* param_1);
void __stdcall LoadShadeTable(void* param_1);
void __stdcall LoadLightTable(void* param_1);
void __stdcall MakeGrayTable(void* param_1);
void __stdcall MakeBlueTable(void* param_1);
void* __cdecl FUN_004d83b0(const char* name, int size);
void LoadSettings();
int __stdcall ReadGameRegistryValue(const char* name, void* buf, int* size);
void FUN_00490fe0();
void ApplyBrightnessAndVolume();
void LoadSideData();
void LoadLogos();
void __stdcall SetBrightness(float param_1);
void __stdcall SetCurrentGuiContext(void* param_1);
void __stdcall FUN_0049fba0(void* param_1, const char* name);
void __stdcall FUN_0049fbf0(void* param_1, const char* name);
void __stdcall FUN_0049fb50(void* param_1, const char* name);
void __stdcall FUN_004aa8e0(void* param_1, int param_2);
void* __stdcall GetGafFrame(void* param_1, int param_2);
void __stdcall FUN_004ab4e0(void* param_1, void* param_2);
void __stdcall LoadGafFile(void* param_1, const char* name);
void __stdcall LoadGafIntoSlot(void* param_1, const char* name, int param_3);
void __stdcall SetTextKeyColor(int param_1);
void __stdcall SetFont(int param_1);
void __stdcall SetCloseHandler(void (__cdecl *param_1)(), int param_2);
void ClearPictureCache();
int __stdcall GetPreferenceInt(const char* name, int param_2);
void FUN_00496a60();
void __cdecl LeaveNetGameCallback();

// FUNCTION: 0x491200
void InitGame()
{
    int size;

    {
        MEMORYSTATUS mem;
        mem.dwLength = 0x20;
        GlobalMemoryStatus(&mem);
    }
    g_game->field_37e1f = GetScreenWidth();
    g_game->field_37e23 = GetScreenHeight();
    g_game->field_37e1b = AllocSurface(DAT_005091d4, g_game->field_37e1f,
                                      g_game->field_37e23);
    SetRestoreSurface(g_game->field_37e1b);
    g_game->field_3923b &= 0xfffe;
    g_game->field_3923b &= 0xfffd;
    g_game->field_39249 = 0;
    FUN_00434ab0(0);
    if (g_game->field_391e9->FUN_00435100() == 3) {
        g_game->field_38a4b = 10;
        g_game->field_38a4d = 10;
    }
    g_game->field_38a43 = 0;
    g_game->field_37ee6 = g_game->field_37eec;
    g_game->field_37eea = g_game->field_37ee6;
    g_game->field_37efe = 3;
    g_game->field_37f2f &= 0xfdff;
    g_game->field_37f2f &= 0xff7f;
    g_game->field_37f2f &= 0xfeff;
    SetPageFlipping(0);
    LoadGameResources();
    InitSound();
    ResetFrontendState();
    InitPacketTables(g_game->field_12ef);
    RegisterAllOrderTypes();
    LoadGameFonts();
    FUN_0042a400();
    LoadAllSound();
    LoadAlphaTable(g_game->field_143a7);
    LoadShadeTable(g_game->field_143a7);
    LoadLightTable(g_game->field_143a7);
    MakeGrayTable(g_game->field_143a7);
    MakeBlueTable(g_game->field_143a7);
    g_game->field_29a0 = FUN_004d83b0(DAT_00509268, 0x22c);
    LoadSettings();
    size = 0xaa0;
    int ok = ReadGameRegistryValue(DAT_00509200, DAT_0051e828, &size);
    if (ok == 0)
        memset(DAT_0051e828, 0, 0xaa0);
    ((Class_004cedc0*)g_game->field_10)->EnableCdAudio(g_game->field_37f14 & 1);
    ((Class_004ce7a0*)g_game->field_10)->SetPlaybackOrder(g_game->field_37f16);
    ((Class_004cd9d0*)g_game->field_10)->SetCdCallback(FUN_00490fe0);
    FUN_00490fe0();
    ((Class_004ce690*)g_game->field_10)->SetTrackCategory(0);
    ApplyBrightnessAndVolume();
    LoadSideData();
    LoadLogos();
    SetBrightness(0.5 - g_game->field_37f08 * -0.041666668f);
    SetCurrentGuiContext(g_game->field_519);
    FUN_0049fba0(g_game->field_519, DAT_00502820);
    FUN_0049fbf0(g_game->field_519, DAT_00502e30);
    FUN_0049fb50(g_game->field_519, DAT_0050338c);
    FUN_004aa8e0(g_game->field_519, g_game->field_391f9);
    FUN_004ab4e0(g_game->field_519, GetGafFrame(g_game->field_148cb, 0));
    LoadGafFile(g_game->field_519, DAT_0050925c);
    LoadGafIntoSlot(g_game->field_519, DAT_00509250, 0);
    LoadGafIntoSlot(g_game->field_519, DAT_00509244, 1);
    g_game->field_52d = g_game->field_521;
    SetTextKeyColor(0xfe);
    SetFont(g_game->field_391f9);
    g_game->field_14280 = 0;
    g_game->field_38c53 = 0;
    g_game->field_38c5f = 0;
    g_game->field_38c63 = 0;
    g_game->field_38c67 = 0;
    g_game->field_38d6b = 0;
    g_game->field_38d7b = 0;
    g_game->field_391f1 = 0;
    g_game->field_391f5 = FUN_00496a60;
    SetCloseHandler(LeaveNetGameCallback, 0);
    {
        MEMORYSTATUS mem2;
        mem2.dwLength = 0x20;
        GlobalMemoryStatus(&mem2);
    }
    ClearPictureCache();
    g_game->field_589 = 1;
    int limit = GetPreferenceInt(DAT_00509238, 0xfa);
    if (limit > 500)
        limit = 500;
    else if (limit < 20)
        limit = 20;
    g_game->field_37eec = limit;
}

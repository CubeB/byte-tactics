// Decompiled by Sonnet, Opus, Space Bunny Free, deepseek-v4.1-flash, deepseek-v4.1, Sonnet 5.5 and space-bunny-free. Names are provisional.
// The game state: the offscreen surface, the game mode and its frame handler,
// the wind, the game speed, the CD list settings in the registry, start-up and
// shutdown of the game, the reset for a new match and the selection of a
// screen.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

class Mission {
public:
    int GetGameType();
};

struct Display_00491a70;
struct Src_004ab400;

#pragma pack(push, 1)
struct Gui_00491d70 {
    char unknown_0[8];
    int field_8;                       // +0x8
    char unknown_c[8];
    int field_14;                      // +0x14
    void* current;                     // +0x18
};

struct Player_004917d0 {
    char unknown_0[0x140];
    int field_140;                     // +0x140
    char unknown_144[0x14b - 0x144];
};

struct Zero11_004917d0 {
    int a;
    int b;
    short c;
    char d;

    void Clear()
    {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
    }
};

struct Game {
    char unknown_0[0xc];
    Display_00491a70* field_c;   // +0xc
    void* cd;   // +0x10
    char unknown_14[0x519 - 0x14];
    Gui_00491d70 gui;   // +0x519
    char unknown_535[0x589 - 0x535];
    int field_589;   // +0x589
    char unknown_58d[0x12ef - 0x58d];
    char field_12ef[4];   // +0x12ef
    char unknown_12f3[0x1b63 - 0x12f3];
    Player_004917d0 players[10];   // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    void* field_29a0;   // +0x29a0
    char unknown_29a4[0x2a44 - 0x29a4];
    union {
        unsigned char flags_2a44;
        unsigned short flags_2a44w;
    };
    unsigned char field_2a46;   // +0x2a46
    char unknown_2a47[0x2bee - 0x2a47];
    union {
        unsigned char field_2bee;
        struct {
            unsigned short pad_2bee : 5;
            unsigned short flags_2bee : 3;
            unsigned short rest_2bee : 8;
        };
    };
    char unknown_2bf0[0x2bf1 - 0x2bf0];
    Zero11_004917d0 zero_2bf1;   // +0x2bf1
    char unknown_2bfc[0x2cba - 0x2bfc];
    unsigned short field_2cba;   // +0x2cba
    char unknown_2cbc[0x2cbe - 0x2cbc];
    signed char selected;   // +0x2cbe
    char unknown_2cbf[0x2cc3 - 0x2cbf];
    unsigned char mode_2cc3;   // +0x2cc3
    unsigned short field_2cc4;   // +0x2cc4
    union {
        unsigned char flags_2cc6;
        struct {
            unsigned char pad_2cc6 : 5;
            unsigned char bit5_2cc6 : 1;
            unsigned char bit6_2cc6 : 1;
            unsigned char rest_2cc6 : 1;
        };
    };
    char unknown_2cc7[0x1425b - 0x2cc7];
    int field_1425b;   // +0x1425b
    int field_1425f;   // +0x1425f
    char unknown_14263[0x14280 - 0x14263];
    unsigned char field_14280;   // +0x14280
    char unknown_14281[0x14383 - 0x14281];
    void* xform;   // +0x14383
    void* projected;   // +0x14387
    void* assem;   // +0x1438b
    char unknown_1438f[0x143a7 - 0x1438f];
    char field_143a7[4];   // +0x143a7
    char unknown_143ab[0x1487f - 0x143ab];
    Src_004ab400* table[1];   // +0x1487f
    char unknown_14883[0x148cb - 0x14883];
    unsigned short* field_148cb;   // +0x148cb
    char unknown_148cf[0x37e1b - 0x148cf];
    int field_37e1b;   // +0x37e1b
    int field_37e1f;   // +0x37e1f
    int field_37e23;   // +0x37e23
    char unknown_37e27[0x37e9c - 0x37e27];
    short field_37e9c;   // +0x37e9c
    char unknown_37e9e[0x37ea0 - 0x37e9e];
    char name[0x1e];   // +0x37ea0
    union {
        unsigned short flags_37ebe;
        struct {
            unsigned short bit0_37ebe : 1;
            unsigned short pad_37ebe : 10;
            unsigned short bit11_37ebe : 1;
            unsigned short rest_37ebe : 4;
        };
    };
    char unknown_37ec0[0x37ec4 - 0x37ec0];
    unsigned int windCounter;   // +0x37ec4
    int field_37ec8;   // +0x37ec8
    int windX;   // +0x37ecc
    char unknown_37ed0[0x37ed4 - 0x37ed0];
    int windZ;   // +0x37ed4
    unsigned short windDirection;   // +0x37ed8
    int windSpeed;   // +0x37eda
    float windStrength;   // +0x37ede
    int windEnabled;   // +0x37ee2
    short field_37ee6;   // +0x37ee6
    char unknown_37ee8[0x37eea - 0x37ee8];
    short field_37eea;   // +0x37eea
    short field_37eec;   // +0x37eec
    char unknown_37eee[0x37efe - 0x37eee];
    int field_37efe;   // +0x37efe
    char unknown_37f02[0x37f08 - 0x37f02];
    int field_37f08;   // +0x37f08
    char unknown_37f0c[0x37f14 - 0x37f0c];
    unsigned char field_37f14;   // +0x37f14
    char unknown_37f15[0x37f16 - 0x37f15];
    unsigned char field_37f16;   // +0x37f16
    char unknown_37f17[0x37f2f - 0x37f17];
    union {
        unsigned short field_37f2f;
        struct {
            unsigned short pad_37f2f : 7;
            unsigned short bit7_37f2f : 1;
            unsigned short bit8_37f2f : 1;
            unsigned short bit9_37f2f : 1;
            unsigned short rest_37f2f : 6;
        };
    };
    char unknown_37f31[0x38a37 - 0x37f31];
    unsigned int field_38a37;   // +0x38a37
    char unknown_38a3b[0x38a43 - 0x38a3b];
    int field_38a43;   // +0x38a43
    unsigned int field_38a47;   // +0x38a47
    union {
        short field_38a4b;
        unsigned short speed_38a4b;
    };
    short field_38a4d;   // +0x38a4d
    char unknown_38a4f[0x38c53 - 0x38a4f];
    int field_38c53;   // +0x38c53
    char unknown_38c57[0x38c5f - 0x38c57];
    int field_38c5f;   // +0x38c5f
    int field_38c63;   // +0x38c63
    int field_38c67;   // +0x38c67
    char unknown_38c6b[0x38d6b - 0x38c6b];
    int field_38d6b;   // +0x38d6b
    char unknown_38d6f[0x38d7b - 0x38d6f];
    int field_38d7b;   // +0x38d7b
    char unknown_38d7f[0x391e9 - 0x38d7f];
    Mission* field_391e9;   // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;   // +0x391f1
    void (*handler)();   // +0x391f5
    int field_391f9;   // +0x391f9
    char unknown_391fd[0x3923b - 0x391fd];
    union {
        unsigned short field_3923b;
        struct {
            unsigned short pad0_3923b : 2;
            unsigned short bit2_3923b : 1;
            unsigned short pad1_3923b : 1;
            unsigned short bit4_3923b : 1;
            unsigned short bit5_3923b : 1;
            unsigned short bit6_3923b : 1;
            unsigned short rest_3923b : 9;
        };
    };
    char unknown_3923d[0x39249 - 0x3923d];
    int field_39249;   // +0x39249
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

extern const char DAT_005091d4[];          // "OFFSCREEN"

int __stdcall AllocSurface(const char* name, int width, int height);
void __stdcall SetRestoreSurface(int param_1);

// FUNCTION: 0x490ac0
void FUN_00490ac0()
{
    g_game->field_37e1b = AllocSurface(DAT_005091d4, g_game->field_37e1f, g_game->field_37e23);
    SetRestoreSurface(g_game->field_37e1b);
}

// Releases the offscreen object created by 0x490ac0.
void __cdecl FUN_004d85a0(int* param_1);
void RestoreScreen();

// FUNCTION: 0x490b00
void FUN_00490b00()
{
    FUN_004d85a0((int*)g_game->field_37e1b);
    g_game->field_37e1b = 0;
    SetRestoreSurface(0);
    RestoreScreen();
}

// Stores the new game mode in g_game->mode (+0x391f1) and installs the state
// handler for it in g_game->handler (+0x391f5), through the 0x490c14 jump
// table. Mode 6 also gets a different quit callback from SetCloseHandler.
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __cdecl LeaveNetGameCallback(int param);
void __cdecl HandleBattleQuitPrompt(int param);
void InitFrame();
void ReturnToMainMenuFrame();
void MenuFrame();
void PreBattleFrame();
void CampaignSetupFrame();
void LoadingScreenFrame();
void BattleFrame();
void EndGameFrame();

// FUNCTION: 0x490b30
void __stdcall SetGameMode(int param)
{
    g_game->mode = param;
    switch (param) {
    case 0:
        g_game->handler = InitFrame;
        break;
    case 1:
        g_game->handler = ReturnToMainMenuFrame;
        break;
    case 2:
        g_game->handler = MenuFrame;
        break;
    case 3:
        g_game->handler = PreBattleFrame;
        break;
    case 4:
        g_game->handler = CampaignSetupFrame;
        break;
    case 5:
        g_game->handler = LoadingScreenFrame;
        break;
    case 6:
        g_game->handler = BattleFrame;
        break;
    case 7:
        g_game->handler = EndGameFrame;
        break;
    default:
        g_game->handler = 0;
        break;
    }
    if (param == 6) {
        SetCloseHandler(HandleBattleQuitPrompt, 0);
    } else {
        SetCloseHandler(LeaveNetGameCallback, 0);
    }
}

// The wind update. While the per-tick counter at +0x37ec4 is below the tick
// count at +0x38a47 it jitters the wind direction by a random amount, picks a
// random wind speed in the range at +0x1425b..+0x1425f and, when that speed is
// not zero, a fresh direction, then turns direction and speed into the two wind
// vector components and a 0..1 strength. Once the counter has caught up with the
// tick count the wind is switched off instead.

// Fixed-point trig helpers, written in assembly: the angle is a short.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall RandomInt(int range);

// FUNCTION: 0x490c40
void __cdecl UpdateWind()
{
    if (g_game->windCounter < g_game->field_38a47) {
        // The __int64 cast keeps the _allmul/_alldiv calls; keep this one
        // expression with no temporary.
        g_game->windCounter += ((int)((__int64)rand() * 10 / 0x8000) + 5) * 30;

        // Separate statements: one expression changes how the low bound is added.
        int range = g_game->field_1425f - g_game->field_1425b;
        int n = RandomInt(range);
        g_game->windSpeed = g_game->field_1425b + n;
        if (g_game->windSpeed != 0)
            g_game->windDirection = RandomInt(0x10000);

        g_game->windX = -FUN_004b70ef(g_game->windDirection, g_game->windSpeed) * 2;
        g_game->windZ = -FUN_004b7123(g_game->windDirection, g_game->windSpeed) * 2;

        g_game->windStrength = (float)g_game->windSpeed / (float)g_game->field_37ec8;
        if (1.0 < g_game->windStrength)
            g_game->windStrength = 1.0f;

        g_game->windEnabled = 1;
    } else {
        g_game->windEnabled = 0;
    }
}

// FUNCTION: 0x490da0
void FUN_00490da0()
{
    if (g_game->field_391e9->GetGameType() == 3) {
        g_game->field_38a4b = 10;
        g_game->field_38a4d = 10;
    }
    g_game->field_38a43 = 0;
}


char* __stdcall Translate(char* text);
int GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall AddMessage(char* text, int param_2, int param_3, int param_4);

// FUNCTION: 0x490df0
void __stdcall SetGameSpeed(int speed, int param_2)
{
    if (speed > 0x14)
        speed = 0x14;
    if (speed < 1)
        speed = 1;
    if (speed != g_game->speed_38a4b) {
        char buf[100];
        int d = speed - 10;
        if (d == 0) {
            strcpy(buf, Translate("Game Speed Normal"));
        } else {
            sprintf(buf, "%s  %c%d\n", Translate("Game Speed"),
                    (d > 0) ? '+' : ' ', d);
        }
        AddMessage(buf, 2, 0, 10);
    }
    g_game->speed_38a4b = speed;
    g_game->field_38a4d = speed;
    if (param_2) {
        char data[3];
        data[0] = 0x19;
        data[1] = 1;
        data[2] = (char)speed;
        BroadcastPacket(GetLocalDpid(), data, 3);
    }
}

// FUNCTION: 0x490ee0
void IncreaseGameSpeed()
{
    unsigned short value = g_game->speed_38a4b;
    if (value < 0x14) {
        SetGameSpeed(value + 1, 1);
    }
}

// The counterpart of 0x490ee0: steps a 16-bit game setting down by one.
// FUNCTION: 0x490f10
void FUN_00490f10()
{
    unsigned short value = g_game->speed_38a4b;
    if (value > 1) {
        SetGameSpeed(value - 1, 1);
    }
}

struct CdLists_490f80 {
    char unknown_0[0x24];
    unsigned char tracks[0xaa0 - 0x24]; // +0x24
};

extern CdLists_490f80 DAT_0051e828;

int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size);

// FUNCTION: 0x490f40
void FUN_00490f40(void)
{
    unsigned int size = 0xaa0;
    if (ReadGameRegistryValue("CDLISTS", &DAT_0051e828, &size) == 0) {
        memset(&DAT_0051e828, 0, sizeof(DAT_0051e828));
    }
}

// Copies the per-track bytes from the CD object into the CD-list settings
// block and saves it to the registry under "CDLISTS".
class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce7e0 {
public:
    unsigned char GetCategoryOfTrack(int param_1);
};

void __stdcall WriteGameRegistryValue(void* key, void* buf, int value);

// FUNCTION: 0x490f80
void SaveCdLists()
{
    for (int i = 0; i < ((Class_004ce450*)g_game->cd)->GetTrackCount(); i++) {
        DAT_0051e828.tracks[i] = ((Class_004ce7e0*)g_game->cd)->GetCategoryOfTrack(i + 1);
    }
    WriteGameRegistryValue("CDLISTS", &DAT_0051e828, sizeof(DAT_0051e828));
}

// Refreshes the CD-list table at DAT_0051e828 from the CD object: it saves and
// restores the object's track table across an MCI close/open, looks the current
// disc id up in the 20-entry table and, when found, moves that entry to the
// front and re-reads its track bytes. A disc not in the table is inserted at the
// front after shifting the others up, provided the drive reports 16 audio
// tracks. The final cleanup call depends on the game mode.

class Class_004ce3e0 {
public:
    void CopyTrackTypeTable(const void* src);
};

class Class_004ce460 {
public:
    int IsFirstTrackData();
};

class Class_004ce680 {
public:
    int GetTrackCategory();
};

class Sound {
public:
    void SetTrackCategory(int param_1);
    int GetDiscSerial();
};

class Class_004ce7a0 {
public:
    int SetPlaybackOrder(int param_1);
};

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004ced40 {
public:
    int StopCdAudio();
};

class Class_004cedc0 {
public:
    void EnableCdAudio(int on);
};

extern int DAT_0051e848;
extern int DAT_0051e84c;
extern int DAT_0051e850;
extern int DAT_0051e854;
extern int DAT_0051e858;
extern int* DAT_0051f2e8;

// FUNCTION: 0x490fe0
void FUN_00490fe0()
{
    char tracks[16] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char buf[0x88];

    int saved = ((Class_004ce680*)g_game->cd)->GetTrackCategory();
    mciSendStringA("stop cdaudio", 0, 0, 0);
    mciSendStringA("close cdaudio", 0, 0, 0);
    mciSendStringA("open cdaudio", 0, 0, 0);
    ((Class_004cedc0*)g_game->cd)->EnableCdAudio(g_game->field_37f14 & 1);
    ((Class_004ce7a0*)g_game->cd)->SetPlaybackOrder(g_game->field_37f16);
    ((Sound*)g_game->cd)->SetTrackCategory(saved);

    int id = ((Sound*)g_game->cd)->GetDiscSerial();
    int index = 0;
    int* slot = &DAT_0051e848;
    // Test *slot != id first, with the break as its own block: else the loop is rotated.
    while (1) {
        if (*slot != id) {
            slot = (int*)((char*)slot + 0x88);
            index++;
            if ((int)slot < (int)&DAT_0051f2e8)
                continue;
            goto newdisc;
        }
        break;
    }
    {
        memcpy(buf, (char*)&DAT_0051e828 + index * 0x88, 0x88);
        for (int j = index; j > 0; j--)
            memcpy((char*)&DAT_0051e828 + j * 0x88,
                   (char*)&DAT_0051e828 + (j - 1) * 0x88, 0x88);
        memcpy(&DAT_0051e828, buf, 0x88);
        ((Class_004ce3e0*)g_game->cd)->CopyTrackTypeTable(&DAT_0051e84c);
    }
newdisc:
    if (index == 0x14) {
        if (((Class_004ce450*)g_game->cd)->GetTrackCount() == 0x10) {
            if (((Class_004ce460*)g_game->cd)->IsFirstTrackData() != 0) {
                ((Class_004ce3e0*)g_game->cd)->CopyTrackTypeTable(tracks);
            }
        }
        // Downward pointer walk against the addresses, not an index loop.
        int p = (int)&DAT_0051e828 + 0xa18;
        for (; p > (int)&DAT_0051e828; p -= 0x88)
            memcpy((void*)p, (void*)(p - 0x88), 0x88);
        DAT_0051e84c = *(int*)&tracks[0];
        DAT_0051e850 = *(int*)&tracks[4];
        DAT_0051e854 = *(int*)&tracks[8];
        DAT_0051e858 = *(int*)&tracks[12];
        DAT_0051e848 = id;
    }
    if ((g_game->flags_2a44 & 4) != 0 && g_game->mode == 6)
        ((Class_004cdb40*)g_game->cd)->PlayNextTrack();
    else
        ((Class_004ced40*)g_game->cd)->StopCdAudio();
}

class Class_004cd9d0 {
public:
    int SetCdCallback(void (*param_1)());
};

extern const char DAT_00509268[];          // "SkirmishInfo"
extern const char DAT_00509200[];          // "CDLISTS"
extern const char DAT_00502820[];          // "guis"
extern const char DAT_00502e30[];          // "anims"
extern const char DAT_0050338c[];          // "fonts"
extern const char DAT_0050925c[];          // "commongui"
extern const char DAT_00509250[];          // "hattfont12"
extern const char DAT_00509244[];          // "hattfont11"
extern const char DAT_00509238[];          // "UnitLimit"

int GetScreenWidth();
int GetScreenHeight();
void __stdcall SetPageFlipping(int param_1);
void __stdcall SetMissionType(int param_1);
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
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void LoadSettings();
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
void ClearPictureCache();
int __stdcall GetPreferenceInt(const char* name, int param_2);

// FUNCTION: 0x491200
void InitGame()
{
    unsigned int size;

    // Each GlobalMemoryStatus call in its own nested block: sets the dwLength
    // store position and shares one stack slot.
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
    SetMissionType(0);
    if (g_game->field_391e9->GetGameType() == 3) {
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
    int ok = ReadGameRegistryValue(DAT_00509200, &DAT_0051e828, &size);
    if (ok == 0)
        memset(&DAT_0051e828, 0, 0xaa0);
    ((Class_004cedc0*)g_game->cd)->EnableCdAudio(g_game->field_37f14 & 1);
    ((Class_004ce7a0*)g_game->cd)->SetPlaybackOrder(g_game->field_37f16);
    ((Class_004cd9d0*)g_game->cd)->SetCdCallback(FUN_00490fe0);
    FUN_00490fe0();
    ((Sound*)g_game->cd)->SetTrackCategory(0);
    ApplyBrightnessAndVolume();
    LoadSideData();
    LoadLogos();
    SetBrightness(0.5 - g_game->field_37f08 * -0.041666668f);
    SetCurrentGuiContext(&g_game->gui);
    FUN_0049fba0(&g_game->gui, DAT_00502820);
    FUN_0049fbf0(&g_game->gui, DAT_00502e30);
    FUN_0049fb50(&g_game->gui, DAT_0050338c);
    FUN_004aa8e0(&g_game->gui, g_game->field_391f9);
    FUN_004ab4e0(&g_game->gui, GetGafFrame(g_game->field_148cb, 0));
    LoadGafFile(&g_game->gui, DAT_0050925c);
    LoadGafIntoSlot(&g_game->gui, DAT_00509250, 0);
    LoadGafIntoSlot(&g_game->gui, DAT_00509244, 1);
    g_game->gui.field_14 = g_game->gui.field_8;
    SetTextKeyColor(0xfe);
    SetFont(g_game->field_391f9);
    g_game->field_14280 = 0;
    g_game->field_38c53 = 0;
    g_game->field_38c5f = 0;
    g_game->field_38c63 = 0;
    g_game->field_38c67 = 0;
    g_game->field_38d6b = 0;
    g_game->field_38d7b = 0;
    g_game->mode = 0;
    g_game->handler = InitFrame;
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

// Saves the per-track bytes from the CD object into the CD-list settings
// block and writes it to the registry under "CDLISTS", then releases the
// CD-list buffers, the track buffer and the rest of the game state.
struct Obj_004aeda0;
struct Obj_004aef80;
struct Class_00452370;

void FreePictureCache();
void __stdcall FUN_004aeda0(Obj_004aeda0* obj, int i);
void __stdcall FUN_004aef80(Obj_004aef80* obj);
void FreeLogos();
void FreeSideFonts();
void FreeSounds();
void FUN_0042a3b0();
void ShutdownSound();
void FreeAnimFiles();
void ClearOrderTypeTable();
void FreeUnitInfo();
void __stdcall ReleasePacketData(Class_00452370* obj);
void FreeOtaEnumCacheAndMission();

// FUNCTION: 0x4916a0
void ShutdownGame(void)
{
    for (int i = 0; i < ((Class_004ce450*)g_game->cd)->GetTrackCount(); i++) {
        DAT_0051e828.tracks[i] = ((Class_004ce7e0*)g_game->cd)->GetCategoryOfTrack(i + 1);
    }
    WriteGameRegistryValue("CDLISTS", &DAT_0051e828, 0xaa0);
    FreePictureCache();
    FUN_004aeda0((Obj_004aeda0*)&g_game->gui, 1);
    FUN_004aeda0((Obj_004aeda0*)&g_game->gui, 0);
    FUN_004aef80((Obj_004aef80*)&g_game->gui);
    FreeLogos();
    FreeSideFonts();
    FreeSounds();
    FUN_0042a3b0();
    ShutdownSound();
    FreeAnimFiles();
    FUN_004d85a0((int*)g_game->field_37e1b);
    g_game->field_37e1b = 0;
    SetRestoreSurface(0);
    RestoreScreen();
    ClearOrderTypeTable();
    FUN_004d85a0((int*)g_game->field_29a0);
    g_game->field_29a0 = 0;
    FreeUnitInfo();
    ReleasePacketData((Class_00452370*)g_game->field_12ef);
    FreeOtaEnumCacheAndMission();
}

// Resets the game state for a new match: clears the scratch fields and flags
// in g_game, runs the per-subsystem reset functions, allocates the three
// transform point buffers, and zeroes the per-player counters and the input
// history.
extern unsigned int DAT_0051f2d8;
extern unsigned int DAT_0051f2dc;
extern int DAT_0051e710[30];

void FUN_00463c80();
void LoadLightBar();
void ResetSpeech();
void LoadTextureGafs();
void LoadFeatureFileList();
void InitUnitCategories();
void NopAfterFeatureEnum();
void CreateParticleLists();
void AllocWeaponArray();
void LoadWeaponTypes();
void LoadTntMap();
void ResetCameraState();
void LoadUnitTypes();
void LoadDownloadMenus();
void AllocateUnitMemory();
void ResolveFeatureLinks();
void FreeFeatureFileList();
void BuildAllPassMaps();
void InitMeteors();
void InitRadar();
void InitPlayers();
void CreatePathfinder();
void InitExplosions();
void InitCommands();
unsigned int GetTicks();

// FUNCTION: 0x4917d0
void FUN_004917d0()
{
    FUN_00463c80();
    memset(&g_game->zero_2bf1, 0, 11);
    g_game->mode_2cc3 = 1;
    g_game->field_2cc4 = 0;
    g_game->bit5_2cc6 = 0;
    g_game->bit6_2cc6 = 0;
    g_game->field_2cba = 0;
    g_game->bit0_37ebe = 0;
    g_game->bit11_37ebe = 0;
    g_game->field_39249 = 0;
    g_game->bit9_37f2f = 0;
    g_game->bit7_37f2f = 0;
    g_game->bit8_37f2f = 0;
    LoadLightBar();
    ResetSpeech();
    LoadTextureGafs();
    LoadFeatureFileList();
    InitUnitCategories();
    NopAfterFeatureEnum();
    CreateParticleLists();
    AllocWeaponArray();
    LoadWeaponTypes();
    LoadTntMap();
    ResetCameraState();
    LoadUnitTypes();
    LoadDownloadMenus();
    AllocateUnitMemory();
    ResolveFeatureLinks();
    FreeFeatureFileList();
    BuildAllPassMaps();
    g_game->field_37ec8 = 5000;
    g_game->windCounter = 0;
    UpdateWind();
    g_game->xform = FUN_004d83b0("TEMP XFORM PTS", 0x960);
    g_game->projected = FUN_004d83b0("TEMP PROJECTED PTS", 0x640);
    g_game->assem = FUN_004d83b0("ASSEM PTS", 0xa0);
    g_game->field_38a37 = GetTicks();
    g_game->field_38a47 = 0;
    if (g_game->field_391e9->GetGameType() == 3) {
        g_game->field_38a4b = 10;
        g_game->field_38a4d = 10;
    }
    g_game->field_38a43 = 0;
    InitMeteors();
    InitRadar();
    InitPlayers();
    CreatePathfinder();
    InitExplosions();
    InitCommands();
    for (int i = 0; i < 10; i++)
        g_game->players[i].field_140 = 0;
    g_game->bit2_3923b = 0;
    g_game->bit4_3923b = 0;
    g_game->bit5_3923b = 0;
    g_game->bit6_3923b = 0;
    g_game->flags_2bee = 0;
    g_game->field_2a46 = 0xff;
    DAT_0051f2dc = 0;
    memset(DAT_0051e710, 0, sizeof(DAT_0051e710));
    DAT_0051f2d8 = 0;
}

struct Display_00491a70 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
};

void __stdcall SetResolution(int x, int y);
void __stdcall SetOffscreenSurface(int param_1);

// FUNCTION: 0x491a70
void FUN_00491a70()
{
    g_game->field_37e1f = 0x280;
    g_game->field_37e23 = 0x1e0;
    if (GetScreenWidth() != 0x280 || GetScreenHeight() != 0x1e0) {
        FUN_004d85a0((int*)g_game->field_37e1b);
        g_game->field_37e1b = 0;
        SetRestoreSurface(0);
        RestoreScreen();
        SetWindowPos(g_game->field_c->hwnd, 0, 0, 0, 0x280, 0x1e0, 4);
        SetResolution(0x280, 0x1e0);
        g_game->field_37e1b = AllocSurface(DAT_005091d4, g_game->field_37e1f, g_game->field_37e23);
        SetRestoreSurface(g_game->field_37e1b);
        SetOffscreenSurface(g_game->field_37e1b);
    }
}

void __cdecl CollectEndGameStats();
void EmptyShutdownPreCleanup();
void FreeUnitMemory();
void DestroyParticleLists();
void FreeExplosions();
void DestroyPathfinder();
void FreePlayers();
void FreeRadar();
void FreeMapResources();
void FreeDownloadMenus();
void FreeUnitTypes();
void FreeWeaponTypes();
void FUN_0042a570();
void FreeWeaponArray();
void FreeMovementClasses();
void FreeUnitCategories();
void CloseNetSession();

// FUNCTION: 0x491b60
void FUN_00491b60()
{
    g_game->flags_2a44w &= 0xfffb;
    ((Class_004ced40*)g_game->cd)->StopCdAudio();
    ((Sound*)g_game->cd)->SetTrackCategory(4);
    CollectEndGameStats();
    EmptyShutdownPreCleanup();
    FreeUnitMemory();
    DestroyParticleLists();
    FreeExplosions();
    DestroyPathfinder();
    FreePlayers();
    FreeRadar();
    FreeMapResources();
    FUN_004d85a0((int*)g_game->assem);
    FUN_004d85a0((int*)g_game->projected);
    FUN_004d85a0((int*)g_game->xform);
    g_game->assem = 0;
    g_game->projected = 0;
    g_game->xform = 0;
    FreeDownloadMenus();
    FreeUnitTypes();
    FreeWeaponTypes();
    FUN_0042a570();
    FreeWeaponArray();
    FreeMovementClasses();
    FreeUnitCategories();
    if (g_game->field_391e9->GetGameType() == 3) {
        CloseNetSession();
    }
}

void BlankScreen();
void RemoveLocalPlayers();
void __stdcall QuitApp(const char*);

// FUNCTION: 0x491c60
void FUN_00491c60()
{
    BlankScreen();
    RemoveLocalPlayers();
    FUN_00491b60();
    QuitApp(0);
}

// Selects entry n (a signed byte at +0x2cbe) and hands the matching table
// entry at +0x1487f to FUN_004ab400 for the object at +0x519.
struct Obj_004ab400;

void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src);

// FUNCTION: 0x491c80
void __stdcall FUN_00491c80(int n)
{
    if (g_game->selected != n) {
        g_game->selected = n;
        FUN_004ab400((Obj_004ab400*)&g_game->gui, g_game->table[n]);
    }
}

// Picks the order/state table entry at +0x1487f and hands it to
// FUN_004ab400 for the object at +0x519, mirroring 0x491c80.
int __cdecl UpdatePlacementGhostValidity(void);
unsigned short __cdecl PickUnitUnderCursor(void);
int __stdcall ResolveCursorModeForSelection(unsigned char mode);

// FUNCTION: 0x491cc0
void __stdcall FUN_00491cc0(int unused)
{
    unsigned char flags = g_game->flags_2cc6;

    if ((flags & 2) != 0 && g_game->mode_2cc3 == 0xe) {
        UpdatePlacementGhostValidity();
        return;
    }
    if ((flags & 2) == 0 && (flags & 1) == 0) {
        if (g_game->selected != 0x13) {
            g_game->selected = 0x13;
            FUN_004ab400((Obj_004ab400*)&g_game->gui, g_game->table[0x13]);
        }
        return;
    }
    g_game->field_2cba = PickUnitUnderCursor();
    int n = ResolveCursorModeForSelection(g_game->mode_2cc3);
    if (g_game->selected != n) {
        g_game->selected = n;
        FUN_004ab400((Obj_004ab400*)&g_game->gui, g_game->table[n]);
    }
}

int __stdcall IsScreenNamed(Gui_00491d70* queue, char* name);
void __stdcall CloseTopScreen(Gui_00491d70* queue);

// FUNCTION: 0x491d70
int __stdcall FUN_00491d70(int force)
{
    unsigned short flags = g_game->flags_37ebe;
    if (((flags & 0x800) || (flags & 0x65) || (g_game->field_2bee & 0xe0)) && force == 0) {
        g_game->flags_37ebe = flags | 0x10;
        return 0;
    }
    g_game->field_37e9c = 0;
    while (g_game->gui.current) {
        if (IsScreenNamed(&g_game->gui, g_game->name))
            return 1;
        CloseTopScreen(&g_game->gui);
    }
    return 0;
}

// Compare 0x491d70.
// FUNCTION: 0x491e10
void FUN_00491e10()
{
    if (!IsScreenNamed(&g_game->gui, g_game->name)) {
        g_game->field_37e9c = 0;
        CloseTopScreen(&g_game->gui);
    }
}

extern int* DAT_0051f2e0;
extern int* DAT_0051f2e4;
extern int* DAT_0051f2ec;

// FUNCTION: 0x491e50
void FUN_00491e50()
{
    if (DAT_0051f2e0 != 0) {
        FUN_004d85a0(DAT_0051f2e0);
    }
    if (DAT_0051f2e4 != 0) {
        FUN_004d85a0(DAT_0051f2e4);
    }
    if (DAT_0051f2e8 != 0) {
        FUN_004d85a0(DAT_0051f2e8);
    }
    DAT_0051f2e8 = 0;
    DAT_0051f2e4 = 0;
    DAT_0051f2e0 = 0;
    if (DAT_0051f2ec != 0) {
        FUN_004d85a0(DAT_0051f2ec);
    }
    DAT_0051f2ec = 0;
}

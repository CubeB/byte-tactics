// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
//
// MATCH (claude-opus-5-5, #4267; was 84.3%). The loading-screen frame: on the
// first call it starts the loader thread (LoadThreadMain), once the loader sets
// the "loaded" bit it restores the game screen and installs the game frame
// handler (FUN_00499200), and otherwise it draws the six progress bars.
// What it took, from the earlier partial:
//  - The six stage bytes at g_game+0x38d6f are volatile, like the flags word
//    after them (the loader thread writes both; 0x456de0 reads the same bytes
//    as volatile). That is what gives each bar's `mov cl, [m]; and ecx, 0xff`.
//  - The flags word is a volatile bitfield union: the bit tests are bitfield
//    reads (`mov dl, [m]; shr dl, N; test dl, 1`), and the b2 test is nested
//    rather than `b2 && FUN_004568c0()`, which folds to `test byte ptr`.
//  - The zeroing is four memsets (10, 10, 6 and 6 bytes) and one 8-byte
//    memset over the stage bytes and the flags word.
//  - Each bar's rect is written left, right, top, bottom.
//  - The two player loops index g_game->players[i]; the explicit offsets of
//    the old version gave the reversed SIB base and index.
//  - `int ok` for LockScreen's result (`cmp eax, ebp`) and a `name` local
//    for the strncpy source (the call comes before `push 100`).
//  - <ddraw.h>: without it the map-name block's x87 schedule differs (97.3%);
//    tools/headers.py found it, and the gadget is a DirectDraw surface lock.
// The tail loop in the "loaded" branch stores each active player's +0x73 byte
// to frame +0x23, the byte just past `rect`, and nothing reads it. A byte local
// is dead-store eliminated, so it is written here as the store past rect that
// the original evidently made.
#include <windows.h>
#include <string.h>
#include <ddraw.h>

#pragma pack(push, 1)

struct PlayerData_00497f40 {
    char unknown_0[0x9b];
    unsigned char flags;               // +0x9b
};

struct PlayerInfo_00497f40 {
    int active;                        // +0x0
    unsigned int id;                   // +0x4
    char unknown_8[0x27 - 0x8];
    PlayerData_00497f40* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char control;             // +0x73
    char unknown_74[0x14b - 0x74];
};

union LoadFlags_00497f40 {
    unsigned short value;
    struct {
        unsigned short started : 1;
        unsigned short loaded : 1;
        unsigned short b2 : 1;
        unsigned short b3 : 1;
        unsigned short rest : 12;
    } bits;
};

struct PlayerSlots_00497f40 {
    int inGame[10];                    // +0x0
    int unknown_28;                    // +0x28
    int flag40[10];                    // +0x2c
    char unknown_54[0x8c - 0x54];
};

struct Game {
    char unknown_0[0xc];
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    char unknown_10[0x519 - 0x14];
    int field_519;                     // +0x519
    int field_51d;                     // +0x51d
    char unknown_521[0x531 - 0x521];
    int field_531;                     // +0x531
    char unknown_535[0x589 - 0x535];
    int field_589;                     // +0x589
    char unknown_58d[0xdcb - 0x58d];
    unsigned char palette[16];         // +0xdcb
    char unknown_ddb[0x11eb - 0xddb];
    int field_11eb;                    // +0x11eb
    char unknown_11ef[0x1b63 - 0x11ef];
    PlayerInfo_00497f40 players[10];   // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    PlayerSlots_00497f40 slots;        // +0x29a4
    char unknown_2a30[0x2cbe - 0x2a30];
    unsigned char field_2cbe;          // +0x2cbe
    char unknown_2cbf[0x148cf - 0x2cbf];
    int field_148cf;                   // +0x148cf
    char unknown_148d3[0x37e1b - 0x148d3];
    int field_37e1b;                   // +0x37e1b
    int field_37e1f;                   // +0x37e1f
    int field_37e23;                   // +0x37e23
    int field_37e27;                   // +0x37e27
    int field_37e2b;                   // +0x37e2b
    int field_37e2f;                   // +0x37e2f
    int field_37e33;                   // +0x37e33
    int field_37e37;                   // +0x37e37
    int field_37e3b;                   // +0x37e3b
    char unknown_37e3f[0x37f1b - 0x37e3f];
    int field_37f1b;                   // +0x37f1b
    int field_37f1f;                   // +0x37f1f
    char unknown_37f23[0x38a37 - 0x37f23];
    unsigned int field_38a37;          // +0x38a37
    int field_38a3b;                   // +0x38a3b
    char pad_38a3f[0x38a47 - 0x38a3f];
    int field_38a47;                   // +0x38a47
    char pad_38a4b[0x38a4f - 0x38a4b];
    short field_38a4f;                 // +0x38a4f
    char unknown_38a51[0x38d6f - 0x38a51];
    volatile unsigned char progress[6]; // +0x38d6f
    volatile LoadFlags_00497f40 flags38d75;     // +0x38d75
    char unknown_38d77[0x391e9 - 0x38d77];
    int field_391e9;                   // +0x391e9
    char pad_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
    void (*field_391f5)();             // +0x391f5
    int field_391f9;                   // +0x391f9
    char unknown_391fd[0x39239 - 0x391fd];
    short field_39239;                 // +0x39239
};

#pragma pack(pop)

extern Game* g_game;

// Embedded surface at game offset 0x143a7.
#define SURFACE_143a7 ((void*)((char*)g_game + 0x143a7))

extern unsigned char DAT_0051f2c8[10];
extern unsigned char DAT_0051e810[10];
extern "C" int DAT_0051e6c8;
extern "C" int DAT_0051e6cc;
extern "C" int DAT_0051f308;
extern "C" int g_usePacketManager;
extern "C" char g_packetManager;
extern "C" unsigned char DAT_0051e820, DAT_0051e821, DAT_0051e822;
extern "C" unsigned char DAT_0051e823, DAT_0051e824, DAT_0051e825;

void LoadThreadMain();
void FUN_00499200();
void __cdecl FUN_004609a0(int);
void __cdecl BlankScreen();
void __cdecl FreePictureCache();
void __cdecl SaveSettings();
void __cdecl HandleNetPackets();
void __cdecl SendLoadProgress();
void __cdecl OnlineUnload();
void __cdecl DrawLightBars();
void __cdecl StopAllSounds();
void __cdecl FUN_00496790();
void __cdecl FUN_004c2870();
void __stdcall UnlockScreen(void*);
void __cdecl RestoreScreen();
void __cdecl FlipScreen();
int __cdecl IsCdPlaying();
void __cdecl FUN_004d85a0(void*);
void __stdcall LoadPictureCached(char*, int, int, int);
void __stdcall BuildDataPath(void*, char*, char*, char*);
void __stdcall FUN_00453320(unsigned int, int);
void __stdcall DrawSyncStatus(void*);
void __stdcall FUN_0049fa70(void*);
int __stdcall GetTextPixelWidth(char*);
void __stdcall FUN_004a50e0(void*, char*, int, int, int, int);
void __stdcall CloseTopScreen(void*);
void __stdcall FUN_004ab400(void*, void*);
void __stdcall FUN_004ac7d0(void*, void*, void*);
void __stdcall SetCloseHandler(void (__cdecl *)(int), int);
void __stdcall SetResolution(int, int);
void __stdcall FatalError(char*);
int __stdcall FUN_004b6b20(void (*)(void), int, int);
void __stdcall FUN_004b6b50(int);
void* __stdcall GetGafFrame(void*, int);
void __stdcall DrawFrame(void*, void*, int, int);
void* __stdcall FindGafEntry(int, char*);
void __stdcall SetPaletteColors(void*, int, int);
void* __stdcall FUN_004bbe50(void*, unsigned int*);
void __stdcall FillRectangle(void*, void*, unsigned char);
void __stdcall SetTextColors(int, int);
void __stdcall SetFont(int);
char* __stdcall FUN_004c5740(char*);
int __stdcall LockScreen(void*);
void __stdcall SetRestoreSurface(int);
void __stdcall SetOffscreenSurface(void*);
void* __stdcall AllocSurface(char*, int, int);
void __stdcall DrawSurface(void*, void*, int, int);
void __stdcall HAPINET_guaranteepackets(int);
class Class_004cdb40 { public: void PlayNextTrack(); };
class Class_004ce690 { public: void SetTrackCategory(int); };
class Class_004ce800 { public: int IsCdPlaying(); };

int __cdecl GetScreenWidth();
int __cdecl GetScreenHeight();
int __cdecl GetTextKeyColor();
int __cdecl GetFontHeight();
int __cdecl FUN_004568c0();
unsigned int __cdecl GetTicks();
char* __cdecl FUN_0049f580();

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435c30 {
public:
    char* FUN_00435c30();
};

class PacketManager {
public:
    void SendAllQueued(int);
};

// FUNCTION: 0x497f40
void LoadingScreenFrame(void)
{
    struct Surface { int width, height; char unknown_8[0x28]; };
    Surface gadget;
    char buf[128];
    char aux[256];
    void* surfaceHandle;
    int i;
    unsigned int color;
    int flash;
    int textWidth;
    unsigned int stamp;
    int rect[4];
    PlayerInfo_00497f40* pi;
    char namebuf[100];

    if (!g_game->flags38d75.bits.started) {
        while (g_game->field_531 != 0) {
            CloseTopScreen(&g_game->field_519);
        }
        FUN_0049fa70(&g_game->field_519);
        if (g_game->field_2cbe != 0x14) {
            g_game->field_2cbe = 0x14;
            FUN_004ab400(&g_game->field_519, (void*)g_game->field_148cf);
        }
        SetFont(g_game->field_391f9);
        SetPaletteColors(SURFACE_143a7, 0, 0x100);
        if (((Class_00435100*)g_game->field_391e9)->FUN_00435100() != 2) {
            SaveSettings();
        }
        while (g_game->field_531 != 0) {
            CloseTopScreen(&g_game->field_519);
        }
        BlankScreen();
        g_game->field_37e1f = 0x280;
        g_game->field_37e23 = 0x1e0;
        if (GetScreenWidth() != 0x280 || GetScreenHeight() != 0x1e0) {
            FUN_004d85a0((void*)g_game->field_37e1b);
            g_game->field_37e1b = 0;
            SetRestoreSurface(0);
            RestoreScreen();
            SetWindowPos(*(HWND*)(g_game->field_c + 0x40), 0, 0, 0, 0x280, 0x1e0, 4);
            SetResolution(0x280, 0x1e0);
            g_game->field_37e1b = (int)AllocSurface("OFFSCREEN", g_game->field_37e1f, g_game->field_37e23);
            SetRestoreSurface(g_game->field_37e1b);
            SetOffscreenSurface((void*)g_game->field_37e1b);
        }
        BuildDataPath(aux, "palettes", "guipal", "PAL");
        surfaceHandle = FUN_004bbe50((unsigned int*)aux, 0);
        FUN_004ac7d0(&g_game->field_519, SURFACE_143a7, surfaceHandle);
        FUN_004d85a0(surfaceHandle);
        g_game->field_38a37 = GetTicks();
        g_game->field_38a3b = 0;
        g_game->field_38a47 = 0;
        g_game->field_38a4f = 0;
        g_game->field_39239 = (short)0xffff;
        g_game->field_37e1f = g_game->field_37f1b;
        g_game->field_37e23 = g_game->field_37f1f;
        g_game->field_37e27 = 0x80;
        g_game->field_37e2b = 0x20;
        g_game->field_37e2f = g_game->field_37e1f - 1;
        g_game->field_37e33 = g_game->field_37e23 - 0x21;
        g_game->field_37e37 = g_game->field_37e2f - g_game->field_37e27 + 1;
        g_game->field_37e3b = g_game->field_37e33 - g_game->field_37e2b + 1;
        LoadPictureCached("loadgame2bg", 0, 0, 0);
        memset(&g_game->slots, 0, sizeof(g_game->slots));
        for (i = 0; i < 10; i++) {
            pi = &g_game->players[i];
            if (pi->active == 0 || (pi->control != 1 && pi->control != 2)) {
                g_game->slots.inGame[i] = 0;
            } else {
                g_game->slots.inGame[i] = 1;
            }
            g_game->slots.flag40[i] = (pi->active != 0 && (pi->data->flags & 0x40) != 0) ? 1 : 0;
        }
        if (!FUN_004b6b20(LoadThreadMain, 0, 0)) {
            FatalError("Unable to start the loading thread!");
        }
        memset(DAT_0051f2c8, 0, 10);
        memset(DAT_0051e810, 0, 10);
        memset(&DAT_0051e6c8, 0, 6);
        memset(&DAT_0051e820, 0, 6);
        g_game->flags38d75.bits.started = 1;
        OnlineUnload();
    }
    if (g_game->flags38d75.bits.loaded) {
        StopAllSounds();
        BlankScreen();
        FreePictureCache();
        if (GetScreenWidth() != g_game->field_37f1b || GetScreenHeight() != g_game->field_37f1f) {
            FUN_004d85a0((void*)g_game->field_37e1b);
            g_game->field_37e1b = 0;
            SetRestoreSurface(0);
            RestoreScreen();
            SetWindowPos(*(HWND*)(g_game->field_c + 0x40), 0, 0, 0, g_game->field_37f1b,
                         g_game->field_37f1f, 4);
            SetResolution(g_game->field_37f1b, g_game->field_37f1f);
            g_game->field_37e1b = (int)AllocSurface("OFFSCREEN", g_game->field_37e1f, g_game->field_37e23);
            SetRestoreSurface(g_game->field_37e1b);
        }
        DrawLightBars();
        FUN_00496790();
        FUN_004c2870();
        g_game->field_391f1 = 6;
        g_game->field_391f5 = FUN_00499200;
        SetCloseHandler(FUN_004609a0, 0);
        g_game->field_589 = 0;
        memset((void*)g_game->progress, 0, 8);
        ((Class_004ce690*)g_game->field_10)->SetTrackCategory(0);
        if (!((Class_004ce800*)g_game->field_10)->IsCdPlaying()) {
            ((Class_004cdb40*)g_game->field_10)->PlayNextTrack();
        }
        for (i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0)
                ((unsigned char*)rect)[19] = g_game->players[i].control;
        }
        return;
    }
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].control == 1 || g_game->players[i].control == 2)) {
            FUN_00453320(g_game->players[i].id, 0);
            if (g_usePacketManager != 0)
                ((PacketManager*)&g_packetManager)->SendAllQueued(1);
        }
    }
    HandleNetPackets();
    if (g_game->flags38d75.bits.b2) {
        if (FUN_004568c0() != 0) {
            g_game->flags38d75.bits.b2 = 0;
            g_game->flags38d75.bits.b3 = 1;
            HAPINET_guaranteepackets(0);
        }
    }
    if (g_usePacketManager != 0) {
        ((PacketManager*)&g_packetManager)->SendAllQueued(1);
    }
    SetOffscreenSurface((void*)g_game->field_37e1b);
    int ok = LockScreen(&gadget);
    if (ok != 0) {
        color = g_game->palette[15];
        stamp = GetTicks();
        if (DAT_0051f308 < (int)stamp) {
            DAT_0051f308 = GetTicks();
            for (i = 0; i < 6; i++) {
                if (((char*)&DAT_0051e6c8)[i] != 0) {
                    ((char*)&DAT_0051e6c8)[i] -= 2;
                }
            }
        }
        SetFont(g_game->field_391f9);
        DrawSurface(&gadget, (void*)g_game->field_11eb, 0, 0);
        if (((Class_00435100*)g_game->field_391e9)->FUN_00435100() != 1) {
            SetTextColors(color, 0xfe);
            char* name = ((Class_00435c30*)g_game->field_391e9)->FUN_00435c30();
            strncpy(namebuf, name, 100);
            namebuf[99] = 0;
            if (FUN_0049f580() != 0 && _strcmpi((const char*)FUN_0049f580(), "english") != 0) {
                _strlwr(namebuf);
            }
            wsprintfA(buf, "%s: %s", (char*)FUN_004c5740("Map"), (char*)FUN_004c5740(namebuf));
            textWidth = GetTextPixelWidth(buf);
            {
                int x = gadget.width / 2 - textWidth / 2;
                FUN_004a50e0(&gadget, buf, x,
                             (int)((double)gadget.height - (double)GetFontHeight() * 1.5), -1, 0);
            }
        }
        {
            void* light = FindGafEntry(g_game->field_51d, "LIGHTBAR");
            void* lightbar = GetGafFrame(light, 0);
            *((short*)lightbar + 3) = 0;
            *((short*)lightbar + 2) = 0;
            color = g_game->palette[g_game->progress[0] < 100 ? 12 : 10];
            SetTextColors(color, GetTextKeyColor());
            if(g_game->progress[0] == 100 && DAT_0051e820 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[0] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[0];
            DAT_0051e820 = g_game->progress[0];
            FUN_004a50e0(&gadget, (char*)FUN_004c5740("Textures"), 0x5a, 0x87, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[0] * 7) / 2 + 0xcd;
            rect[1] = 0x87;
            rect[3] = 0x9b;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[1] < 100 ? 12 : 10];
            SetTextColors(color, GetTextKeyColor());
            if(g_game->progress[1] == 100 && DAT_0051e821 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[1] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[1];
            DAT_0051e821 = g_game->progress[1];
            FUN_004a50e0(&gadget, (char*)FUN_004c5740("Terrain"), 0x5a, 0xb1, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[1] * 7) / 2 + 0xcd;
            rect[1] = 0xb1;
            rect[3] = 0xc5;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[2] < 100 ? 12 : 10];
            SetTextColors(color, GetTextKeyColor());
            if(g_game->progress[2] == 100 && DAT_0051e822 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[2] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[2];
            DAT_0051e822 = g_game->progress[2];
            FUN_004a50e0(&gadget, (char*)FUN_004c5740("Units"), 0x5a, 0xda, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[2] * 7) / 2 + 0xcd;
            rect[1] = 0xda;
            rect[3] = 0xee;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[3] < 100 ? 12 : 10];
            SetTextColors(color, GetTextKeyColor());
            if(g_game->progress[3] == 100 && DAT_0051e823 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[3] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[3];
            DAT_0051e823 = g_game->progress[3];
            FUN_004a50e0(&gadget, (char*)FUN_004c5740("Animation"), 0x5a, 0x106, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[3] * 7) / 2 + 0xcd;
            rect[1] = 0x106;
            rect[3] = 0x11a;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[4] < 100 ? 12 : 10];
            SetTextColors(color, GetTextKeyColor());
            if(g_game->progress[4] == 100 && DAT_0051e824 != 100) {
                ((unsigned char*)&DAT_0051e6cc)[0] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6cc)[0];
            DAT_0051e824 = g_game->progress[4];
            FUN_004a50e0(&gadget, (char*)FUN_004c5740("3D Data"), 0x5a, 0x130, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[4] * 7) / 2 + 0xcd;
            rect[1] = 0x130;
            rect[3] = 0x144;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
            DrawFrame(&gadget, lightbar, rect[0] + *((short*)lightbar + 2),
                         rect[1] + *((short*)lightbar + 3));
            color = g_game->palette[g_game->progress[5] < 100 ? 12 : 10];
            SetTextColors(color, GetTextKeyColor());
            if(g_game->progress[5] == 100 && DAT_0051e825 != 100) {
                ((unsigned char*)&DAT_0051e6cc)[1] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6cc)[1];
            DAT_0051e825 = g_game->progress[5];
            FUN_004a50e0(&gadget, (char*)FUN_004c5740("Explosions"), 0x5a, 0x15b, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[5] * 7) / 2 + 0xcd;
            rect[1] = 0x15b;
            rect[3] = 0x16f;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
        }
        if (((Class_00435100*)g_game->field_391e9)->FUN_00435100() == 3) {
            DrawSyncStatus(&gadget);
            SendLoadProgress();
        }
        UnlockScreen(&gadget);
        FlipScreen();
    }
    FUN_004b6b50(200);
}

// Decompiled by Opus, Haiku, Sonnet, Claude Opus 5.5, deepseek-v4.1-flash, DeepSeek V4.1 Flash and GPT-6. Names are provisional.
// The end-of-game module (0x41d920 to 0x420610), Cavedog's endgame.cpp: the
// game object's creation, the end-of-mission outcome screen, the statistics
// screen and its bars, the screen and palette fades, the CD check dialog,
// the end-game state machine, and the memory cache shutdown. The module's
// files gathered in address order; 0x41dc20 (a gap region) stays in
// endgame_41dc20.cpp, and 41e260's raw offset view is folded into the Game
// type below.
#include <windows.h>
#include <string.h>
#include <new.h>
#include <stdlib.h>

#include "../sound/sound.h"

class Mission {
public:
    int GetGameType();
    char* GetNameSlot(int index);
    int GetMissionIndex();
    int MissionExists(int index);
    int SelectMission(int param_1);
    int BuildMissionList(int* param_1);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void LoadBriefing();
    void BuildCampaignFilePath(int index, char* path, char* dir, char* ext);
    int GetDescription();
    char* GetMissionName();
    int GetTerrainLength();
    bool HasMissionName();
    int GetTranslatedName();
    unsigned int ComputeMapChecksum();
    void LoadCampaign(char* name);
    char unknown_0[0xd54];
    float killMul;                     // +0xd54
    float timeMul;                     // +0xd58
};

class CMemoryCache {
public:
    void ClearPointers();
    void FreeBuffer();
};

struct Amount {
    int current;                       // +0x0
    int required;                      // +0x4
};

struct Grid {
    void* cells;                       // +0x0
    int width;                         // +0x4
    int height;                        // +0x8
    int field_c;                       // +0xc
    Grid() { width = 0; height = 0; field_c = 0; cells = 0; }
};

#pragma pack(push, 1)

struct Surface_0041df20;

struct Rect {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Data {
    char unknown_0[0x14];
    void* items;                       // +0x14
    char unknown_18[8];
};

// One 0x15b-byte GUI entry. The views disagree about the bytes at +0xba:
// 41ea30's and 41f7f0's Amount against 41ec50's short field_ba and 41e420's
// image pointer at +0xbe; the union keeps both readings.
struct Entry {
    unsigned char type;                // +0x0
    char unknown_1;
    char name[0x11];                   // +0x2
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attr;                          // +0x1b
    int color;                         // +0x1f
    int color2;                        // +0x23
    char unknown_27[2];
    unsigned char flag;                // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                       // +0xb6
    char unknown_b8[2];
    union {                            // +0xba
        Amount amount;                 // 41ea30's and 41f7f0's
        struct {
            short field_ba;            // 41ec50's
            char unknown_bc[2];
            void* image;               // +0xbe, 41e420's
            char unknown_c2[4];
            unsigned short field_c6;   // +0xc6, 41e420's
        };
    };
    char unknown_c8[0x136 - 0xc8];
    short field_136;                   // +0x136
    char unknown_138[0x142 - 0x138];
    short field_142;                   // +0x142
    char unknown_144[0x15b - 0x144];
};

struct Layer;
struct Menu {
    char unknown_0[8];
    void* font_8;                      // +0x8
    void* font_c;                      // +0xc
    char unknown_10[4];
    void* font;                        // +0x14
    Layer* layer;                      // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Layer {
    int unknown_0;
    Entry* entries;                    // +0x4
    void (__stdcall* handler)(Menu*);  // +0x8
    Data* data;                        // +0xc
    char unknown_10[0x24 - 0x10];
    void* surface;                     // +0x24
};

struct Display {
    char unknown_0[0xd4];
    int width;                         // +0xd4
    int height;                        // +0xd8
    char unknown_dc[0xf0 - 0xdc];
    unsigned short low : 1;            // +0xf0
    unsigned short network : 1;
    unsigned short high : 14;
    char unknown_f2[0x614 - 0xf2];
    float paletteBrightness;           // +0x614
};

struct PlayerInfo {
    char unknown_0[0x95];
    unsigned char flag_95;             // +0x95
    unsigned char color;               // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned short bits_9b_0 : 6;      // +0x9b
    unsigned short flag_9b_6 : 1;
    unsigned short bits_9b_7 : 9;
};

// The per-player statistics row: 41e420 indexes the seven ints at +0x1e;
// 41dc20's names for them stay as their own view, unused here, since the
// union's symbols keep the allocation (docs/c2-regalloc.md).
struct Slot {                          // 0x3a bytes
    char name[0x1e];                   // +0x0
    union {
        int stats[7];                  // +0x1e
        struct {
            int kills;
            int losses;
            int field_26;
            int field_2a;
            int field_2e;
            int field_32;
            int score;
        };
    };
};

struct Header {
    unsigned char type;                // +0x0
    char unknown_1;
    char name[0x11];                   // +0x2
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attr;                          // +0x1b
    int color;                         // +0x1f
    int color2;                        // +0x23
    char unknown_27[2];
    unsigned char flag;                // +0x29
    char unknown_2a[0xb6 - 0x2a];
};

struct Bar {                           // 0xd6 bytes, passed to AddBarGadget
    Header h;
    int max;                           // +0xb6
    int field_ba;                      // +0xba
    int value;                         // +0xbe
    int field_c2;                      // +0xc2
    int field_c6;                      // +0xc6
    float scale;                       // +0xca
    int field_ce;                      // +0xce
    int field_d2;                      // +0xd2
};

struct Button {                        // 0xcc bytes, passed to AddHotspotGadget
    Header h;
    char unknown_b6[0xc8 - 0xb6];
    int flags;                         // +0xc8
};

struct Options {
    char unknown_0[0x228];
    int difficulty;                    // +0x228
};

class Player {                         // 0x14b bytes
public:
    Player();
    // Unused here: the symbol id this declaration takes keeps 0x41f0a0's and
    // 0x41f7f0's register allocation (docs/c2-regalloc.md).
    void SetType(int param_1);
    int active;                        // +0x00
    char unknown_4[0x22 - 0x4];
    unsigned char message;             // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo* info;                  // +0x27
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char type;                // +0x73
    char unknown_74[0xac - 0x74];
    double field_ac;                   // +0xac
    double field_b4;                   // +0xb4
    char unknown_bc[0xcc - 0xbc];
    double field_cc;                   // +0xcc
    double field_d4;                   // +0xd4
    char unknown_dc[0xfc - 0xdc];
    short kills;                       // +0xfc
    short losses;                      // +0xfe
    char unknown_100[0x140 - 0x100];
    int field_140;                     // +0x140
    char unknown_144[0x146 - 0x144];
    char field_146;                    // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    int unknown_0;
    const char* build_date;            // +0x4
    const char* build_time;            // +0x8
    char unknown_c[0x10 - 0xc];
    Sound* sound;                      // +0x10
    char unknown_14[0x519 - 0x14];
    Menu menu;                         // +0x519
    char unknown_57d[0xdcb - 0x57d];
    unsigned char textColor;           // +0xdcb
    char unknown_dcc[0xdcf - 0xdcc];
    unsigned char color1;              // +0xdcf
    char unknown_dd0[3];
    unsigned char color2;              // +0xdd3
    char unknown_dd4[0xdda - 0xdd4];
    unsigned char shadowColor;         // +0xdda
    char unknown_ddb[0x1b63 - 0xddb];
    Player players[11];                // +0x1b63
    char unknown_299c[0x29a0 - 0x299c];
    Options* options;                  // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned short bit0_2a44 : 1;      // +0x2a44
    unsigned short bit1_2a44 : 1;
    unsigned short bit2_2a44 : 1;
    unsigned short bit3_2a44 : 1;
    unsigned short bits4_2a44 : 12;
    char unknown_2a46[0x2bc0 - 0x2a46];
    unsigned char frontendSubstateRequest;  // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned short bits0 : 4;          // +0x2bee
    unsigned short flag4 : 1;          // +0x2bee, bit 4
    unsigned short bits5 : 11;
    char unknown_2bf0[0x2c7e - 0x2bf0];
    int advance;                       // +0x2c7e
    char unknown_2c82[0x1428f - 0x2c82];
    Grid grid_1428f;                   // +0x1428f
    Grid grid_1429f;                   // +0x1429f
    int field_142af;                   // +0x142af
    int field_142b3;                   // +0x142b3
    char unknown_142b7[0x14813 - 0x142b7];
    unsigned short* image_14813;       // +0x14813
    unsigned short* image_14817;       // +0x14817
    char unknown_1481b[0x148db - 0x1481b];
    void* logos32;                     // +0x148db
    char unknown_148df[0x37e1b - 0x148df];
    void* surface;                     // +0x37e1b
    int width;                         // +0x37e1f
    int height;                        // +0x37e23
    char unknown_37e27[0x37eee - 0x37e27];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x38dd9 - 0x38a4b];
    Slot slots[10];                    // +0x38dd9
    char unknown_3901d[0x39057 - 0x3901d];
    int state;                         // +0x39057
    unsigned int deadline;             // +0x3905b
    unsigned int nextTime;             // +0x3905f
    int done;                          // +0x39063
    int steps;                         // +0x39067
    int bar;                           // +0x3906b
    float field_3906f;                 // +0x3906f
    int skip;                          // +0x39073
    void* lastFrame;                   // +0x39077
    void* image_3907b;                 // +0x3907b
    unsigned char* palette_3907f;      // +0x3907f
    unsigned char* current;            // +0x39083
    unsigned char* target;             // +0x39087
    char* delta;                       // +0x3908b
    char glamour[0x100];               // +0x3908f
    union {                            // +0x3918f
        int maxStats[7];               // 41e420's
        struct {                       // 41dc20's
            int maxKills;
            int maxLosses;
            int max_26;
            int max_2a;
            int max_2e;
            int max_32;
            int maxScore;
        };
    };
    int mission;                       // +0x391ab
    int field_391af;                   // +0x391af
    char unknown_391b3[0x391cf - 0x391b3];
    char missionFlags[0x1a];           // +0x391cf
    Mission* campaign;                 // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short bits0_3923b : 2;    // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short bit4_3923b : 1;
    unsigned short bits5_3923b : 11;
    char unknown_3923d[0x3924d - 0x3923d];

    Game() : build_date("Jul 30 1998"), build_time("11:16:36"),
        field_142af(0), field_142b3(0) {}
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
extern int g_endGameGlamourSoundStarted;
extern CMemoryCache g_debrisMemCache;
extern void __cdecl FreeMemoryCache();

void __cdecl AllocNotifyNop(int);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
unsigned int __cdecl GetTicks();
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall FadeRectangle(Surface_0041df20* dst, void* rect, int level);
Display* GetDisplay();
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall FillSurface(void* param_1, int param_2);
void BlankScreen();
void FlipScreen();
void __stdcall FreeSurface(void* image);
void LeaveNetGame();
void ShowLoadGameScreen();
void ShowSaveGameScreen();
void RegisterDataArchives();
void EnterMainMenuState();
void __stdcall SetGameMode(int a);
void __stdcall SetCursorOverlayEnabled(int param);
void __stdcall SetFrontendState(int state, int line, const char* file);
void __stdcall SetMissionType(int param);
void __stdcall SetCursorMode(int n);
void Force640x480Surfaces();
void ApplyDifficultyButtons();
void StartGlamourSound();
char __stdcall FindGameCdDrive(int param_1);
const char* __stdcall Translate(const char* text);
void __stdcall OpenMessageBox(Menu* menu, const char* text, int param_3, int param_4, int param_5);
void __stdcall PlaySoundByName(const char* name, int param_2);
int __stdcall IsCurrentGadgetNamed(Menu* gadget, char* name);
int __stdcall FindGadgetIndex(Entry* entries, char* name, int type);
Entry* __stdcall FindGadgetChecked(Entry* entries, char* name);
void __stdcall ClearSelectedGadget(Menu* menu);
int __stdcall AddHotspotGadget(Menu* menu, Button* record);
int __stdcall AddBarGadget(Menu* menu, Bar* record);
int GetFontLineHeight();
int __stdcall AddTextGadget(Layer* holder, char* type, char* text, int x, int y,
                           int width, int attr);
Layer* __stdcall LoadGuiLayer(Menu* menu, const char* name, int flags);
void __stdcall SelectGadgetByName(Menu* menu, const char* name);
char* __stdcall BuildScrollItems1(char* names, char* flags, int count);
void __stdcall ConfigureListBoxByName(Menu* menu, char* name, void* items, int count, int flag);
Entry* __stdcall FUN_004a0200(char* entries, char* name);
void __stdcall SetListBoxScrollByName(Menu* menu, char* name, int index);
void __stdcall SetGadgetActiveByName(Menu* menu, const char* name, int value);
void __stdcall SetTranslatedTextByName(Menu* menu, char* name, char* text, int param_4);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
void __stdcall DrawFrame(void* param_1, int param_2, int x, int y);
void __stdcall SetKeyboardInput(Menu* menu, int value);
void __stdcall RenderLayer(Menu* menu, int value);
void __stdcall FUN_0049fad0(Menu* menu);
void __stdcall MarkChanged(Menu* menu);
void __stdcall EnableKeyCommands(Menu* menu);
void __stdcall UpdateMenu(Menu* menu);
void __stdcall BlitMenuLayers(Menu* menu, void* param_2, void* param_3);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall DrawMessages(void* surface);
void __stdcall SetOffscreenSurface(void* surface);
void* __stdcall AllocSurface(const char* name, int width, int height);
void HandleNetPackets();
void __stdcall ReportGameEvent(int event);
const char* __stdcall GetRejectReasonText(unsigned reason);
int GetTickRate();
void __stdcall GetCurrentMouseEvent(int* event);
int PopKey();
void __stdcall DrawOutlinedString(void* surface, const char* text, int color, int color2, int y);
void ShowSoftwareCursor();
void HideSoftwareCursor();
void __stdcall StartPaletteFade(unsigned char* target, unsigned char* current, int steps);
void __stdcall SendPlayerEconomy(Player* player, int a, int b);
int __stdcall IsScreenNamed(Menu* menu, const char* name);

// Creates the global game object at a random offset (0..6993 bytes) inside a
// zeroed allocation, using placement new.
// FUNCTION: 0x41d920
void CreateGameObject()
{
    unsigned int offset = GetTickCount() % 1000 * 7;
    unsigned int size = offset + sizeof(Game);
    char* mem = (char*)operator new(size);
    memset(mem, 0, size);
    AllocNotifyNop((int)mem);
    g_game = new (mem + offset) Game;
}

// FUNCTION: 0x41d9f0
void __stdcall SetEndGameState(unsigned int param_1)
{
    unsigned int v = param_1 & 0xff;
    g_game->state = v;
}

// FUNCTION: 0x41da10
void __stdcall SetMissionStatus(int param1, int param2, int param3)
{
    char mask;
    if (param3)
        mask = -1;
    else
        mask = 0;
    *(unsigned char*)(param2 + param1) = (unsigned char)((mask & 0xb) + 0x4c);
}

// FUNCTION: 0x41da30
void InitMissionStatus(void)
{
    memset(&g_game->missionFlags[0], 0x55, 25);
    g_game->missionFlags[25] = 0;
}

// Sets up the end-of-mission screen: allocates the fade table and palette
// buffers, forces the display's +0x614 value to 1.0 (saving the old one for
// HandleEndMissionClick to restore), then either loads the campaign's "glamour"
// picture (falling back to glamour\Arm01.PCX) or opens the Outcome1 or
// Outcome0 screen.
// FUNCTION: 0x41da60
void SetUpEndMissionScreen()
{
    char path[256];
    g_game->delta = (char*)GameAllocIgnoreTag("FadeTable", 0x400);
    g_game->target = (unsigned char*)GameAllocIgnoreTag("desiredPalette", 0x400);
    g_game->current = (unsigned char*)GameAllocIgnoreTag("currentPalette", 0x400);
    Display* display = GetDisplay();
    g_game->field_3906f = display->paletteBrightness;
    display->paletteBrightness = 1.0f;
    unsigned char* palette = (unsigned char*)GameAllocIgnoreTag("Palette", 0x400);
    char* name = g_game->campaign->GetNameSlot(5);
    if (name == 0)
        g_game->image_3907b = 0;
    if (((Mission*)g_game->campaign)->GetGameType() == 1
        && g_game->bit4_3923b && name != 0) {
        BuildDataPath(path, "bitmaps\\glamour", name + 1, "PCX");
        if (HAPI_FileLengthByName(path) == 0)
            strncpy(g_game->glamour, "glamour\\Arm01.PCX", 0x100);
        else
            strncpy(g_game->glamour, path + 8, 0x100);
        g_game->image_3907b = LoadBitmapByName(g_game->glamour, palette);
        g_game->palette_3907f = palette;
        return;
    }
    if (((Mission*)g_game->campaign)->GetGameType() == 1)
        LoadPictureCached("Outcome1", 0, 0, 1);
    else
        LoadPictureCached("Outcome0", 0, 0, 1);
}

// Starts the fade handled by 0x41df20: ten steps, the first one due on the
// next tick.
// FUNCTION: 0x41dee0
void StartScreenFade(void)
{
    g_game->steps = 10;
    g_game->nextTime = GetTicks() + 1;
    g_game->done = 0;
}

// FUNCTION: 0x41df20
void StepScreenFade(void)
{
    Rect rect;
    rect.left = rect.top = 0;
    rect.right = g_game->width;
    rect.bottom = g_game->height;
    if (g_game->nextTime < GetTicks()) {
        FadeRectangle(0, &rect, g_game->steps - 0x1d);
        g_game->nextTime = GetTicks() + 1;
        g_game->steps--;
        if (g_game->steps == 0)
            g_game->done = 1;
    }
}

// Starts a palette fade: keeps copies of the target and current palettes,
// shows the current one and works out a signed step per channel (at least
// 1 towards the target) that 0x41e270 then adds on each tick.
#define FADE_STEP(k)                                                        \
    if (current[k] > target[k]) {                                           \
        g_game->delta[k] = min(-1, (target[k] - current[k]) / steps);       \
    } else if (current[k] == target[k]) {                                   \
        g_game->delta[k] = 0;                                               \
    } else {                                                                \
        g_game->delta[k] = max(1, (target[k] - current[k]) / steps);        \
    }

// FUNCTION: 0x41dfc0
void __stdcall StartPaletteFade(unsigned char* target, unsigned char* current, int steps)
{
    memcpy(g_game->target, target, 0x400);
    memcpy(g_game->current, current, 0x400);
    SetPaletteColors(current, 0, 0x100);
    g_game->done = 0;
    for (int i = 0; i < 0x100; i++) {
        FADE_STEP(i * 4)
        FADE_STEP(i * 4 + 1)
        FADE_STEP(i * 4 + 2)
        FADE_STEP(i * 4 + 3)
    }
}

// FUNCTION: 0x41e240
void ScheduleFadeTick(void)
{
    g_game->nextTime = GetTicks() + 1;
}

// FUNCTION: 0x41e260
int IsFadeDone()
{
    return g_game->done;
}

// One tick of the palette fade set up by 0x41dfc0: once the next tick is
// due, moves every channel of the current palette by its step without
// overshooting the target, marks the fade done when the palettes are equal,
// shows the palette and schedules the next tick.
#define FADE_TICK(k)                                                        \
    {                                                                       \
        int v = g_game->current[k] + g_game->delta[k];                      \
        g_game->current[k] = g_game->delta[k] < 0                           \
            ? (v < g_game->target[k] ? g_game->target[k] : v)               \
            : (v > g_game->target[k] ? g_game->target[k] : v);              \
    }

// FUNCTION: 0x41e270
void StepPaletteFade(void)
{
    if (g_game->nextTime <= GetTicks()) {
        for (int i = 0; i < 0x100; i++) {
            FADE_TICK(i * 4)
            FADE_TICK(i * 4 + 1)
            FADE_TICK(i * 4 + 2)
            FADE_TICK(i * 4 + 3)
        }
        if (memcmp(g_game->current, g_game->target, 0x400) == 0)
            g_game->done = 1;
        SetPaletteColors(g_game->current, 0, 0x100);
        g_game->nextTime = GetTicks() + 1;
    }
}

// Fills the end-of-game statistics screen: for each of the 10 player slots
// in use, adds a "PlayerColor<n>" button (given the logo image and the
// player's colour), the player's name as a TEXT gadget, and seven bars
// (Kills, Losses, EProduced, MProduced, EWasted, MWasted, Score) scaled
// against the per-stat values at +0x3918f, one row every 0x14 pixels.
#define STAT_BAR(n, px, fmt)                                                \
    bar.h.x = px;                                                           \
    bar.value = g_game->slots[i].stats[n];                                  \
    bar.max = g_game->maxStats[n];                                          \
    bar.scale = max(bar.value * 0.06666667f, 1.0f);                         \
    wsprintfA(bar.h.name, fmt, i);                                          \
    AddBarGadget(&g_game->menu, &bar);

// FUNCTION: 0x41e420
void FillEndGameStatistics(void)
{
    Bar bar;
    Button button;
    char name[64];
    Menu* menu = &g_game->menu;
    Entry* entries = menu->layer->entries;
    // Keep the setup statements in this order: it fixes the register choice.
    memset(&bar, 0, sizeof(bar));
    bar.h.flag = 0;
    bar.h.y = 0x5d;
    bar.h.width = 0x43;
    bar.h.height = 0x12;
    bar.h.color = g_game->color1;
    bar.h.color2 = g_game->color2;
    bar.field_d2 = 1;
    bar.field_ce = 1;
    bar.field_ba = 0;
    bar.field_c2 = 1;
    memset(&button, 0, sizeof(button));
    button.h.flag = 1;
    button.h.attr = 0x400;
    button.flags |= 1;
    g_game->bar = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->slots[i].name[0]) {
            wsprintfA(name, "PlayerColor%d", i);
            strcpy(button.h.name, name);
            button.h.x = 0x10;
            button.h.y = bar.h.y;
            button.h.width = 0x5b;
            button.h.height = 0x15;
            AddHotspotGadget(&g_game->menu, &button);
            int idx = FindGadgetIndex(entries, name, 6);
            if (idx != -1) {
                Entry* e = &entries[idx];
                if (e) {
                    e->image = g_game->logos32;
                    e->field_c6 = g_game->players[i].info->color;
                }
            }
            menu->font = menu->font_c;
            int h = GetFontLineHeight();
            AddTextGadget(g_game->menu.layer, "TEXT", g_game->slots[i].name, 0x10,
                         (0x14 - h) / 2 + bar.h.y, -1, 2);
            entries[entries->count].attr = 2;
            entries[entries->count].width = 0x5a;
            menu->font = menu->font_8;
            STAT_BAR(0, 0x70, "Kills%d")
            STAT_BAR(1, 0xba, "Losses%d")
            STAT_BAR(2, 0x104, "EProduced%d")
            STAT_BAR(3, 0x14e, "MProduced%d")
            STAT_BAR(4, 0x198, "EWasted%d")
            STAT_BAR(5, 0x1e2, "MWasted%d")
            STAT_BAR(6, 0x22c, "Score%d")
            bar.h.y += 0x14;
        }
    }
}

// FUNCTION: 0x41ea30
int AreStatBarsComplete(void)
{
    Entry* entries = g_game->menu.layer->entries;
    int count = entries->count;
    for (int i = 0; i < count; i++) {
        if (entries[i].type == 0xd) {
            Amount* amount = &entries[i].amount;
            if (entries[i].flag == 0 || amount->current < amount->required) {
                return 0;
            }
        }
    }
    return 1;
}

// Builds a block of 128-byte scroll-list item strings from a packed list of
// NUL-terminated names, prefixing each with marker glyphs chosen by its flag
// character ('L', 'W', 'U'). Frees the packed name list and returns the block.

// FUNCTION: 0x41eaa0
char* __stdcall BuildScrollItems1(char* names, char* flags, int count)
{
    char* items = (char*)GameAllocIgnoreTag("ScrollItems1", count << 7);
    memset(items, 0, count << 7);
    char* src = names;
    char* out = items;
    for (int i = 0; i < count; i++) {
        if (flags[i] == 'L') {
            *out++ = (char)0xff;
            *out++ = ' ';
        } else if (flags[i] == 'W') {
            *out++ = (char)0xfe;
            *out++ = ' ';
        }
        if (flags[i] == 'U') {
            *out++ = (char)0xfd;
            *out++ = ' ';
        }
        strcpy(out, src);
        int len = strlen(src) + 1;
        src += len;
        out += len;
    }
    GameFreeThunk(names);
    return items;
}

// Builds a block of 258-byte scroll-list item strings from a packed list of
// NUL-terminated names. The first min(a, b) names are copied verbatim; any
// remaining slots up to b get a "&G" marker prefixed to the next name. Frees
// the packed name list and returns the block.

// FUNCTION: 0x41eb60
char* __stdcall BuildScrollItems2(char* names, int a, int b)
{
    char* items = (char*)GameAllocIgnoreTag("ScrollItems2", b * 0x102);
    if (a > b) {
        a = b;
    }
    char* src = names;
    char* out = items;
    int i = 0;
    for (; i < a; i++) {
        strcpy(out, src);
        int len = strlen(src) + 1;
        src += len;
        out += len;
    }
    for (; i < b; i++) {
        strcpy(out, "&G");
        out += 2;
        strcpy(out, src);
        int len = strlen(src) + 1;
        src += len;
        out += len;
    }
    GameFreeThunk(names);
    return items;
}

// Click handler of the end-of-mission screen (ENDMSN.GUI, opened by
// OpenEndMissionScreen). On close (field +0x60 == -1) it frees the outcome images
// and the Missions list; otherwise it handles LoadGame, SaveGame,
// Start/Missions (checks the campaign CD and starts the chosen mission),
// MainMenu and Difficulty (cycles easy, medium, hard).
// FUNCTION: 0x41ec50
void __stdcall HandleEndMissionClick(Menu* gadget)
{
    Entry* entries = gadget->layer->entries;
    Data* data = gadget->layer->data;
    if (gadget->field_60 == -1) {
        BlankScreen();
        if (g_game->lastFrame != 0)
            FreeSurface(g_game->lastFrame);
        if (g_game->image_3907b != 0)
            FreeSurface(g_game->image_3907b);
        if (g_game->palette_3907f != 0)
            GameFreeThunk(g_game->palette_3907f);
        if (g_game->current != 0)
            GameFreeThunk(g_game->current);
        if (g_game->target != 0)
            GameFreeThunk(g_game->target);
        if (g_game->delta != 0)
            GameFreeThunk(g_game->delta);
        g_game->lastFrame = 0;
        g_game->image_3907b = 0;
        g_game->palette_3907f = 0;
        g_game->current = 0;
        g_game->target = 0;
        g_game->delta = 0;
        GameFreeThunk(data->items);
        GameFreeThunk(data);
        if (g_game->flag4)
            LeaveNetGame();
        g_game->sound->SetTrackCategory(4);
        Display* display = GetDisplay();
        display->paletteBrightness = g_game->field_3906f;
        return;
    }
    // LoadGame and SaveGame reset the gadget (ClearSelectedGadget) twice in a row;
    // the second call is redundant.
    if (IsCurrentGadgetNamed(gadget, "LoadGame")) {
        PlaySoundByName("BigButton", 0);
        ShowLoadGameScreen();
        ClearSelectedGadget(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SaveGame")) {
        PlaySoundByName("BigButton", 0);
        ShowSaveGameScreen();
        ClearSelectedGadget(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "Start") || IsCurrentGadgetNamed(gadget, "Missions")) {
        if (!FindGameCdDrive(0)) {
            OpenMessageBox(&g_game->menu,
                         Translate("Please insert the Campaign CD (Disc 2) and try again"),
                         200, 1, 1);
            ClearSelectedGadget(&g_game->menu);
        }
        RegisterDataArchives();
        PlaySoundByName("BigButton", 0);
        g_game->frontendSubstateRequest = 10;
        SetCursorOverlayEnabled(1);
        SetCursorMode(0x14);
        if (g_game->campaign->SelectMission(FindGadgetChecked(entries, "Missions")->field_ba)) {
            EnterMainMenuState();
            g_game->bit2_2a44 = 0;
            g_game->bit3_2a44 = 1;
            g_game->bit0_2a44 = 0;
            SetMissionType(1);
            g_game->bit4_3923b = 0;
            g_game->bit2_3923b = 0;
            SetFrontendState(13, 757, "c:\\cavedog\\wargame\\endgame.cpp");
            SetGameMode(2);
            return;
        }
    } else if (IsCurrentGadgetNamed(gadget, "MainMenu")) {
        PlaySoundByName("BigButton", 0);
        SetFrontendState(2, 770, "c:\\cavedog\\wargame\\endgame.cpp");
        SetGameMode(1);
        SetCursorOverlayEnabled(1);
        SetCursorMode(0x14);
        return;
    } else if (IsCurrentGadgetNamed(gadget, "Difficulty")) {
        PlaySoundByName("SKirmish", 0);
        if (g_game->difficulty == 0) {
            g_game->options->difficulty = 1;
            g_game->difficulty = 1;
            ClearSelectedGadget(gadget);
            return;
        }
        if (g_game->difficulty == 1) {
            g_game->options->difficulty = 2;
            g_game->difficulty = 2;
            ClearSelectedGadget(gadget);
            return;
        }
        if (g_game->difficulty == 2) {
            g_game->options->difficulty = 0;
            g_game->difficulty = 0;
            ClearSelectedGadget(gadget);
            return;
        }
    }
    ClearSelectedGadget(gadget);
}

// FUNCTION: 0x41f040
int ShouldShowNextMission()
{
    if (((Mission*)g_game->campaign)->GetGameType() == 1 &&
        ((g_game->field_391af == 0 &&
          ((Mission*)g_game->campaign)->MissionExists(g_game->mission + 1) == 0) ||
         ((Mission*)g_game->campaign)->MissionExists(g_game->mission + 1) != 0)) {
        return 1;
    }
    return 0;
}

// Inlined copy of ShouldShowNextMission.
static inline int HasNextMission()
{
    if (g_game->campaign->GetGameType() == 1 &&
        ((g_game->field_391af == 0 &&
          ((Mission*)g_game->campaign)->MissionExists(g_game->mission + 1) == 0) ||
         ((Mission*)g_game->campaign)->MissionExists(g_game->mission + 1) != 0)) {
        return 1;
    }
    return 0;
}

// Opens the end-of-mission screen (ENDMSN.GUI) with HandleEndMissionClick as its
// handler. When the campaign goes on to another mission (the inlined
// ShouldShowNextMission) it plays "outcome1", makes Start the default button and
// fills the Missions list; otherwise it plays "outcome0" and focuses
// MainMenu. Then it draws the outcome image and shows the menu.
// __stdcall although it takes no arguments: as __cdecl the load of `entries` moves up.
// FUNCTION: 0x41f0a0
void __stdcall OpenEndMissionScreen()
{
    BlankScreen();
    FillSurface(g_game->surface, 0);
    FlipScreen();
    Layer* layer = LoadGuiLayer(&g_game->menu, "ENDMSN.GUI", 0x80);
    layer->handler = HandleEndMissionClick;
    Data* data = (Data*)GameAllocIgnoreTag("EndMsnGUI", 0x20);
    data->items = 0;
    layer->data = data;
    char* entries = (char*)layer->entries;
    char next = HasNextMission();
    if (next) {
        ((Mission*)g_game->campaign)->SelectMission(g_game->mission);
        LoadPictureCached("outcome1", 1, 1, 0);
        strcpy((char*)layer->entries + 0xcc, "Start");
    } else {
        LoadPictureCached("outcome0", 1, 1, 0);
        SelectGadgetByName(&g_game->menu, "MainMenu");
    }
    next = HasNextMission();
    if (next) {
        int count = ((Mission*)g_game->campaign)->BuildMissionList((int*)&data->items);
        data->items = BuildScrollItems1((char*)data->items, g_game->missionFlags, count);
        // Suspected original bug: this finds the first 'U' mission flag but
        // the index is never used (perhaps a lost "select the first
        // unplayed mission" step).
        int i;
        for (i = 0; i < 0x19; i++) {
            if (g_game->missionFlags[i] == 'U')
                break;
        }
        ConfigureListBoxByName(&g_game->menu, "Missions", data->items, count, 0);
        Entry* knob = FUN_004a0200(entries, "KNOB");
        knob->field_136 = knob->height - knob->field_142 - 3;
        SetListBoxScrollByName(&g_game->menu, "Missions", g_game->mission);
        SetListBoxScrollByName(&g_game->menu, "Missions", g_game->mission + (g_game->field_391af != 0));
        ApplyDifficultyButtons();
    }
    Player* player = &g_game->players[g_game->localPlayer];
    int x = g_game->width / 2;
    if (g_game->field_391af != 0 && (player->active == 0 || !player->info->flag_9b_6)) {
        DrawFrame(layer->surface, GetGafFrame(g_game->image_14813, 0), x, 0x1c);
    } else {
        DrawFrame(layer->surface, GetGafFrame(g_game->image_14817, 0), x, 0x1c);
    }
    if (g_game->flag4)
        SetTranslatedTextByName(&g_game->menu, "MainMenu", "OK", 0);
    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    SetCursorMode(0x13);
}

// Enables the end-of-mission buttons: all of them when the campaign goes on
// to another mission (the inlined ShouldShowNextMission), otherwise only MainMenu,
// whose entry field +0x15 is set to 0x1a0. Then focuses Missions or
// MainMenu and refreshes the menu.
// FUNCTION: 0x41f400
void EnableEndMissionButtons()
{
    char next = HasNextMission();
    if (next) {
        SetGadgetActiveByName(&g_game->menu, "Start", 1);
        SetGadgetActiveByName(&g_game->menu, "LoadGame", 1);
        SetGadgetActiveByName(&g_game->menu, "SaveGame", 1);
        SetGadgetActiveByName(&g_game->menu, "KNOB", 1);
        SetGadgetActiveByName(&g_game->menu, "Missions", 1);
        SetGadgetActiveByName(&g_game->menu, "Difficulty", 1);
        SetGadgetActiveByName(&g_game->menu, "AdjustDiff", 1);
        SetGadgetActiveByName(&g_game->menu, "MainMenu", 1);
        SelectGadgetByName(&g_game->menu, "Missions");
    } else {
        SetGadgetActiveByName(&g_game->menu, "MainMenu", 1);
        Entry* entries = g_game->menu.layer->entries;
        entries[FindGadgetIndex(entries, "MainMenu", 1)].y = 0x1a0;
        SelectGadgetByName(&g_game->menu, "MainMenu");
    }
    FUN_0049fad0(&g_game->menu);
    MarkChanged(&g_game->menu);
}

// For each of the 10 slots at +0x38dd9 that is in use, sets the menu entry
// named "<prefix><slot>" to 1.
// FUNCTION: 0x41f5c0
void __stdcall ActivatePlayerGadgets(char* prefix)
{
    char name[64];
    for (int i = 0; i < 10; i++) {
        if (g_game->slots[i].name[0]) {
            wsprintfA(name, "%s%d", prefix, i);
            SetGadgetActiveByName(&g_game->menu, name, 1);
        }
    }
}

// FUNCTION: 0x41f630
void ShowEndMissionScreen()
{
    BlankScreen();
    OpenEndMissionScreen();
    FillEndGameStatistics();
    EnableEndMissionButtons();
    FUN_0049fad0(&g_game->menu);
    MarkChanged(&g_game->menu);
    SetGameMode(7);
    g_game->state = 7;
}

// FUNCTION: 0x41f680
void __stdcall HandleCdCheckClick(Menu* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            if (FindGameCdDrive(0)) {
                g_game->state = 5;
                return;
            }
            OpenMessageBox(&g_game->menu,
                         Translate("Please insert the Campaign CD (Disc 2) and try again"),
                         200, 1, 1);
        }
        ClearSelectedGadget(gadget);
    }
}

// Opens the CD check dialog (CDCHECK.GUI) with HandleCdCheckClick as its handler.
// FUNCTION: 0x41f700
void OpenCdCheckDialog()
{
    LoadGuiLayer(&g_game->menu, "CDCHECK.GUI", 0x101)->handler = HandleCdCheckClick;
    SetCursorOverlayEnabled(1);
    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// Called through a pointer (no direct callers): when the game state at
// +0x39057 is 1, draws the image at +0x39077 onto the surface at +0x37e1b at
// the display's position, then refreshes the GUI at +0x519.
// FUNCTION: 0x41f760
int DrawEndGameFrame()
{
    if (g_game->state == 1) {
        Display* d = GetDisplay();
        DrawSurface(g_game->surface, g_game->lastFrame, d->width, d->height);
        DrawMessages(g_game->surface);
        UpdateMenu(&g_game->menu);
        BlitMenuLayers(&g_game->menu, 0, 0);
        ShowSoftwareCursor();
        FlipScreen();
        return 1;
    }
    return 0;
}

#define ENABLE_BARS(label) \
    for(int i=0;i<10;++i) { \
        if(g_game->slots[i].name[0]) { \
            wsprintfA(text,"%s%d",label,i); \
            SetGadgetActiveByName(&g_game->menu,text,1); \
        } \
    }

static inline int StatsComplete()
{
    Entry* entries=g_game->menu.layer->entries;
    int count=entries->count;
    for(int i=0;i<count;++i) {
        if(entries[i].type==13) {
            if(!entries[i].flag || entries[i].amount.current<entries[i].amount.required) return 0;
        }
    }
    return 1;
}

// FUNCTION: 0x41f7f0
void __stdcall RunEndGameState()
{
    char text[64];
    int event[6];
    unsigned palette[256];
    SetOffscreenSurface(g_game->surface);
    HandleNetPackets();
    switch(g_game->state) {
    case 0:
        if(g_game->campaign->GetGameType()==3) {
            Display* e=GetDisplay();
            g_game->lastFrame=AllocSurface("Copy of last game frame",e->width,e->height);
            DrawSurface(g_game->lastFrame,g_game->surface,e->width,e->height);
            ReportGameEvent(7);
            g_game->state=1;
            Player* player=&g_game->players[g_game->localPlayer];
            if(player->message && player->message!=2) {
                const char* name=GetRejectReasonText(player->message);
                OpenMessageBox(&g_game->menu,Translate(name),320,1,1);
                MarkChanged(&g_game->menu);
                FUN_0049fad0(&g_game->menu);
                player->message=0;
            }
        } else { g_game->lastFrame=0; g_game->state=2; }
        break;
    case 1:
        if(IsScreenNamed(&g_game->menu,"MSGBOX.GUI")) {
            if(g_game->state==1) {
                Display* e=GetDisplay();
                DrawSurface(g_game->surface,g_game->lastFrame,e->width,e->height);
                DrawMessages(g_game->surface);
                UpdateMenu(&g_game->menu);
                BlitMenuLayers(&g_game->menu,0,0);
                ShowSoftwareCursor();
                FlipScreen();
            }
        } else g_game->state=2;
        break;
    case 2:
        g_game->steps=10;
        g_game->nextTime=GetTicks()+1;
        g_game->done=0;
        SetCursorOverlayEnabled(0);
        g_game->state=3;
        break;
    case 3:
        if(!g_game->done) {
            event[1]=0; event[0]=0;
            event[2]=g_game->width; event[3]=g_game->height;
            if(g_game->nextTime<GetTicks()) {
                FadeRectangle(0,event,g_game->steps-29);
                g_game->nextTime=GetTicks()+1;
                --g_game->steps;
                if(!g_game->steps) g_game->done=1;
            }
        } else {
            Force640x480Surfaces(); g_game->state=4;
            SetOffscreenSurface(g_game->surface);
        }
        break;
    case 4:
        if(g_game->campaign->GetGameType()==1 && !FindGameCdDrive(0)) {
            Layer* l=LoadGuiLayer(&g_game->menu,"CDCHECK.GUI",0x101);
            l->handler=HandleCdCheckClick;
            SetCursorOverlayEnabled(1);
            SetKeyboardInput(&g_game->menu,1);
            RenderLayer(&g_game->menu,0x40);
            g_game->state=8;
        } else g_game->state=5;
        break;
    case 5: {
        SetUpEndMissionScreen();
        int next=((Mission*)g_game->campaign)->MissionExists(g_game->mission+1);
        if(g_game->campaign->GetGameType()==1 && g_game->bit4_3923b && !next && !g_game->skip) {
            if((unsigned char)GetDisplay()->network) {
                if(!g_game->players[0].info->flag_95) SetFrontendState(4,0x4ce,"c:\\cavedog\\wargame\\endgame.cpp");
                else SetFrontendState(5,0x4d3,"c:\\cavedog\\wargame\\endgame.cpp");
            } else SetFrontendState(2,0x4d9,"c:\\cavedog\\wargame\\endgame.cpp");
            SetGameMode(2);
        } else if(g_game->campaign->GetGameType()==1 && g_game->bit4_3923b && g_game->image_3907b) {
            memset(palette,0,sizeof(palette));
            StartPaletteFade(g_game->palette_3907f,(unsigned char*)palette,5);
            g_game->nextTime=GetTicks()+1;
            g_game->state=6;
            DrawSurface(g_game->surface,g_game->image_3907b,0,0);
        } else {
            OpenEndMissionScreen(); FillEndGameStatistics(); EnableEndMissionButtons();
            FUN_0049fad0(&g_game->menu); MarkChanged(&g_game->menu);
            g_game->state=7;
        }
        break;
    }
    case 6:
        if(!g_game->done) {
            StepPaletteFade();
            unsigned now=GetTicks();
            now+=GetTickRate();
            g_game->deadline=now;
            g_endGameGlamourSoundStarted=0;
        } else {
            if(!g_endGameGlamourSoundStarted) { StartGlamourSound(); g_endGameGlamourSoundStarted=1; }
            if(g_game->deadline<GetTicks()) {
                GetCurrentMouseEvent(event);
                if(PopKey() || g_game->advance) {
                    g_game->sound->StopStream();
                    OpenEndMissionScreen(); EnableEndMissionButtons(); FillEndGameStatistics();
                    FUN_0049fad0(&g_game->menu); MarkChanged(&g_game->menu);
                    g_game->state=7;
                }
                unsigned deadline=GetTickRate()*5+g_game->deadline;
                if(deadline<GetTicks())
                    DrawOutlinedString(g_game->surface,Translate("Click to continue."),g_game->textColor,g_game->shadowColor,g_game->height-20);
            }
        }
        break;
    case 7: {
        if(StatsComplete()) {
            EnableKeyCommands(&g_game->menu);
            g_game->state=8; SetCursorMode(19); SetCursorOverlayEnabled(1);
            break;
        }
        UpdateMenu(&g_game->menu); BlitMenuLayers(&g_game->menu,0,0);
        int skip=0;
        int clicked=PopKey();
        if(clicked && g_game->campaign->GetGameType()!=3) skip=1;
        if(g_game->deadline<GetTicks() || skip) {
            if(clicked) {
                { ENABLE_BARS("Kills") }
                { ENABLE_BARS("Losses") }
                { ENABLE_BARS("EProduced") }
                { ENABLE_BARS("MProduced") }
                { ENABLE_BARS("EWasted") }
                { ENABLE_BARS("MWasted") }
                { ENABLE_BARS("Score") }
                PlaySoundByName("ActivateAllStatBars",0);
            }
            switch(g_game->bar) {
            case 0: { ENABLE_BARS("Kills") } PlaySoundByName("EndGameStatBar",0); break;
            case 1: { ENABLE_BARS("Losses") } PlaySoundByName("EndGameStatBar",0); break;
            case 2: { ENABLE_BARS("EProduced") } PlaySoundByName("EndGameStatBar",0); break;
            case 3: { ENABLE_BARS("MProduced") } PlaySoundByName("EndGameStatBar",0); break;
            case 4: { ENABLE_BARS("EWasted") } PlaySoundByName("EndGameStatBar",0); break;
            case 5: { ENABLE_BARS("MWasted") } PlaySoundByName("EndGameStatBar",0); break;
            case 6: { ENABLE_BARS("Score") } PlaySoundByName("EndGameScore",0); break;
            }
            if(g_game->campaign->GetGameType()==3) {
                Player* player=&g_game->players[g_game->localPlayer];
                for(int j=0;j<2;++j) SendPlayerEconomy(player,0,0);
            }
            g_game->deadline=GetTicks()+10;
            ++g_game->bar;
        }
        break;
    }
    case 8:
        HideSoftwareCursor(); UpdateMenu(&g_game->menu); ShowSoftwareCursor(); FlipScreen();
        HideSoftwareCursor(); BlitMenuLayers(&g_game->menu,0,0); ShowSoftwareCursor();
        break;
    }
    FlipScreen();
}

// FUNCTION: 0x4205f0
void InitMemoryCache(void)
{
    g_debrisMemCache.ClearPointers();
    atexit(FreeMemoryCache);
}

// FUNCTION: 0x420610
void __cdecl FreeMemoryCache()
{
    g_debrisMemCache.FreeBuffer();
}

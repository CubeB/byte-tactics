// Decompiled by DeepSeek V4.1 Flash, Haiku, Opus, deepseek-v4.1-flash, space-bunny-free, Space Bunny Free, longcat-2.5-preview-free and Claude Opus 5.5. Names are provisional.
//
// The frontend module (frontend.cpp): the front-end state machine and the
// state helpers it logs through, the main menu and its click handlers, the
// movie player, the warp-level loader, the picture cache and the debug key
// handler, in address order.
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../util/tdf.h"

class Sound {
public:
    int HasCdPlayerWindow();
    void SetTrackCategory(int value);
    int HasNoDriver();
    void CloseCdPlayerWindow();
};

class Mission {
public:
    void LoadCampaign(char* name);
    int SelectMission(int value);
    void LoadMissionByName(int param);
};

// A player block's method view (the players array of the game object).
struct Player {
    void SetType(int param);
};

class Class_0047bf20 {
public:
    void Close();
};

class MoviePlayer {
public:
    char pad[0x5b8];
    MoviePlayer(char* path, int a, int b, int c, int d, int e);
    void Play();
};

struct V4i {
    int a;
    int b;
    int c;
    int d;
};

// The flag words at +0x2a44, +0x2aaf, +0x2b4c and +0x2bee of the game object.
struct Bits_00426e80 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short : 11;
};

#pragma pack(push, 1)
// The player's data object, reached from the player block's owner pointer:
// the flag at +0x95 and the ready/synced bits at +0x97 and +0x9b.
struct PlayerOwner_004269d0 {
    char unknown_0[0x95];
    unsigned char flag;                // +0x95
    char unknown_96[0x97 - 0x96];
    unsigned short ready : 1;          // +0x97
    unsigned short rest_97 : 15;
    char unknown_99[2];
    unsigned short : 6;                // +0x9b
    unsigned short b6 : 1;             // +0x9b, mask 0x40
    unsigned short : 9;
};

// One of the game object's ten player blocks, 0x14b bytes each.
struct Player_004269d0 {
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x21 - 8];
    unsigned char field_21;            // +0x21
    char unknown_22[0x27 - 0x22];
    PlayerOwner_004269d0* owner;       // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char field_73;            // +0x73
    char unknown_74[0x14b - 0x74];
};

// 13-byte smoke puff record: the loop steps the pointer by 13.
struct Smoke_00425b80 {
    short x;                           // +0x0
    short y;                           // +0x2
    unsigned char active;              // +0x4
    char dx;                           // +0x5
    char dy;                           // +0x6
    unsigned char life;                // +0x7
    unsigned char fuel;                // +0x8
    int pos;                           // +0x9
};

// The cell map, a byte per cell whose low nibble is the feature there.
struct Map_00425b80 {
    char unknown_0[0xc];
    unsigned char* cells;              // +0xc
};

struct World_00425b80 {
    char unknown_0[0xbc];
    Map_00425b80* map;                 // +0xbc
};

// The front-end's current screen layer, the object at g_game+0x531: the
// gadget records at +0x4 (Dialog_004263b0.gadgets, Holder_428b60.entries,
// Terrain_00425b80.world), the handler at +0x8 and the per-tick callback at
// +0x1c, and the picture surface at +0x24 (Owner_00428730.surface,
// Terrain_00425b80.smoke).
struct Dialog_004263b0 {
    char unknown_0[4];
    char* gadgets;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
    int field_c;                       // +0xc
    char unknown_10[0x1c - 0x10];
    void (__stdcall* field_1c)();      // +0x1c
    char unknown_20[0x24 - 0x20];
    void* field_24;                    // +0x24
};

// The layer stack at g_game+0x519: the current layer at +0x18.
struct Sub_004263b0 {
    char unknown_0[0x18];
    Dialog_004263b0* current;          // +0x18
};

// One gadget record (0x15b bytes each) of a layer; the first record is the
// header with the count.
struct Entry_428b60 {
    unsigned char type;                // +0x00
    char unknown_1[0x1b - 1];
    unsigned int flags;                // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6
    char unknown_b8[0x15b - 0xb8];
};

// One picture-cache record (0x28 bytes each) of g_pictureCache.
struct Entry_00428730 {
    void* surface;                     // +0x00
    int* data;                         // +0x04
    char name[0x20];                   // +0x08
};

// The gadget record a click handler is called with.
struct Gadget_00425d80 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct GadgetOwner_00426190 {
    int unknown_0;
    int field_4;                       // +0x4
};

struct Gadget_00426190 {
    char unknown_0[0x18];
    GadgetOwner_00426190* owner;       // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

// The display object GetDisplay returns: bit 1 at +0xf0 is fullscreen.
struct Display_00425d80 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;           // +0xf0
    unsigned short fullscreen : 1;
    unsigned short rest : 14;
};

// The game object.
struct Game {
    char unknown_0[0x10];
    Sound* sound;                      // +0x10
    char unknown_14[0x4e5 - 0x14];
    int field_4e5;                     // +0x4e5
    char unknown_4e9[0x519 - 0x4e9];
    Sub_004263b0 sub;                  // +0x519
    char unknown_535[0xdda - 0x535];
    unsigned char field_dda;           // +0xdda
    char unknown_ddb[0x11eb - 0xddb];
    void* surface;                     // +0x11eb
    char field_11ef[0x1b63 - 0x11ef];  // +0x11ef
    Player_004269d0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[1];
    Bits_00426e80 flags;               // +0x2a44
    char unknown_2a46[0x2aaf - 0x2a46];
    Bits_00426e80 field_2aaf;          // +0x2aaf
    char unknown_2ab1[0x2b4c - 0x2ab1];
    Bits_00426e80 field_2b4c;          // +0x2b4c
    char unknown_2b4e[0x2ba2 - 0x2b4e];
    V4i field_2ba2;                    // +0x2ba2
    char unknown_2bb2[0x2bbe - 0x2bb2];
    char field_2bbe;                   // +0x2bbe
    char field_2bbf;                   // +0x2bbf
    char field_2bc0;                   // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    Bits_00426e80 field_2bee;          // +0x2bee
    unsigned char field_2bf0;          // +0x2bf0
    char unknown_2bf1[0x2c7e - 0x2bf1];
    int field_2c7e;                    // +0x2c7e
    char unknown_2c82[0x37e1b - 0x2c82];
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x37eee - 0x37e1f];
    int field_37eee;                   // +0x37eee
    char unknown_37ef2[0x38d7b - 0x37ef2];
    void* field_38d7b;                 // +0x38d7b
    char unknown_38d7f[0x391e9 - 0x38d7f];
    Mission* level;                    // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
    char unknown_391f5[0x391f9 - 0x391f5];
    void* field_391f9;                 // +0x391f9
    char unknown_391fd[0x39201 - 0x391fd];
    char field_39201[0x10];            // +0x39201
    char unknown_39211[0x3923d - 0x39211];
    int field_3923d;                   // +0x3923d
    int field_39241;                   // +0x39241
    int field_39245;                   // +0x39245
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// GLOBAL: 0x511fb8
extern char g_frontendErrorText[];

// GLOBAL: 0x512c80
extern int DAT_00512c80;

// GLOBAL: 0x503004
extern char g_frontendSourceFile[];

// GLOBAL: 0x50329c
extern char g_zrbMovie1[];

// GLOBAL: 0x503294
extern char g_zrbMovie2[];

// GLOBAL: 0x50328c
extern char g_zrbMovie5[];

// GLOBAL: 0x503284
extern char g_zrbMovie3[];

// GLOBAL: 0x50327c
extern char g_zrbMovie4[];

// GLOBAL: 0x50324c
extern char g_serviceErrorMessage[];

// GLOBAL: 0x502f9c
extern char g_frontendStateChangeFormat[];

// GLOBAL: 0x4fcdc8
extern char DAT_004fcdc8[];

// GLOBAL: 0x4fcdb8
extern char DAT_004fcdb8[];

// GLOBAL: 0x4fcda8
extern char DAT_004fcda8[];

// GLOBAL: 0x4fdaf0
extern V4i DAT_004fdaf0;

extern char g_directXWarningText[];
extern Smoke_00425b80* g_menuSparks;
extern int DAT_00512288;
extern int g_gpfCheckDone;
extern int g_noSoundDriverShown;
extern int g_directXCheckDone;
extern int g_cdPlayerDialogShown;
extern Entry_00428730 g_pictureCache[10];

int CodeChecksumFailed(void);
void __stdcall OpenMessageBox(Sub_004263b0* sub, char* text, int param_3, int param_4, int param_5);
int __stdcall GetTextPixelWidth(char* text);
void __stdcall SetOffscreenSurface(int param_1);
void __stdcall FillSurface(int param_1, int param_2);
void __stdcall FlipScreen(void);
int __stdcall PopKey(void);
void __stdcall SetCursorOverlayEnabled(int param);
void __stdcall SetMissionType(int param);
void __stdcall PlayMovie(char* param);
int __stdcall LoadPictureCached(const char* name, int param_2, int param_3, int param_4);
void __stdcall FUN_0049fad0(void* menu);
void RegisterDataArchives();
char __stdcall FindGameCdDrive(int param_1);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __cdecl FUN_004d85a0(void* p);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall SetCursorMode(int param_1);
int __stdcall IsCurrentGadgetNamed(Gadget_00425d80* gadget, char* name);
void __stdcall ClearSelectedGadget(void* param_1);
char* __stdcall Translate(char* text);
Display_00425d80* __stdcall GetDisplay(void);
void __stdcall CloseTopScreen(Sub_004263b0* sub);
void HideSoftwareCursor();
void Force640x480Surfaces();
Dialog_004263b0* __stdcall LoadGuiLayer(Sub_004263b0* sub, const char* name, int flags);
void __stdcall PlayLoopingSoundByName(const char* name, int param_2);
void __stdcall FUN_0049fa50(Sub_004263b0* sub);
void* __stdcall HAPI_LoadFile(char* name, int flag);
void __stdcall RemapPaletteToClosestIndices(Sub_004263b0* sub, int value, void* palette);
void __stdcall RenderLayer(Sub_004263b0* sub, int value);
void __stdcall FUN_0049fb10(Sub_004263b0* sub, int value);
void __stdcall SetFont(void* param);
void __stdcall FUN_004a0570(Sub_004263b0* sub, const char* name, int value);
void __stdcall SetGadgetTextByName(Sub_004263b0* sub, const char* name, const char* text);
int __stdcall FindGadgetIndex(char* gadgets, const char* name, int type);
int GetTextKeyColor();
void __stdcall SetTextColors(unsigned int a, int b);
void ShowSoftwareCursor();
void ClearMouseEventQueue();
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
int __stdcall CheckDirectXVersion(int a, int b, int c, int d, int e);
void __stdcall CheckGpfVersion();
int __stdcall IsGadgetNamed(int param1, int param2, char* name);
void __stdcall FUN_004a0bf0(char* sub, const char* name, const char* text, int param_4);
void __stdcall SelectGadgetByName(char* sub, const char* name);
void StopAllSounds();
void __stdcall BuildCdFilePath(char* dest, const char* a, const char* b, const char* c);
int __stdcall HAPI_FileLengthByName(char* path);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void __stdcall GetStartDirectory(char* param);
int GetTicks(void);
int __stdcall InitNetConnection(void);
void __stdcall OpenNewGameMenu(int param);
void __stdcall SetGameMode(int param);
void __stdcall SetEndGameState(int param);
int __stdcall SelectConnection(int param);
int __stdcall CreateLocalPlayer(unsigned char playerIndex, int param2);
int __stdcall JoinNetGame(V4i v, int idx);
void __stdcall AddNetPlayer(int param);
void __stdcall QuitApp(int param);
void __stdcall HAPINET_guaranteepackets(int param);
void __stdcall HAPINET_quitgame(int param);
void __stdcall InitPacketManager(int param1, int param2);
void __stdcall OpenMainMenu(void);
void __stdcall SaveSettings(void);
int __stdcall InitLobbiedConnection(void);
void __stdcall ResetPlayerSlots(void);
void __stdcall OpenSingleMenu(void);
void __stdcall LoadSettings(void);
void __stdcall OpenMissionBriefing(void);
void __stdcall OpenSkirmishMenu(void);
void __stdcall FillProviderList(void);
void __stdcall OpenModemDialog(void);
void __stdcall OpenSerialDialog(void);
void __stdcall OpenTcpDialog(void);
void __stdcall OpenSelectGameDialog(void);
void __stdcall CloseNetSession(void);
void __stdcall CreateNetGame(void);
void __stdcall LeaveNetGame(void);
void __stdcall BroadcastPlayerInfo(void);
void __stdcall OpenOptionsPanel(void);
void __stdcall FinishUnitSync(void);
void __stdcall OpenBattleRoom(void);
void __stdcall UpdateBattleRoom(void);
int __stdcall GetServiceProviderIndex(void);
void __stdcall ReportGameEvent(int param);
void __stdcall DeleteUnitSync(void);
void __stdcall ShutdownScoreTables(void);
int __stdcall InitScoreReporting(void);
void __stdcall ShowEndMissionScreen(void);
void __stdcall CheckFrontendStateChange(int line, char* file);
void __stdcall SetFrontendSubState(char state, int line, char* file);
void __stdcall SetFrontendState(char state, int line, char* file);
void __stdcall SetFrontendErrorText(char* text);
void ShowFrontendErrorText();
void BlankScreen();
void PresentFrontendFrame();
void __stdcall HandleMainMenuClick(Gadget_00425d80* gadget);
void __stdcall HandleCloseCdPlayerChoice(Gadget_00426190* gadget);
void __stdcall OpenCloseCdPlayerDialog();
void __stdcall FreeSurface(void* param_1);
void* __stdcall LoadBitmapByName(const char* name, int param_2);
int __stdcall SetBackgroundSurface(int param_1, int param_2);
int __stdcall SetPaletteColors(unsigned char* palette, int first, int count);

// The original calls this out of line from the state helpers; with the whole
// module in one file the compiler would inline it into them, so keep it out.
#pragma auto_inline(off)
// FUNCTION: 0x4256d0
void __stdcall CheckFrontendStateChange(int line, char* file)
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", line, file);
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x425730
void __stdcall SetFrontendErrorText(char* param_1)
{
    strncpy(g_frontendErrorText, param_1, 0xf9);
}

// FUNCTION: 0x425750
void ShowFrontendErrorText()
{
    if (strlen(g_frontendErrorText) != 0) {
        OpenMessageBox(&g_game->sub, g_frontendErrorText, GetTextPixelWidth(g_frontendErrorText) + 0x14, 1, 1);
        g_frontendErrorText[0] = 0;
    }
}

// FUNCTION: 0x4257a0
void BlankScreen()
{
    SetOffscreenSurface(g_game->field_37e1b);
    FillSurface(0, 0);
    FlipScreen();
}

// Drains the ring buffer that PopKey pops from (0 when empty).
// FUNCTION: 0x4257c0
void FlushKeyQueue(void)
{
    while (PopKey() != 0) {
    }
}

// The state machine helpers call this out of line where the original did; with
// the definition in this file the compiler would inline it into them.
#pragma auto_inline(off)
// FUNCTION: 0x4257e0
void __stdcall SetFrontendSubState(char state, int line, char* file)
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", line, file);
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbf = state;
    g_game->field_2bc0 = state;
}
#pragma auto_inline(on)

// Same: the state machine's UseServiceCalls calls this out of line.
#pragma auto_inline(off)
// FUNCTION: 0x425860
void __stdcall SetFrontendState(char state, int line, char* file)
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", line, file);
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbe = state;
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 155, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
}
#pragma auto_inline(on)

// Applies a pending front-end state change (+0x2bc0) to the current state
// (+0x2bbf). A debug check (CodeChecksumFailed, compiled to return 0) would report
// a code segment checksum error, with the line and file of the change.
// FUNCTION: 0x425930
void ApplyPendingSubState()
{
    char buf[256];
    char next = g_game->field_2bc0;
    if (next != g_game->field_2bbf) {
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    163, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
        }
        g_game->field_2bbf = next;
        g_game->field_2bc0 = next;
    }
}

// FUNCTION: 0x4259b0
void ResetFrontendState()
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0xa9, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbe = 0;
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x9b, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
    g_game->field_2bf0 = 0;
    *(unsigned short*)&g_game->field_2aaf &= 0xfffe;
    g_frontendErrorText[0] = 0;
}

// Resets front-end state (state 2) and clears the pending state-change markers
// (+0x2bbf, +0x2bc0). Two debug checks (CodeChecksumFailed, compiled to return 0)
// would report a code segment checksum error with the line and file.
// FUNCTION: 0x425a90
void EnterMainMenuState()
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                178, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbe = 2;
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                155, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
    LoadPictureCached(0, 0, 0, 0);
}

// FUNCTION: 0x425b60
void PresentFrontendFrame()
{
    SetOffscreenSurface(g_game->field_37e1b);
    HideSoftwareCursor();
    ShowSoftwareCursor();
    FlipScreen();
}

// Per-tick update of the smoke layer: a hundred live smoke puffs walking the
// 640x480 cell map, one cell per tick. A puff that is not running is dropped
// at a random cell that still holds a burnt feature (low nibble > 12); from
// then on it steps one cell per tick, is killed by leaving the map, by running
// out of life, or by meeting a cell that is not burnt, and on every hit it
// paints 0xaa and burns one unit of fuel. When the fuel runs out it turns:
// a puff drifting vertically starts drifting horizontally, and the other way
// round, three cells a tick in the direction its (parity) cell suggests, and
// refills its fuel.
// Suspected original bug: a running puff writes the smoke template byte over
// the feature nibble of the cell it stands on (`dest[s->pos] = src[s->pos]`,
// and 0xaa, whose low nibble is 10) instead of only setting a smoke bit, so
// burnt features under a puff stop looking burnt and later puffs die there.
// FUNCTION: 0x425b80
void UpdateMenuSparks()
{
    Dialog_004263b0* terrain = g_game->sub.current;
    if (terrain->field_24 != 0) {
        unsigned char* src = ((Map_00425b80*)terrain->field_24)->cells;
        Smoke_00425b80* s = g_menuSparks;
        unsigned char* dest = ((World_00425b80*)terrain->gadgets)->map->cells;
        int count = 100;
        do {
            if (s->active) {
                dest[s->pos] = src[s->pos];
                s->y += s->dy;
                s->x += s->dx;
                if (s->x < 0 || s->x >= 640 || s->y >= 480 || s->y < 0 || s->life == 0) {
                    s->active = 0;
                } else {
                    s->life--;
                    s->pos = s->pos + s->dx;
                    if (s->dy != 0) {
                        s->pos = s->pos + s->dy * 640;
                    }
                    if ((dest[s->pos] & 0xf) < 13) {
                        s->active = 0;
                    } else {
                        dest[s->pos] = 0xaa;
                        if (s->fuel == 0) {
                            if (s->dy != 0) {
                                s->dy = 0;
                                // Char-typed arms keep the computation in 8-bit registers.
                                s->dx = (s->x & 1) ? (char)-3 : (char)3;
                                s->fuel = (rand() & 0xf) + 1;
                            } else {
                                // if/else, not a conditional: stops the dx = 0 store hoisting above.
                                if (s->y & 1) {
                                    s->dy = 3;
                                } else {
                                    s->dy = -3;
                                }
                                s->dx = 0;
                                s->fuel = (rand() & 0xf) + 1;
                            }
                        } else {
                            s->fuel--;
                        }
                    }
                }
            } else {
                s->x = rand() % 640;
                s->y = rand() % 220;
                s->pos = s->x + s->y * 640;
                // `> 12`, not `<= 12` with continue: every path must reach the s++ below.
                if ((dest[s->pos] & 0xf) > 12) {
                    s->active = 1;
                    s->life = rand() + 1;
                    s->fuel = (rand() & 0x1f) + 1;
                    if (s->life & 1) {
                        if (s->y & 1) {
                            s->dy = -3;
                            s->dx = 0;
                        } else {
                            s->dy = 3;
                            s->dx = 0;
                        }
                    } else {
                        if (s->x & 1) {
                            s->dx = -3;
                        } else {
                            s->dx = 3;
                        }
                        s->dy = 0;
                    }
                }
            }
            s++;
        } while (--count);
        FUN_0049fad0((char*)g_game + 0x519);
    }
}

// Title-screen click handler: dispatches on the gadget name, SINGLE/MULTI
// set the front-end state (+0x2bc0) to 5/6, INTRO checks the CD and starts
// the movie state (clearing +0x2bbe/+0x2bbf/+0x2bc0), EXIT goes to 8 and
// Credits to 9; unknown gadgets just release themselves.
// FUNCTION: 0x425d80
void __stdcall HandleMainMenuClick(Gadget_00425d80* gadget)
{
    char buf[256];

    if (gadget->field_60 == -1) {
        FUN_004d85a0(g_menuSparks);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SINGLE")) {
        PlaySoundByName("BigButton", 0);
        SetCursorMode(0x14);
        g_game->field_2bc0 = 5;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "MULTI")) {
        PlaySoundByName("BigButton", 0);
        SetCursorMode(0x14);
        RegisterDataArchives();
        BuildDataPath(buf, "maps", "multiplay", "tdf");
        TdfFile obj;
        if ((&obj)->LoadFile(buf) != 0) {
            g_game->field_2bc0 = 6;
            SetOffscreenSurface(g_game->field_37e1b);
            FillSurface(0, 0);
            FlipScreen();
            return;
        }
        OpenMessageBox(&g_game->sub,
                     Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                     200, 1, 1);
        ClearSelectedGadget(&g_game->sub);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "INTRO")) {
        PlaySoundByName("smlButton", 0);
        Display_00425d80* display = GetDisplay();
        if (!display->fullscreen) {
            OpenMessageBox(&g_game->sub,
                         "Debug:  You must be in full-screen mode to play a movie.",
                         200, 1, 1);
            ClearSelectedGadget(&g_game->sub);
            return;
        }
        if (!FindGameCdDrive(0) && !FindGameCdDrive(1)) {
            OpenMessageBox(&g_game->sub,
                         Translate("Please insert a Total Annihilation CD and try again"),
                         200, 1, 1);
            ClearSelectedGadget(&g_game->sub);
            return;
        }
        SetCursorMode(0x14);
        if (GetKeyState(0x10) < 0) {
            g_game->field_39241 = 1;
        } else {
            g_game->field_39241 = 0;
        }
        while (PopKey()) {
        }
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    580, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
        }
        g_game->field_2bbe = 1;
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    155, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
        }
        g_game->field_2bbf = 0;
        g_game->field_2bc0 = 0;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "EXIT")) {
        PlaySoundByName("exit", 0);
        SetCursorMode(0x14);
        g_game->field_2bc0 = 8;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "Credits")) {
        PlaySoundByName("smlButton", 0);
        Display_00425d80* display = GetDisplay();
        if (!display->fullscreen) {
            OpenMessageBox(&g_game->sub,
                         "Debug:  You must be in full-screen mode to play a movie.",
                         200, 1, 1);
            ClearSelectedGadget(&g_game->sub);
            return;
        }
        if (!FindGameCdDrive(0) && !FindGameCdDrive(1)) {
            OpenMessageBox(&g_game->sub,
                         Translate("Please insert a Total Annihilation CD and try again"),
                         200, 1, 1);
            ClearSelectedGadget(&g_game->sub);
            return;
        }
        SetCursorMode(0x14);
        g_game->field_2bc0 = 9;
        return;
    }
    ClearSelectedGadget(gadget);
}

// Handler for a two-choice dialog gadget (same shape as 0x446020): plays
// the button sound, "CHOICE1" closes the game window, "CHOICE2" does
// nothing, anything else is passed on.
// FUNCTION: 0x426190
void __stdcall HandleCloseCdPlayerChoice(Gadget_00426190* gadget)
{
    int owner = gadget->owner->field_4;
    if (gadget->field_60 == -1)
        return;
    PlaySoundByName("SmallButton", 0);
    if (IsGadgetNamed(owner, gadget->field_60, "CHOICE1")) {
        g_game->sound->CloseCdPlayerWindow();
    } else if (!IsGadgetNamed(owner, gadget->field_60, "CHOICE2")) {
        ClearSelectedGadget(gadget);
    }
}

// Opens the YESORNO.GUI dialog asking "Close Windows CD Player?", relabels
// the CHOICE1 / CHOICE2 gadgets, puts the localised "Yes" and "No" on the two
// gadgets the lookups found, and installs HandleCloseCdPlayerChoice as the
// gadget handler. SelectGadgetByName is then told about "CHOICE1".
// FUNCTION: 0x426200
void __stdcall OpenCloseCdPlayerDialog()
{
    Dialog_004263b0* gadget = LoadGuiLayer(&g_game->sub, "YESORNO.GUI", 0x100);
    if (gadget != 0) {
        FUN_0049fb10(&g_game->sub, 1);
        char* entries = gadget->gadgets;
        char* choice1 = entries + 0x15b * FindGadgetIndex(entries, "CHOICE1", 1);
        char* choice2 = entries + 0x15b * FindGadgetIndex(entries, "CHOICE2", 1);
        strcpy(entries + 0xcc, "CHOICE1");
        strcpy(entries + 0xdc, "CHOICE2");
        strcpy(choice1 + 0xb6, Translate("Yes"));
        strcpy(choice2 + 0xb6, Translate("No"));
        FUN_004a0bf0((char*)&g_game->sub, "TITLE", Translate("Close Windows CD Player?"), 0);
        SelectGadgetByName((char*)&g_game->sub, "CHOICE1");
        gadget->handler = (void (__stdcall*)(void*))HandleCloseCdPlayerChoice;
        RenderLayer(&g_game->sub, 0x40);
    }
}

// Looks a picture-cache entry up by name and returns its palette data.
static char* Lookup_004263b0(const char* name)
{
    if (name != 0) {
        for (int i = 0; i < 10; i++) {
            if (strcmp(g_pictureCache[i].name, name) == 0)
                return (char*)g_pictureCache[i].data;
        }
    }
    return 0;
}

// FUNCTION: 0x4263b0
void __stdcall OpenMainMenu()
{
    while (g_game->sub.current != 0) {
        CloseTopScreen(&g_game->sub);
    }

    HideSoftwareCursor();
    SetOffscreenSurface(g_game->field_37e1b);
    FillSurface(0, 0);
    FlipScreen();
    Force640x480Surfaces();

    Dialog_004263b0* dialog = LoadGuiLayer(&g_game->sub, "MAINMENU.GUI", 0x80);
    dialog->handler = (void (__stdcall*)(void*))HandleMainMenuClick;
    dialog->field_c = 0;
    dialog->field_1c = UpdateMenuSparks;

    LoadPictureCached("FrontendX", 1, 1, 0);
    PlayLoopingSoundByName("BGM", 0);
    g_game->sound->SetTrackCategory(4);
    FUN_0049fa50(&g_game->sub);

    char* name = "FrontendX";
    char* found = Lookup_004263b0(name);

    char version[32];
    char palpath[256];
    char text[300];

    BuildDataPath(palpath, "palettes", "guipal", "PAL");
    void* palette = HAPI_LoadFile(palpath, 0);
    RemapPaletteToClosestIndices(&g_game->sub, (int)found, palette);
    FUN_004d85a0(palette);
    RenderLayer(&g_game->sub, 0xc0);
    FUN_0049fb10(&g_game->sub, 1);
    SetFont(g_game->field_391f9);

    strcpy(version, "v3.1");
    strcpy(palpath, version);
    FUN_004a0570(&g_game->sub, "DebugString", 1);
    SetGadgetTextByName(&g_game->sub, "DebugString", palpath);

    char* gadgets = g_game->sub.current->gadgets;
    int width = GetTextPixelWidth(palpath);
    short* px = (short*)(gadgets + 0x15b * FindGadgetIndex(gadgets, "DebugString", 5) + 0x13);
    *px += -(width / 2);

    SetTextColors(g_game->field_dda, GetTextKeyColor());
    ShowSoftwareCursor();
    ClearMouseEventQueue();

    g_menuSparks = (Smoke_00425b80*)FUN_004d83b0("SPARKS", 0x514);
    memset(g_menuSparks, 0, 0x145 * 4);

    if (g_cdPlayerDialogShown == 0) {
        if (g_game->sound->HasCdPlayerWindow()) {
            OpenCloseCdPlayerDialog();
            g_cdPlayerDialogShown = 1;
        }
    }

    if (g_directXCheckDone == 0) {
        g_directXCheckDone = 1;
        if (CheckDirectXVersion(4, 5, 0, 0x9b, 3) == 0) {
            if (_snprintf(text, 300, Translate(g_directXWarningText), "\n", "\n", "\n", "\n") < 0) {
                text[299] = 0;
            }
            OpenMessageBox(&g_game->sub, text, 200, 1, 1);
        }
    }

    if (g_noSoundDriverShown == 0) {
        if (g_game->sound->HasNoDriver()) {
            OpenMessageBox(&g_game->sub, Translate("No sound driver is available for use.\n"), 500, 1, 1);
            g_noSoundDriverShown = 1;
        }
    }

    if (g_gpfCheckDone == 0) {
        CheckGpfVersion();
        g_gpfCheckDone = 1;
    }
}

// FUNCTION: 0x426780
void __stdcall PlayMovie(char* param_1)
{
    char path[256];

    StopAllSounds();
    BuildCdFilePath(path, "Data", param_1, "zrb");
    if (HAPI_FileLengthByName(path) != 0) {
        SetOffscreenSurface(g_game->field_37e1b);
        FillSurface(0, 0);
        FlipScreen();
        SetCursorOverlayEnabled(0);
        do {
            g_game->field_38d7b = new MoviePlayer(path, 0, 600000, 1, 2000000, 1);
            ((MoviePlayer*)g_game->field_38d7b)->Play();
            Class_0047bf20* p = (Class_0047bf20*)g_game->field_38d7b;
            if (p != 0) {
                p->Close();
                operator delete(p);
            }
        } while (g_game->field_39241 != 0);
        g_game->field_38d7b = 0;
        while (PopKey() != 0) {
        }
        SetOffscreenSurface(g_game->field_37e1b);
        FillSurface(0, 0);
        FlipScreen();
    }
}

// FUNCTION: 0x4268b0
void __stdcall LoadWarpLevel(int param_1)
{
    char key[128];
    char path[256];
    char value[256];

    GetStartDirectory(path);
    strcat(path, "\\Warp.ini");
    wsprintfA(key, "warp%dcampaign", param_1);
    GetPrivateProfileStringA("WARPLEVELS", key, "default", value, 0x100, path);
    wsprintfA(key, "warp%dmission", param_1);
    int n = GetPrivateProfileIntA("WARPLEVELS", key, 0, path);
    SetMissionType(1);
    g_game->level->LoadCampaign(value);
    if (g_game->level->SelectMission(n)) {
        g_game->flags.b3 = 1;
        g_game->flags.b2 = 1;
    }
}

// FUNCTION: 0x4269d0
void HandleFrontendDebugKey(void)
{
    char key[128];
    char buf[256];
    char path[256];

    int event = PopKey();
    switch (event) {
    case 'A':
    case 'a':
        g_game->players[g_game->localPlayer].owner->flag = 0;
        return;
    case 'C':
    case 'c':
        g_game->players[g_game->localPlayer].owner->flag = 1;
        return;
    case 'E':
    case 'e':
        g_game->field_37eee = 0;
        return;
    case 'M':
    case 'm':
        g_game->field_37eee = 1;
        return;
    case 'H':
    case 'h':
        g_game->field_37eee = 2;
        return;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        GetStartDirectory(path);
        strcat(path, "\\Warp.ini");
        wsprintfA(key, "warp%dcampaign", event - 0x30);
        GetPrivateProfileStringA("WARPLEVELS", key, "default", buf, 0x100, path);
        wsprintfA(key, "warp%dmission", event - 0x30);
        {
            int n = GetPrivateProfileIntA("WARPLEVELS", key, 0, path);
            SetMissionType(1);
            g_game->level->LoadCampaign(buf);
            if (g_game->level->SelectMission(n)) {
                g_game->flags.b3 = 1;
                g_game->flags.b2 = 1;
            }
        }
        return;
    default:
        if (event == 0 && g_game->field_2c7e == 0
            && DAT_00512288 >= (int)GetTicks())
            return;
        PlaySoundByName("MAINMENU", 0);
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    938, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
        }
        g_game->field_2bbe = 2;
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    155, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
        }
        g_game->field_2bbf = 0;
        g_game->field_2bc0 = 0;
        return;
    }
}

// FUNCTION: 0x426d20
int ConnectToService(void)
{
    char buf[256];

    if (InitNetConnection()) {
        g_game->flags.b0 = 1;
        return 1;
    }
    strncpy(g_frontendErrorText, "An error occurred trying to use this service", 0xf9);
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                956, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbe = 0xf;
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                155, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                957, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
    return 0;
}

// Inlined copies of the front-end state helpers, one per inlining outcome in
// the original code. LogStateChange is CheckFrontendStateChange.
static void LogStateChange(int line, char* file)
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, g_frontendStateChangeFormat, line, file);
        OpenMessageBox(&g_game->sub, buf, 500, 1, 1);
    }
}

// SetFrontendSubState with its log call left out of line.
static void SetSubState(char state, int line, char* file)
{
    CheckFrontendStateChange(line, file);
    g_game->field_2bbf = state;
    g_game->field_2bc0 = state;
}

static void SetSubStateLogged(char state, int line, char* file)
{
    LogStateChange(line, file);
    g_game->field_2bbf = state;
    g_game->field_2bc0 = state;
}

// SetFrontendState with the sub-state change inlined too.
static void SetState(char state, int line, char* file)
{
    CheckFrontendStateChange(line, file);
    g_game->field_2bbe = state;
    SetSubState(0, 0x9b, g_frontendSourceFile);
}

// SetFrontendState with the sub-state change left out of line.
static void SetStateSubCall(char state, int line, char* file)
{
    CheckFrontendStateChange(line, file);
    g_game->field_2bbe = state;
    SetFrontendSubState(0, 0x9b, g_frontendSourceFile);
}

static void SetStateLogged(char state, int line, char* file)
{
    LogStateChange(line, file);
    g_game->field_2bbe = state;
    SetFrontendSubState(0, 0x9b, g_frontendSourceFile);
}

// ApplyPendingSubState.
static void UpdateSubState()
{
    char next = g_game->field_2bc0;
    if (next != g_game->field_2bbf)
        SetSubState(next, 0xa3, g_frontendSourceFile);
}

// ConnectToService, as inlined in case 15 and in case 20.
static int UseService(void)
{
    if (InitNetConnection()) {
        g_game->flags.b0 = 1;
        return 1;
    }
    SetFrontendErrorText(g_serviceErrorMessage);
    SetStateSubCall(0xf, 0x3bc, g_frontendSourceFile);
    SetFrontendSubState(0, 0x3bd, g_frontendSourceFile);
    return 0;
}

static int UseServiceCalls(void)
{
    if (InitNetConnection()) {
        g_game->flags.b0 = 1;
        return 1;
    }
    SetFrontendErrorText(g_serviceErrorMessage);
    SetFrontendState(0xf, 0x3bc, g_frontendSourceFile);
    SetFrontendSubState(0, 0x3bd, g_frontendSourceFile);
    return 0;
}

// The front-end state machine of frontend.cpp (line numbers in the
// CheckFrontendStateChange calls are the original __LINE__ values).
// FUNCTION: 0x426e80
void RunFrontendStateMachine(void)
{
    UpdateSubState();

    switch ((unsigned char)g_game->field_2bbe) {
    case 0: {
        Display_00425d80* p = GetDisplay();
        SetCursorOverlayEnabled(0);
        if (p->fullscreen) {
            if (g_game->field_3923d) {
                PlayMovie(g_zrbMovie1);
                SetState(1, 0x3dc, g_frontendSourceFile);
                g_game->field_3923d = 0;
                SaveSettings();
                return;
            }
            if (g_game->field_39245 == 0) {
                PlayMovie(g_zrbMovie1);
                SetState(2, 0x3e6, g_frontendSourceFile);
            } else
                SetState(2, 0x3e9, g_frontendSourceFile);
        } else
            SetState(2, 0x3ed, g_frontendSourceFile);
        break;
    }

    case 2:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            if (InitLobbiedConnection()) {
                g_game->flags.b0 = 1;
                g_game->field_2bee.b4 = 1;
                SetState(0x10, 0x403, g_frontendSourceFile);
                SetSubState(0x12, 0x404, g_frontendSourceFile);
                SetCursorOverlayEnabled(1);
                return;
            }
            SetMissionType(0);
            g_game->field_2bee.b4 = 0;
            OpenMainMenu();
            if (DAT_00512c80 == 0) {
                SetSubState(1, 0x40d, g_frontendSourceFile);
                SetCursorOverlayEnabled(1);
                return;
            }
            SetSubState(6, 0x40f, g_frontendSourceFile);
            SetCursorOverlayEnabled(1);
            return;
        case 1:
            PresentFrontendFrame();
            return;
        case 6:
            SetMissionType(3);
            g_game->flags.b3 = 0;
            SetState(0xf, 0x41c, g_frontendSourceFile);
            return;
        case 5:
            g_game->flags.b3 = 1;
            SetState(7, 0x421, g_frontendSourceFile);
            return;
        case 7:
            SetState(0, 0x425, g_frontendSourceFile);
            return;
        case 9:
            SetState(3, 0x429, g_frontendSourceFile);
            return;
        case 8:
            BlankScreen();
            QuitApp(0);
            return;
        }
        break;

    case 1:
        PlayMovie(g_zrbMovie2);
        SetState(2, 0x437, g_frontendSourceFile);
        break;

    case 3:
        PlayMovie(g_zrbMovie5);
        SetState(2, 0x43c, g_frontendSourceFile);
        break;

    case 4:
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            SetSubState(1, 0x443, g_frontendSourceFile);
            return;
        case 1:
            PlayMovie(g_zrbMovie3);
            PlayMovie(g_zrbMovie5);
            g_game->flags.b2 = 0;
            SetState(2, 0x44a, g_frontendSourceFile);
            SetGameMode(2);
            return;
        }
        break;

    case 5:
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            SetSubState(1, 0x454, g_frontendSourceFile);
            return;
        case 1:
            PlayMovie(g_zrbMovie4);
            PlayMovie(g_zrbMovie5);
            g_game->flags.b2 = 0;
            SetState(2, 0x45b, g_frontendSourceFile);
            SetGameMode(2);
            return;
        }
        break;

    case 7:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            OpenSingleMenu();
            ResetPlayerSlots();
            SetSubState(1, 0x47c, g_frontendSourceFile);
            return;
        case 1:
            PresentFrontendFrame();
            return;
        case 10:
            OpenNewGameMenu(1);
            SetState(8, 0x485, g_frontendSourceFile);
            SetSubState(1, 0x486, g_frontendSourceFile);
            return;
        case 11:
            LoadSettings();
            SetMissionType(2);
            SetState(9, 0x493, g_frontendSourceFile);
            return;
        case 13:
            SetState(0xa, 0x497, g_frontendSourceFile);
            OpenOptionsPanel();
            SetSubState(1, 0x499, g_frontendSourceFile);
            return;
        case 3:
            SetState(2, 0x49d, g_frontendSourceFile);
            return;
        case 14:
            OpenNewGameMenu(1);
            SetState(8, 0x4a2, g_frontendSourceFile);
            SetSubState(1, 0x4a3, g_frontendSourceFile);
            return;
        }
        break;

    case 10:
        switch ((unsigned char)g_game->field_2bbf) {
        case 1:
            PresentFrontendFrame();
            return;
        case 3:
            SetState(7, 0x4b1, g_frontendSourceFile);
            return;
        }
        break;

    case 8:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 1:
            PresentFrontendFrame();
            return;
        case 15:
            SetMissionType(1);
            SetState(0xb, 0x4c2, g_frontendSourceFile);
            return;
        case 16:
            SetMissionType(1);
            SetState(0xc, 0x4c7, g_frontendSourceFile);
            return;
        case 3:
            SetState(7, 0x4cb, g_frontendSourceFile);
            return;
        }
        break;

    case 9:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            OpenSkirmishMenu();
            SetSubState(1, 0x4d9, g_frontendSourceFile);
            return;
        case 1:
            PresentFrontendFrame();
            return;
        case 2:
            g_game->flags.b2 = 1;
            return;
        case 3:
            SetState(7, 0x4e5, g_frontendSourceFile);
            return;
        }
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            OpenMissionBriefing();
            SetSubState(1, 0x4f5, g_frontendSourceFile);
            return;
        case 1:
            PresentFrontendFrame();
            return;
        case 2:
            g_game->flags.b2 = 1;
            return;
        case 3:
            switch ((unsigned char)g_game->field_2bbe) {
            case 0xb:
                OpenNewGameMenu(0);
                SetState(8, 0x505, g_frontendSourceFile);
                SetSubState(1, 0x506, g_frontendSourceFile);
                return;
            case 0xc:
                OpenNewGameMenu(1);
                SetState(8, 0x50a, g_frontendSourceFile);
                SetSubState(1, 0x50b, g_frontendSourceFile);
                return;
            case 0xd:
                HideSoftwareCursor();
                ShowEndMissionScreen();
                SetGameMode(7);
                SetEndGameState(7);
                ShowSoftwareCursor();
                return;
            case 0xe:
                SetGameMode(2);
                SetState(7, 0x516, g_frontendSourceFile);
                return;
            }
            break;
        }
        break;

    case 15:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            g_game->field_2aaf.b1 = 0;
            g_game->field_2aaf.b0 = 0;
            if (InitLobbiedConnection()) {
                g_game->flags.b0 = 1;
                SetState(0x10, 0x52d, g_frontendSourceFile);
                SetSubState(0x12, 0x52e, g_frontendSourceFile);
                return;
            }
            FillProviderList();
            if (SelectConnection(-1))
                SetSubState(2, 0x534, g_frontendSourceFile);
            else
                SetSubState(1, 0x536, g_frontendSourceFile);
            return;
        case 13:
            SetState(0xa, 0x53b, g_frontendSourceFile);
            OpenOptionsPanel();
            SetSubState(1, 0x53d, g_frontendSourceFile);
            return;
        // Stays between cases 13 and 2: its position decides case 20 registers.
        case 1:
            PresentFrontendFrame();
            return;
        case 2:
            if (memcmp((char*)g_game + 0x39201, DAT_004fcdc8, 0x10) == 0) {
                SetState(0x14, 0x547, g_frontendSourceFile);
                SetSubState(1, 0x548, g_frontendSourceFile);
                OpenModemDialog();
                return;
            }
            if (memcmp((char*)g_game + 0x39201, DAT_004fcdb8, 0x10) == 0) {
                SetState(0x14, 0x54e, g_frontendSourceFile);
                SetSubState(1, 0x54f, g_frontendSourceFile);
                OpenSerialDialog();
                return;
            }
            if (memcmp((char*)g_game + 0x39201, DAT_004fcda8, 0x10) == 0) {
                SetState(0x14, 0x555, g_frontendSourceFile);
                SetSubState(1, 0x556, g_frontendSourceFile);
                OpenTcpDialog();
                return;
            }
            if (UseService()) {
                SetState(0x10, 0x55d, g_frontendSourceFile);
                SetSubState(0, 0x55e, g_frontendSourceFile);
            }
            return;
        case 3:
            SetState(2, 0x564, g_frontendSourceFile);
            // break, not return: lets case 7:11 cross-jump into case 16:17.
            break;
        }
        break;

    case 20:
        switch ((unsigned char)g_game->field_2bbf) {
        case 1:
            if (g_game->field_2aaf.b1) {
                if (UseServiceCalls()) {
                    SetStateSubCall(0x10, 0x571, g_frontendSourceFile);
                    SetSubState(0, 0x572, g_frontendSourceFile);
                } else {
                    g_game->field_2aaf.b0 = 0;
                    g_game->field_2aaf.b1 = 0;
                    SetStateSubCall(0xf, 0x577, g_frontendSourceFile);
                    SetSubState(0, 0x578, g_frontendSourceFile);
                }
            }
            PresentFrontendFrame();
            return;
        }
        break;

    case 16:
        PopKey();
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            HAPINET_guaranteepackets(1);
            SetSubState(1, 0x587, g_frontendSourceFile);
            ResetPlayerSlots();
            if (memcmp((char*)g_game + 0x39201, DAT_004fcdc8, 0x10) == 0 ||
                memcmp((char*)g_game + 0x39201, DAT_004fcdb8, 0x10) == 0) {
                if (g_game->field_2aaf.b0) {
                    SetSubState(0x11, 0x58f, g_frontendSourceFile);
                    return;
                }
                *(V4i*)((char*)g_game + 0x2ba2) = DAT_004fdaf0;
            }
            OpenSelectGameDialog();
            ShowFrontendErrorText();
            return;
        case 1:
            PresentFrontendFrame();
            ShowFrontendErrorText();
            return;
        case 17:
            CreateNetGame();
            if (CreateLocalPlayer(g_game->localPlayer, 1))
                AddNetPlayer(g_game->players[(unsigned char)g_game->localPlayer].field_4);
            SetState(0x11, 0x5ab, g_frontendSourceFile);
            return;
        case 19:
            g_game->players[(unsigned char)g_game->localPlayer].owner->b6 = 1;
        case 18:
            if (JoinNetGame(*(V4i*)((char*)g_game + 0x2ba2), (unsigned char)g_game->localPlayer) == 0) {
                SetSubState(0, 0x5b3, g_frontendSourceFile);
                return;
            }
            if (g_game->field_2bbf == 0x12) {
                BlankScreen();
                Force640x480Surfaces();
                if (InitScoreReporting()) {
                    ClearSelectedGadget(&g_game->sub);
                    SetSubState(0x14, 0x5c1, g_frontendSourceFile);
                } else
                    SetSubState(0x15, 0x5c4, g_frontendSourceFile);
            } else
                SetSubState(0x15, 0x5c7, g_frontendSourceFile);
            return;
        case 20:
            PresentFrontendFrame();
            return;
        case 21: {
            int unit = g_game->field_4e5;
            if (unit)
                g_game->players[(unsigned char)g_game->localPlayer].owner->ready = *(unsigned int*)(unit + 4) >> 1;
            else
                g_game->players[(unsigned char)g_game->localPlayer].owner->ready = 0;
            if (g_game->field_2bbf == 0x13)
                g_game->players[(unsigned char)g_game->localPlayer].owner->b6 = 1;
            unsigned char* q = (unsigned char*)g_game + 0x14b * (unsigned char)g_game->localPlayer + 0x1b84;
            *q = (g_game->field_2b4c.b4 << 1) | (*q & 0xfd);
            if (g_game->field_2b4c.b4) {
                g_game->level->LoadMissionByName((int)((char*)g_game + 0x2ab1));
                for (int i = 0; i < 10; i++) {
                    if (g_game->players[i].field_0) {
                        char t = g_game->players[i].field_73;
                        if (t == 1 || t == 2)
                            AddNetPlayer(g_game->players[i].field_4);
                    }
                }
                g_game->flags.b2 = 1;
                return;
            }
            SetStateLogged(0x11, 0x5ea, g_frontendSourceFile);
            return;
        }
        case 3:
            CloseNetSession();
            SetStateLogged(0xf, 0x5f0, g_frontendSourceFile);
            return;
        }
        break;

    case 17:
        switch ((unsigned char)g_game->field_2bbf) {
        case 0:
            if (!g_game->flags.b2) {
                OpenBattleRoom();
                ReportGameEvent(1);
                ReportGameEvent(2);
                SetSubState(1, 0x5ff, g_frontendSourceFile);
                BroadcastPlayerInfo();
                return;
            }
            FinishUnitSync();
            SetSubState(0x11, 0x605, g_frontendSourceFile);
            BroadcastPlayerInfo();
            return;
        case 1:
            UpdateBattleRoom();
            PresentFrontendFrame();
            if (g_game->flags.b2) {
                FinishUnitSync();
                CloseTopScreen(&g_game->sub);
                SetSubStateLogged(0x11, 0x613, g_frontendSourceFile);
            }
            return;
        case 17: {
            char* p = (char*)g_game + 0x1b63;
            for (int i = 0; i < 10; i++) {
                if (p[0x73] == 4)
                    ((Player*)p)->SetType(0);
                p += 0x14b;
            }
            FinishUnitSync();
            g_game->flags.b2 = 1;
            return;
        }
        case 3:
            g_game->flags.b0 = 1;
            HAPINET_quitgame((int)((char*)g_game + 0x14));
            InitPacketManager(2, 100);
            DeleteUnitSync();
            ReportGameEvent(8);
            ShutdownScoreTables();
            if (g_game->field_2bee.b4) {
                LeaveNetGame();
                return;
            }
            switch (GetServiceProviderIndex()) {
            case 0:
            case 3:
                SetStateLogged(0xf, 0x640, g_frontendSourceFile);
                break;
            default:
                SetStateLogged(0x10, 0x643, g_frontendSourceFile);
                break;
            }
            return;
        }
        break;
    }
}

// FUNCTION: 0x428730
void FreePictureCache()
{
    for (int i = 0; i < 10; i++) {
        if (g_pictureCache[i].surface != 0) {
            FreeSurface(g_pictureCache[i].surface);
            FUN_004d85a0(g_pictureCache[i].data);
            if (g_game->surface == g_pictureCache[i].surface) {
                g_game->surface = 0;
            }
            if (g_game->sub.current != 0 && g_game->sub.current->field_24 == g_pictureCache[i].surface) {
                g_game->sub.current->field_24 = 0;
            }
            g_pictureCache[i].surface = 0;
            g_pictureCache[i].data = 0;
            memset(g_pictureCache[i].name, 0, sizeof(g_pictureCache[i].name));
        }
    }
}

// FUNCTION: 0x4287b0
void ClearPictureCache()
{
    memset(g_pictureCache, 0, 0x64 * 4);
}

// Looks an entry of the table cleared by FreePictureCache up by name.
// FUNCTION: 0x4287d0
int* __stdcall FindCachedPicturePalette(const char* name)
{
    if (name == 0)
        return 0;
    for (int i = 0; i < 10; i++) {
        if (strcmp(g_pictureCache[i].name, name) == 0)
            return g_pictureCache[i].data;
    }
    return 0;
}

// FUNCTION: 0x428850
int __stdcall FindCachedPicture(char* name)
{
    if (name == 0)
        return 0;
    for (int i = 0; i < 10; i++) {
        if (strcmp(g_pictureCache[i].name, name) == 0)
            return (int)g_pictureCache[i].surface;
    }
    return 0;
}

// PALETTE CACHE.
// Suspected original bug: the name search matches a cached entry whose surface
// is already 0, moves it to the front, then the alloc path shifts and inserts a
// second entry with the same name at index 0, leaving a duplicate at index 1.
// FUNCTION: 0x4288d0
int __stdcall LoadPictureCached(const char* name, int param_2, int param_3, int param_4)
{
    void* surface = 0;
    int* data = 0;
    Entry_00428730 saved;

    if (param_2 != 0) {
        SetOffscreenSurface(g_game->field_37e1b);
        FillSurface(0, 0);
        FlipScreen();
    }

    if (name != 0) {
        for (int i = 0; i < 10; i++) {
            if (strcmp(g_pictureCache[i].name, name) == 0) {
                surface = g_pictureCache[i].surface;
                data = g_pictureCache[i].data;
                saved = g_pictureCache[i];
                for (int j = i - 1; j >= 0; j--)
                    g_pictureCache[j + 1] = g_pictureCache[j];
                g_pictureCache[0] = saved;
                break;
            }
        }
        if (surface == 0) {
            void* buf = FUN_004d83b0("Palette", 0x400);
            surface = LoadBitmapByName(name, (int)buf);
            data = (int*)buf;
            if (g_game->field_391f1 != 6) {
                if (g_pictureCache[9].surface != 0) {
                    FreeSurface(g_pictureCache[9].surface);
                    FUN_004d85a0(g_pictureCache[9].data);
                }
                for (int j = 9; j > 0; j--)
                    g_pictureCache[j] = g_pictureCache[j - 1];
                g_pictureCache[0].surface = surface;
                g_pictureCache[0].data = data;
                strcpy(g_pictureCache[0].name, name);
            }
        }
    } else {
        surface = 0;
        // Shared tail via goto: places that block after the return.
        goto after;
    }

after:
    // Nested if, one trailing return 0: both failure exits share one return block.
    if (param_4 == 0) {
        if (g_game->sub.current != 0) {
            SetBackgroundSurface((int)&g_game->sub, (int)surface);
            if (param_3 != 0)
                SetPaletteColors((unsigned char*)data, 0, 0x100);
        } else {
            g_game->surface = surface;
            if (name != 0)
                strcpy(g_game->field_11ef, name);
        }
        if (surface != 0 || name == 0) {
            if (name != 0) {
                strcpy(g_game->field_11ef, name);
                return 1;
            }
            g_game->field_11ef[0] = 0;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 0x428b60
void OrLabelAttribs(void)
{
    Entry_428b60* entries = (Entry_428b60*)g_game->sub.current->gadgets;
    for (int i = 1; i <= entries->count; i++) {
        if (entries[i].type == 5) {
            entries[i].flags |= 8;
        }
    }
}

// FUNCTION: 0x428bb0
void EmptyPreFrontendInitHook(void)
{
}

// 0x428bc0 (CodeChecksumFailed) stays in frontend_428bc0.cpp: it must not be
// visible to its callers or the compiler inlines its `return 0` into every
// checksum check and deletes the branch.

// FUNCTION: 0x428bd0
void EmptyPostSimStepHook(void)
{
}

// FUNCTION: 0x428be0
void EmptyPostSimStepHook_B(void)
{
}

// FUNCTION: 0x428bf0
void EmptyPostSimStepHook_C(void)
{
}

// FUNCTION: 0x428c00
int MainLoopContinueStub(void)
{
    return 1;
}

// FUNCTION: 0x428c10
int MainLoopContinueStub_B(void)
{
    return 1;
}

// FUNCTION: 0x428c20
int MainLoopContinueStub_C(void)
{
    return 1;
}

// FUNCTION: 0x428c30
void EmptyMainLoopHook(void)
{
}

// FUNCTION: 0x428c40
void EmptyMainLoopHook_B(void)
{
}

// FUNCTION: 0x428c50
void EmptyMainLoopHook_C(void)
{
}

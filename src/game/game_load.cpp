// Decompiled by deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, Claude Fable 5.1,
// GPT-6 Astra, space-bunny-free, Claude Opus 5.5, GPT-6, claude-opus-5-5 and Opus.
// Names are provisional.
//
// One Game from the four views (the menu object at +0x519 and the byte and word
// forms of the network flags at +0x38d75 sit in anonymous unions, as do the
// player records, the loaded/slots block and the map rectangles), one Mission,
// one PacketManager and the module's classes.
//
// <memory.h> stays before <windows.h>: it fixes the map-name block's x87 schedule.
#include <memory.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <time.h>
// Needed: without it the x87 schedule of the map-name block differs.
#include <ddraw.h>

#pragma pack(push, 1)

struct Settings_00496e10 {
    int value;                          // +0x0
    int flag_4;                         // +0x4
    int flag_8;                         // +0x8
    int flag_c;                         // +0xc
};

struct ViewFlags_497180 {               // g_game + 0x14281
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};

union Fixed_497180 {
    int i;                              // 16.16
    struct {
        short frac;
        short whole;
    } h;
};

struct FixedPos_497180 {
    Fixed_497180 x;
    Fixed_497180 y;
    Fixed_497180 z;
};

struct RecFlag_497180 {
    unsigned short started : 1;
    unsigned short : 15;
};

struct Display_00497f40 {
    char unknown_0[0x40];
    HWND hwnd;                          // +0x40
};

struct Sub_497180 {
    char unknown_0[0x10];
};

struct Gadget_497180 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Gadget_497180*);   // +0x8
    char* owner;                                 // +0xc
};

struct Rect_00497ce0 {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct Menu_00497ce0 {
    int field_51d;                      // +0x4 (g_game + 0x51d)
    char unknown_8[0x18 - 0x8];
    void* data;                         // +0x18 (g_game + 0x531)
};

#include "../network/player_info.h"

// Unused here: real functions declared to keep the file's symbol count (docs/c2-regalloc.md).
int CheckDirectXVersion(int, int, int, int, int);
void EnumPlayersCallback(int, int, int, int, int);
int AimCobStub(int, int, int, int);
void EmitBubbles(int, int, int, short);

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
    int inGame[10];                     // +0x0
    int unknown_28;                     // +0x28
    int flag40[10];                     // +0x2c
    char unknown_54[0x8c - 0x54];
};

// The ten player records at g_game+0x1b63 are 0x14b bytes. Three functions view
// them under different names; the fields that disagree over the same bytes sit
// in anonymous unions.
struct PlayerRec_00497f40 {             // 0x14b bytes, array at g_game+0x1b63
    union {
        int present;                    // +0x0
        int active;
    };
    unsigned int id;                    // +0x4
    char unknown_8[0x20 - 0x8];
    unsigned char percent;              // +0x20
    unsigned char field_21;             // +0x21
    char unknown_22[0x27 - 0x22];
    PlayerInfo* data;                   // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    union {
        unsigned char team;             // +0x73
        unsigned char control;
    };
    char unknown_74[0xdc - 0x74];
    float size1;                        // +0xdc
    float size2;                        // +0xe0
    char unknown_e4[0x146 - 0xe4];
    unsigned char kind;                 // +0x146
    unsigned char field_147;            // +0x147
    char unknown_148[0x149 - 0x148];
    RecFlag_497180 flags_149;           // +0x149
};

typedef PlayerRec_00497f40 PlayerInfo_00497f40;
typedef PlayerRec_00497f40 PlayerRec_00497ce0;

struct Point_00498cd0 {
    int x;                              // +0x0
    int y;                              // +0x4
};

struct View_00498cd0 {
    char unknown_0[0x2c];
    int x;                              // +0x2c
    int y;                              // +0x30
};

struct Fixed_00498d00 {
    unsigned short frac;                // +0x0
    short whole;                        // +0x2
};

struct Pos_00498d00 {
    Fixed_00498d00 x;                   // +0x0
    Fixed_00498d00 y;                   // +0x4
    Fixed_00498d00 z;                   // +0x8
};

struct Rect_00498d00 {
    int x;                              // +0x0
    int y;                              // +0x4
    int unknown_8[4];
};

struct Pos_00498da0 {
    unsigned int x;                     // +0x0
    unsigned int y;                     // +0x4
    unsigned int z;                     // +0x8
};

struct Rect_00498da0 {
    int left;                           // +0x0
    int top;                            // +0x4
    int right;                          // +0x8
    int bottom;                         // +0xc
};

struct Point_00498da0 {
    short x;                            // +0x0
    short y;                            // +0x2
};

struct BitFlags_00498da0 {
    unsigned char b0:1;
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char b6:1;
    unsigned char b7:1;
};

union Flags_00498da0 {
    unsigned char value;
    BitFlags_00498da0 bits;
};

struct View_00498da0 {
    int x;                              // +0x0
    int y;                              // +0x4
    char unknown_8[0x18 - 0x8];
};

#include "../map/cell.h"

struct Slot_00497180 {                  // 0x18 bytes
    int kind;                           // +0x0, 1 or 2 when the slot is in use
    char unknown_4[0x18 - 0x4];
};

struct Options_00497180 {               // at g_game+0x29a0
    Slot_00497180 slots[10];            // +0x0
    char unknown_f0[0x108 - 0xf0];
    Settings_00496e10 settings;         // +0x108
    int fixedloc;                       // +0x118
};

struct SideName_00497180 {              // 0x232 bytes, table at g_game+0x37f5b
    char name[0x232];
};

// One view of the game state. The ranges two views name differently sit in
// anonymous unions, so each function keeps the names it matched with.
class Sound;
#include "../util/hapi_bank.h"

struct Game {
    char unknown_0[0xc];
    Display_00497f40* displayContext;   // +0xc
    Sound* sound;                       // +0x10
    char unknown_10[0x519 - 0x14];
    union {                             // +0x519
        Menu_00497ce0 menu;
        struct {
            int field_519;
            int gaf;
            char unknown_521[0x531 - 0x521];
            int field_531;
        };
    };
    char unknown_535[0x589 - 0x535];
    int field_589;                      // +0x589
    char unknown_58d[0xdcb - 0x58d];
    union {                             // +0xdcb
        unsigned char palette[16];
        struct {
            char unknown_dcb[4];
            unsigned char color1;       // +0xdcf
            char unknown_dd0[5];
            unsigned char color2;       // +0xdd5
            char unknown_dd6[5];
        };
    };
    char unknown_ddb[0x11eb - 0xddb];
    int surface;                        // +0x11eb
    char unknown_11ef[0x1b63 - 0x11ef];
    PlayerRec_00497f40 players[10];     // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Options_00497180* options;          // +0x29a0
    union {                             // +0x29a4
        PlayerSlots_00497f40 slots;
        int loaded[10];
    };
    char unknown_2a30[0x2a42 - 0x2a30];
    unsigned char localPlayer;          // +0x2a42
    unsigned char playerIndex;          // +0x2a43
    char unknown_2a44[0x2c76 - 0x2a44];
    Rect_00498d00 view;                 // +0x2c76
    Point_00498da0 point;               // +0x2c8e
    char unknown_2c92[0x2caa - 0x2c92];
    Pos_00498da0 pos;                   // +0x2caa
    char unknown_2cb6[0x2cbc - 0x2cb6];
    unsigned short cellFeature;         // +0x2cbc
    unsigned char cursorMode;           // +0x2cbe
    char unknown_2cbf[0x2cc6 - 0x2cbf];
    Flags_00498da0 flags;               // +0x2cc6
    char unknown_2cc7[0x14223 - 0x2cc7];
    int mapWidthWorld;                  // +0x14223
    int mapHeightWorld;                 // +0x14227
    union { int mapPixelWidth; int worldW; };    // +0x1422b
    union { int mapPixelHeight; int worldH; };   // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    union {                             // +0x14281
        ViewFlags_497180 mapFlags;
        unsigned short mapFlagsWord;
    };
    char unknown_14283[0x142bb - 0x14283];
    Rect_00498da0 minimapHitRect;       // +0x142bb
    char unknown_142cb[0x142e7 - 0x142cb];
    union { short minimapGadgetX; short originX; };  // +0x142e7
    union { short minimapGadgetY; short originY; };  // +0x142e9
    union { short minimapGadgetW; short screenW; };  // +0x142eb
    union { short minimapGadgetH; short screenH; };  // +0x142ed
    char unknown_142ef[0x1431f - 0x142ef];
    int scrollX;                        // +0x1431f
    int scrollY;                        // +0x14323
    char unknown_14327[0x143a7 - 0x14327];
    char paletteRgba[0x400];            // +0x143a7
    char unknown_147a7[0x148cf - 0x147a7];
    int cursorHourglass;                // +0x148cf
    char unknown_148d3[0x37e1b - 0x148d3];
    int offscreen;                      // +0x37e1b
    int screenWidth;                    // +0x37e1f
    int screenHeight;                   // +0x37e23
    union {                             // +0x37e27
        Rect_00498da0 lim;
        struct {
            int viewCullMinX;
            int viewCullMinY;
            int viewCullMaxX;
            int viewCullMaxY;
        };
    };
    int viewPixelWidth;                 // +0x37e37
    int viewPixelHeight;                // +0x37e3b
    char unknown_37e3f[0x37ea0 - 0x37e3f];
    char mainHudLayoutName[0x1e];       // +0x37ea0
    char unknown_37ebe[0x37ee6 - 0x37ebe];
    unsigned short maxUnits;            // +0x37ee6
    char unknown_37ee8[0x37eec - 0x37ee8];
    unsigned short unitLimitIni;        // +0x37eec
    char unknown_37eee[0x37ef6 - 0x37eee];
    int battleCommanderDeath;           // +0x37ef6
    char unknown_37efa[0x37f1b - 0x37efa];
    int displayWidth;                   // +0x37f1b
    int displayHeight;                  // +0x37f1f
    char unknown_37f23[0x37f5b - 0x37f23];
    SideName_00497180 sideNames[2];     // +0x37f5b, only the two sides are named here
    char unknown_383bf[0x38a37 - 0x383bf];
    unsigned int lastSimBudgetTick;     // +0x38a37
    int simStepsPending;                // +0x38a3b
    char pad_38a3f[0x38a47 - 0x38a3f];
    int gameTick;                       // +0x38a47
    char pad_38a4b[0x38a4f - 0x38a4b];
    short speedHysteresis;              // +0x38a4f
    unsigned short flags_38a51;         // +0x38a51
    char unknown_38a53[0x38d6b - 0x38a53];
    HapiBank* pendingSaveStore;         // +0x38d6b
    // volatile: the loader thread writes these; gives the bars' byte loads.
    volatile unsigned char progress[6]; // +0x38d6f
    union {                             // +0x38d75
        volatile LoadFlags_00497f40 flags38d75;
        struct {
            unsigned short : 3;
            unsigned short synced : 1;
            unsigned short : 12;
        } netBits;
    };
    char unknown_38d77[0x38d81 - 0x38d77];
    int numSkirmishPlayers;             // +0x38d81
    char unknown_38d85[0x391e9 - 0x38d85];
    class Mission* mapInfo;             // +0x391e9
    char pad_391ed[0x391f1 - 0x391ed];
    int frontendState;                  // +0x391f1
    void (*handler)();                  // +0x391f5
    int fontComix;                      // +0x391f9
    char unknown_391fd[0x39219 - 0x391fd];
    Settings_00496e10 singleSettings;   // +0x39219
    char unknown_39229[0x39239 - 0x39229];
    short endGameCountdown;             // +0x39239
};

#pragma pack(pop)

extern Game* g_game;

extern unsigned char g_scorePanelKillFlash[10];
extern unsigned char g_scorePanelLossFlash[10];
extern "C" int g_loadingBarFlashAlpha;
extern "C" int DAT_0051e6cc;
extern "C" int g_loadingBarFlashDecayTick;
extern int g_usePacketManager;
extern "C" unsigned char g_loadingBarPrevPercent, DAT_0051e821, DAT_0051e822;
extern "C" unsigned char DAT_0051e823, DAT_0051e824, DAT_0051e825;
extern int g_nonCampaignGame;

class Mission {
public:
    int GetGameType();
    char* GetMissionName();
    void LoadMissionByName(void* player);
    int GetStartPosition(FixedPos_497180* pos, int id);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void LoadBriefing();
    int CountMissions();
    void LoadCampaign(char* name);
    int LoadMission(char* name);
    int MissionExists(int index);
    int BuildMissionList(int* list);
    void BuildCampaignFilePath(int index, char* path, char* dir, char* ext);
};

class PacketManager {
public:
    void SendAllQueued(int a);
};

extern PacketManager g_packetManager;

#include "../sound/sound.h"
// Unused here: the symbol ids these declarations take keep the allocation of LoadMatch,
// LoadingScreenFrame and OffsetWorldPosFromView (docs/c2-regalloc.md).
int ScanDirectory();
void RegisterUnitOrders(void);
void RegisterGroundOrders(void);
void EnableAICommands(void);
void RegisterAICommands(void);
void ResetAIPlayers(void);
void RegisterVtolOrders(void);
void StepAllGafSequences(void);
void ResetNetStats(void);
void InitCommands(void);
void RefreshSelectionOrders(void);
void DispatchOrdersPanelPageFlags(void);
void ResetCameraState(void);
void FindLocalCommander(void);
void ClampCameraPosition(void);
void ClampCameraTarget(void);
void UpdateScreenShake(void);
void UpdateCameraFollow(void);
void BeginMouseScroll(void);
void EndMouseScroll(void);
void UpdateMouseScroll(void);
void UpdateEdgeScroll(void);
void CenterCameraOnRadarClick(void);
void RegisterDataArchives(void);
void InitMissionStatus(void);
void ScheduleFadeTick(void);
void InitExplosions(void);
void FreeExplosions(void);
void UpdateExplosions(void);

void __stdcall SeedRandom(int x);
void __stdcall SleepMilliseconds(int x);
int __stdcall RandomInt(int x);
unsigned char __stdcall FindHostSlot();
void ApplyUseOnlyUnits();
void HandleNetPackets();
void __stdcall LoadPlayerControllers(void* mission);
void ApplySlotsToGamePlayers();
void LoadBattleAssets();
void InitPlayerResources();
void __stdcall RecalculateLineOfSight(int x);
void __stdcall LoadSavedGameState(void* mission);
void CreateMissionUnits();
void CenterCameraOnStartPosition();
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void BroadcastPlayerInfo();
void UpdateNetGameInfo();
void UpdatePlayers();
void __stdcall ReportGameEvent(int x);
void RebuildAIFeatureCells();
void __stdcall SetCameraPosition(int x, int y, int z);
unsigned short __stdcall FindUnitTypeId(const char* name);
void __stdcall SpawnCommanderAtStartPos(int team, int startpos);
void __stdcall CreateUnit(unsigned char team, unsigned short id, FixedPos_497180 pos, int a,
    int b, int c);
Gadget_497180* __stdcall LoadGuiLayer(Sub_497180* sub, const char* name, int flags);
void __stdcall HandleMain2LayoutEvent(Gadget_497180* gadget);
void __cdecl operator delete(void* p);

void __stdcall RenderLayer(Menu_00497ce0* menu, int value);
void __stdcall MarkLayerChanged(Menu_00497ce0* menu);
void __stdcall BlitMenuLayers(Menu_00497ce0* menu, int a, int b);
void __stdcall FillRectangle(void* surface, void* rect, int color);
void __stdcall DrawTextClipped(void* surface, const char* text, int x, int y, int len, int flag);
char* __stdcall Translate(char* s);

void LoadThreadMain();
void BattleFrame();
void __cdecl HandleBattleQuitPrompt(int);
void __cdecl BlankScreen();
void __cdecl FreePictureCache();
void __cdecl SaveSettings();
void __cdecl SendLoadProgress();
void __cdecl OnlineUnload();
void __cdecl DrawLightBars();
void __cdecl StopAllSounds();
void __cdecl MainLoopTick();
void __cdecl ShowSoftwareCursor();
void __stdcall UnlockScreen(void*);
void __cdecl RestoreScreen();
void __cdecl FlipScreen();
int __cdecl IsCdPlaying();
void __cdecl GameFreeThunk(void*);
void __stdcall BuildDataPath(void*, char*, char*, char*);
void __stdcall SendProbe(unsigned int, int);
void __stdcall DrawSyncStatus(void*);
void __stdcall DisableKeyCommands(void*);
int __stdcall GetTextPixelWidth(char*);
void __stdcall CloseTopScreen(void*);
void __stdcall SetCursorAnimation(void*, void*);
void __stdcall RemapPaletteToClosestIndices(void*, void*, void*);
void __stdcall SetCloseHandler(void (__cdecl *)(int), int);
void __stdcall SetResolution(int, int);
void __stdcall FatalError(char*);
int __stdcall StartThread(void (*)(void), int, int);
void* __stdcall GetGafFrame(void*, int);
void __stdcall DrawFrame(void*, void*, int, int);
void* __stdcall FindGafEntry(int, char*);
void __stdcall SetPaletteColors(void*, int, int);
void* __stdcall HAPI_LoadFile(void*, unsigned int*);
void __stdcall SetTextColors(int, int);
void __stdcall SetFont(int);
int __stdcall LockScreen(void*);
void __stdcall SetRestoreSurface(int);
void __stdcall SetOffscreenSurface(void*);
void* __stdcall AllocSurface(char*, int, int);
void __stdcall DrawSurface(void*, void*, int, int);
void __stdcall HAPINET_guaranteepackets(int);
int __cdecl GetScreenWidth();
int __cdecl GetScreenHeight();
int __cdecl GetTextKeyColor();
int __cdecl GetFontHeight();
int __cdecl AssignStartPositions();
unsigned int __cdecl GetTicks();
char* __cdecl GetPreferredLanguage();

int __stdcall ClampWorldPosToTerrain(int x, int y, int param_3);
void __stdcall ClampWorldPosToTerrain(int x, int y, Pos_00498da0* out);
int __stdcall GetGroundHeight(Pos_00498d00* pos);
int __stdcall PointInRect(Rect_00498da0* r, int x, int y);
Cell* __stdcall GetMapCell(int x, int y);
unsigned short __stdcall GetCellFeature(Cell* cell);


// Game start: seeds the random generators, loads the match settings for the
// current network mode (1 = skirmish defaults, 2 = multiplayer host block,
// 3 = joined game, from the host player's record), runs the between-missions
// summary, spawns each player's commander and opens the MAIN2 GUI.
//
// Kept as written, each for the registers it gives:
// - Cases 1 and 2 are inlined calls of ApplyMissionOptionFlags (matched on its own in
//   0x496e10.cpp, zero callers in the exe because /Ob2 inlined every call):
//   it copies the settings block's first int to +0x37ef6 and three int flags
//   into bits 2, 0 and 1 of the view-flags word at +0x14281. The inlined
//   `int -> 1-bit field` assignments are what mask with the CSE'd constant 1
//   (`mov edi,1` at the switch head, `and ebx,edi`), while the hand-written
//   `(w & ~M) | (b & M)` lanes narrow to `and bl,1; movzx si,bl`.
// - Case 3 and the post-wait block copy BITFIELDS of the player's
//   `unsigned short` flags at +0x9b: bits 8, 9 and 10 sit in the byte at
//   +0x9c, and `view->bit1 = pf->b9` (same bit position on both sides) is what
//   MSVC 5 compiles to the zero-extended byte load plus `and ebx,2` with no
//   shift; the 2-bit field at bits 11-12 and the 1-bit field at 13 give the
//   `shr ecx,0xb; and ecx,3` and `shr ecx,0xd; and ecx,1` extracts.
// - Case 2's statement order is copy 0x37eec -> 0x37ee6 first, then
//   g_nonCampaignGame = 1, then the settings block; the store through g_game makes
//   MSVC reload g_game for the block, as the original does.
// - The commander spawn loop is `for (int i = 0; i < 10; i++)` indexing
//   0x14b-byte records; MSVC strength-reduces it to the byte offset in ebx and
//   that puts g_game in the SIB base. A hand-written `off += 0x14b` loop swaps
//   the base and index.
// - QueryPerformanceCounter's halves: the sum is written straight from the two
//   fields, `SeedRandom(perfCount.LowPart + perfCount.HighPart)`. Through named
//   locals the addition lands in the other register.
// Also kept: the network flags at +0x38d75 written as volatile
// (the field the guide names); the +0x9b bit-6 test as a 1-bit bitfield; the
// mission-count loop with `i++` before `def += 6`; the ten-player walks with
// `(unsigned char)i` so the multiply is not strength-reduced; the
// std::random_shuffle call; rec+0x149 as a 1-bit bitfield (direct `or byte`);
// `pos.x.i` before `pos.y.i = 0`.
// Inlined into cases 1 and 2 below (matched on its own in 0x496e10.cpp).
inline void __stdcall ApplyMissionOptionFlags(Settings_00496e10* s)
{
    g_game->battleCommanderDeath = s->value;
    g_game->mapFlags.bit2 = s->flag_c;
    g_game->mapFlags.bit0 = s->flag_4;
    g_game->mapFlags.bit1 = s->flag_8;
}

// FUNCTION: 0x497180
void __cdecl LoadMatch(void*)
{
    LARGE_INTEGER perfCount;
    FixedPos_497180 pos;
    FixedPos_497180 start;
    int order[10];

    QueryPerformanceCounter(&perfCount);
    SeedRandom(perfCount.LowPart + perfCount.HighPart);
    srand((unsigned)time(NULL));
    g_game->gameTick = 0;

    // Six of these casts stay: taking more than two out in this function moves
    // the symbol state that LoadingScreenFrame's allocation depends on
    // (docs/c2-regalloc.md).
    switch (((Mission*)g_game->mapInfo)->GetGameType()) {
    case 1:
        g_nonCampaignGame = 0;
        ApplyMissionOptionFlags(&g_game->singleSettings);
        ApplyUseOnlyUnits();
        break;
    case 2:
        g_game->maxUnits = g_game->unitLimitIni;
        g_nonCampaignGame = 1;
        ApplyMissionOptionFlags(&g_game->options->settings);
        break;
    case 3: {
        g_game->maxUnits = g_game->unitLimitIni;
        g_nonCampaignGame = 1;
        g_game->flags_38a51 &= 0xfffe;

        int sel = FindHostSlot();
        unsigned char cur = g_game->localPlayer;
        if (g_game->players[cur].field_21 & 2) {
            do {
                PlayerInfo* p = g_game->players[g_game->localPlayer].data;
                if (g_usePacketManager)
                    g_packetManager.SendAllQueued(1);
                HandleNetPackets();
                sel = FindHostSlot();
                SleepMilliseconds(0x32);
                if (sel == 10)
                    continue;
                if (p->color == 0xff)
                    continue;
                if (p->unknown_8f == 0)
                    continue;
                break;
            } while (1);
            SleepMilliseconds(0x32);
        }

        ((Mission*)g_game->mapInfo)->LoadMissionByName(g_game->players[sel].data);
        if (FindHostSlot() == 10)
            break;

        int sel2 = FindHostSlot();
        PlayerInfo* p2 = g_game->players[sel2].data;
        g_nonCampaignGame = p2->cheating;
        g_game->battleCommanderDeath = p2->commander;
        g_game->mapFlags.bit1 = p2->los;
        g_game->mapFlags.bit2 = p2->losType;
        g_game->mapFlags.bit0 = p2->mapping;
        g_game->maxUnits = p2->maxUnits;
        break;
    }
    default:
        break;
    }

    if (g_game->pendingSaveStore != 0) {
        g_game->pendingSaveStore->OpenAccount("summary");
        if (g_game->pendingSaveStore->HasItem("BetweenMissions") == 0) {
            LoadPlayerControllers(g_game->pendingSaveStore);
            if (((Mission*)g_game->mapInfo)->GetGameType() == 2) {
                int count = 0;
                Slot_00497180* def = g_game->options->slots;
                int i = 0;
                while (i < 10) {
                    if (def->kind == 1 || def->kind == 2)
                        count = i + 1;
                    i++;
                    def++;
                }
                int cur = g_game->numSkirmishPlayers;
                if (count > cur)
                    cur = count;
                g_game->numSkirmishPlayers = cur;
                ApplySlotsToGamePlayers();
            }
        }
    }

    LoadBattleAssets();

    if (g_game->mapInfo->GetGameType() != 1) {
        if (g_game->mapInfo->GetGameType() == 3) {
            g_game->flags38d75.value |= 4;
            while ((g_game->flags38d75.value & 8) == 0)
                SleepMilliseconds(0x32);

            int sel = FindHostSlot();
            PlayerInfo* pl = g_game->players[sel].data;
            g_game->mapFlags.bit0 = pl->mapping;
            g_game->mapFlags.bit1 = pl->los;
            g_game->mapFlags.bit2 = pl->losType;
            g_game->battleCommanderDeath = pl->commander;

            for (int i = 0; i < 10; i++) {
                PlayerRec_00497f40* rec = &g_game->players[i];
                if (rec->active == 0)
                    continue;
                unsigned char st = rec->team;
                if (st != 1 && st != 2)
                    continue;
                pos.x.i = (RandomInt(g_game->mapWidthWorld - 0xa0) + 0x50) << 16;
                pos.y.i = 0;
                pos.z.i = (RandomInt(g_game->mapHeightWorld - 0xa0) + 0x50) << 16;
                if (rec->active != 0 && (rec->data->flags_9b & 0x40))
                    continue;
                PlayerInfo* pl2 = rec->data;
                int side = pl2->side;
                int which = rec->field_147;
                ((Mission*)g_game->mapInfo)->GetStartPosition(&pos, which);
                if (rec->active != 0 && rec->team == 1)
                    start = pos;
                unsigned short id = FindUnitTypeId(g_game->sideNames[side].name + 4);
                CreateUnit(rec->kind, id, pos, 1, 1, 0);
                int s1 = pl->energy * 100;
                int s2 = pl->metal * 100;
                rec->flags_149.started = 1;
                rec->size1 = (float)(s1 >= 200 ? s1 : 200);
                rec->size2 = (float)(s2 >= 200 ? s2 : 200);
            }

            unsigned char li = g_game->localPlayer;
            PlayerInfo* lp = g_game->players[li].data;
            int cx;
            int cz;
            if (lp->bit6) {
                g_game->mapFlagsWord &= 0xfffe;
                g_game->mapFlagsWord &= 0xfffd;
                cx = g_game->viewPixelWidth / 2;
                cz = g_game->viewPixelHeight / 2;
            } else {
                cx = start.x.h.whole - g_game->viewPixelWidth / 2;
                cz = start.z.h.whole - g_game->viewPixelHeight / 2;
            }
            SetCameraPosition(cx, cz, 0);
            ReportGameEvent(6);
        } else if (((Mission*)g_game->mapInfo)->GetGameType() == 2 &&
            g_game->pendingSaveStore == 0) {
            if (g_game->options->fixedloc != 0) {
                for (int i1 = 0; i1 < 10; i1++) {
                    if ((unsigned char)i1 < 10) {
                        PlayerRec_00497f40* rec = &g_game->players[(unsigned char)i1];
                        if (rec->active != 0) {
                            unsigned char st = rec->team;
                            if ((st == 1 || st == 2 || st == 3) &&
                                rec->kind != 10)
                                SpawnCommanderAtStartPos(i1, i1);
                        }
                    }
                }
            } else {
                for (int i2 = 0; i2 < 10; i2++)
                    order[i2] = -1;
                int n = 0;
                for (int i3 = 0; i3 < 10; i3++) {
                    if ((unsigned char)i3 < 10) {
                        PlayerRec_00497f40* rec = &g_game->players[(unsigned char)i3];
                        if (rec->active != 0) {
                            unsigned char st = rec->team;
                            if ((st == 1 || st == 2 || st == 3) &&
                                rec->kind != 10)
                                order[n++] = i3;
                        }
                    }
                }
                if (n > 2 || (int)(((__int64)rand() * 2) / 0x8000) != 0) {
                    std::random_shuffle(order, order + n);
                }
                int k = 0;
                for (int i4 = 0; i4 < 10; i4++) {
                    if ((unsigned char)i4 < 10) {
                        PlayerRec_00497f40* rec = &g_game->players[(unsigned char)i4];
                        if (rec->active != 0) {
                            unsigned char st = rec->team;
                            if ((st == 1 || st == 2 || st == 3) &&
                                rec->kind != 10)
                                SpawnCommanderAtStartPos(i4, order[k++]);
                        }
                    }
                }
            }
            InitPlayerResources();
        }
    }

    RecalculateLineOfSight(1);

    if (g_game->pendingSaveStore != 0) {
        g_game->pendingSaveStore->OpenAccount("summary");
        if (g_game->pendingSaveStore->HasItem("BetweenMissions") == 0) {
            LoadSavedGameState(g_game->pendingSaveStore);
            goto tail;
        }
    } else if (((Mission*)g_game->mapInfo)->GetGameType() != 1) {
        goto tail;
    }
    CreateMissionUnits();
    CenterCameraOnStartPosition();

tail:
    {
        int pnum = g_game->playerIndex;
        PlayerRec_00497f40* rec = &g_game->players[pnum];
        LoadPictureCached(0, 0, 0, 0);
        PlayerInfo* pl = rec->data;
        int side = pl->side;
        sprintf(g_game->mainHudLayoutName, "%sMAIN2.GUI", g_game->sideNames[side].name);
    }
    Gadget_497180* gadget =
        LoadGuiLayer((Sub_497180*)&g_game->menu, g_game->mainHudLayoutName, 0x20);
    gadget->handler = HandleMain2LayoutEvent;
    gadget->owner = (char*)g_game;

    PlayerRec_00497f40* currec = &g_game->players[g_game->localPlayer];
    currec->data->flags_9b |= 0x10;
    BroadcastPlayerInfo();
    UpdateNetGameInfo();
    UpdatePlayers();
    InitPlayerResources();

    HapiBank* mission = g_game->pendingSaveStore;
    if (mission != 0) {
        mission->CloseBank();
        operator delete(mission);
        g_game->pendingSaveStore = 0;
    }
    RebuildAIFeatureCells();

    g_game->flags38d75.value |= 2;
}
//
// Kept as written, each for the registers it gives:
// - Vec3::operator- is the explicit-component form the matched sibling 0x413d80
//   (same translation unit) uses. That makes the state 3 block byte exact, but
//   on its own it ties `range` and `order` at priority 130 (c2prio), and range
//   wins the tie on its +0x40 key, so order and range trade esi and edi.
// - `int ok = AddBuildProgress(...); if (ok)` adds a candidate to a block that
//   references order, which raises order to 134 and gives it esi again.
// - The operand order of the six bounds adds (pos.x + min.x and so on) follows
//   the symbol ids, so it moves with the headers and with code-neutral
//   spellings. What puts all six in place (found by the permuter): <memory.h>
//   plus <windows.h>, the state 0 test written as two nested ifs, and an empty
//   `do {} while (0);` in UnitRef::Get(), a debug check that compiles to
//   nothing. Without the do-while, or without the `ok` local, the registers
//   come out differently.
// Reading the record address through a named base (a temp_intro the permuter
// found) is what keeps g_game in the SIB base and the offset in the index.
static inline char* PlayerRecordAt(Game* game, int off) { return (char*)game->players + off; }

// FUNCTION: 0x497ce0
void __stdcall DrawSyncStatus(void* surface)
{
    int off;
    extern Game* g_game;

    RenderLayer(&g_game->menu, 0x40);
    MarkLayerChanged(&g_game->menu);
    BlitMenuLayers(&g_game->menu, 0, 0);

    const char* text;
    if (g_game->netBits.synced) {
        text = Translate("Synchronization complete");
    } else {
        int countA = 0;
        int countB = 0;
        for (int i = 0; i < 10; i++) {
            PlayerRec_00497ce0* p = &g_game->players[i];
            if (p->present && (p->team == 1 || p->team == 2 || p->team == 3) && p->kind != 10)
                countA++;
            if (p->present && (p->team == 1 || p->team == 2 || p->team == 3) && p->kind != 10 &&
                p->percent == 100 && g_game->loaded[i] != 0)
                countB++;
        }

        int x;
        int slot = 620 / countA;
        Rect_00497ce0 r;
        r.x1 = 10;
        r.y1 = 420;
        r.y2 = 435;
        off = 0;
        x = 11;
        for (; off < 10 * (int)sizeof(PlayerRec_00497ce0); off += sizeof(PlayerRec_00497ce0)) {
            Game* game = g_game;
            PlayerRec_00497ce0* q = (PlayerRec_00497ce0*)PlayerRecordAt(game, off);
            if (!(q->present && (q->team == 1 || q->team == 2 || q->team == 3) && q->kind != 10))
                continue;
            {
                r.x1 = x;
                r.x2 = slot + x - 2;
                FillRectangle(surface, &r, g_game->color1);
                int pc = q->percent;
                r.x2 = r.x1 + (pc * (slot - 2)) / 100;
                FillRectangle(surface, &r, g_game->color2);
                DrawTextClipped(surface, q->name, r.x1, 420, slot - 2, 0);
                x += slot;
            }
        }

        const char* pr;
        int ready = countB == 1;
        if (ready)
            pr = Translate("player ready");
        else
            pr = Translate("players ready");
        char buf[128];
        sprintf(buf, "%s.  %i %s", Translate("Waiting for other players"), countB, pr);
        text = buf;
    }

    DrawTextClipped(surface, text, 10, 400, -1, 0);
}
// Draws one loading bar: its label, then the filled rect and the lightbar frame.
// The flash alpha is passed as base plus slot: a pointer already offset by the
// caller changes the register choice.
static inline void DrawLoadingBar(void* gadget, void* lightbar, int index, unsigned char* prev, unsigned char* alphas, int slot, char* label, int y, int* rect)
{
    unsigned int color;
    int flash;
    color = g_game->palette[g_game->progress[index] < 100 ? 12 : 10];
    SetTextColors(color, GetTextKeyColor());
    if (g_game->progress[index] == 100 && *prev != 100) {
        alphas[slot] = 0x1e;
    }
    flash = alphas[slot];
    *prev = g_game->progress[index];
    DrawTextClipped(gadget, (char*)Translate(label), 0x5a, y, -1, flash);
    // Each bar's rect is written left, right, top, bottom.
    rect[0] = 0xcd;
    rect[2] = ((int)g_game->progress[index] * 7) / 2 + 0xcd;
    rect[1] = y;
    rect[3] = y + 0x14;
    FillRectangle(gadget, rect, color);
    DrawFrame(gadget, lightbar, rect[0], rect[1]);
}
// The loading-screen frame: on the first call it starts the loader thread
// (LoadThreadMain), once the loader sets the "loaded" bit it restores the game
// screen and installs the game frame handler (BattleFrame), and otherwise it
// draws the six progress bars.
// Unused here: the symbol ids these declarations take keep the allocation
// (docs/c2-regalloc.md).
extern int Pad_497f40_e0;
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
    int textWidth;
    unsigned int stamp;
    int rect[4];
    PlayerInfo_00497f40* pi;
    char namebuf[100];

    if (!g_game->flags38d75.bits.started) {
        while (g_game->field_531 != 0) {
            CloseTopScreen(&g_game->field_519);
        }
        DisableKeyCommands(&g_game->field_519);
        if (g_game->cursorMode != 0x14) {
            g_game->cursorMode = 0x14;
            SetCursorAnimation(&g_game->field_519, (void*)g_game->cursorHourglass);
        }
        SetFont(g_game->fontComix);
        SetPaletteColors(g_game->paletteRgba, 0, 0x100);
        if (g_game->mapInfo->GetGameType() != 2) {
            SaveSettings();
        }
        while (g_game->field_531 != 0) {
            CloseTopScreen(&g_game->field_519);
        }
        BlankScreen();
        g_game->screenWidth = 0x280;
        g_game->screenHeight = 0x1e0;
        if (GetScreenWidth() != 0x280 || GetScreenHeight() != 0x1e0) {
            GameFreeThunk((void*)g_game->offscreen);
            g_game->offscreen = 0;
            SetRestoreSurface(0);
            RestoreScreen();
            SetWindowPos(g_game->displayContext->hwnd, 0, 0, 0, 0x280, 0x1e0, 4);
            SetResolution(0x280, 0x1e0);
            g_game->offscreen = (int)AllocSurface("OFFSCREEN", g_game->screenWidth, g_game->screenHeight);
            SetRestoreSurface(g_game->offscreen);
            SetOffscreenSurface((void*)g_game->offscreen);
        }
        BuildDataPath(aux, "palettes", "guipal", "PAL");
        surfaceHandle = HAPI_LoadFile((unsigned int*)aux, 0);
        RemapPaletteToClosestIndices(&g_game->field_519, g_game->paletteRgba, surfaceHandle);
        GameFreeThunk(surfaceHandle);
        g_game->lastSimBudgetTick = GetTicks();
        g_game->simStepsPending = 0;
        g_game->gameTick = 0;
        g_game->speedHysteresis = 0;
        g_game->endGameCountdown = (short)0xffff;
        g_game->screenWidth = g_game->displayWidth;
        g_game->screenHeight = g_game->displayHeight;
        g_game->viewCullMinX = 0x80;
        g_game->viewCullMinY = 0x20;
        g_game->viewCullMaxX = g_game->screenWidth - 1;
        g_game->viewCullMaxY = g_game->screenHeight - 0x21;
        g_game->viewPixelWidth = g_game->viewCullMaxX - g_game->viewCullMinX + 1;
        g_game->viewPixelHeight = g_game->viewCullMaxY - g_game->viewCullMinY + 1;
        LoadPictureCached("loadgame2bg", 0, 0, 0);
        memset(&g_game->slots, 0, sizeof(g_game->slots));
        for (i = 0; i < 10; i++) {
            pi = &g_game->players[i];
            if (pi->active == 0 || (pi->control != 1 && pi->control != 2)) {
                g_game->slots.inGame[i] = 0;
            } else {
                g_game->slots.inGame[i] = 1;
            }
            g_game->slots.flag40[i] = (pi->active != 0 && (pi->data->flags_9b & 0x40) != 0) ? 1 : 0;
        }
        if (!StartThread(LoadThreadMain, 0, 0)) {
            FatalError("Unable to start the loading thread!");
        }
        // Four memsets (10, 10, 6, 6 bytes); the stage bytes get one 8-byte memset.
        memset(g_scorePanelKillFlash, 0, 10);
        memset(g_scorePanelLossFlash, 0, 10);
        memset(&g_loadingBarFlashAlpha, 0, 6);
        memset(&g_loadingBarPrevPercent, 0, 6);
        g_game->flags38d75.bits.started = 1;
        OnlineUnload();
    }
    if (g_game->flags38d75.bits.loaded) {
        StopAllSounds();
        BlankScreen();
        FreePictureCache();
        if (GetScreenWidth() != g_game->displayWidth || GetScreenHeight() != g_game->displayHeight) {
            GameFreeThunk((void*)g_game->offscreen);
            g_game->offscreen = 0;
            SetRestoreSurface(0);
            RestoreScreen();
            SetWindowPos(g_game->displayContext->hwnd, 0, 0, 0, g_game->displayWidth,
                         g_game->displayHeight, 4);
            SetResolution(g_game->displayWidth, g_game->displayHeight);
            g_game->offscreen = (int)AllocSurface("OFFSCREEN", g_game->screenWidth, g_game->screenHeight);
            SetRestoreSurface(g_game->offscreen);
        }
        DrawLightBars();
        MainLoopTick();
        ShowSoftwareCursor();
        g_game->frontendState = 6;
        g_game->handler = BattleFrame;
        SetCloseHandler(HandleBattleQuitPrompt, 0);
        g_game->field_589 = 0;
        memset((void*)g_game->progress, 0, 8);
        g_game->sound->SetTrackCategory(0);
        if (!g_game->sound->IsCdPlaying()) {
            g_game->sound->PlayNextTrack();
        }
        // Index players[i], not explicit offsets: keeps the SIB base and index order.
        for (i = 0; i < 10; i++) {
            // Store past rect: a byte local would be dead-store eliminated.
            if (g_game->players[i].active != 0)
                ((unsigned char*)rect)[19] = g_game->players[i].control;
        }
        return;
    }
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].control == 1 || g_game->players[i].control == 2)) {
            SendProbe(g_game->players[i].id, 0);
            if (g_usePacketManager != 0)
                (&g_packetManager)->SendAllQueued(1);
        }
    }
    HandleNetPackets();
    // Nested, not `b2 && AssignStartPositions()`: that folds to a test on the byte.
    if (g_game->flags38d75.bits.b2) {
        if (AssignStartPositions() != 0) {
            g_game->flags38d75.bits.b2 = 0;
            g_game->flags38d75.bits.b3 = 1;
            HAPINET_guaranteepackets(0);
        }
    }
    if (g_usePacketManager != 0) {
        (&g_packetManager)->SendAllQueued(1);
    }
    SetOffscreenSurface((void*)g_game->offscreen);
    // The result stays in a local: it gives the compare against a register.
    int ok = LockScreen(&gadget);
    if (ok != 0) {
        color = g_game->palette[15];
        stamp = GetTicks();
        if (g_loadingBarFlashDecayTick < (int)stamp) {
            g_loadingBarFlashDecayTick = GetTicks();
            for (i = 0; i < 6; i++) {
                if (((char*)&g_loadingBarFlashAlpha)[i] != 0) {
                    ((char*)&g_loadingBarFlashAlpha)[i] -= 2;
                }
            }
        }
        SetFont(g_game->fontComix);
        DrawSurface(&gadget, (void*)g_game->surface, 0, 0);
        if (g_game->mapInfo->GetGameType() != 1) {
            SetTextColors(color, 0xfe);
            // Local for the strncpy source: the call comes before the length push.
            char* name = g_game->mapInfo->GetMissionName();
            strncpy(namebuf, name, 100);
            namebuf[99] = 0;
            if (GetPreferredLanguage() != 0 && _strcmpi((const char*)GetPreferredLanguage(), "english") != 0) {
                _strlwr(namebuf);
            }
            wsprintfA(buf, "%s: %s", (char*)Translate("Map"), (char*)Translate(namebuf));
            textWidth = GetTextPixelWidth(buf);
            {
                int x = gadget.width / 2 - textWidth / 2;
                DrawTextClipped(&gadget, buf, x,
                             (int)((double)gadget.height - (double)GetFontHeight() * 1.5), -1, 0);
            }
        }
        {
            void* light = FindGafEntry(g_game->gaf, "LIGHTBAR");
            void* lightbar = GetGafFrame(light, 0);
            *((short*)lightbar + 3) = 0;
            *((short*)lightbar + 2) = 0;
            DrawLoadingBar(&gadget, lightbar, 0, &g_loadingBarPrevPercent, (unsigned char*)&g_loadingBarFlashAlpha, 0, "Textures", 0x87, rect);
            DrawLoadingBar(&gadget, lightbar, 1, &DAT_0051e821, (unsigned char*)&g_loadingBarFlashAlpha, 1, "Terrain", 0xb1, rect);
            DrawLoadingBar(&gadget, lightbar, 2, &DAT_0051e822, (unsigned char*)&g_loadingBarFlashAlpha, 2, "Units", 0xda, rect);
            DrawLoadingBar(&gadget, lightbar, 3, &DAT_0051e823, (unsigned char*)&g_loadingBarFlashAlpha, 3, "Animation", 0x106, rect);
            DrawLoadingBar(&gadget, lightbar, 4, &DAT_0051e824, (unsigned char*)&DAT_0051e6cc, 0, "3D Data", 0x130, rect);
            DrawFrame(&gadget, lightbar, rect[0] + *((short*)lightbar + 2),
                         rect[1] + *((short*)lightbar + 3));
            DrawLoadingBar(&gadget, lightbar, 5, &DAT_0051e825, (unsigned char*)&DAT_0051e6cc, 1, "Explosions", 0x15b, rect);
        }
        if (g_game->mapInfo->GetGameType() == 3) {
            DrawSyncStatus(&gadget);
            SendLoadProgress();
        }
        UnlockScreen(&gadget);
        FlipScreen();
    }
    SleepMilliseconds(200);
}
// Converts a point by the view origin at +0x2c/+0x30 (less a 0x80 by 0x20
// border) and passes it on to ClampWorldPosToTerrain.
// FUNCTION: 0x498cd0
void __stdcall OffsetWorldPosFromView(Point_00498cd0* p, View_00498cd0* view, int param_3)
{
    ClampWorldPosToTerrain((view->x - 0x80) + p->x, (view->y - 0x20) + p->y, param_3);
}
// Converts the screen position stored at +0x2c76 into a 16.16 world position
// (scaled from the view origin), with the height taken from the ground there.
#include <string.h>
// FUNCTION: 0x498d00
void __stdcall MinimapCursorToWorldPos(Pos_00498d00* out)
{
    Rect_00498d00 r = g_game->view;
    int dx = r.x - g_game->minimapGadgetX;
    int dy = r.y - g_game->minimapGadgetY;
    memset(out, 0, sizeof(Pos_00498d00));
    out->x.whole = g_game->mapPixelWidth * dx / g_game->minimapGadgetW;
    out->z.whole = g_game->mapPixelHeight * dy / g_game->minimapGadgetH;
    out->y.whole = GetGroundHeight(out);
}
// Turns a screen point into a world position. The caller copies the 24-byte
// view rectangle at g_game+0x2c76 onto its own stack and passes the address, so
// this function sees a snapshot of the view.
//
// If the point is inside the rect at +0x142bb and bit 3 of the flags byte is
// clear, the screen offset from the view origin is simply scaled by world size
// over screen size. Otherwise the point is clamped to the visible limit rect at
// +0x37e27 and measured from the world origin, which lets the camera sit still
// at the edge of the map. The three low flag bits record which of the two
// happened: bit 0 direct, bit 1 inside the limit rect, bit 2 both. Then the
// point is converted to a map position, the cell it landed in is stored, and
// that cell's feature id is stored with it.
//
// Layout notes, for whoever reads the neighbours of this code:
// - the view rectangle is 0x18 bytes (int x, int y, then 16 bytes this
//   function never looks at); only x and y are read.
// - +0x2caa is the world position, three 20.12 fixed-point values. The
//   conversion back to a cell is an unsigned `>> 20` into a pair of shorts at
//   +0x2c8e, written as one 4-byte copy, and the feature id of that cell goes
//   to +0x2cbc.
// - +0x2cc6 is one byte of flags reached through a bitfield union. Writing bit
//   0 hoists the byte into bl and stores it after the arithmetic, while
//   clearing bit 1 and setting bit 2 are straight-to-memory masks. Reading the
//   two bits back for bit 2 gives the `test cl, 3` the original does.
// - +0x37e27 is the visible limit rect (left, top, right, bottom). Its address
//   is taken once and kept in a register, because the same rect is both the
//   clamp bounds and the argument of the second PointInRect call.
// - +0x1422b and +0x1422f are the world size, +0x142e7..+0x142ed the view
//   origin and screen size as shorts, +0x1431f and +0x14323 the world origin
//   the clamped point is measured from.
// - the MIN(MAX()) nesting is not decoration: the original expands the max
//   twice, once for the comparison and once for the value it keeps.
// - `Pos v = *p` with only v.x and v.z read is what leaves the store of the
//   middle word in the frame; MSVC folds the other two into their consumers.
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// FUNCTION: 0x498da0
void __stdcall UpdateCursorWorldPos(View_00498da0* r)
{
    int mx, my;

    if (PointInRect(&g_game->minimapHitRect, r->x, r->y) && !(g_game->flags.value & 8)) {
        mx = (r->x - g_game->originX) * g_game->worldW / g_game->screenW;
        my = (r->y - g_game->originY) * g_game->worldH / g_game->screenH;
        g_game->flags.value |= 1;
        g_game->flags.value &= ~2;
    } else {
        Rect_00498da0* lim = &g_game->lim;
        mx = g_game->scrollX + MIN(MAX(r->x, lim->left), lim->right) - lim->left;
        my = g_game->scrollY + MIN(MAX(r->y, lim->top), lim->bottom) - lim->top;
        g_game->flags.value &= ~1;
        g_game->flags.bits.b1 = PointInRect(lim, r->x, r->y);
    }
    g_game->flags.bits.b2 = g_game->flags.bits.b0 || g_game->flags.bits.b1;
    ClampWorldPosToTerrain(mx, my, &g_game->pos);
    {
        Pos_00498da0* p = &g_game->pos;
        Point_00498da0 pt;
        Pos_00498da0 v = *p;
        pt.x = (short)(v.x >> 20);
        pt.y = (short)(v.z >> 20);
        g_game->point = pt;
    }
    g_game->cellFeature = GetCellFeature(GetMapCell(g_game->point.x, g_game->point.y));
}

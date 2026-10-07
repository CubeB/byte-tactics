// Decompiled by deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, Claude Fable 5.1,
// GPT-6 Astra, space-bunny-free, Claude Opus 5.5, GPT-6, claude-opus-5-5 and Opus.
// Names are provisional.
//
// One Game from the four views (the menu object at +0x519 and the byte and word
// forms of the network flags at +0x38d75 sit in anonymous unions, as do the
// player records, the loaded/slots block and the map rectangles), one Mission,
// one PacketManager and the module's classes.
//
// <memory.h> before <windows.h>: without it the x87 schedule of LoadingScreenFrame's
// map-name block and the two pointer parameters of FUN_00498cd0 land differently.
#include <memory.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <time.h>
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

struct PlayerFlags_497180 {             // player + 0x9b
    unsigned short low : 8;
    unsigned short b8 : 1;              // +0x9c bit 0
    unsigned short b9 : 1;              // +0x9c bit 1
    unsigned short b10 : 1;             // +0x9c bit 2
    unsigned short b11_12 : 2;
    unsigned short b13 : 1;
    unsigned short rest : 2;
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

struct PlFlags_497180 {
    unsigned short : 6;
    unsigned short b6 : 1;
    unsigned short : 9;
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

struct PlayerData_00497f40 {
    char unknown_0[0x9b];
    unsigned char flags;                // +0x9b
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
    char unknown_21[0x27 - 0x21];
    PlayerData_00497f40* data;          // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    union {
        unsigned char team;             // +0x73
        unsigned char control;
    };
    char unknown_74[0x146 - 0x74];
    unsigned char kind;                 // +0x146
    char unknown_147[0x14b - 0x147];
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

struct Cell_00498da0 {
    char unknown_0[8];
    unsigned short feature;             // +0x8
    unsigned char offsetY;              // +0xa
    unsigned char offsetX;              // +0xb
    char unknown_c;
};

// One view of the game state. The ranges two views name differently sit in
// anonymous unions, so each function keeps the names it matched with.
struct Game {
    char unknown_0[0xc];
    int field_c;                        // +0xc
    int field_10;                       // +0x10
    char unknown_10[0x519 - 0x14];
    union {                             // +0x519
        Menu_00497ce0 menu;
        struct {
            int field_519;
            int field_51d;
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
    int field_11eb;                     // +0x11eb
    char unknown_11ef[0x1b63 - 0x11ef];
    PlayerRec_00497f40 players[10];     // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    union {                             // +0x29a4
        PlayerSlots_00497f40 slots;
        int loaded[10];
    };
    char unknown_2a30[0x2c76 - 0x2a30];
    Rect_00498d00 view;                 // +0x2c76
    Point_00498da0 point;               // +0x2c8e
    char unknown_2c92[0x2caa - 0x2c92];
    Pos_00498da0 pos;                   // +0x2caa
    char unknown_2cb6[0x2cbc - 0x2cb6];
    unsigned short cellFeature;         // +0x2cbc
    unsigned char field_2cbe;           // +0x2cbe
    char unknown_2cbf[0x2cc6 - 0x2cbf];
    Flags_00498da0 flags;               // +0x2cc6
    char unknown_2cc7[0x1422b - 0x2cc7];
    union { int world_w; int worldW; };          // +0x1422b
    union { int world_h; int worldH; };          // +0x1422f
    char unknown_14233[0x142bb - 0x14233];
    Rect_00498da0 viewLimit;            // +0x142bb
    char unknown_142cb[0x142e7 - 0x142cb];
    union { short origin_x; short originX; };    // +0x142e7
    union { short origin_y; short originY; };    // +0x142e9
    union { short screen_w; short screenW; };    // +0x142eb
    union { short screen_h; short screenH; };    // +0x142ed
    char unknown_142ef[0x1431f - 0x142ef];
    int mapOriginX;                     // +0x1431f
    int mapOriginY;                     // +0x14323
    char unknown_14327[0x148cf - 0x14327];
    int field_148cf;                    // +0x148cf
    char unknown_148d3[0x37e1b - 0x148d3];
    int field_37e1b;                    // +0x37e1b
    int field_37e1f;                    // +0x37e1f
    int field_37e23;                    // +0x37e23
    union {                             // +0x37e27
        Rect_00498da0 lim;
        struct {
            int field_37e27;
            int field_37e2b;
            int field_37e2f;
            int field_37e33;
        };
    };
    int field_37e37;                    // +0x37e37
    int field_37e3b;                    // +0x37e3b
    char unknown_37e3f[0x37f1b - 0x37e3f];
    int field_37f1b;                    // +0x37f1b
    int field_37f1f;                    // +0x37f1f
    char unknown_37f23[0x38a37 - 0x37f23];
    unsigned int field_38a37;           // +0x38a37
    int field_38a3b;                    // +0x38a3b
    char pad_38a3f[0x38a47 - 0x38a3f];
    int field_38a47;                    // +0x38a47
    char pad_38a4b[0x38a4f - 0x38a4b];
    short field_38a4f;                  // +0x38a4f
    char unknown_38a51[0x38d6f - 0x38a51];
    volatile unsigned char progress[6]; // +0x38d6f
    union {                             // +0x38d75
        volatile LoadFlags_00497f40 flags38d75;
        struct {
            unsigned short : 3;
            unsigned short synced : 1;
            unsigned short : 12;
        } netBits;
    };
    char unknown_38d77[0x391e9 - 0x38d77];
    int field_391e9;                    // +0x391e9
    char pad_391ed[0x391f1 - 0x391ed];
    int field_391f1;                    // +0x391f1
    void (*field_391f5)();              // +0x391f5
    int field_391f9;                    // +0x391f9
    char unknown_391fd[0x39239 - 0x391fd];
    short field_39239;                  // +0x39239
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
extern int g_usePacketManager;
extern "C" unsigned char DAT_0051e820, DAT_0051e821, DAT_0051e822;
extern "C" unsigned char DAT_0051e823, DAT_0051e824, DAT_0051e825;
extern int DAT_005091cc;

static inline ViewFlags_497180* g_game_view() { return (ViewFlags_497180*)((char*)g_game + 0x14281); }

class Mission {
public:
    int FUN_00435100();
    char* FUN_00435c30();
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

class HapiBank {
public:
    void OpenAccount(const char* name);
    int HasItem(const char* name);
    void CloseBank();
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    char* GetStringItem(char*, char*);
    double GetDoubleItem(char*, double);
    int OpenBank(char*, char*, void*);
    int SaveBank(char*, char*, int, int);
};

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

extern PacketManager g_packetManager;

class Sound {
public:
    void SetTrackCategory(int);
    int IsCdPlaying();
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    int QueryDisc();
    void PlayLooping(int sample, int volume);
    void Set3DDistances(int minimum, int maximum);
};

void __stdcall SeedRandom(int x);
void __stdcall SleepMilliseconds(int x);
int __stdcall RandomInt(int x);
unsigned char __stdcall FindHostSlot();
void FUN_00431740();
void HandleNetPackets();
void __stdcall LoadPlayerControllers(void* mission);
void FUN_0047a760();
void FUN_004917d0();
void FUN_00465e30();
void __stdcall RecalculateLineOfSight(int x);
void __stdcall LoadSavedGameState(void* mission);
void CreateMissionUnits();
void FUN_0041d1f0();
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void BroadcastPlayerInfo();
void UpdateNetGameInfo();
void FUN_00464f80();
void __stdcall ReportGameEvent(int x);
void FUN_004649d0();
void __stdcall SetCameraPosition(int x, int y, int z);
unsigned short __stdcall FindUnitTypeId(const char* name);
void __stdcall FUN_00496ee0(int team, int startpos);
void __stdcall CreateUnit(unsigned char team, unsigned short id, FixedPos_497180 pos, int a,
    int b, int c);
Gadget_497180* __stdcall LoadGuiLayer(Sub_497180* sub, const char* name, int flags);
void __stdcall FUN_00494890(Gadget_497180* gadget);
void __cdecl operator delete(void* p);

void __stdcall RenderLayer(Menu_00497ce0* menu, int value);
void __stdcall FUN_0049fad0(Menu_00497ce0* menu);
void __stdcall FUN_004ab170(Menu_00497ce0* menu, int a, int b);
void __stdcall FillRectangle(void* surface, void* rect, int color);
void __stdcall FUN_004a50e0(void* surface, const char* text, int x, int y, int len, int flag);
char* __stdcall Translate(char* s);

void LoadThreadMain();
void BattleFrame();
void __cdecl FUN_004609a0(int);
void __cdecl BlankScreen();
void __cdecl FreePictureCache();
void __cdecl SaveSettings();
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
void __stdcall BuildDataPath(void*, char*, char*, char*);
void __stdcall FUN_00453320(unsigned int, int);
void __stdcall DrawSyncStatus(void*);
void __stdcall FUN_0049fa70(void*);
int __stdcall GetTextPixelWidth(char*);
void __stdcall CloseTopScreen(void*);
void __stdcall FUN_004ab400(void*, void*);
void __stdcall FUN_004ac7d0(void*, void*, void*);
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
int __cdecl FUN_004568c0();
unsigned int __cdecl GetTicks();
char* __cdecl FUN_0049f580();

int __stdcall FUN_00484b50(int x, int y, int param_3);
void __stdcall FUN_00484b50(int x, int y, Pos_00498da0* out);
int __stdcall GetGroundHeight(Pos_00498d00* pos);
int __stdcall PointInRect(Rect_00498da0* r, int x, int y);
Cell_00498da0* __stdcall GetMapCell(int x, int y);
unsigned short __stdcall GetCellFeature(Cell_00498da0* cell);


// Game start: seeds the random generators, loads the match settings for the
// current network mode (1 = skirmish defaults, 2 = multiplayer host block,
// 3 = joined game, from the host player's record), runs the between-missions
// summary, spawns each player's commander and opens the MAIN2 GUI.
//
// What took this from 82.8% to MATCH (issue 4408):
// - Cases 1 and 2 are inlined calls of FUN_00496e10 (matched on its own in
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
//   DAT_005091cc = 1, then the settings block; the store through g_game makes
//   MSVC reload g_game for the block, as the original does.
// - The commander spawn loop is `for (int i = 0; i < 10; i++)` indexing
//   0x14b-byte records; MSVC strength-reduces it to the byte offset in ebx and
//   that puts g_game in the SIB base. A hand-written `off += 0x14b` loop swaps
//   the base and index.
// - QueryPerformanceCounter's halves: the sum is written straight from the two
//   fields, `SeedRandom(perfCount.LowPart + perfCount.HighPart)`. Through named
//   locals the addition lands in the other register (99.7%).
// Earlier passes fixed: the network flags at +0x38d75 written as volatile
// (the field the guide names); the +0x9b bit-6 test as a 1-bit bitfield; the
// mission-count loop with `i++` before `def += 6`; the ten-player walks with
// `(unsigned char)i` so the multiply is not strength-reduced; the
// std::random_shuffle call; rec+0x149 as a 1-bit bitfield (direct `or byte`);
// `pos.x.i` before `pos.y.i = 0`.
// Inlined into cases 1 and 2 below (matched on its own in 0x496e10.cpp).
inline void __stdcall FUN_00496e10(Settings_00496e10* s)
{
    *(int*)((char*)g_game + 0x37ef6) = s->value;
    g_game_view()->bit2 = s->flag_c;
    g_game_view()->bit0 = s->flag_4;
    g_game_view()->bit1 = s->flag_8;
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
    *(int*)((char*)g_game + 0x38a47) = 0;

    switch (((Mission*)*(void**)((char*)g_game + 0x391e9))->FUN_00435100()) {
    case 1:
        DAT_005091cc = 0;
        FUN_00496e10((Settings_00496e10*)((char*)g_game + 0x39219));
        FUN_00431740();
        break;
    case 2:
        *(unsigned short*)((char*)g_game + 0x37ee6) = *(unsigned short*)((char*)g_game + 0x37eec);
        DAT_005091cc = 1;
        FUN_00496e10((Settings_00496e10*)((char*)*(void**)((char*)g_game + 0x29a0) + 0x108));
        break;
    case 3: {
        *(unsigned short*)((char*)g_game + 0x37ee6) = *(unsigned short*)((char*)g_game + 0x37eec);
        DAT_005091cc = 1;
        *(unsigned short*)((char*)g_game + 0x38a51) &= 0xfffe;

        int sel = FindHostSlot();
        unsigned char cur = *(unsigned char*)((char*)g_game + 0x2a42);
        if (*(unsigned char*)((char*)g_game + 0x1b63 + 0x14b * cur + 0x21) & 2) {
            do {
                char* p = *(char**)((char*)g_game + 0x1b63 + 0x14b * *(unsigned char*)((char*)g_game + 0x2a42) + 0x27);
                if (g_usePacketManager)
                    g_packetManager.SendAllQueued(1);
                HandleNetPackets();
                sel = FindHostSlot();
                SleepMilliseconds(0x32);
                if (sel == 10)
                    continue;
                if (*(unsigned char*)(p + 0x96) == 0xff)
                    continue;
                if (*(char*)(p + 0x8f) == 0)
                    continue;
                break;
            } while (1);
            SleepMilliseconds(0x32);
        }

        ((Mission*)*(void**)((char*)g_game + 0x391e9))
            ->LoadMissionByName(*(void**)((char*)g_game + 0x1b63 + 0x14b * sel + 0x27));
        if (FindHostSlot() == 10)
            break;

        int sel2 = FindHostSlot();
        char* p2 = *(char**)((char*)g_game + 0x1b63 + 0x14b * sel2 + 0x27);
        PlayerFlags_497180* pf = (PlayerFlags_497180*)(p2 + 0x9b);
        DAT_005091cc = pf->b13;
        *(int*)((char*)g_game + 0x37ef6) = pf->b11_12;
        g_game_view()->bit1 = pf->b9;
        g_game_view()->bit2 = pf->b10;
        g_game_view()->bit0 = pf->b8;
        *(unsigned short*)((char*)g_game + 0x37ee6) = *(unsigned short*)(p2 + 0xa5);
        break;
    }
    default:
        break;
    }

    if (*(void**)((char*)g_game + 0x38d6b) != 0) {
        ((HapiBank*)*(void**)((char*)g_game + 0x38d6b))->OpenAccount("summary");
        if (((HapiBank*)*(void**)((char*)g_game + 0x38d6b))->HasItem("BetweenMissions") ==
            0) {
            LoadPlayerControllers(*(void**)((char*)g_game + 0x38d6b));
            if (((Mission*)*(void**)((char*)g_game + 0x391e9))->FUN_00435100() == 2) {
                int count = 0;
                int* def = (int*)*(void**)((char*)g_game + 0x29a0);
                int i = 0;
                while (i < 10) {
                    if (*def == 1 || *def == 2)
                        count = i + 1;
                    i++;
                    def += 6;
                }
                int cur = *(int*)((char*)g_game + 0x38d81);
                if (count > cur)
                    cur = count;
                *(int*)((char*)g_game + 0x38d81) = cur;
                FUN_0047a760();
            }
        }
    }

    FUN_004917d0();

    if (((Mission*)*(void**)((char*)g_game + 0x391e9))->FUN_00435100() != 1) {
        if (((Mission*)*(void**)((char*)g_game + 0x391e9))->FUN_00435100() == 3) {
            *(volatile unsigned short*)((char*)g_game + 0x38d75) |= 4;
            while ((*(unsigned short*)((char*)g_game + 0x38d75) & 8) == 0)
                SleepMilliseconds(0x32);

            int sel = FindHostSlot();
            char* pl = *(char**)((char*)g_game + 0x1b63 + 0x14b * sel + 0x27);
            PlayerFlags_497180* pf = (PlayerFlags_497180*)(pl + 0x9b);
            g_game_view()->bit0 = pf->b8;
            g_game_view()->bit1 = pf->b9;
            g_game_view()->bit2 = pf->b10;
            *(int*)((char*)g_game + 0x37ef6) = pf->b11_12;

            for (int i = 0; i < 10; i++) {
                char* rec = (char*)g_game + 0x1b63 + 0x14b * i;
                if (*(int*)rec == 0)
                    continue;
                unsigned char st = *(unsigned char*)(rec + 0x73);
                if (st != 1 && st != 2)
                    continue;
                pos.x.i = (RandomInt(*(int*)((char*)g_game + 0x14223) - 0xa0) + 0x50) << 16;
                pos.y.i = 0;
                pos.z.i = (RandomInt(*(int*)((char*)g_game + 0x14227) - 0xa0) + 0x50) << 16;
                if (*(int*)rec != 0 &&
                    (*(unsigned char*)(*(char**)(rec + 0x27) + 0x9b) & 0x40))
                    continue;
                char* pl2 = *(char**)(rec + 0x27);
                int side = *(unsigned char*)(pl2 + 0x95);
                int which = *(unsigned char*)(rec + 0x147);
                ((Mission*)*(void**)((char*)g_game + 0x391e9))
                    ->GetStartPosition(&pos, which);
                if (*(int*)rec != 0 && *(unsigned char*)(rec + 0x73) == 1)
                    start = pos;
                unsigned short id =
                    FindUnitTypeId((char*)g_game + 0x37f5f + 0x232 * side);
                CreateUnit(*(unsigned char*)(rec + 0x146), id, pos, 1, 1, 0);
                int s1 = *(unsigned short*)(pl + 0xa1) * 100;
                int s2 = *(unsigned short*)(pl + 0xa3) * 100;
                ((RecFlag_497180*)(rec + 0x149))->started = 1;
                *(float*)(rec + 0xdc) = (float)(s1 >= 200 ? s1 : 200);
                *(float*)(rec + 0xe0) = (float)(s2 >= 200 ? s2 : 200);
            }

            unsigned char li = *(unsigned char*)((char*)g_game + 0x2a42);
            char* lp = *(char**)((char*)g_game + 0x1b63 + 0x14b * li + 0x27);
            int cx;
            int cz;
            if (((PlFlags_497180*)(lp + 0x9b))->b6) {
                *(unsigned short*)((char*)g_game + 0x14281) &= 0xfffe;
                *(unsigned short*)((char*)g_game + 0x14281) &= 0xfffd;
                cx = *(int*)((char*)g_game + 0x37e37) / 2;
                cz = *(int*)((char*)g_game + 0x37e3b) / 2;
            } else {
                cx = start.x.h.whole - *(int*)((char*)g_game + 0x37e37) / 2;
                cz = start.z.h.whole - *(int*)((char*)g_game + 0x37e3b) / 2;
            }
            SetCameraPosition(cx, cz, 0);
            ReportGameEvent(6);
        } else if (((Mission*)*(void**)((char*)g_game + 0x391e9))->FUN_00435100() == 2 &&
            *(void**)((char*)g_game + 0x38d6b) == 0) {
            if (*(int*)((char*)*(void**)((char*)g_game + 0x29a0) + 0x118) != 0) {
                for (int i1 = 0; i1 < 10; i1++) {
                    if ((unsigned char)i1 < 10) {
                        char* rec = (char*)g_game + 0x1b63 + 0x14b * (unsigned char)i1;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
                                FUN_00496ee0(i1, i1);
                        }
                    }
                }
            } else {
                for (int i2 = 0; i2 < 10; i2++)
                    order[i2] = -1;
                int n = 0;
                for (int i3 = 0; i3 < 10; i3++) {
                    if ((unsigned char)i3 < 10) {
                        char* rec = (char*)g_game + 0x1b63 + 0x14b * (unsigned char)i3;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
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
                        char* rec = (char*)g_game + 0x1b63 + 0x14b * (unsigned char)i4;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
                                FUN_00496ee0(i4, order[k++]);
                        }
                    }
                }
            }
            FUN_00465e30();
        }
    }

    RecalculateLineOfSight(1);

    if (*(void**)((char*)g_game + 0x38d6b) != 0) {
        ((HapiBank*)*(void**)((char*)g_game + 0x38d6b))->OpenAccount("summary");
        if (((HapiBank*)*(void**)((char*)g_game + 0x38d6b))->HasItem("BetweenMissions") ==
            0) {
            LoadSavedGameState(*(void**)((char*)g_game + 0x38d6b));
            goto tail;
        }
    } else if (((Mission*)*(void**)((char*)g_game + 0x391e9))->FUN_00435100() != 1) {
        goto tail;
    }
    CreateMissionUnits();
    FUN_0041d1f0();

tail:
    {
        int pnum = *(unsigned char*)((char*)g_game + 0x2a43);
        char* rec = (char*)g_game + 0x1b63 + 0x14b * pnum;
        LoadPictureCached(0, 0, 0, 0);
        char* pl = *(char**)(rec + 0x27);
        int side = *(unsigned char*)(pl + 0x95);
        sprintf((char*)g_game + 0x37ea0, "%sMAIN2.GUI", (char*)g_game + 0x37f5b + 0x232 * side);
    }
    Gadget_497180* gadget =
        LoadGuiLayer((Sub_497180*)((char*)g_game + 0x519), (char*)g_game + 0x37ea0, 0x20);
    gadget->handler = FUN_00494890;
    gadget->owner = (char*)g_game;

    char* currec = (char*)g_game + 0x1b63 + 0x14b * *(unsigned char*)((char*)g_game + 0x2a42);
    *(unsigned char*)(*(char**)(currec + 0x27) + 0x9b) |= 0x10;
    BroadcastPlayerInfo();
    UpdateNetGameInfo();
    FUN_00464f80();
    FUN_00465e30();

    void* mission = *(void**)((char*)g_game + 0x38d6b);
    if (mission != 0) {
        ((HapiBank*)mission)->CloseBank();
        operator delete(mission);
        *(void**)((char*)g_game + 0x38d6b) = 0;
    }
    FUN_004649d0();

    *(volatile unsigned short*)((char*)g_game + 0x38d75) |= 2;
}
//
// What made this match (91.3% before):
// - Vec3::operator- is the explicit-component form the matched sibling 0x413d80
//   (same translation unit) uses. That makes the state 3 block byte exact, but
//   on its own it ties `range` and `order` at priority 130 (c2prio), and range
//   wins the tie on its +0x40 key, so order and range trade esi and edi.
// - `int ok = AddBuildProgress(...); if (ok)` adds a candidate to a block that
//   references order, which raises order to 134 and gives it esi again (97.1%
//   with <stdlib.h>; only the six bounds adds were left).
// - The operand order of the six bounds adds (pos.x + min.x and so on) follows
//   the symbol ids, so it moves with the headers and with code-neutral
//   spellings. What puts all six in place (found by the permuter): <memory.h>
//   plus <windows.h>, the state 0 test written as two nested ifs, and an empty
//   `do {} while (0);` in UnitRef::Get(), a debug check that compiles to
//   nothing. Without the do-while no header set gets past 97.1%; without the
//   `ok` local the function drops to 73.3%.
// Reading the record address through a named base (a temp_intro the permuter
// found) is what keeps g_game in the SIB base and the offset in the index.
static inline char* PlayerRecordAt(Game* game, int off) { return (char*)game->players + off; }

// FUNCTION: 0x497ce0
void __stdcall DrawSyncStatus(void* surface)
{
    int off;
    extern Game* g_game;

    RenderLayer(&g_game->menu, 0x40);
    FUN_0049fad0(&g_game->menu);
    FUN_004ab170(&g_game->menu, 0, 0);

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
                FUN_004a50e0(surface, q->name, r.x1, 420, slot - 2, 0);
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

    FUN_004a50e0(surface, text, 10, 400, -1, 0);
}
//
// MATCH (claude-opus-5-5, #4267; was 84.3%). The loading-screen frame: on the
// first call it starts the loader thread (LoadThreadMain), once the loader sets
// the "loaded" bit it restores the game screen and installs the game frame
// handler (BattleFrame), and otherwise it draws the six progress bars.
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
        if (((Mission*)g_game->field_391e9)->FUN_00435100() != 2) {
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
        surfaceHandle = HAPI_LoadFile((unsigned int*)aux, 0);
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
        if (!StartThread(LoadThreadMain, 0, 0)) {
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
        g_game->field_391f5 = BattleFrame;
        SetCloseHandler(FUN_004609a0, 0);
        g_game->field_589 = 0;
        memset((void*)g_game->progress, 0, 8);
        ((Sound*)g_game->field_10)->SetTrackCategory(0);
        if (!((Sound*)g_game->field_10)->IsCdPlaying()) {
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
        if (((Mission*)g_game->field_391e9)->FUN_00435100() != 1) {
            SetTextColors(color, 0xfe);
            char* name = ((Mission*)g_game->field_391e9)->FUN_00435c30();
            strncpy(namebuf, name, 100);
            namebuf[99] = 0;
            if (FUN_0049f580() != 0 && _strcmpi((const char*)FUN_0049f580(), "english") != 0) {
                _strlwr(namebuf);
            }
            wsprintfA(buf, "%s: %s", (char*)Translate("Map"), (char*)Translate(namebuf));
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
            FUN_004a50e0(&gadget, (char*)Translate("Textures"), 0x5a, 0x87, -1, flash);
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
            FUN_004a50e0(&gadget, (char*)Translate("Terrain"), 0x5a, 0xb1, -1, flash);
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
            FUN_004a50e0(&gadget, (char*)Translate("Units"), 0x5a, 0xda, -1, flash);
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
            FUN_004a50e0(&gadget, (char*)Translate("Animation"), 0x5a, 0x106, -1, flash);
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
            FUN_004a50e0(&gadget, (char*)Translate("3D Data"), 0x5a, 0x130, -1, flash);
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
            FUN_004a50e0(&gadget, (char*)Translate("Explosions"), 0x5a, 0x15b, -1, flash);
            rect[0] = 0xcd;
            rect[2] = ((int)g_game->progress[5] * 7) / 2 + 0xcd;
            rect[1] = 0x15b;
            rect[3] = 0x16f;
            FillRectangle(&gadget, rect, color);
            DrawFrame(&gadget, lightbar, rect[0], rect[1]);
        }
        if (((Mission*)g_game->field_391e9)->FUN_00435100() == 3) {
            DrawSyncStatus(&gadget);
            SendLoadProgress();
        }
        UnlockScreen(&gadget);
        FlipScreen();
    }
    SleepMilliseconds(200);
}
// Converts a point by the view origin at +0x2c/+0x30 (less a 0x80 by 0x20
// border) and passes it on to FUN_00484b50.
// FUNCTION: 0x498cd0
void __stdcall FUN_00498cd0(Point_00498cd0* p, View_00498cd0* view, int param_3)
{
    FUN_00484b50((view->x - 0x80) + p->x, (view->y - 0x20) + p->y, param_3);
}
// Converts the screen position stored at +0x2c76 into a 16.16 world position
// (scaled from the view origin), with the height taken from the ground there.
#include <string.h>
// FUNCTION: 0x498d00
void __stdcall FUN_00498d00(Pos_00498d00* out)
{
    Rect_00498d00 r = g_game->view;
    int dx = r.x - g_game->origin_x;
    int dy = r.y - g_game->origin_y;
    memset(out, 0, sizeof(Pos_00498d00));
    out->x.whole = g_game->world_w * dx / g_game->screen_w;
    out->z.whole = g_game->world_h * dy / g_game->screen_h;
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
void __stdcall FUN_00498da0(View_00498da0* r)
{
    int mx, my;

    if (PointInRect(&g_game->viewLimit, r->x, r->y) && !(g_game->flags.value & 8)) {
        mx = (r->x - g_game->originX) * g_game->worldW / g_game->screenW;
        my = (r->y - g_game->originY) * g_game->worldH / g_game->screenH;
        g_game->flags.value |= 1;
        g_game->flags.value &= ~2;
    } else {
        Rect_00498da0* lim = &g_game->lim;
        mx = g_game->mapOriginX + MIN(MAX(r->x, lim->left), lim->right) - lim->left;
        my = g_game->mapOriginY + MIN(MAX(r->y, lim->top), lim->bottom) - lim->top;
        g_game->flags.value &= ~1;
        g_game->flags.bits.b1 = PointInRect(lim, r->x, r->y);
    }
    g_game->flags.bits.b2 = g_game->flags.bits.b0 || g_game->flags.bits.b1;
    FUN_00484b50(mx, my, &g_game->pos);
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

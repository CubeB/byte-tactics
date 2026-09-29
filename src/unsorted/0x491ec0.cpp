// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Load-game info panel: fills the GAMES menu fields from the selected save.
//
// PARTIAL 91.9% (1126 vs 1124 bytes) after 1 real check run, up from 86.4%.
// Control flow, struct layout, frame offsets and the whole call sequence match;
// every remaining difference is one of two root causes.
//
// What was fixed to get here (all worth points together):
//  * The prologue wants the layer pointer computed from `g_game` itself, not
//    from the `menu` local: `mov eax,[g_game]; lea ebx,[eax+0x519];
//    mov eax,[eax+0x531]`.  Writing `g_game->menu.layer` as its own
//    expression (a `layer` local) keeps `eax` live across the `lea`, which is
//    what produces that exact schedule.  `menu->layer->entries` instead gives
//    `mov ecx,[eax+0x531]` hoisted above `push ebx`.
//  * The failure block (the six `FUN_004a0bf0` calls plus `FUN_004a0880`) is
//    ONE block reached from four tests, so the `if (file != 0)` must have no
//    else arm: falling out of the outer then-block lands on the outer else.
//  * The "MISSION" assignment is duplicated in the `gametype == 1` and the
//    `else` arm.  MSVC tail merges them and emits the original's layout:
//    then-arm `jmp` to the merged block, else-arm falling into it.
//  * `path` must be 0x100 bytes so the frame is `sub esp, 0x144` (with the
//    4-byte `gametype` field in the same aggregate, the totals are 0x148).
//    `gametype` is the first member of the `Buf` struct, which puts it at
//    [esp+0x10], name at [esp+0x14] and diffs at [esp+0x48] as the original has.
//  * `FUN_0049fa90(&g_game->menu)` at the end, not the `menu` local: the
//    original rematerialises `mov edx,[g_game]; add edx,0x519`.
//
// Still differing, both from single upstream causes:
//  1. Register roles for `games`/`entries`/`index`.  Original: menu=ebx,
//     games=ebp, entries=esi (then index coalesced into esi), edi saved late
//     for the inlined strcpy.  Ours: menu=ebx, games=esi, entries=edi,
//     index=ebp, so the prologue push order, the `mov ax,[ebp+0xba]` /
//     `push esi` in the failure block and the epilogue pop order all follow.
//     Three attempts at promoting `entries` over `games` (use `entries` for
//     the RADAR lookup, 83.4%; keep the `layer` local alive and use
//     `layer->entries`, 82.8%) all made it worse.
//  2. The second read of `gametype`: original `cmp dword ptr [esp+0x14],1`
//     straight out of the frame after the `push 0`, ours `mov eax,[esp+0x10]`
//     then `cmp eax,1`.  That one extra `mov` is exactly the 2-byte size
//     difference.  Moving `gametype` out of the struct into a plain local
//     changes nothing, so it is the load/compare fold, not the storage.
#include <string.h>
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_00491ec0 {
    char unknown_0[0xba];
    short field_ba;                    // +0xba selected game index
    char unknown_bc[0xc2 - 0xbc];
    char* field_c2;                    // +0xc2 text/gaf pointer
    char unknown_c6[0x15b - 0xc6];
};

struct Layer_00491ec0 {
    int unknown_0;
    Entry_00491ec0* entries;           // +0x04
};

struct Menu_00491ec0 {
    char unknown_0[0x18];
    Layer_00491ec0* layer;             // +0x18
};

struct Game_00491ec0 {
    char unknown_0[0x519];
    Menu_00491ec0 menu;                // +0x519
};
#pragma pack(pop)

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, char* def);
};

class Class_004b48a0 {
public:
    char* FUN_004b48a0(char* name, char* def);
};

class Class_004b4ba0 {
public:
    void FUN_004b4ba0(char* name);
};

extern Game_00491ec0* g_game;
extern char* DAT_005091c8;
extern char* DAT_0051f2e0;
extern char* DAT_0051f2e4;
extern char* DAT_0051f2e8;
extern char* DAT_0051f2ec;
extern char DAT_005119b8[];
extern char DAT_0051e6f8[];

void __stdcall FUN_0049fa90(Menu_00491ec0* menu);
Entry_00491ec0* __stdcall FUN_0049ff90(Entry_00491ec0* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_00491ec0* entries, char* name, int type);
void __stdcall FUN_004a0880(Menu_00491ec0* menu, int index, char* text);
Entry_00491ec0* __stdcall FUN_004a0280(Entry_00491ec0* entries, char* name);
void __stdcall FUN_004a0570(Menu_00491ec0* menu, char* name, int value);
void __stdcall FUN_004a0bf0(Menu_00491ec0* menu, char* name, char* text, int param_4);
char* __stdcall FUN_004b6af0(char* table, int index);
void __stdcall FUN_004b8ae0(void* dst, void* src);
void __stdcall FUN_004c6ac0(void* image);
void* __stdcall FUN_004c6f80(Class_004b48a0* obj);
Class_004b48a0* __stdcall FUN_00432520(char* name);
void __stdcall FUN_00432590(Class_004b48a0* obj);

// FUNCTION: 0x491ec0
void FUN_00491ec0()
{
    Menu_00491ec0* menu = &g_game->menu;
    Layer_00491ec0* layer = g_game->menu.layer;
    Entry_00491ec0* entries = layer->entries;
    Entry_00491ec0* games = FUN_0049ff90(entries, "GAMES");
    if (games == 0)
        return;

    int index = FUN_0049fdf0(entries, "GAMENAME", 3);
    char path[0x100];
    struct Buf { int gametype; char name[0x34]; char* diffs[3]; } b;
#define gametype b.gametype
#define name b.name
#define diffs b.diffs

    char* desc;
    if (games->field_ba > -1
        && (desc = FUN_004b6af0(DAT_0051f2e4, games->field_ba)) != 0
        && strlen(desc) != 0) {
        FUN_004a0880(menu, index, desc);
        char* fname = FUN_004b6af0(DAT_0051f2e0, games->field_ba);
        sprintf(path, "%s\\%s", DAT_005091c8, fname);
        Class_004b48a0* file = FUN_00432520(path);
        if (file != 0) {
            ((Class_004b4ba0*)file)->FUN_004b4ba0("Radar Image");
            Entry_00491ec0* radar = FUN_004a0280(menu->layer->entries, "RADAR");
            if (DAT_0051f2ec != 0)
                FUN_004c6ac0(DAT_0051f2ec);
            DAT_0051f2ec = (char*)FUN_004c6f80(file);
            if (DAT_0051f2ec != 0) {
                FUN_004b8ae0(DAT_0051e6f8, DAT_0051f2ec);
                radar->field_c2 = DAT_0051e6f8;
            }
            FUN_004a0570(menu, "RADAR", DAT_0051f2ec != 0);

            int players = ((Class_004b4800*)file)->FUN_004b4800("Players", 0);
            gametype = ((Class_004b4800*)file)->FUN_004b4800("Gametype", 0);
            if (players != 0) {
                if (gametype == 1)
                    strcpy(name, "Single");
                else
                    sprintf(name, "Skirmish (%d players)", players);
            } else {
                strcpy(name, "???");
            }
            FUN_004a0bf0(menu, "GAMETYPE", name, 0);

            if (gametype == 1) {
                char* campaign = ((Class_004b48a0*)file)->FUN_004b48a0("Campaign", 0);
                if (campaign != 0) {
                    strcpy(name, campaign);
                    FUN_004a0bf0(menu, "CAMPAIGN", name, 0);
                    FUN_004a0570(menu, "CAMPTEXT", 1);
                    FUN_004a0570(menu, "CAMPAIGN", 1);
                }
                char* mission = ((Class_004b48a0*)file)->FUN_004b48a0("Mission", 0);
                if (mission != 0) {
                    strcpy(name, mission);
                    FUN_004a0bf0(menu, "MISSION", name, 0);
                }
            } else {
                FUN_004a0570(menu, "CAMPTEXT", 0);
                FUN_004a0570(menu, "CAMPAIGN", 0);
                char* mission = ((Class_004b48a0*)file)->FUN_004b48a0("Map", 0);
                if (mission != 0) {
                    strcpy(name, mission);
                    FUN_004a0bf0(menu, "MISSION", name, 0);
                }
            }

            int time = ((Class_004b4800*)file)->FUN_004b4800("Game Time", 0);
            sprintf(name, "%02d:%02d:%02d", time / 108000, time / 1800 % 60,
                    time / 30 % 60);
            FUN_004a0bf0(menu, "TIME", name, 0);

            if (DAT_0051f2e8 != 0) {
                int side = ((Class_004b4800*)file)->FUN_004b4800("Side", 0);
                strcpy(name, FUN_004b6af0(DAT_0051f2e8, side));
            } else {
                strcpy(name, "???");
            }
            FUN_004a0bf0(menu, "SIDE", name, 0);

            diffs[0] = "Easy";
            diffs[1] = "Medium";
            diffs[2] = "Hard";
            sprintf(name, "%s",
                    diffs[((Class_004b4800*)file)->FUN_004b4800("Difficulty", 0)]);
            FUN_004a0bf0(menu, "DIFF", name, 0);
            FUN_00432590(file);
        }
    } else {
        FUN_004a0880(menu, index, DAT_005119b8);
        FUN_004a0bf0(menu, "SIDE", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "DIFF", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "MISSION", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "CAMPAIGN", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "GAMETYPE", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "TIME", DAT_005119b8, 0);
    }
    FUN_0049fa90(&g_game->menu);
#undef gametype
#undef name
#undef diffs
}

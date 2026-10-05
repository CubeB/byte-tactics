// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Started by deepseek-v4.1-flash, space-bunny-free and GPT-6; kept as they left it.
// Partial: 96.0%, 1126 bytes versus 1124 (one extra instruction).
// The only difference is the second gametype test at 0x492078: the original
// emits `push 0 / cmp dword ptr [esp+0x14], 1 / jne` (direct memory compare)
// while ours emits `mov eax, dword ptr [esp+0x10] / push 0 / cmp eax, 1 /
// jne`, 2 bytes longer. Every other instruction is identical and only the
// jump targets shift by 2, so the frame is right (0x144, 81 locals) and the
// locals sit at the original slots (gametype 0x10, name 0x14, diffs 0x48,
// path 0x54).
// Tried and rejected, all 96.0 percent or worse: plain locals instead of the
// Buf struct (90.8, name buffer drifts to esp+0x20), gametype split out of
// the struct in two declaration orders, an int/union alias for the second
// read, Buf& / Buf* indirection, an int temp or a bool temp for the second
// test, `1 == gametype`, an unsigned cast, (int) cast of the field address,
// `!= 1` with the arms swapped, and inlining the single-use fname temp. A
// switch for the second test costs 4 bytes (`mov eax,[..] / dec eax / je`).
// A minimal repro with the same flags shows MSVC5 always reloads a
// call-result local into a register at its second use after intervening
// calls, using the memory form only at a later use in another block, so this
// looks like a function-wide allocator decision, not a local source lever.
// deepseek-v4.1-flash re-checked every local lever by compiling each variant to
// /Fa and reading the selector output: a separate `int gametype`, `char name`
// and `char* diffs[3]`, a struct field read as `b.gametype`, an initialized
// copy, a temp before the FUN_004a0bf0 call, a pointer local, nested blocks,
// reordered first-test arms, reordered struct fields, a switch, and every pure
// `== 1` spelling (`1 ==`, `== 1U`, `== 0x1`, `- 1 == 0`, `!(gametype - 1)`,
// `== 1 ? 1 : 0`) all emit `mov eax,[esp+0x10] / push 0 / cmp eax,1`. The
// direct memory compare appears only when the condition has a SECOND unproven
// operand (`gametype == 1 && players`, or `&& menu`), as
// `cmp [esp+0x14],1 / jne / test ..,.. / je`, which adds a test the original
// does not have. So the memory-versus-register choice is upstream allocation,
// not the condition expression, and `&&` arms cannot be folded away.

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

struct Game {
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

extern Game* g_game;
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
void __stdcall FrameFromSurface(void* dst, void* src);
void __stdcall FreeSurface(void* image);
void* __stdcall LoadSurface(Class_004b48a0* obj);
Class_004b48a0* __stdcall FUN_00432520(char* name);
void __stdcall FUN_00432590(Class_004b48a0* obj);

// FUNCTION: 0x491ec0
void __stdcall FUN_00491ec0()
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
                FreeSurface(DAT_0051f2ec);
            DAT_0051f2ec = (char*)LoadSurface(file);
            if (DAT_0051f2ec != 0) {
                FrameFromSurface(DAT_0051e6f8, DAT_0051f2ec);
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
            goto done;
        }
    }
    {
        FUN_004a0880(menu, index, DAT_005119b8);
        FUN_004a0bf0(menu, "SIDE", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "DIFF", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "MISSION", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "CAMPAIGN", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "GAMETYPE", DAT_005119b8, 0);
        FUN_004a0bf0(menu, "TIME", DAT_005119b8, 0);
    }
done:
    FUN_0049fa90(&g_game->menu);
#undef gametype
#undef name
#undef diffs
}

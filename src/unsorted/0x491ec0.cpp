// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Load-game info panel: fills the GAMES menu fields from the selected save.
// Partial 86.4% (1092 vs 1124 bytes). What still differs, all register roles
// and schedule, not structure:
//  - the prologue hoists `mov ecx,[eax+0x531]` before push ebx; the original
//    keeps it after `lea ebx,[eax+0x519]` and loads entries after push "GAMES".
//    Inlining menu->layer->entries at both call sites does not fix it (84.8%).
//  - FUN_004b6af0/sprintf args: original wants edx=table ecx=index, ours eax/edx.
//  - `setne cl` vs our `setne dl` for the RADAR boolean argument.
//  - the mission/Map arm: original reloads and re-tests with a separate
//    `mov ecx,ebp; call; test eax,eax; je; jmp` chain, ours folds it.
// Frame layout is now correct: gametype [esp+0x10], name [esp+0x14],
// diffs[3] [esp+0x48], path [esp+0x54]. That needed name and diffs wrapped in
// one local struct (separate declarations put diffs at 0x14 and name at 0x20).
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
    Entry_00491ec0* entries = menu->layer->entries;
    Entry_00491ec0* games = FUN_0049ff90(entries, "GAMES");
    if (games == 0)
        return;

    int index = FUN_0049fdf0(entries, "GAMENAME", 3);
    char path[0x100];
    int gametype;
    char* desc;
    struct Buf { char name[0x34]; char* diffs[3]; } b;
#define name b.name
#define diffs b.diffs

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

            char* mission;
            if (gametype == 1) {
                char* campaign = ((Class_004b48a0*)file)->FUN_004b48a0("Campaign", 0);
                if (campaign != 0) {
                    strcpy(name, campaign);
                    FUN_004a0bf0(menu, "CAMPAIGN", name, 0);
                    FUN_004a0570(menu, "CAMPTEXT", 1);
                    FUN_004a0570(menu, "CAMPAIGN", 1);
                }
                mission = ((Class_004b48a0*)file)->FUN_004b48a0("Mission", 0);
            } else {
                FUN_004a0570(menu, "CAMPTEXT", 0);
                FUN_004a0570(menu, "CAMPAIGN", 0);
                mission = ((Class_004b48a0*)file)->FUN_004b48a0("Map", 0);
            }
            if (mission != 0) {
                strcpy(name, mission);
                FUN_004a0bf0(menu, "MISSION", name, 0);
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

            sprintf(name, "%s",
                    diffs[((Class_004b4800*)file)->FUN_004b4800("Difficulty", 0)]);
            FUN_004a0bf0(menu, "DIFF", name, 0);
            FUN_00432590(file);
        } else {
            FUN_004a0880(menu, index, DAT_005119b8);
            FUN_004a0bf0(menu, "SIDE", DAT_005119b8, 0);
            FUN_004a0bf0(menu, "DIFF", DAT_005119b8, 0);
            FUN_004a0bf0(menu, "MISSION", DAT_005119b8, 0);
            FUN_004a0bf0(menu, "CAMPAIGN", DAT_005119b8, 0);
            FUN_004a0bf0(menu, "GAMETYPE", DAT_005119b8, 0);
            FUN_004a0bf0(menu, "TIME", DAT_005119b8, 0);
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
    FUN_0049fa90(menu);
#undef name
#undef diffs
}

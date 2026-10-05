// Decompiled by deepseek-v4.1, edited by deepseek-v4.1 and GPT-6.1-sol, deepseek-v4.1-flash, mimo-v2.6-pro and Space Bunny Free, finished by opus. Names are provisional.
// MATCH (opus pass). The last 12 points came from the shape of the command
// chain after "PrevMenu": it is one if / else-if chain whose arms fall through
// to a single FUN_004ab0a0(menu) at the end of the function. MSVC duplicates
// that small tail block (call plus epilogue) into the arms, and because the
// block was built at the join point, every copy reloads menu from its stack
// slot ("mov edx,[esp+0x84]"). With a "FUN_004ab0a0(menu); return;" in every
// arm instead, the arms reuse whatever register already held menu (the
// Energy and Metal arms kept it in ebp for the whole arm), which left the
// file 71 bytes short at 88.2%.
// Earlier findings that still hold: the Energy/Metal clamps are windef.h's
// min()/max() on the pointee (the down clamp written "*p + -0x1f4"); the c2/c1
// player counts are nested loop1 / test-c2 / loop2 / test-c1 with one shared
// error stub; the LineOfSight "field_114 = 1" stores precede the strcpy; the
// Difficulty arm calls PlaySoundByName("SKirmish", 0) with the original's typo'd
// literal (0x502a6c), not "Skirmish".
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Entry_0047ae60 {                // GUI entry, 0x15b bytes
    char unknown_0[0x33];
    char text[0xb6 - 0x33];            // +0x33
    short field_b6;                    // +0xb6
    char unknown_b8[0xbe - 0xb8];
    void* field_be;                    // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;           // +0xc6
    char unknown_c8[0x137 - 0xc8];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Player_0047ae60 {               // 0x18 bytes
    int active;                        // +0x00
    int shade;                         // +0x04
    int type;                          // +0x08
    int metal;                         // +0x0c
    int energy;                        // +0x10
    short color;                       // +0x14
    char unknown_16[2];
};

struct Table_0047ae60 {
    Player_0047ae60 players[11];       // +0x00 .. +0x108
    int field_108;                     // +0x108
    int field_10c;                     // +0x10c
    int field_110;                     // +0x110
    int field_114;                     // +0x114
    int field_118;                     // +0x118
    char mapName[0x220 - 0x11c];       // +0x11c
    int field_220;                     // +0x220
    int field_224;                     // +0x224
    int field_228;                     // +0x228
};

struct Holder_0047ae60 {
    int unknown_0;
    Entry_0047ae60* entries;           // +0x04
    char unknown_8[0x37 - 8];
    int field_37;                      // +0x37
};

struct Menu_0047ae60 {
    char unknown_0[0x18];
    Holder_0047ae60* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

class Class_00435a20 { public: int LoadMissionByName(char* name); };
class Class_00437300 { public: int CountStartPositions(); };

struct Frame_0047ae60 {
    char sA[0xc];
    char sB[0xc];
    int ev[6];
    char bf[0x40];
};
#pragma pack(pop)

extern char* g_game;                   // 0x511de8

void __stdcall GetGadgetName(Entry_0047ae60* entries, char* text, int id);
int __stdcall FindGadgetIndex(Entry_0047ae60* entries, char* name, int flag);
int __stdcall IsCurrentGadgetNamed(Menu_0047ae60* menu, char* name);
void __stdcall PlaySoundByName(char* name, int value);
char __stdcall FindGameCdDrive(int param_1);
void RegisterDataArchives();
void FUN_0041da30();
void SaveSettings();
void RefreshAllyIcons();
int AreAllPlayersInOneAllyGroup();
void __stdcall CycleSlotController(int param_1);
void FUN_0047a760();
void OpenSkirmishMapSelector();
void __stdcall CyclePlayerColor(int param_1);
void __stdcall FUN_00491c80(int param_1);
void __stdcall UpdateHelpText(void* param_1);
void __stdcall FUN_004a0bf0(Menu_0047ae60* menu, char* key, char* value, int flag);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall OpenMessageBox(void* menu, char* text, int width, int a, int b);
void __stdcall FUN_004c2340(int* out);
char* __stdcall Translate(char* text);

// FUNCTION: 0x47ae60
void __stdcall HandleSkirmishClick(Menu_0047ae60* menu)
{
    Frame_0047ae60 frame;

    Entry_0047ae60* entries = menu->holder->entries;
    int cmd = menu->field_60;
    if (cmd == -1) {
        return;
    }
    GetGadgetName(entries, frame.bf, cmd);

    int player = atoi(&frame.bf[strlen(frame.bf) - 1]);
    Table_0047ae60* table = *(Table_0047ae60**)(g_game + 0x29a0);
    table->field_224 = player;
    frame.bf[strlen(frame.bf) - 1] = 0;

    if (IsCurrentGadgetNamed(menu, "Start")) {
        PlaySoundByName("BigButton", 0);
        if (!FindGameCdDrive(1)) {
            OpenMessageBox(g_game + 0x519,
                         Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                         0xc8, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
        }
        RegisterDataArchives();

        int n = 0;
        int count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
            do {
                if (p->active == 2)
                    n++;
                p++;
            } while (--count);
        }
        *(short*)(g_game + 0x2a3c) = n + 1;

        if ((*(Class_00435a20**)(g_game + 0x391e9))->LoadMissionByName((*(Table_0047ae60**)(g_game + 0x29a0))->mapName) == 0) {
            OpenMessageBox(g_game + 0x519,
                         Translate("The terrain for the selected map does not exist."),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        int n2 = *(int*)(g_game + 0x38d81);
        int c2 = 0;
        if (n2 > 0) {
            Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
            for (int i = n2; i > 0; i--) {
                if (p->active == 2)
                    c2++;
                p++;
            }
        }
        if (c2 >= 1) {
            int c1 = 0;
            if (n2 > 0) {
                Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
                for (; n2 > 0; n2--) {
                    if (p->active == 1)
                        c1++;
                    p++;
                }
            }
            if (c1 >= 1) {
                int maxPlayers = (*(Class_00437300**)(g_game + 0x391e9))->CountStartPositions();
                if ((int)(unsigned short)*(short*)(g_game + 0x2a3c) > maxPlayers) {
                    OpenMessageBox(g_game + 0x519,
                                 Translate("There are too many players enabled for this map"),
                                 0x1e0, 1, 1);
                    FUN_004ab0a0(menu);
                    return;
                }

                if (AreAllPlayersInOneAllyGroup() != 0) {
                    OpenMessageBox(g_game + 0x519,
                                 Translate("All players may not be in the same allied group."),
                                 0x1e0, 1, 1);
                    FUN_004ab0a0(menu);
                    return;
                }

                int n3 = *(int*)(g_game + 0x38d81);
                c2 = 0;
                if (n3 > 0) {
                    Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
                    for (int i = n3; i > 0; i--) {
                        if (p->active == 2)
                            c2++;
                        p++;
                    }
                }
                c1 = 0;
                if (n3 > 0) {
                    Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
                    for (; n3 > 0; n3--) {
                        if (p->active == 1)
                            c1++;
                        p++;
                    }
                }
                *(short*)(g_game + 0x2a3c) = c1 + c2;

                FUN_0047a760();
                FUN_0041da30();
                SaveSettings();
                *(char*)(g_game + 0x2bc0) = 2;
                FUN_00491c80(0x14);
                return;
            }
        }
        OpenMessageBox(g_game + 0x519,
                     Translate("There must be at least one player and one computer opponent"),
                     0x1e0, 1, 1);
        FUN_004ab0a0(menu);
        return;
    }

    if (IsCurrentGadgetNamed(menu, "PrevMenu")) {
        PlaySoundByName("Previous", 0);
        FUN_00491c80(0x14);
        *(char*)(g_game + 0x2bc0) = 3;
        return;
    }


    if (strcmp(frame.bf, "Player") == 0) {
        PlaySoundByName("Skirmish", 0);
        CycleSlotController(player);
    } else if (strcmp(frame.bf, "Side") == 0) {
        PlaySoundByName("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        int* p = (int*)((char*)t + t->field_224 * 24 + 4);
        *p = (*p + 1) % *(int*)(g_game + 0x37f39);
    } else if (strcmp(frame.bf, "Allies") == 0) {
        PlaySoundByName("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        int* p = (int*)((char*)t + t->field_224 * 24 + 8);
        *p = (*p + 1) % 6;
        RefreshAllyIcons();
    } else if (strcmp(frame.bf, "Color") == 0) {
        PlaySoundByName("Skirmish", 0);
        FUN_004c2340(frame.ev);
        if (menu->holder->field_37 == 1) {
            CyclePlayerColor(0);
        }
        if (menu->holder->field_37 == 2) {
            CyclePlayerColor(1);
        }
    } else if (strcmp(frame.bf, "Energy") == 0) {
        FUN_004c2340(frame.ev);
        if (menu->holder->field_37 == 1) {
            PlaySoundByName("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0x10);
            *p = min(*p + 0x1f4, 0x2710);
            Table_0047ae60* t2 = *(Table_0047ae60**)(g_game + 0x29a0);
            int* q = (int*)((char*)t2 + player * 24 + 0x10);
            if (*q == 0x2bc)
                *q = 0x1f4;
            wsprintfA(frame.sB, "Energy%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].energy, frame.sA, 10);
            FUN_004a0bf0(menu, frame.sB, frame.sA, 10);
        }
        if (menu->holder->field_37 == 2) {
            PlaySoundByName("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0x10);
            *p = max(*p + -0x1f4, 0xc8);
            wsprintfA(frame.sB, "Energy%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].energy, frame.sA, 10);
            FUN_004a0bf0(menu, frame.sB, frame.sA, 10);
        }
    } else if (strcmp(frame.bf, "Metal") == 0) {
        if (menu->holder->field_37 == 1) {
            PlaySoundByName("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0xc);
            *p = min(*p + 0x1f4, 0x2710);
            Table_0047ae60* t2 = *(Table_0047ae60**)(g_game + 0x29a0);
            int* q = (int*)((char*)t2 + player * 24 + 0xc);
            if (*q == 0x2bc)
                *q = 0x1f4;
            wsprintfA(frame.sA, "Metal%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].metal, frame.sB, 10);
            FUN_004a0bf0(menu, frame.sA, frame.sB, 10);
        }
        if (menu->holder->field_37 == 2) {
            PlaySoundByName("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0xc);
            *p = max(*p + -0x1f4, 0xc8);
            wsprintfA(frame.sA, "Metal%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].metal, frame.sB, 10);
            FUN_004a0bf0(menu, frame.sA, frame.sB, 10);
        }
    } else if (IsCurrentGadgetNamed(menu, "CommanderDeath")) {
        PlaySoundByName("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        t->field_108 ^= 1;
        int index = FindGadgetIndex(entries, "CommanderDeath", 1);
        Entry_0047ae60* e = &entries[index];
        if ((*(Table_0047ae60**)(g_game + 0x29a0))->field_108 != 0)
            strcpy(e->text, Translate("Game ends when commander is destroyed."));
        else
            strcpy(e->text, Translate("Game continues after Commander is destroyed."));
        UpdateHelpText(g_game + 0x519);
    } else if (IsCurrentGadgetNamed(menu, "StartLocation")) {
        PlaySoundByName("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        t->field_118 ^= 1;
        int index = FindGadgetIndex(entries, "StartLocation", 1);
        Entry_0047ae60* e = &entries[index];
        if ((*(Table_0047ae60**)(g_game + 0x29a0))->field_118 != 0)
            strcpy(e->text, Translate("Commanders are placed at pre-determined locations."));
        else
            strcpy(e->text, Translate("Commanders are randomly placed on the battle field."));
        UpdateHelpText(g_game + 0x519);
    } else if (IsCurrentGadgetNamed(menu, "Mapping")) {
        PlaySoundByName("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        t->field_10c ^= 1;
        int index = FindGadgetIndex(entries, "Mapping", 1);
        Entry_0047ae60* e = &entries[index];
        if ((*(Table_0047ae60**)(g_game + 0x29a0))->field_10c != 0)
            strcpy(e->text, Translate("Terrain is blacked out until explored."));
        else
            strcpy(e->text, Translate("Terrain is visible."));
        UpdateHelpText(g_game + 0x519);
    } else if (IsCurrentGadgetNamed(menu, "LineOfSight")) {
        PlaySoundByName("Skirmish", 0);
        int index = FindGadgetIndex(entries, "LineOfSight", 1);
        Entry_0047ae60* e = &entries[index];
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        if (t->field_110 == 0) {
            t->field_110 = 1;
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_114 = 1;
            strcpy(e->text, Translate("Terrain elevations affect a unit's view."));
        } else if (t->field_114 == 1) {
            t->field_114 = 0;
            strcpy(e->text, Translate("Terrain elevations do not affect a unit's view."));
        } else {
            t->field_110 = 0;
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_114 = 1;
            strcpy(e->text, Translate("All mapped terrain is visible."));
        }
        UpdateHelpText(g_game + 0x519);
    } else if (IsCurrentGadgetNamed(menu, "SelectMap")) {
        PlaySoundByName("Skirmish", 0);
        FUN_00491c80(0x14);
        OpenSkirmishMapSelector();
    } else if (IsCurrentGadgetNamed(menu, "Difficulty")) {
        PlaySoundByName("SKirmish", 0);
        int d = *(int*)(g_game + 0x37eee);
        if (d == 0) {
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_228 = 1;
            *(int*)(g_game + 0x37eee) = 1;
        } else if (d == 1) {
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_228 = 2;
            *(int*)(g_game + 0x37eee) = 2;
        } else if (d == 2) {
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_228 = 0;
            *(int*)(g_game + 0x37eee) = 0;
        }
    }

    FUN_004ab0a0(menu);
}

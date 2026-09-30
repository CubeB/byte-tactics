// Decompiled by deepseek-v4.1, edited by deepseek-v4.1. Names are provisional.
// PARTIAL: 77.8%. Frame, prologue and the first ~89 instructions (through the
// terrain check) match byte for byte. Now 2961 bytes vs the original 2947, so
// what is left is register picks and block placement, not missing code:
//  - 0x47af91 / 0x47b04c: the original keeps the player count in ecx with the
//    active==2 tally in esi and the loop copy in edx (base in edi); ours keeps
//    the g_game base live in esi from the first count loop, so the tally lands
//    in edi and count/copy land in edx/ecx. Both pairs of loops are swapped the
//    same way, so it is a single allocator decision about esi staying live
//    across the FUN_00435a20 terrain call.  Declaring the copy before/inside the
//    if, renaming it, or splitting the count into a fresh variable (the compiler
//    CSEs it back) did not move it.
//  - the shared "FUN_004c5740; strcpy(e->text, ...); FUN_004a0090(g_game +
//    0x519); FUN_004ab0a0(menu)" tail now exists (ours at 0x47b79e, the
//    original at 0x47b88b) but a few arms still emit their own copy, e.g. the
//    third LineOfSight arm and part of the SelectMap/Difficulty tail.
//  - small tails (strcmp("Player"), the "Skirmish" branches) load the menu
//    argument into eax in ours and edx in the original, and the g_game
//    temporary is ecx in ours and edx in the original; same instructions.
// Fixed here: c2<1 and c1<1 must be one "||" test (one shared error block, two
// "jl" to it) - that alone took this from 70.3 to 77.8.
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

class Class_00435a20 { public: int FUN_00435a20(char* name); };
class Class_00437300 { public: int FUN_00437300(); };

struct Frame_0047ae60 {
    char sA[0xc];
    char sB[0xc];
    int ev[6];
    char bf[0x40];
};
#pragma pack(pop)

extern char* g_game;                   // 0x511de8

void __stdcall FUN_0049fed0(Entry_0047ae60* entries, char* text, int id);
int __stdcall FUN_0049fdf0(Entry_0047ae60* entries, char* name, int flag);
int __stdcall FUN_0049fd60(Menu_0047ae60* menu, char* name);
void __stdcall FUN_0047f1a0(char* name, int value);
char __stdcall FUN_0041d6a0(int param_1);
void FUN_0041d4c0();
void FUN_0041da30();
void FUN_00430f00();
void FUN_00479660();
int FUN_00479760();
void __stdcall FUN_004797e0(int param_1);
void FUN_0047a760();
void FUN_0047aaf0();
void __stdcall FUN_0047acd0(int param_1);
void __stdcall FUN_00491c80(int param_1);
void __stdcall FUN_004a0090(void* param_1);
void __stdcall FUN_004a0bf0(Menu_0047ae60* menu, char* key, char* value, int flag);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_004abd90(void* menu, char* text, int width, int a, int b);
void __stdcall FUN_004c2340(int* out);
char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x47ae60
void __stdcall FUN_0047ae60(Menu_0047ae60* menu)
{
    Frame_0047ae60 frame;

    Entry_0047ae60* entries = menu->holder->entries;
    int cmd = menu->field_60;
    if (cmd == -1) {
        return;
    }
    FUN_0049fed0(entries, frame.bf, cmd);

    int player = atoi(&frame.bf[strlen(frame.bf) - 1]);
    Table_0047ae60* table = *(Table_0047ae60**)(g_game + 0x29a0);
    table->field_224 = player;
    frame.bf[strlen(frame.bf) - 1] = 0;

    if (FUN_0049fd60(menu, "Start")) {
        FUN_0047f1a0("BigButton", 0);
        if (!FUN_0041d6a0(1)) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"),
                         0xc8, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
        }
        FUN_0041d4c0();

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

        if ((*(Class_00435a20**)(g_game + 0x391e9))->FUN_00435a20((*(Table_0047ae60**)(g_game + 0x29a0))->mapName) == 0) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("The terrain for the selected map does not exist."),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        int c2 = 0;
        count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            int i = count;
            Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
            do {
                if (p->active == 2)
                    c2++;
                p++;
            } while (--i);
        }
        int c1 = 0;
        if (count > 0) {
            Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
            do {
                if (p->active == 1)
                    c1++;
                p++;
            } while (--count);
        }
        if (c2 < 1 || c1 < 1) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There must be at least one player and one computer opponent"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        int maxPlayers = (*(Class_00437300**)(g_game + 0x391e9))->FUN_00437300();
        if ((int)(unsigned short)*(short*)(g_game + 0x2a3c) > maxPlayers) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There are too many players enabled for this map"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        if (FUN_00479760() != 0) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("All players may not be in the same allied group."),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        c2 = 0;
        count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
            int i = count;
            do {
                if (p->active == 2)
                    c2++;
                p++;
            } while (--i);
        }
        c1 = 0;
        if (count > 0) {
            Player_0047ae60* p = (Player_0047ae60*)*(int*)(g_game + 0x29a0);
            do {
                if (p->active == 1)
                    c1++;
                p++;
            } while (--count);
        }
        *(short*)(g_game + 0x2a3c) = c1 + c2;

        FUN_0047a760();
        FUN_0041da30();
        FUN_00430f00();
        *(char*)(g_game + 0x2bc0) = 2;
        FUN_00491c80(0x14);
        return;
    }

    if (FUN_0049fd60(menu, "PrevMenu")) {
        FUN_0047f1a0("Previous", 0);
        FUN_00491c80(0x14);
        *(char*)(g_game + 0x2bc0) = 3;
        return;
    }

    if (strcmp(frame.bf, "Player") == 0) {
        FUN_0047f1a0("Skirmish", 0);
        FUN_004797e0(player);
        FUN_004ab0a0(menu);
        return;
    }

    if (strcmp(frame.bf, "Side") == 0) {
        FUN_0047f1a0("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        int* p = (int*)((char*)t + t->field_224 * 24 + 4);
        *p = (*p + 1) % *(int*)(g_game + 0x37f39);
        FUN_004ab0a0(menu);
        return;
    }

    if (strcmp(frame.bf, "Allies") == 0) {
        FUN_0047f1a0("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        int* p = (int*)((char*)t + t->field_224 * 24 + 8);
        *p = (*p + 1) % 6;
        FUN_00479660();
        FUN_004ab0a0(menu);
        return;
    }

    if (strcmp(frame.bf, "Color") == 0) {
        FUN_0047f1a0("Skirmish", 0);
        FUN_004c2340(frame.ev);
        if (menu->holder->field_37 == 1) {
            FUN_0047acd0(0);
        }
        if (menu->holder->field_37 == 2) {
            FUN_0047acd0(1);
        }
        FUN_004ab0a0(menu);
        return;
    }

    if (strcmp(frame.bf, "Energy") == 0) {
        FUN_004c2340(frame.ev);
        if (menu->holder->field_37 == 1) {
            FUN_0047f1a0("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0x10);
            int v = *p + 0x1f4;
            if (v >= 0x2710)
                v = 0x2710;
            *p = v;
            Table_0047ae60* t2 = *(Table_0047ae60**)(g_game + 0x29a0);
            int* q = (int*)((char*)t2 + player * 24 + 0x10);
            if (*q == 0x2bc)
                *q = 0x1f4;
            wsprintfA(frame.sB, "Energy%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].energy, frame.sA, 10);
            FUN_004a0bf0(menu, frame.sB, frame.sA, 10);
        }
        if (menu->holder->field_37 == 2) {
            FUN_0047f1a0("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0x10);
            int v = *p - 0x1f4;
            if (v <= 0xc8)
                v = 0xc8;
            *p = v;
            wsprintfA(frame.sB, "Energy%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].energy, frame.sA, 10);
            FUN_004a0bf0(menu, frame.sB, frame.sA, 10);
        }
        FUN_004ab0a0(menu);
        return;
    }

    if (strcmp(frame.bf, "Metal") == 0) {
        if (menu->holder->field_37 == 1) {
            FUN_0047f1a0("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0xc);
            int v = *p + 0x1f4;
            if (v >= 0x2710)
                v = 0x2710;
            *p = v;
            Table_0047ae60* t2 = *(Table_0047ae60**)(g_game + 0x29a0);
            int* q = (int*)((char*)t2 + player * 24 + 0xc);
            if (*q == 0x2bc)
                *q = 0x1f4;
            wsprintfA(frame.sA, "Metal%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].metal, frame.sB, 10);
            FUN_004a0bf0(menu, frame.sA, frame.sB, 10);
        }
        if (menu->holder->field_37 == 2) {
            FUN_0047f1a0("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0xc);
            int v = *p - 0x1f4;
            if (v <= 0xc8)
                v = 0xc8;
            *p = v;
            wsprintfA(frame.sA, "Metal%d", player);
            _itoa((*(Table_0047ae60**)(g_game + 0x29a0))->players[player].metal, frame.sB, 10);
            FUN_004a0bf0(menu, frame.sA, frame.sB, 10);
        }
        FUN_004ab0a0(menu);
        return;
    }

    if (FUN_0049fd60(menu, "CommanderDeath")) {
        FUN_0047f1a0("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        t->field_108 ^= 1;
        int index = FUN_0049fdf0(entries, "CommanderDeath", 1);
        Entry_0047ae60* e = &entries[index];
        if ((*(Table_0047ae60**)(g_game + 0x29a0))->field_108 != 0)
            strcpy(e->text, FUN_004c5740("Game ends when commander is destroyed."));
        else
            strcpy(e->text, FUN_004c5740("Game continues after Commander is destroyed."));
        FUN_004a0090(g_game + 0x519);
        FUN_004ab0a0(menu);
        return;
    }

    if (FUN_0049fd60(menu, "StartLocation")) {
        FUN_0047f1a0("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        t->field_118 ^= 1;
        int index = FUN_0049fdf0(entries, "StartLocation", 1);
        Entry_0047ae60* e = &entries[index];
        if ((*(Table_0047ae60**)(g_game + 0x29a0))->field_118 != 0)
            strcpy(e->text, FUN_004c5740("Commanders are placed at pre-determined locations."));
        else
            strcpy(e->text, FUN_004c5740("Commanders are randomly placed on the battle field."));
        FUN_004a0090(g_game + 0x519);
        FUN_004ab0a0(menu);
        return;
    }

    if (FUN_0049fd60(menu, "Mapping")) {
        FUN_0047f1a0("Skirmish", 0);
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        t->field_10c ^= 1;
        int index = FUN_0049fdf0(entries, "Mapping", 1);
        Entry_0047ae60* e = &entries[index];
        if ((*(Table_0047ae60**)(g_game + 0x29a0))->field_10c != 0)
            strcpy(e->text, FUN_004c5740("Terrain is blacked out until explored."));
        else
            strcpy(e->text, FUN_004c5740("Terrain is visible."));
        FUN_004a0090(g_game + 0x519);
        FUN_004ab0a0(menu);
        return;
    }

    if (FUN_0049fd60(menu, "LineOfSight")) {
        FUN_0047f1a0("Skirmish", 0);
        int index = FUN_0049fdf0(entries, "LineOfSight", 1);
        Entry_0047ae60* e = &entries[index];
        Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
        if (t->field_110 == 0) {
            t->field_110 = 1;
            strcpy(e->text, FUN_004c5740("Terrain elevations affect a unit's view."));
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_114 = 1;
        } else if (t->field_114 == 1) {
            t->field_114 = 0;
            strcpy(e->text, FUN_004c5740("Terrain elevations do not affect a unit's view."));
        } else {
            t->field_110 = 0;
            strcpy(e->text, FUN_004c5740("All mapped terrain is visible."));
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_114 = 1;
        }
        FUN_004a0090(g_game + 0x519);
        FUN_004ab0a0(menu);
        return;
    }

    if (FUN_0049fd60(menu, "SelectMap")) {
        FUN_0047f1a0("Skirmish", 0);
        FUN_00491c80(0x14);
        FUN_0047aaf0();
        FUN_004ab0a0(menu);
        return;
    }

    if (FUN_0049fd60(menu, "Difficulty")) {
        FUN_0047f1a0("SKirmish", 0);
        int d = *(int*)(g_game + 0x37eee);
        if (d == 0) {
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_228 = 1;
            *(int*)(g_game + 0x37eee) = 1;
            FUN_004ab0a0(menu);
            return;
        }
        if (d == 1) {
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_228 = 2;
            *(int*)(g_game + 0x37eee) = 2;
            FUN_004ab0a0(menu);
            return;
        }
        if (d == 2) {
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_228 = 0;
            *(int*)(g_game + 0x37eee) = 0;
        }
    }

    FUN_004ab0a0(menu);
}

// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 2947-byte skirmish setup dialog handler. Full control-flow chain
// transcribed (Start validation, PrevMenu, Player/Side/Allies/Color/Energy/
// Metal dispatch, CommanderDeath/StartLocation/Mapping/LineOfSight, SelectMap,
// Difficulty). Best score 49.0%.
// Remaining diff hunks:
//   0x47ae60-0x47aea4: frame is sub esp,0x74 instead of 0x70 (one int, probably
//     `player` or `cmd`, spills where the original keeps every int in a
//     register), so buf is at [esp+0x54] instead of [esp+0x40]; the entries
//     pointer lands in ebx, the original keeps it in ebp and the menu arg in
//     esi. Fixing the frame arithmetic first would shift all the later slots.
//   0x47af91-0x47b08b: the two player-count loops use a different reload of the
//     table pointer (ours re-reads [g_game+0x29a0] into eax vs the original's
//     edi/esi live copies) and the c1 counter loop's register pairing differs.
//   0x47b0fa-0x47b9e0: the strcmp dispatch chain is present but the two 12-byte
//     scratch buffers at [esp+0x10]/[esp+0x1c], whose roles swap between the
//     Energy and Metal branches, and the FUN_0049fdf0 index-to-entry scaling
//     (index * 0x15b) still allocate differently.

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

class Class_0047ae60 {
public:
    int FUN_00435a20(char* name);
    int FUN_00437300();
};

struct Frame_0047ae60 {
    char pd[0x10];                     // +0x00
    char sA[0xc];                      // +0x10
    char sB[0xc];                      // +0x1c
    int ev[6];                         // +0x28
    char bf[0x30];                     // +0x40
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
            Player_0047ae60* p = table->players;
            int i = count;
            do {
                if (p->active == 2)
                    n++;
                p++;
            } while (--i);
        }
        *(short*)(g_game + 0x2a3c) = n + 1;

        if ((*(Class_0047ae60**)(g_game + 0x391e9))->FUN_00435a20(table->mapName) == 0) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("The terrain for the selected map does not exist."),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        int c2 = 0;
        count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            Player_0047ae60* p = table->players;
            int i = count;
            do {
                if (p->active == 2)
                    c2++;
                p++;
            } while (--i);
        }
        if (c2 < 1) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There must be at least one player and one computer opponent"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        int c1 = 0;
        if (count > 0) {
            Player_0047ae60* p = table->players;
            do {
                if (p->active == 1)
                    c1++;
                p++;
            } while (--count);
        }
        if (c1 < 1) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There must be at least one player and one computer opponent"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(menu);
            return;
        }

        if ((int)(unsigned short)*(short*)(g_game + 0x2a3c) >
            (*(Class_0047ae60**)(g_game + 0x391e9))->FUN_00437300()) {
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
            Player_0047ae60* p = table->players;
            int i = count;
            do {
                if (p->active == 2)
                    c2++;
                p++;
            } while (--i);
        }
        c1 = 0;
        if (count > 0) {
            Player_0047ae60* p = table->players;
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
        FUN_004797e0(1);
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
            _itoa(*q, frame.sA, 10);
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
            _itoa(*p, frame.sA, 10);
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
            _itoa(*q, frame.sB, 10);
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
            _itoa(*p, frame.sB, 10);
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
        if (t->field_108 != 0)
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
        if (t->field_118 != 0)
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
        if (t->field_10c != 0)
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

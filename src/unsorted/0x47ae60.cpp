// Decompiled by deepseek-v4.1, edited by deepseek-v4.1 and GPT-6.1-sol. Names are provisional.
// PARTIAL: 85.5% (ours 2868 bytes vs the original 2947).
// What fixed 84.2 -> 85.5: the c2/c1 player count block. The original lays it
// out loop1 / test-c2 / loop2 / test-c1 with ONE shared error stub at
// 0x47b0bf (both "jl 0x47b0bf"). "if (c2 < 1 || c1 < 1)" and two separate ifs
// both put the compares after the second loop (75.4). The fix is to nest:
//   loop1 counts c2; if (c2 >= 1) { loop2 counts c1; if (c1 >= 1) { ...success,
//   return; } } then the single players-computer error block falls at the end.
// This yields test-c2 right after loop1 and a single shared error stub, matching
// the original block order (error stub sits after the success return).
// LineOfSight's two "field_114 = 1" stores must precede the strcpy so the seven
// toggle-arm message tails merge into one strcpy tail at 0x47b88b.
// The Difficulty arm calls FUN_0047f1a0("SKirmish", 0): the original pushes
// 0x502a6c (the typo'd literal), not 0x507ccc "Skirmish". Do not correct it.
// STILL DIFFERS (register-allocation tie-breaks):
//  - Energy/Metal clamp arms: original "mov ecx,[..]; lea eax,[..]; add ecx,
//    0x1f4 / add ecx,0xfffffe0c; mov [eax],ecx" (value in ecx, address in eax
//    reusing the base reg). Ours is the mirror "lea ecx; mov eax; add eax; mov
//    [ecx],eax". Tried the double-dereference form (recompute the store address
//    after the load): that dropped to 79.6%. Compound "v += -0x1f4" is flat
//    (still sub eax,0x1f4 vs the original add ecx,0xfffffe0c). Keeping the
//    value load first and the address second is the wall.
//  - menu reload: the original keeps menu in its stack home and reloads
//    "mov edx,[esp+0x84]; push edx" before each FUN_004ab0a0 / FUN_004a0bf0;
//    ours keeps menu in a callee-saved register in the toggle/Energy/Metal arms
//    ("push esi"/"push ebp"). The Color tail is a pure scratch-reg tie-break
//    ("mov edx,[esp+0x84]" original vs "mov eax,[esp+0x84]" ours).
//  - g_game reload in the toggle arms lands in a different scratch register
//    (original mov edx / ours mov ecx or mov eax).
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
                FUN_00430f00();
                *(char*)(g_game + 0x2bc0) = 2;
                FUN_00491c80(0x14);
                return;
            }
        }
        FUN_004abd90(g_game + 0x519,
                     FUN_004c5740("There must be at least one player and one computer opponent"),
                     0x1e0, 1, 1);
        FUN_004ab0a0(menu);
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
            int v = *p;
            v += 0x1f4;
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
            int v = *p + -0x1f4;
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
            int v = *p;
            v += 0x1f4;
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
            int v = *p + -0x1f4;
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
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_114 = 1;
            strcpy(e->text, FUN_004c5740("Terrain elevations affect a unit's view."));
        } else if (t->field_114 == 1) {
            t->field_114 = 0;
            strcpy(e->text, FUN_004c5740("Terrain elevations do not affect a unit's view."));
        } else {
            t->field_110 = 0;
            (*(Table_0047ae60**)(g_game + 0x29a0))->field_114 = 1;
            strcpy(e->text, FUN_004c5740("All mapped terrain is visible."));
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

// Decompiled by deepseek-v4.1, edited by deepseek-v4.1 and GPT-6.1-sol, finished by deepseek-v4.1-flash, mimo-v2.6-pro and Space Bunny Free. Names are provisional.
// PARTIAL: 88.2% (ours 2876 bytes vs the original 2947).
// Space Bunny Free pass: the 71 byte gap is now fully accounted for and it
// splits into exactly two items, both of them c1xx register/immediate choices
// rather than missing statements. Re-derivation used the disassembly in
// build/scratch/0x47ae60 (gen*.py, v0.cpp baseline) plus objdump of our own
// object; the arm bodies themselves are byte identical.
//  (1) 27 bytes: nine hoisted constants. Both versions hoist the same nine
//      (ebx=1 eight times, edi=2, esi=6) into the same registers at the same
//      points, but the original emits the imm8 form ("mov ebx, 1", 2 bytes)
//      and ours the imm32 form ("mov ebx, 0x1", 5 bytes). 3 bytes x 9 = 27.
//      Sites: the ebx=1 after "BigButton" (feeds FUN_0041d6a0(1) and the two
//      trailing 1s of FUN_004abd90), edi=2 at the "Player" arm, esi=6 at the
//      "Allies" arm, ebx=1 in the Color arm (feeds field_37==1 and
//      FUN_0047acd0(1)), ebx=1 in the four toggle arms, ebx=1 in the
//      Difficulty arm. Not steerable: character literals ('\1','\2','\6') and
//      char-typed parameters (FUN_0041d6a0, FUN_0047acd0, FUN_004abd90's a/b)
//      were all tried and are flat at 2876 bytes.
//  (2) 42 bytes: seven parameter reloads. The original has 17 "mov
//      <reg>,[esp+0x84]" reloads of the stack-home parameter, ours has 10.
//      The extra seven are Color tail (orig 0x47b305), Energy arm-2 (0x47b48b
//      "mov edx,[esp+0x90]" plus 0x47b4a7), Metal arm-2 (0x47b63f, 0x47b656
//      region) and the SelectMap/Difficulty tails (0x47b8c4, 0x47b902,
//      0x47b95f, 0x47b999, 0x47b9cc). 6 bytes x 7 = 42.
//      Why: c1xx gives the parameter a live RANGE, not a fixed register. In
//      the original each strcmp literal lands in esi (mov esi, 0x50837c
//      "Color"), which kills the range that held menu, and every later use
//      starts a fresh range in whatever scratch register is free - hence the
//      mixed ecx/eax/edx/esi/ebp targets and the reuse of esi for the zero in
//      the Difficulty arm ("xor esi, esi" where ours does "xor ebx, ebx").
//      Ours instead keeps ONE long range and reuses the register. The
//      divergence starts at reload 3 of 17: original edx, ours eax.
//      Not steerable, all flat at 2876 bytes: a Menu* alias local feeding
//      only the tails and the arm-2 calls; a static void redraw(Menu*) helper
//      called from the tails; taking the parameter's address (&menu, both as
//      a dead guard and as *(Menu**)&menu at every call site) - this confirms
//      the parameter is ALREADY stack-home resident, since the address-taken
//      form only added 8 bytes of guard code; callee prototype narrowing
//      (FUN_004ab0a0 as int or char*, FUN_004a0bf0 first arg as void*);
//      extra braces around the arm bodies; else-if instead of two ifs; a
//      per-sub-block local for the Energy/Metal arm-2 pointer; a shared goto
//      tail; and tools/permute.py (best 2188 -> 2178 in 7.65 min, never
//      improved on the starting score).
//  (3) 4 bytes, not yet recovered: in the four toggle arms the original has
//      "lea esi, [ebp+ecx*2]" (3 bytes) and ours "lea esi, [ebp+edx*2+0x0]"
//      (4 bytes); MSVC folds a zero displacement into the entry address.
//      "&entries[index]", "entries + index" and swapping the strcpy branches
//      are all flat. This is downstream of item (2): the toggle arms also
//      disagree on which scratch register holds g_game (original edx, ours
//      ecx or eax), so fixing the reload pattern may fix this for free.
// Retry note (mimo-v2.6-pro, second pass): what fixed 86.7 -> 88.2 was the
// clamp tail shape. The up clamps are now windef.h's min() and the down
// clamps max(), applied on the pointee:
//   *p = min(*p + 0x1f4, 0x2710);        (Energy/Metal up)
//   *p = max(*p + -0x1f4, 0xc8);         (Energy/Metal down)
// The min/max macro evaluates its argument twice, which keeps MSVC's
// load-then-lea allocation (value in ecx, address in eax) AND lands the
// single conditional store of a register ("cmp; jl L; mov ecx, 0x2710;
// mov [eax], ecx") exactly as the original. The negative immediate survives
// as "add ecx, 0xfffffe0c" only when written "*p + -0x1f4" (max's second
// evaluation of the argument keeps the add-imm form; "*p - 0x1f4" gives
// "sub reg, 0x1f4" and scores 86.9%). TRIED for the tail shape and rejected:
// the plain v-form ("int v = *p; v += 0x1f4; if (v >= 0x2710) v = 0x2710;
// *p = v;") gives the right tail but mirrored registers (value in eax,
// address in ecx); the ternary "int v = *p + 0x1f4; *p = v >= 0x2710 ? 0x2710
// : v;" is the same (86.1-86.2%); the typed-field form folds the lea away
// entirely (81.9-82.4%). Per-arm scores for the min/max arms: 86.8% (min on
// Energy-1), 87.5% (min on Metal-1 or max on Metal-2), 88.2% (all four).
// Earlier: 85.5 -> 86.7 came from the compound "*p += ..." form on the four
// clamps; 84.2 -> 85.5 from the c2/c1 player count block nested as loop1 /
// test-c2 / loop2 / test-c1 with ONE shared error stub at 0x47b0bf; the
// LineOfSight "field_114 = 1" stores must precede the strcpy so the seven
// toggle-arm message tails merge at 0x47b88b. The Difficulty arm calls
// FUN_0047f1a0("SKirmish", 0): the original pushes 0x502a6c (the typo'd
// literal), not 0x507ccc "Skirmish". Do not correct it.
// TRIED and flat: tools/headers.py (128 sets, all 86.7-86.8%); an N-dummy
// sweep (0 to 400 extern int dummyN; in steps of 4) is flat at 88.2% for
// every N, so compiler state is not the remaining lever. The Menu* alias
// local that was listed as "first thing to try next" has since been scored
// and is also flat; see items (1) and (2) above for the full list.
// STILL DIFFERS, small scratch-reg ties worth 0 bytes: the Player tail
// "mov edx,[esp+0x84]" (ours mov eax) and the g_game reload in the toggle
// arms landing in a different scratch register (original mov edx, ours
// mov ecx or mov eax). The strcmp-chain tail addresses drift by the size
// gap above.
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
            FUN_0047f1a0("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0x10);
            *p = max(*p + -0x1f4, 0xc8);
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
            FUN_0047f1a0("Skirmish", 0);
            Table_0047ae60* t = *(Table_0047ae60**)(g_game + 0x29a0);
            int* p = (int*)((char*)t + player * 24 + 0xc);
            *p = max(*p + -0x1f4, 0xc8);
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

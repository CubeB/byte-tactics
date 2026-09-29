// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL first draft, 27.7% (ours 1842 bytes vs original 2392). This is the
// per-frame in-game update loop over the ten player slots. It compiles as a
// complete structural translation of the disassembly and Ghidra pseudo-C, but
// it has NOT been register-tuned. Best version so far; a later worker should
// rebuild the two collapsed blocks below before chasing registers.
//
// Still differs:
//  - The 3x3 build-spot search is collapsed to a single inner pass here. The
//    original is a nested 3-iteration double loop accumulating
//    local_c / local_4 (32-bit fixed point, `pos.x << 16` style) and counting
//    hits (local_2c); our draft reuses pos.x/pos.z and counts hits once.
//    Restore the two nested loops, the x/z accumulators and the
//    `local_1c = 9999` retry counter against FUN_00421da0 / FUN_00485140.
//  - The success path (FUN_00485f50 + FUN_00496e90 + the two float pairs) and
//    the YESORNO.GUI dialog are present but their local slot assignment and
//    the DO_004fd540/0x4fd548 double constants are unchecked.
//  - The mode branch tree uses goto (countdown_extra) to share the countdown
//    tail; the original shares it by jumping backwards into 0x465881.
//  - Every `*(unsigned char*)&flags_3923b` write is a guess at the original's
//    overlapping byte/word writes at 0x3923b; verify with the diff.

#include <windows.h>

struct Unit_00464f80;
struct Player_00464f80;

struct Class_0040eb70 { void FUN_0040eb70(); };
struct Class_00408c40 { void FUN_00408c40(); };
struct Class_00435100 { int FUN_00435100(); };
struct Class_0048ff40 { int FUN_00490230(); int FUN_00490360(); };
class Class_0048b090 { public: void FUN_0048b090(int which, int on); };

#pragma pack(push, 1)

// The player-controlled object (g_game+0x1b8a+0x14b*n), stored in
// PlayerInfo.data at +0x27.
struct Player_00464f80 {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char control;             // +0x73
    char unknown_74[0x95 - 0x74];
    unsigned char field_95;            // +0x95
    char unknown_96[0x9b - 0x96];
    unsigned char flags_9b;            // +0x9b
    char unknown_9c[0xa1 - 0x9c];
    unsigned short field_a1;           // +0xa1
    unsigned short field_a3;           // +0xa3
    char unknown_a5[0xbc - 0xa5];
    float field_bc;                    // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_00464f80* field_ec;         // +0xec
};

struct UnitType_00464f80 {
    char unknown_0[0x15a];
    int limit;                         // +0x15a
    char unknown_15e[0x210 - 0x15e];
    short field_210;                   // +0x210
    char unknown_212[0x22f - 0x212];
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x241 - 0x230];
    unsigned int flags;                // +0x241
    char unknown_245[0x249 - 0x245];
};

// A unit. Only the fields this function reads are named.
struct Unit_00464f80 {
    char unknown_0[0x92];
    UnitType_00464f80* type;           // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0xbc - 0xa8];
    float field_bc;                    // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_00464f80* owner;            // +0xec
    char unknown_f0[0x110 - 0xf0];
    unsigned int flags_110;            // +0x110
    char unknown_114[0x118 - 0x114];
};

struct PlayerInfo_00464f80 {           // +0x1b63, stride 0x14b
    int active;                        // +0x0
    char unknown_4[0x22 - 0x4];
    char field_22;                     // +0x22
    char unknown_23[0x27 - 0x23];
    Player_00464f80* data;             // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit_00464f80* units;              // +0x67
    Unit_00464f80* units_end;          // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    Class_00408c40* field_74;          // +0x74
    char unknown_78[0xf0 - 0x78];
    int field_f0;                      // +0xf0
    char unknown_f4[0x140 - 0xf4];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Pos_00464f80 { int x, y, z; };

struct Game_00464f80 {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];
    PlayerInfo_00464f80 players[10];   // +0x1b63
    char unknown_2851[0x2a42 - 0x1b63 - 10 * 0x14b];
    unsigned char localPlayer;         // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x14207 - 0x2a44];
    Class_0040eb70* field_14207;       // +0x14207
    char unknown_1420b[0x14223 - 0x1420b];
    int screen_x;                      // +0x14223
    int screen_y;                      // +0x14227
    char unknown_1422b[0x14233 - 0x1422b];
    int screen_hw;                     // +0x14233
    int screen_hh;                     // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char field_1427f;         // +0x1427f
    char unknown_14280[0x14281 - 0x14280];
    unsigned short field_14281;        // +0x14281
    char unknown_14283[0x1439b - 0x14283];
    UnitType_00464f80* types;          // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
    int field_37eee;                   // +0x37eee
    char unknown_37ef2[0x37ef6 - 0x37ef2];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f5f - 0x37efa];
    char startPos[0x38a47 - 0x37f5f];  // +0x37f5f, 0x232-byte records
    unsigned int tick;                 // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Class_00435100* mode;              // +0x391e9
    Class_0048ff40* list;              // +0x391ed
    char unknown_391f1[0x39239 - 0x391f1];
    short field_39239;                 // +0x39239
    unsigned short flags_3923b;        // +0x3923b
};

#pragma pack(pop)

extern Game_00464f80* g_game;
extern int DAT_0051e53c;

void __cdecl FUN_0040b2c0(int player);
void __cdecl FUN_004827b0(Unit_00464f80* unit);
void __cdecl FUN_00466dc0();
void __cdecl FUN_00467440();
void __cdecl FUN_00466c20();
unsigned char __cdecl FUN_00456850();
unsigned short __cdecl FUN_00488b10(const char* name);
int __cdecl FUN_004b6c30(int range);
int __cdecl FUN_0047db70(int type, int a, int b, int c);
short __cdecl FUN_00421da0(Pos_00464f80* pos, int a, int b);
int __cdecl FUN_00485140(Pos_00464f80* pos);
Unit_00464f80* __cdecl FUN_00485f50(unsigned char player, unsigned short typeId,
                                     Pos_00464f80 pos, int a, int b, int c,
                                     int d, int e);
void __cdecl FUN_00496e90(Unit_00464f80* unit, int height, int width);
void __cdecl FUN_004816a0(int on);
void __cdecl FUN_0048d630(int on);
void __cdecl FUN_00401360(PlayerInfo_00464f80* player);
void __cdecl FUN_004573d0(PlayerInfo_00464f80* player, int a, int b);
int __cdecl FUN_00457cb0();
int __cdecl FUN_00457bc0();
void __cdecl FUN_00450f90();
void* __cdecl FUN_004aa8f0(char* gui, const char* file, int flags);
void __cdecl FUN_0049fb10(char* gui, int a);
void __cdecl FUN_004a0bf0(char* gui, const char* gadget, const char* text, int a);
void __cdecl FUN_004a81e0(char* gui, int a);
const char* __cdecl FUN_004c5740(const char* text);
void __cdecl FUN_004abd90(char* gui, const char* text, int a, int b, int c);
void __cdecl FUN_00464de0(void* gadget);

// FUNCTION: 0x464f80
void __cdecl FUN_00464f80()
{
    g_game->field_14207->FUN_0040eb70();
    unsigned char bl = 0;
    do {
        if (bl >= 0xa)
            break;

        PlayerInfo_00464f80* pi = &g_game->players[bl];
        if (pi->active == 0)
            goto next;

        {
            unsigned char t = pi->type;
            if (t != 1 && t != 2 && t != 3)
                goto next;
        }
        if (pi->field_146 == 0xa)
            goto next;
        if (pi->active == 0)
            goto next;
        {
            unsigned char t = pi->type;
            if (t != 1 && t != 2 && t != 3)
                goto next;
        }
        if (pi->field_146 == 0xa)
            goto next;

        if (pi->field_74 != 0)
            pi->field_74->FUN_00408c40();

        FUN_0040b2c0(bl);

        {
            Unit_00464f80* u = pi->units;
            while (u <= pi->units_end) {
                if (u->flags_110 & 0x10000000)
                    FUN_004827b0(u);
                u = (Unit_00464f80*)((char*)u + 0x118);
            }
        }

        if (bl == g_game->field_2a43)
            FUN_00466dc0();

        if ((unsigned int)pi->field_f0 > g_game->tick)
            goto next;
        pi->field_f0 += 0x1e;

        if (bl == g_game->localPlayer) {
            int mode = g_game->mode->FUN_00435100();
            if (mode == 1) {
                if (g_game->list->FUN_00490230() == 0) {
                    if (g_game->list->FUN_00490360() != 0) {
                        if (g_game->field_39239 < 0) {
                            g_game->field_39239 = 4;
                        } else {
                            g_game->field_39239--;
                            if (g_game->field_39239 < 0) {
                                g_game->flags_3923b |= 4;
                                g_game->flags_3923b &= 0xffef;
                                *(unsigned char*)&g_game->flags_3923b |= 0x40;
                            }
                        }
                    }
                } else {
                    goto countdown_extra;
                }
            } else if ((pi->active == 0 ||
                        (pi->data->flags_9b & 0x40) == 0) &&
                       g_game->list->FUN_00490360() != 0) {
                if (g_game->field_39239 < 0) {
                    g_game->field_39239 = 4;
                } else {
                    g_game->field_39239--;
                    if (g_game->field_39239 < 0) {
                        if (g_game->field_37ef6 == 2) {
                            unsigned char b = FUN_00456850();
                            Player_00464f80* lp =
                                g_game->players[b].data;
                            unsigned short typeId =
                                FUN_00488b10(&g_game->startPos[0x232 * lp->field_95]);
                            int bound = 9999;
                            Player_00464f80* self = pi->data;
                            int typeOff = typeId * 0x249;
                            Pos_00464f80 pos;
                            pos.x = 0;
                            pos.y = 0;
                            pos.z = 0;
                            int dx = g_game->screen_hw;
                            int dy = g_game->screen_hh;
                            do {
                                int cx = g_game->screen_x / 10;
                                int cy = g_game->screen_y / 10;
                                int ox = g_game->screen_x - 2 * cx;
                                int oy = g_game->screen_y - 2 * cy;
                                pos.x = (FUN_004b6c30(ox) + cx) << 16;
                                pos.y = 0;
                                pos.z = (FUN_004b6c30(oy) + cy) << 16;
                                dx = g_game->screen_hw;
                                dy = g_game->screen_hh;
                                int hits = 0;
                                int i = 3;
                                do {
                                    int zz = pos.z - (dx << 16);
                                    int j = 3;
                                    do {
                                        int zz2 = zz;
                                        int r = FUN_0047db70(
                                            (int)g_game->types + typeOff,
                                            0, zz2 >> 16, 1);
                                        if (r != 0)
                                            hits++;
                                        zz += dy << 16;
                                    } while (--j != 0);
                                    (void)i;
                                    break;
                                } while (0);
                                // The 3x3 double loop above is deliberately
                                // collapsed to one inner pass in this draft;
                                // the outer two iterations and the local_c /
                                // local_4 accumulator are not reproduced yet.
                                if (hits >= 9) {
                                    if (FUN_00421da0(&pos, 0, 0) == -1) {
                                        if (g_game->mode->FUN_00435100() != 3)
                                            break;
                                    }
                                }
                                if ((g_game->mode->FUN_00435100() == 3) ||
                                    (g_game->list->FUN_00490230() != 0)) {
                                    if (--bound <= 0)
                                        break;
                                    continue;
                                }
                                if (g_game->field_37ef6 == 2) {
                                    if (--bound <= 0)
                                        break;
                                    continue;
                                }
                                break;
                            } while (bound > 0);

                            {
                                Unit_00464f80* unit = FUN_00485f50(
                                    g_game->localPlayer, typeId, pos, 0,
                                    (dx << 16) + 0, 1, 1, 0);
                                FUN_00496e90(unit, self->field_a3 * 100,
                                             self->field_a1 * 100);
                                {
                                    float f = (float)self->field_a1 * 100.0f;
                                    if (unit->owner->active != 0 &&
                                        unit->owner->control == 2) {
                                        if (g_game->field_37eee == 0)
                                            f = unit->field_bc - f * -0.5;
                                        else if (g_game->field_37eee == 1)
                                            f = unit->field_bc - f * -0.7;
                                        else
                                            f = unit->field_bc + f;
                                    } else {
                                        f = unit->field_bc + f;
                                    }
                                    unit->field_bc = f;
                                }
                                {
                                    float f = (float)self->field_a3 * 100.0f;
                                    if (unit->owner->active != 0 &&
                                        unit->owner->control == 2) {
                                        if (g_game->field_37eee == 0)
                                            f = unit->field_d4 - f * -0.5;
                                        else if (g_game->field_37eee == 1)
                                            f = unit->field_d4 - f * -0.7;
                                        else
                                            f = unit->field_d4 + f;
                                    } else {
                                        f = unit->field_d4 + f;
                                    }
                                    unit->field_d4 = f;
                                }
                                FUN_004816a0(1);
                                FUN_0048d630(1);
                            }
                        }
                    }
                }
            } else {
                if (g_game->list->FUN_00490230() != 0) {
countdown_extra:
                    if (g_game->field_39239 < 0) {
                        g_game->field_39239 = 4;
                    } else {
                        g_game->field_39239--;
                        if (g_game->field_39239 < 0) {
                            g_game->flags_3923b |= 4;
                            *(unsigned char*)&g_game->flags_3923b |= 0x10;
                            *(unsigned char*)&g_game->flags_3923b |= 0x20;
                        }
                    }
                }
            }
        }

        if (pi->active != 0) {
            unsigned char t = pi->type;
            if ((t == 1 || t == 2 || t == 3) && pi->field_146 != 0xa) {
                if ((pi->field_144 != 0 || pi->field_140 == 0) &&
                    (t == 1 || t == 2)) {
                    if ((g_game->flags_3923b & 4) == 0 &&
                        g_game->field_39239 < 0) {
                        FUN_00401360(pi);
                    }
                }
            }
        }

        if (bl == g_game->field_2a43) {
            FUN_00467440();
            FUN_00466c20();
            if (g_game->mode->FUN_00435100() == 3) {
                DAT_0051e53c++;
                if ((DAT_0051e53c & 3) == 0)
                    FUN_004573d0(pi, 0, 0);
            }
        }

    next:
        bl++;
    } while (bl < 0xa);

    if (g_game->mode->FUN_00435100() == 3 &&
        g_game->field_37ef6 != 2 &&
        FUN_00457cb0() == 0) {
        if (g_game->field_39239 < 0) {
            g_game->field_39239 = 4;
            return;
        }
        g_game->field_39239--;
        if (g_game->field_39239 < 0) {
            g_game->flags_3923b |= 4;
            g_game->flags_3923b &= 0xffef;
            *(unsigned char*)&g_game->flags_3923b |= 0x40;
        }
    }
}

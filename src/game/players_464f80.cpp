// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, refined by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, refined by GPT-6.1-sol. Names are provisional.
#include <windows.h>
#include <string.h>

struct Unit;
struct Player_00464f80;

struct Pathfinder { void RunSearches(); };
struct SquadManager { void TickIfActive(); };
struct Mission {
    char unknown_0[0xd44];
    int field_d44;                     // +0xd44
    int GetGameType();
};
struct MissionConditions { int CheckVictory(); int CheckDefeat(); };
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
    // unsigned short bitfields: the only spelling that gives a direct
    // `or byte ptr [m], K`.
    union {
        unsigned char flags_9b;        // +0x9b
        struct {
            unsigned short padb : 6;
            unsigned short bit6b : 1;
            unsigned short bit7b : 1;
            unsigned short restb : 8;
        } fb;
    };
    char unknown_9d[0xa1 - 0x9d];
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
struct Unit {
    char unknown_0[0x92];
    UnitType_00464f80* type;           // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0xbc - 0xa8];
    float field_bc;                    // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_00464f80* field_ec;         // +0xec
    char unknown_f0[0x110 - 0xf0];
    unsigned int flags_110;            // +0x110
    char unknown_114[0x118 - 0x114];
    void SetStateBits(int which, int on);
};

struct PlayerInfo_00464f80 {           // +0x1b63, stride 0x14b
    int active;                        // +0x0
    char unknown_4[0x22 - 0x4];
    char field_22;                     // +0x22
    char unknown_23[0x27 - 0x23];
    Player_00464f80* data;             // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* units;                       // +0x67
    Unit* units_end;                   // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    SquadManager* field_74;            // +0x74
    char unknown_78[0xf0 - 0x78];
    int field_f0;                      // +0xf0
    char unknown_f4[0x140 - 0xf4];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Pos_00464f80 { int x, y, z; };

struct Point16 { short x, y; };

struct UnitDef_00464f80 { char unknown_0[0x249]; };

struct Struct_00496e90 {
    char unknown_0[0xdc];
    float width;                       // +0xdc
    float height;                      // +0xe0
    char unknown_e4[0x149 - 0xe4];
    unsigned short flag_149 : 1;       // +0x149
};

struct Widget_00464f80 {
    char unknown_0[0xcc];
    char field_cc[0x10];               // +0xcc
    char field_dc[0x20];               // +0xdc
};

struct Dialog_00464f80 {
    char unknown_0[4];
    Widget_00464f80* field_4;          // +0x4
    void* field_8;                     // +0x8
};

struct Game {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];
    PlayerInfo_00464f80 players[10];   // +0x1b63
    char unknown_2851[0x2a42 - 0x1b63 - 10 * 0x14b];
    unsigned char localPlayer;         // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x14207 - 0x2a44];
    Pathfinder* field_14207;           // +0x14207
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
    Mission* mode;                     // +0x391e9
    MissionConditions* list;           // +0x391ed
    char unknown_391f1[0x39239 - 0x391f1];
    short field_39239;                 // +0x39239
    // unsigned short bitfields: the only spelling that gives a direct
    // `or byte ptr [m], K`.
    union {
        unsigned short w;
        struct {
            unsigned short padb2 : 2;
            unsigned short bit2 : 1;
            unsigned short bit3 : 1;
            unsigned short bit4 : 1;
            unsigned short bit5 : 1;
            unsigned short bit6 : 1;
            unsigned short rest2 : 9;
        } b;
    } flags_3923b;                     // +0x3923b
};

#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e53c;

void __stdcall UpdatePlayerAI(int player);
void __stdcall UpdateUnitLineOfSight(Unit* unit);
void DrawRadarUnits();
void UpdateSensorRadarAndCloak();
void UpdateRadarMapped();
unsigned char __stdcall FindHostSlot();
unsigned short __stdcall FindUnitTypeId(const char* name);
int __stdcall RandomInt(int range);
int __stdcall FUN_0047db70(UnitDef_00464f80* type, int a, Point16 cell, int c);
short __stdcall FindFeatureAtPos(Pos_00464f80* pos, int a, int b);
int __stdcall GetCellMeanHeight(Pos_00464f80* pos);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short typeId,
                                     Pos_00464f80 pos, int a, int b, int c);
void __stdcall SetStartingStorageBonus(Struct_00496e90* obj, int height, int width);
void __stdcall RecalculateLineOfSight(int on);
void __stdcall FUN_0048d630(int on);
void __stdcall UpdatePlayerEconomy(PlayerInfo_00464f80* player);
void __stdcall SendPlayerEconomy(PlayerInfo_00464f80* player, int a, int b);
int __stdcall FUN_00457cb0();
int __stdcall FUN_00457bc0();
void __stdcall BroadcastPlayerInfo();
void* __stdcall LoadGuiLayer(char* gui, const char* file, int flags);
void __stdcall FUN_0049fb10(char* gui, int a);
void __stdcall FUN_004a0bf0(char* gui, const char* gadget, const char* text, int a);
void __stdcall RenderLayer(char* gui, int a);
const char* __stdcall Translate(const char* text);
void __stdcall OpenMessageBox(char* gui, const char* text, int a, int b, int c);
void __stdcall FUN_00464de0(void* gadget);

static int loopCond_00464f80(unsigned char i)
{
    if (i >= 0xa)
        return 0;
    return 1;
}

// The loop's latch test. It has to be a second, separately spelled inlined
// helper: with the same expression at both test sites MSVC folds one of them
// away, and the original keeps both.
static int more_00464f80(unsigned char i)
{
    if (i < 0xa)
        return 1;
    return 0;
}

// The three scale constants, named so that each lands in .rdata as its own
// object of exactly the original's width, and in this order: MSVC 5 emits a
// float LITERAL in an 8-byte slot but a named static const float in 4, and
// literals come after statics, so with 100.0f spelled as a literal it is the
// first .rdata object and the checker reads its slot's 4 padding bytes as
// part of it (the original's next constant is another function's 12700.0f).
static const double kNegSeven = -0.7;
static const double kNegHalf = -0.5;
static const float kHundred = 100.0f;

// FUNCTION: 0x464f80
void __stdcall FUN_00464f80()
{
    g_game->field_14207->RunSearches();
    // bl is declared before the guard and pi before the first goto, or the jump
    // is rejected.
    unsigned char bl = 0;
    // `for (;;)` with loopCond at the top and more() at next_bl: keeps both the
    // entry guard and the latch test.
    for (;;) {
        if (!loopCond_00464f80(bl))
            goto next_bl;
        PlayerInfo_00464f80* pi;
        // Array form, not through pi: gives the original's load-then-lea order.
        if (g_game->players[bl].active == 0)
            goto next_bl;
        pi = &g_game->players[bl];

        {
            unsigned char t = pi->type;
            if (t != 1 && t != 2 && t != 3)
                goto next_bl;
        }
        if (pi->field_146 == 0xa)
            goto next_bl;
        {
            // Fresh pointer for the whole second group: stops it being merged with
            // the first.
            PlayerInfo_00464f80* pi2 = &g_game->players[bl];
            if (pi2->active == 0)
                goto next_bl;
            unsigned char t2 = pi2->type;
            if (t2 != 1 && t2 != 2 && t2 != 3)
                goto next_bl;
            if (pi2->field_146 == 0xa)
                goto next_bl;
        }

        if (pi->field_74 != 0)
            pi->field_74->TickIfActive();

        UpdatePlayerAI(bl);

        {
            Unit* u = pi->units;
            while (u <= pi->units_end) {
                if (u->flags_110 & 0x10000000)
                    UpdateUnitLineOfSight(u);
                u = (Unit*)((char*)u + 0x118);
            }
        }

        if (bl == g_game->field_2a43)
            DrawRadarUnits();

        if ((unsigned int)pi->field_f0 > g_game->tick)
            goto next_bl;
        pi->field_f0 += 0x1e;

        if (bl == g_game->localPlayer) {
            if (g_game->mode->GetGameType() == 1) {
                if (g_game->list->CheckVictory() == 0) {
                    if (((MissionConditions*)g_game->list)->CheckDefeat() != 0) {
                        if (g_game->field_39239 < 0) {
                            g_game->field_39239 = 4;
                        } else {
                            g_game->field_39239--;
                            if (g_game->field_39239 < 0) {
                                g_game->flags_3923b.w |= 4;
                                g_game->flags_3923b.w &= 0xffef;
                                g_game->flags_3923b.b.bit6 = 1;
                            }
                        }
                    }
                } else {
                    // This is a second copy of the countdown_extra block, and
                    // the duplication is load-bearing: with a `goto` here MSVC
                    // 5 leaves the `mov eax,[g_game]` reload after the `jne`
                    // and the jump lands on it, where the original hoists the
                    // reload above the branch and jumps past it. Written out
                    // twice, MSVC tail-merges the copies and hoists it.
                    if (g_game->field_39239 < 0) {
                        g_game->field_39239 = 4;
                    } else {
                        g_game->field_39239--;
                        if (g_game->field_39239 < 0) {
                            g_game->flags_3923b.w |= 4;
                            g_game->flags_3923b.b.bit4 = 1;
                            g_game->flags_3923b.b.bit5 = 1;
                        }
                    }
                    goto skip508;
                }
            } else if ((pi->active == 0 ||
                        (pi->data->flags_9b & 0x40) == 0) &&
                       ((MissionConditions*)g_game->list)->CheckDefeat() != 0) {
                if (g_game->field_39239 < 0) {
                    g_game->field_39239 = 4;
                } else {
                    g_game->field_39239--;
                    if (g_game->field_39239 < 0) {
                        if (g_game->field_37ef6 == 2) {
                            Player_00464f80* self =
                                g_game->players[FindHostSlot()].data;
                            // `unsigned int` with the 0xffff mask: avoids a spilled raw result.
                            unsigned int typeId;
                            typeId = FindUnitTypeId(
                                &g_game->startPos[0x232 *
                                    g_game->players[g_game->localPlayer].data->field_95]) & 0xffff;
                            int bound = 9999;
                            int typeOff = typeId * 0x249;
                            Pos_00464f80 pos;
                            do {
                                int cx = g_game->screen_x / 10;
                                int cy = g_game->screen_y / 10;
                                pos.x = (RandomInt(g_game->screen_x - 2 * cx) + cx) << 16;
                                pos.y = 0;
                                pos.z = (RandomInt(g_game->screen_y - 2 * cy) + cy) << 16;
                                // Declared in the order hh, hits, zacc, outer, hw: places the
                                // `shl` where the original has it.
                                int hh = g_game->screen_hh << 16;
                                int hits = 0;
                                unsigned int zacc =
                                    (unsigned int)pos.z - (unsigned int)hh;
                                int outer = 3;
                                int hw = g_game->screen_hw << 16;
                                do {
                                    unsigned int xacc =
                                        (unsigned int)pos.x - (unsigned int)hw;
                                    int inner = 3;
                                    Point16 cell;
                                    cell.y = zacc >> 20;
                                    do {
                                        cell.x = xacc >> 20;
                                        if (FUN_0047db70(
                                                (UnitDef_00464f80*)((char*)g_game->types + typeOff),
                                                0, cell, 1) != 0)
                                            hits++;
                                        xacc += hw;
                                    } while (--inner != 0);
                                    zacc += hh;
                                } while (--outer != 0);
                                if (hits >= 9 && FindFeatureAtPos(&pos, 0, 0) == -1) {
                                    if (g_game->mode->field_d44 == 0)
                                        break;
                                    if (GetCellMeanHeight(&pos) >
                                        (int)g_game->field_1427f)
                                        break;
                                }
                            } while (--bound > 0);

                            {
                                Unit* unit = CreateUnit(
                                    g_game->localPlayer, typeId, pos, 1, 1, 0);
                                SetStartingStorageBonus((Struct_00496e90*)pi,
                                             self->field_a3 * 100,
                                             self->field_a1 * 100);
                                {
                                    // The slot is written through a local
                                    // pointer because that is what makes MSVC 5
                                    // re-read unit->field_ec in the next block:
                                    // it cannot prove the store disjoint from
                                    // it. Written as `unit->field_bc = f` the
                                    // pointer is forwarded from the first block
                                    // instead and the second `mov eax,
                                    // [esi+0xec]` disappears.
                                    float* slot = &unit->field_bc;
                                    float f = (float)self->field_a1 * kHundred;
                                    if (unit->field_ec->active != 0 &&
                                        unit->field_ec->control == 2) {
                                        switch (g_game->field_37eee) {
                                        case 0: f = *slot - f * kNegHalf; break;
                                        case 1: f = *slot - f * kNegSeven; break;
                                        default: f = *slot + f; break;
                                        }
                                    } else {
                                        f = *slot + f;
                                    }
                                    *slot = f;
                                }
                                {
                                    float* slot = &unit->field_d4;
                                    float f = (float)self->field_a3 * kHundred;
                                    if (unit->field_ec->active != 0 &&
                                        unit->field_ec->control == 2) {
                                        switch (g_game->field_37eee) {
                                        case 0: f = *slot - f * kNegHalf; break;
                                        case 1: f = *slot - f * kNegSeven; break;
                                        default: f = *slot + f; break;
                                        }
                                    } else {
                                        f = *slot + f;
                                    }
                                    *slot = f;
                                }
                                RecalculateLineOfSight(1);
                                FUN_0048d630(1);
                            }
                        } else {
                            goto watch_check;
                        }
                    }
                }
            } else {
                goto check230;
            }
        }

    skip508:
        if (pi->active != 0) {
            unsigned char t = pi->type;
            if ((t == 1 || t == 2 || t == 3) && pi->field_146 != 0xa) {
                if ((pi->field_144 != 0 || pi->field_140 == 0) &&
                    (t == 1 || t == 2)) {
                    if ((g_game->flags_3923b.w & 4) == 0 &&
                        g_game->field_39239 < 0) {
                        UpdatePlayerEconomy(pi);
                    }
                }
            }
        }

        if (bl == g_game->field_2a43) {
            UpdateSensorRadarAndCloak();
            UpdateRadarMapped();
            if (g_game->mode->GetGameType() == 3) {
                DAT_0051e53c++;
                if ((DAT_0051e53c & 3) == 0)
                    SendPlayerEconomy(pi, 0, 0);
            }
        }
        goto next_bl;

    watch_check:
        if (g_game->mode->GetGameType() == 3 &&
            pi->field_22 == 0) {
            if ((g_game->players[FindHostSlot()].data->flags_9b & 0x80) != 0 ||
                FUN_00457bc0() > 0) {
                pi->data->fb.bit6b = 1;
                if (bl == g_game->localPlayer) {
                    g_game->field_14281 &= 0xfffe;
                    g_game->field_14281 &= 0xfffd;
                    RecalculateLineOfSight(1);
                    BroadcastPlayerInfo();
                    if (FUN_00457bc0() == 0) {
                        Dialog_00464f80* dlg = (Dialog_00464f80*)
                            LoadGuiLayer(g_game->gui, "YESORNO.GUI", 0x900);
                        if (dlg != 0) {
                            FUN_0049fb10(g_game->gui, 1);
                            Widget_00464f80* w = dlg->field_4;
                            FUN_004a0bf0(g_game->gui, "CHOICE1", "Yes", 0);
                            FUN_004a0bf0(g_game->gui, "CHOICE2", "No", 0);
                            FUN_004a0bf0(g_game->gui, "TITLE",
                                         "You're out!  Continue Watching?", 0);
                            strcpy(w->field_cc, "CHOICE1");
                            strcpy(w->field_dc, "CHOICE2");
                            dlg->field_8 = (void*)FUN_00464de0;
                            RenderLayer(g_game->gui, 0x40);
                        }
                        goto skip508;
                    }
                    if (FUN_00457cb0() <= 0)
                        goto skip508;
                    OpenMessageBox(g_game->gui,
                                 Translate("You are placed in watch mode because you are hosting AI players which are still alive.  If you exit, they will be terminated."),
                                 500, 1, 1);
                    g_game->flags_3923b.w &= 0xffef;
                    goto skip508;
                }
                goto skip508;
            }
        }

    flags82e:
        g_game->flags_3923b.w |= 4;
        g_game->flags_3923b.w &= 0xffef;
        if (pi->field_22 == 0)
            g_game->flags_3923b.b.bit6 = 1;
        goto skip508;

    check230:
        if (g_game->list->CheckVictory() != 0)
            goto countdown_extra;
        goto skip508;

    countdown_extra:
        if (g_game->field_39239 < 0) {
            g_game->field_39239 = 4;
        } else {
            g_game->field_39239--;
            if (g_game->field_39239 < 0) {
                g_game->flags_3923b.w |= 4;
                g_game->flags_3923b.b.bit4 = 1;
                g_game->flags_3923b.b.bit5 = 1;
            }
        }
        goto skip508;

    next_bl:
        if (!more_00464f80(++bl))
            break;
    }

    if (g_game->mode->GetGameType() == 3 &&
        g_game->field_37ef6 != 2 &&
        FUN_00457cb0() == 0) {
        if (g_game->field_39239 < 0) {
            g_game->field_39239 = 4;
            return;
        }
        g_game->field_39239--;
        if (g_game->field_39239 < 0) {
            g_game->flags_3923b.w |= 4;
            g_game->flags_3923b.w &= 0xffef;
            g_game->flags_3923b.b.bit6 = 1;
        }
    }
}

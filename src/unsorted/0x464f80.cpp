// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, refined by GPT-6.1-sol. Names are provisional.
// deepseek-v4.1-flash 2026-10-01 (retry 4, timeboxed): no new gains, stays at
// the 79.7% / 2385-byte best. Open sites unchanged: the duplicate player guard
// still CSEs, the typeId copy is `mov ecx,eax` (original `mov cx,ax`), and the
// three byte `or`s to flags_3923b/0x9b still go through dl instead of a direct
// `or byte ptr [mem],imm`; the dl spelling is used for plain `|=` writes in
// both the union member and the plain unsigned char member, so it is not
// union-specific.

// deepseek-v4.1-flash retry 2026-10-01: re-spelling the second type/field_146 reads as *(unsigned char*)((char*)pi + 0x73/0x146) does not defeat the CSE (79.7%, 2385 bytes, same 14 hunks), so the original reload at 0x46500a needs a source shape that recomputes the player pointer, not a different lvalue spelling.
// PARTIAL: 79.6% (was 72.2%). The frame is now the original 0x34 and the slot
// order matches (byte idx 0x10, player 0x14, hits 0x18, cell 0x1c, inner 0x20,
// outer 0x24, 9999 0x28, typeOff 0x2c, typeId 0x30, self 0x34, pos 0x38/0x3c/
// 0x40); the old extra slot came from the screen_hw step being spilled, so the
// step values are now shifted (hw<<16, hh<<16) before the loops and live in
// edi/ebp. Ours is still 21 bytes shorter and every branch target is shifted.
// Pass 2 (deepseek-v4.1): the missing loop head guard is now emitted: the
// counter must be tested through a single-use inlined helper
// (`static int loopCond(unsigned char i){ if (i >= 0xa) return 0; return 1; }`
// as the for condition), exactly as the 0x48ad30 fact on SHARED.md says; that
// blocks the trip-count pass. 79.6 -> 79.7, ours 2376 bytes. The helper's
// guard store is still scheduled before the cmp instead of after it.
// Still open: the duplicated player guard (0x464fe1..0x465024) is still CSE'd
// into one copy, so 38 bytes before 0x4655a6 are missing and every later
// branch target stays shifted; the typeId copy is `mov ecx,eax` where the
// original uses `mov cx,ax`; and two byte flags writes go through dl.
// Pass 3 (deepseek-v4.1, 4 check runs on scratch variants): the duplicate
// guard IS reproducible: give the second guard group its own player pointer
// (`PlayerInfo_00464f80* pi2 = &g_game->players[bl];` used for the active /
// type / field_146 tests). Module-wide CSE then cannot fold the loads, so
// `cmp dword ptr [edi],0` and `mov al, byte ptr [edi+0x73]` come back
// (build/scratch/0x464f80/v2.cpp and v4.cpp). The cost: MSVC then swaps the
// edi/esi roles (index in edi, player in esi) and emits one extra type cmp
// chain, at 2417 bytes / 79.5%, still under this file's 79.7. The plain
// respellings (g_game->players[bl] inline, casts, signed char temp) all stay
// CSE'd at 2376 bytes / 79.7.
// Pass 4 (deepseek-v4.1, 4 more check runs on variants): the duplicate guard
// group is the whole 31-byte front deficit (original group2 spans 0x465001 to
// 0x465029; ours has one shared type chain). Three respellings tried and all
// CSE'd to byte-identical 2376-byte output: (a) reading the second group
// through `char* pb = (char*)pi` with raw int/byte accesses, (b) a single-use
// `static int guard2(PlayerInfo*)` helper returning 1/0, called as
// `if (!guard2(pi)) continue;`, (c) same as (a) but re-taking
// `pi = &g_game->players[bl]` before the second group: this one does emit 2401
// bytes but drops to 75.6%, so it stays out. The loop shape is the same story:
// the original's head guard and every `continue` share one address (0x4655a6,
// the rotated increment block `inc bl / cmp bl,0xa / mov [esp+0x10],bl /
// jb 0x464fab`), while ours has the continue target 0xb bytes before the
// head-exit target. Still open: that merge, the `mov cx,ax` vs `mov ecx,eax`
// typeId copy (ours also spills the raw call result to [esp+0x30], +4 bytes),
// and the `or byte ptr [eax+0x3923b],0x10` that ours writes through dl.
// Pass 5 (deepseek-v4.1): that dl write is NOT caused by the `(unsigned char*)`
// cast: declaring flags_3923b as `union { unsigned short w; unsigned char b; }`
// and using `.b` for the byte wise writes is byte-identical (2376 / 79.7%),
// so the load-modify-store there is a scheduler choice, not a type-alias one.
// Pass 6 (deepseek-v4.1, 9 check runs on scratch variants): the *front* of
// the original's second guard IS reproducible. Writing the FIRST active test
// as `g_game->players[bl].active` (not through `pi`, which is declared right
// after it) makes MSVC emit the original load-then-lea sequence and keeps the
// second `cmp dword ptr [edi],0 / je` (it cannot prove the two expressions
// equal), so ours is now 2385 bytes with that 4-byte pair matching. MSVC
// still forwards the byte `al` from the first type test and deletes the
// second type and field_146 chains, so 18 of the 22 remaining bytes are still
// missing there. Forcing `al` interlopers (an int copy of active, a char*
// alias, a union member at +0x73, fresh `pi2 = &g_game->players[bl]`) either
// stays byte-identical or rotates edi/esi, so the reload is a register
// allocation choice, not a source alias one. The loop is also non-rotated in
// the original (head `cmp bl,0xa / mov [esp+0x10],bl / jae 0x4655a6` plus a
// tail `inc bl / cmp / mov / jb 0x464fab`): our for-loop emits only a rotated
// tail test that jumps to the store (`jb 0x464fa2`), and rewriting it as an
// `if (loopCond(bl)) do { ... } while (loopCond(++bl));` folds the entry test
// to true and drops it. Do not chase either further without a compiler-state
// lever.
// Pass 7 (deepseek-v4.1, 4 check runs): the pi2 shape is the ONLY guard lever found.
// Variant v4b (`PlayerInfo_00464f80* pi2 = &g_game->players[bl];` for the whole second
// guard group) reproduces the duplicated guard byte-for-byte and lands at 2401 bytes,
// but it swaps the loop pointers globally: index->edi, player->esi (75.6%). Passing the
// index through `unsigned char b2 = bl; &g_game->players[b2]` changes nothing (same
// 2401 / esi-edi swap), and byte-typed reads (`char t2 = ((char*)pi)[0x73];`,
// `char t2 = *(char*)&pi->type;`) still CSE against the first group's load, so MSVC
// keys that CSE on the POINTER VALUE, not the load's type or address.
// The original is thus likely pi-plus-fresh-pointer in the Cavedog source, with a
// register assignment we cannot steer from these respellings.
// Pass 8 (GPT-6.1-sol): changing the 0x488b10 declaration among unsigned short,
// int, and short, introducing owner-pointer locals, and moving the loop bound to
// an explicit top-of-loop break all retained 79.7%. The best remains 2385 bytes.
// Previous note: Still differs: the loop head test (cmp bl,0xa / jae taken to
// the increment)
// is dropped as provably true even as a while loop, the duplicated player
// guards (0x464fe1..0x465024) are CSE'd into one copy, the typeId copy is
// `mov ecx,eax` where the original uses `mov cx,ax`, and two byte flags writes
// go through dl (mov dl,[eax+0x39x]; or dl,imm; mov [eax+0x39x],dl) where the
// original uses a direct `or byte ptr [eax+0x39x], imm`.
#include <windows.h>
#include <string.h>

struct Unit_00464f80;
struct Player_00464f80;

struct Class_0040eb70 { void FUN_0040eb70(); };
struct Class_00408c40 { void FUN_00408c40(); };
struct Class_00435100 {
    char unknown_0[0xd44];
    int field_d44;                     // +0xd44
    int FUN_00435100();
};
struct Class_0048ff40 { int FUN_00490230(); };
struct Class_00490360 { int FUN_00490360(); };
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
    union { unsigned short w; unsigned char b; } flags_3923b;  // +0x3923b
};

#pragma pack(pop)

extern Game_00464f80* g_game;
extern int DAT_0051e53c;

void __stdcall FUN_0040b2c0(int player);
void __stdcall FUN_004827b0(Unit_00464f80* unit);
void FUN_00466dc0();
void FUN_00467440();
void FUN_00466c20();
unsigned char __stdcall FUN_00456850();
unsigned short __stdcall FUN_00488b10(const char* name);
int __stdcall FUN_004b6c30(int range);
int __stdcall FUN_0047db70(UnitDef_00464f80* type, int a, Point16 cell, int c);
short __stdcall FUN_00421da0(Pos_00464f80* pos, int a, int b);
int __stdcall FUN_00485140(Pos_00464f80* pos);
Unit_00464f80* __stdcall FUN_00485f50(unsigned char player, unsigned short typeId,
                                     Pos_00464f80 pos, int a, int b, int c);
void __stdcall FUN_00496e90(Struct_00496e90* obj, int height, int width);
void __stdcall FUN_004816a0(int on);
void __stdcall FUN_0048d630(int on);
void __stdcall FUN_00401360(PlayerInfo_00464f80* player);
void __stdcall FUN_004573d0(PlayerInfo_00464f80* player, int a, int b);
int __stdcall FUN_00457cb0();
int __stdcall FUN_00457bc0();
void __stdcall FUN_00450f90();
void* __stdcall FUN_004aa8f0(char* gui, const char* file, int flags);
void __stdcall FUN_0049fb10(char* gui, int a);
void __stdcall FUN_004a0bf0(char* gui, const char* gadget, const char* text, int a);
void __stdcall FUN_004a81e0(char* gui, int a);
const char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(char* gui, const char* text, int a, int b, int c);
void __stdcall FUN_00464de0(void* gadget);

static int loopCond_00464f80(unsigned char i)
{
    if (i >= 0xa)
        return 0;
    return 1;
}

// FUNCTION: 0x464f80
void __stdcall FUN_00464f80()
{
    g_game->field_14207->FUN_0040eb70();
    unsigned char bl;
    for (bl = 0; loopCond_00464f80(bl); bl++) {
        if (g_game->players[bl].active == 0)
            continue;
        PlayerInfo_00464f80* pi = &g_game->players[bl];

        {
            unsigned char t = pi->type;
            if (t != 1 && t != 2 && t != 3)
                continue;
        }
        if (pi->field_146 == 0xa)
            continue;
        if (pi->active == 0)
            continue;
        {
            unsigned char t2 = pi->type;
            if (t2 != 1 && t2 != 2 && t2 != 3)
                continue;
        }
        if (pi->field_146 == 0xa)
            continue;

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
            continue;
        pi->field_f0 += 0x1e;

        if (bl == g_game->localPlayer) {
            if (g_game->mode->FUN_00435100() == 1) {
                if (g_game->list->FUN_00490230() == 0) {
                    if (((Class_00490360*)g_game->list)->FUN_00490360() != 0) {
                        if (g_game->field_39239 < 0) {
                            g_game->field_39239 = 4;
                        } else {
                            g_game->field_39239--;
                            if (g_game->field_39239 < 0) {
                                g_game->flags_3923b.w |= 4;
                                g_game->flags_3923b.w &= 0xffef;
                                g_game->flags_3923b.b |= 0x40;
                            }
                        }
                    }
                } else {
                    goto countdown_extra;
                }
            } else if ((pi->active == 0 ||
                        (pi->data->flags_9b & 0x40) == 0) &&
                       ((Class_00490360*)g_game->list)->FUN_00490360() != 0) {
                if (g_game->field_39239 < 0) {
                    g_game->field_39239 = 4;
                } else {
                    g_game->field_39239--;
                    if (g_game->field_39239 < 0) {
                        if (g_game->field_37ef6 == 2) {
                            Player_00464f80* self =
                                g_game->players[FUN_00456850()].data;
                            unsigned short typeId;
                            typeId = FUN_00488b10(
                                &g_game->startPos[0x232 *
                                    g_game->players[g_game->localPlayer].data->field_95]);
                            int bound = 9999;
                            int typeOff = typeId * 0x249;
                            Pos_00464f80 pos;
                            do {
                                int cx = g_game->screen_x / 10;
                                int cy = g_game->screen_y / 10;
                                pos.x = (FUN_004b6c30(g_game->screen_x - 2 * cx) + cx) << 16;
                                pos.y = 0;
                                pos.z = (FUN_004b6c30(g_game->screen_y - 2 * cy) + cy) << 16;
                                int hw = g_game->screen_hw << 16;
                                int hh = g_game->screen_hh << 16;
                                int hits = 0;
                                unsigned int zacc =
                                    (unsigned int)pos.z - (unsigned int)hh;
                                int outer = 3;
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
                                if (hits >= 9 && FUN_00421da0(&pos, 0, 0) == -1) {
                                    if (g_game->mode->field_d44 == 0)
                                        break;
                                    if (FUN_00485140(&pos) >
                                        (int)g_game->field_1427f)
                                        break;
                                }
                            } while (--bound > 0);

                            {
                                Unit_00464f80* unit = FUN_00485f50(
                                    g_game->localPlayer, typeId, pos, 1, 1, 0);
                                FUN_00496e90((Struct_00496e90*)pi,
                                             self->field_a3 * 100,
                                             self->field_a1 * 100);
                                {
                                    float f = (float)self->field_a1 * 100.0f;
                                    if (unit->owner->active != 0 &&
                                        unit->owner->control == 2) {
                                        switch (g_game->field_37eee) {
                                        case 0: f = unit->field_bc - f * -0.5; break;
                                        case 1: f = unit->field_bc - f * -0.7; break;
                                        default: f = unit->field_bc + f; break;
                                        }
                                    } else {
                                        f = unit->field_bc + f;
                                    }
                                    unit->field_bc = f;
                                }
                                {
                                    float f = (float)self->field_a3 * 100.0f;
                                    if (unit->owner->active != 0 &&
                                        unit->owner->control == 2) {
                                        switch (g_game->field_37eee) {
                                        case 0: f = unit->field_d4 - f * -0.5; break;
                                        case 1: f = unit->field_d4 - f * -0.7; break;
                                        default: f = unit->field_d4 + f; break;
                                        }
                                    } else {
                                        f = unit->field_d4 + f;
                                    }
                                    unit->field_d4 = f;
                                }
                                FUN_004816a0(1);
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
        continue;

    watch_check:
        if (g_game->mode->FUN_00435100() == 3 &&
            pi->field_22 == 0) {
            if ((g_game->players[FUN_00456850()].data->flags_9b & 0x80) != 0 ||
                FUN_00457bc0() > 0) {
                pi->data->flags_9b |= 0x40;
                if (bl == g_game->localPlayer) {
                    g_game->field_14281 &= 0xfffe;
                    g_game->field_14281 &= 0xfffd;
                    FUN_004816a0(1);
                    FUN_00450f90();
                    if (FUN_00457bc0() == 0) {
                        Dialog_00464f80* dlg = (Dialog_00464f80*)
                            FUN_004aa8f0(g_game->gui, "YESORNO.GUI", 0x900);
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
                            FUN_004a81e0(g_game->gui, 0x40);
                        }
                        goto skip508;
                    }
                    if (FUN_00457cb0() <= 0)
                        goto skip508;
                    FUN_004abd90(g_game->gui,
                                 FUN_004c5740("You are placed in watch mode"),
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
            g_game->flags_3923b.b |= 0x40;
        goto skip508;

    check230:
        if (g_game->list->FUN_00490230() != 0)
            goto countdown_extra;
        goto skip508;

    countdown_extra:
        if (g_game->field_39239 < 0) {
            g_game->field_39239 = 4;
        } else {
            g_game->field_39239--;
            if (g_game->field_39239 < 0) {
                g_game->flags_3923b.w |= 4;
                g_game->flags_3923b.b |= 0x10;
                g_game->flags_3923b.b |= 0x20;
            }
        }
        goto skip508;

    }

    if (g_game->mode->FUN_00435100() == 3 &&
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
            g_game->flags_3923b.b |= 0x40;
        }
    }
}

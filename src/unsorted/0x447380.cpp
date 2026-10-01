// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by GPT-6.1-sol. Names are provisional.
//
// deepseek-v4.1-flash pass (issue 3538, 10 minute box): one check.py run, best
// stays 95.1% / 1318 bytes with the lstrcpynA-before-group probe. Residual is
// unchanged: the probe realigns the group to the original ecx/edx/eax/ecx but
// then the tail from FUN_0049fdf0 on is one allocator step off; the natural
// order is one step early at the group. No new spelling was found in this box;
// the invisible pre-group allocation described above is still the only fix.
//
// deepseek-v4.1-flash retry (issue 2844): kept the 95.1% probe (lstrcpynA before
// the four body sprintfs), which remains the best known. This pass re-confirmed
// the natural statement order (build/scratch/0x447380/base.cpp) is 93.5% and
// systematically probed the "one invisible register node before the body group"
// theory without moving the phase, all at 93.5% / 1318 bytes unless noted:
// inline accessors for p->active, p->name, p->info and g_game->localPlayer, a
// PlayerAt(i) accessor for &g_game->players[i], IsType/IsPlaying/IsCounted as
// bool (72.5%, bad) and as single-return bodies (81.5%, bad), IsDead/NoCount/
// HasIcon/IsActive helper splits, an unsigned char local for the type byte, a
// type-range test (86.7%), an Entry* alias used by all three entry calls, and
// folded tautology conjuncts (p->active == p->active, (p->type == 1) ==
// (p->type == 1), p->field_4 == p->field_4). A doubled direct `if (!p->active)`
// inside IsPlaying also folded away and did NOT advance the allocation phase, so
// the phase is not a simple node count and the invisible node stays unfound.
// The probe stays because its output is closer (group matches, tail one step
// off); see the older notes above for the exact residual.
// deepseek-v4.1 pass (issue 2633): the residual is a scratch-register rotation,
// not a structural difference. Byte-diffing ours against the original shows the
// first differing byte at +0x1ae (0x44752e, the first group `lea`) and the last
// inside the FUN_0049ff10 argument pair; the prefix 0x447380..0x44752c and the
// tail 0x447601..0x4478a4 are byte-identical. In the window the original is
// exactly one rotation step ahead of the natural statement order in the cycle
// eax -> ecx -> edx: group ecx/edx/eax/ecx (ours eax/ecx/edx/eax), lstrcpynA
// dest edx (ours ecx), FUN_0049fdf0 player lea eax (ours edx), entries ecx
// (ours eax), the inlined *0x15b lea chain edx/ecx/edx/ecx (ours ecx/...).
// Probed this pass, all still 93.5% / 1318 bytes: a coalesced copy of `n`
// (`int k = n;` used by all four group sprintfs), a real `Entry* ent = entries;`
// used by the fdf0/ff10 calls, a real `char* pname = p->name;` used by
// lstrcpynA and both strcats, `PlayerInfo* info2 = p->info;` used by the tail
// store, a named enum constant for the two 0x80 sizes, const-qualified helper
// parameters, `unsigned int act`, `unsigned int param_1`, `(entries + idx)->`,
// `&entries[0]`, and `f96` in a byte local. An empty inline helper call
// (`Dummy_00447380();`) and a local object with an empty inline ctor/dtor
// before the group are eliminated outright and do not move the phase either.
// Conclusion: the missing step is consumed by a real temporary in the original
// that our AST does not create, and it cannot be a dead node; the only
// constructs found to move the phase are whole statements that emit code (a
// duplicated `lstrcpynA(name, p->name, 0x80);` before the group, or an extra
// FUN_004a0570 call statement, both +19 bytes and 85% or worse), so no spelling
// reachable from this source shape gives the step for free.
// Scoring harness for more variants: build/scratch/0x447380/rank.py (reproduces
// the check.py ratio in about a second), bdiff.py (masked byte diff by region),
// batch*.py (variant batches applied to nat.cpp / cur.cpp).
// GPT-6 retry: helper return types and member forms did not improve 93.5%.
// 93.5%, and the code is exactly the right size (1318 bytes). Everything from the
// function entry to the end of the big `if` condition matches byte for byte, and
// so does everything from the end of the strcat block to the epilogue.
//
// What is left is ONE allocator phase, and it is a single cause: from the second
// "PLAYER%d/LOGO%d/ALLY%d/TEAMICONS%d" sprintf block (the one with `n`, inside the
// if) onwards the original hands out scratch registers exactly one step later in
// its rotation than we do. Ours starts that block at eax/ecx/edx/eax, the original
// at ecx/edx/eax/ecx, and every single allocation after it (the `name` lea, the
// FUN_0049fdf0 argument pair, the *0x15b scale chain, the entries reload) is off by
// the same one step, with identical semantics. The first such block (the one with
// `i`, at the loop top) matches byte for byte, so the phase is right at the loop
// top and wrong after the condition, with identical code in between: the original
// must build one more register-holding node across the a0570 group plus the
// condition than we do, from a node the front end folds away.
//
// Tried and did NOT help (all keep 1318 bytes, all stay 93.5%): swapping the
// declaration order of i and n; `int i = 0, n = 0;` in one statement; unsigned
// counters; moving the `entries` initialiser; passing `g_game->gui` or
// `&g_game->gui[0]` instead of `(char*)g_game + 0x519`; a file-scope
// `static const int 0` as the third a0570 argument; `(unsigned char)` casts on the
// type/field_146/0xff compares; reading p->type once into a local instead of three
// times; one-return `&&`/`||` helper bodies; the whole big condition wrapped in
// one inline helper. Things that change the size and are therefore wrong: dropping
// the `int act = p->active;` local (-8 bytes, and it is what produces the original's
// second `test eax,eax`), using `!act` for the repeat check (the two tests fold), a
// fully inlined condition (-8 bytes), and a shared inline helper for the eight
// sprintf/a0570 calls (not inlined at all, +98 bytes).
//
// The i/n prologue order IS fixed by `for (i = 0, n = 0; i < 10; i++)` with
// uninitialised `int i; int n;` above it: that is what moved this file 92.7% to
// 93.5% by putting `xor esi,esi`/`xor edi,edi` and the two stack stores in the
// original's order.
//
// DeepSeek V4.1 Flash retry, all flat at 93.5% (or worse) and all still 1318
// bytes unless noted: `while (i < 10) {...; i++;}` (69.3%), `n = 0;` before the
// for (92.5%), reversed `g_game->localPlayer != i` (93.2%), `(flags & 4) == 0`,
// `!param_1`, a hoisted `int cond;` holding the whole condition, single-line
// IsPlaying bodies, IsCounted with an `int r` result local (-4 bytes, 74.0%),
// `p` declared before the sprintf block (-17 bytes... +17 bytes, 65.8%),
// IsType as `(unsigned char)(p->type - 1) < 3` (-44 bytes, 76.6%), and an
// Info96 helper. The one-step scratch rotation that begins at the second
// sprintf block is unchanged by all of them; the extra register-holding node
// the original builds across the loop-top a0570 group plus the condition is
// still not identified.
// deepseek-v4.1 sweep, all flat at 93.5% / 1318 bytes (scratch vA..vZ in
// build/scratch/0x447380/): splitting the && chain at the cond2/cond3 and at the
// cond4 boundary (nested ifs), a folded `&& 1`, `!x` vs `x == 0` for param_1, for
// flags_2a44, and in all three inline helpers, `IsPlaying(...) != 0`, swapped
// `0xff != p->info->field_96` compare operands, reference parameters for the
// helpers, a reference local for p, a dead `PlayerInfo_00447380* info = p->info;`
// first statement in the body, an `int idx;` declared at the top of the body, and
// `&player[0]`-style sprintf destinations. So the condition tree shape is inert:
// the one-step rotation is seeded between the loop-top a0570 group and the body
// and is not reachable by any condition or helper spelling tried so far.
// deepseek-v4.1: 95.1% with lstrcpynA placed BEFORE the four body sprintfs.
// The reorder is semantically identical (the four buffers and `name` do not
// overlap) but it is NOT the original's order: the exe emits the sprintf group
// at 0x44752c and lstrcpynA at 0x447578. Its value is as a probe: a call
// statement in front of the group moves the group's scratch rotation from
// eax/ecx/edx/eax (93.5% version) to ecx/edx/eax/ecx, which is exactly the
// original's. So the missing thing is one register-holding node allocated
// before the group inside the body block, and it is not reachable from
// condition spellings or dead locals.
// 95.1% notes: the group now matches; the tail after it is still one step off
// (lea edx/ecx for lstrcpynA, lea eax/edx for the FUN_0049fdf0 buffer, the
// *0x15b lea chain and the entries reload), and our fdf0 statement also hoists
// the entries load above the add esp,0xc.
//
// STILL DIFFERS (deepseek-v4.1, final state of this attempt): the natural
// statement order (four body sprintfs, then lstrcpynA, kept as
// build/scratch/0x447380/nat.cpp) emits the group at eax/ecx/edx/eax and is
// then uniformly exactly ONE rotation step early for the whole run from the
// group through the FUN_0049fdf0 chain and the FUN_0049ff10 argument pair
// (lstrcpynA dst ecx vs edx, player lea edx vs eax, entries eax vs ecx, the
// chain ecx/edx vs edx/ecx); it realigns inside the inlined strcat block and
// matches again from 0x447601 to the epilogue. Adding one allocator step
// before the body group would fix the whole region at once, but the step
// cannot come from moving lstrcpynA (that also removes one step after the
// group, so the tail stays at the natural phase, which is what the 95.1
// variant shows) and it cannot come from a dead expression: p->active,
// p->type, p->field_146, p->info->field_96, p->name, entries, entries->field_0,
// g_game->flags_2a44 and an empty `if (p->active) {}` read before the group are
// all eliminated before register allocation and leave the phase untouched.
// build/scratch/0x447380/rank.py reproduces check.py's difflib score locally.
// deepseek-v4.1 pass (issue 2762): re-verified against the original. This file
// keeps the 95.1% probe on purpose (the comment above says why it is not the
// original's statement order). The natural order is in
// build/scratch/0x447380/nat.cpp and scores 93.51%: its body group starts one
// rotation step early (eax/ecx/edx/eax vs ecx/edx/eax/ecx) and everything to
// 0x447600 follows it, realigning inside the inlined strcat. The probe spends
// one allocation in front of the group (so the group matches) and the tail is
// still one step off: ours emits player->edx and entries->eax for the
// FUN_0049fdf0 pair, then the chain ecx/edx/ecx/edx, where the original has
// player->eax, entries->ecx and edx/ecx/edx/ecx, and ours hoists the entries
// reload above `add esp,0xc`. Six further tail spellings were tried this pass
// (`char* pbuf = player;`, `char* dst = name;`, `int idx;` assigned on its own
// line, a local `Entry_00447380* ent = entries;`, `(char*)player`, `&player[0]`)
// and all six stay at 95.06% / 1318 bytes, so the pair's register choice is not
// reachable from the call expression's spelling.
// GPT-6.1-sol retry (issue 3011): best remains 95.1% with lstrcpynA before all four body sprintfs. A lookupName alias had no effect; moving lstrcpynA after the first, second, or third sprintf scored 94.3%, 93.0%, and 92.7%, respectively. The remaining diff is the tail lookup argument/register allocation already described below.
// deepseek-v4.1-flash pass (issue 3460, 10 min): one check run; an `Entry* ent = entries;` alias feeding both FUN_0049fdf0 and FUN_0049ff10 on top of this probe is byte-neutral (95.1%, 1318 bytes), same as without it. Residual is still exactly the two hunks: the hoisted lstrcpynA block (ours at 0x44752e, original after the fourth body sprintf) and the eax/ecx/edx rotation of the FUN_0049fdf0 result chain and FUN_0049ff10 argument pair.
// deepseek-v4.1-flash pass (issue 3485, 2 recorded check runs on top of the
// 95.1 probe): natural order + a reused `char* dst` for the four body sprintf
// destinations is byte-neutral (93.5%, 1318 bytes), so the missing step is not
// a named destination node. The rotation is a strict eax/ecx/edx allocator
// cursor: the original sits exactly ONE allocation ahead of our source for the
// whole body, and moving lstrcpynA before the group shifts the cursor and the
// entire tail with it, which is why the probe realigns the group but not the
// tail. Only an invisible allocation before the group can fix both.
#include <stdio.h>
#include <string.h>
#include <windows.h>

#pragma pack(push, 1)
struct PlayerInfo_00447380 {
    char unknown_0[0x94];
    unsigned char field_94;             // +0x94
    char unknown_95[0x96 - 0x95];
    unsigned char field_96;             // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned char flags_9b;             // +0x9b
};

struct Player_00447380 {                // 0x14b bytes
    int active;                         // +0x00
    int field_4;                        // +0x04
    char unknown_8[0x27 - 0x8];
    PlayerInfo_00447380* info;          // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    unsigned char type;                 // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;             // +0x13f
    int field_140;                      // +0x140
    short field_144;                    // +0x144
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Entry_00447380 {                 // 0x15b bytes
    unsigned char field_0;              // +0x00
    char unknown_1[0x1b - 1];
    int field_1b;                       // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char field_29;             // +0x29
    char unknown_2a[0xbe - 0x2a];
    int field_be;                       // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;            // +0xc6
    unsigned int field_c8;              // +0xc8
    char unknown_cc[0x15b - 0xcc];
};

struct Layer_00447380 {
    int unknown_0;
    Entry_00447380* entries;            // +0x4
};

struct Game_00447380 {
    char unknown_0[0x519];
    char gui[0x531 - 0x519];            // +0x519
    Layer_00447380* table;              // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_00447380 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned char flags_2a44;           // +0x2a44
    char unknown_2a45[0x148db - 0x2a45];
    int field_148db;                    // +0x148db
};
#pragma pack(pop)

extern Game_00447380* g_game;

int __stdcall FUN_0049fdf0(Entry_00447380* entries, char* name, int type);
Entry_00447380* __stdcall FUN_0049ff10(Entry_00447380* entries, char* name);
Entry_00447380* __stdcall FUN_004a0280(Entry_00447380* entries, char* name);
void __stdcall FUN_004a0570(void* obj, char* name, int value);
void __stdcall FUN_004a0bf0(void* obj, char* name, char* text, int size);
void __stdcall FUN_004a1450(void* obj, char* name, int value);
void __stdcall FUN_0049f930(void* obj, char* name, char* text);

static inline int IsType_00447380(Player_00447380* p)
{
    return p->type == 1 || p->type == 2 || p->type == 3;
}

static inline int IsPlaying_00447380(Player_00447380* p)
{
    int act = p->active;
    if (!act)
        return 0;
    if (p->info->flags_9b & 0x40)
        return 0;
    if (!p->active)
        return 0;
    if (!IsType_00447380(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    return 1;
}

static inline int IsCounted_00447380(Player_00447380* p)
{
    if (!IsType_00447380(p))
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

// FUNCTION: 0x447380
void __stdcall FUN_00447380(int param_1)
{
    Entry_00447380* entries = g_game->table->entries;
    int i;
    int n;
    Player_00447380* local = &g_game->players[g_game->localPlayer];
    char player[20];
    char teamicons[20];
    char ally[20];
    char live[20];
    char logo[20];
    char name[0x80];

    for (i = 0, n = 0; i < 10; i++) {
        sprintf(player, "PLAYER%d", i);
        sprintf(logo, "LOGO%d", i);
        sprintf(ally, "ALLY%d", i);
        sprintf(teamicons, "TEAMICONS%d", i);
        FUN_004a0570((char*)g_game + 0x519, player, 0);
        FUN_004a0570((char*)g_game + 0x519, logo, 0);
        FUN_004a0570((char*)g_game + 0x519, ally, 0);
        FUN_004a0570((char*)g_game + 0x519, teamicons, 0);

        Player_00447380* p = &g_game->players[i];
        if (IsPlaying_00447380(p)
            && (i != g_game->localPlayer || param_1 == 0)
            && (!(g_game->flags_2a44 & 4) || IsCounted_00447380(p))
            && p->info->field_96 != 0xff) {
            lstrcpynA(name, p->name, 0x80);
            sprintf(player, "PLAYER%d", n);
            sprintf(logo, "LOGO%d", n);
            sprintf(ally, "ALLY%d", n);
            sprintf(teamicons, "TEAMICONS%d", n);

            int idx = FUN_0049fdf0(entries, player, 0xe);
            if (entries[idx].field_0 == 1) {
                Entry_00447380* e = FUN_0049ff10(entries, player);
                if (e != 0 && (e->field_1b & 0x4000)) {
                    strcat(name, "|");
                    strcat(name, p->name);
                }
            }

            FUN_004a0bf0((char*)g_game + 0x519, player, name, 0x80);
            FUN_004a0570((char*)g_game + 0x519, player, 1);
            sprintf(live, "LIVEPLYR%d", i);
            FUN_0049f930((char*)g_game + 0x519, player, live);

            if (p->active != 0
                && IsType_00447380(p)
                && p->field_146 != 10
                && (p->field_144 != 0 || p->field_140 == 0)
                && p->type != 1
                && p->type != 2
                && !(p->type == 3 && p->info->field_94 == 2)) {
                Player_00447380* q = &g_game->players[g_game->localPlayer];
                if (q->active != 0
                    && IsType_00447380(q)
                    && q->field_146 != 10
                    && (q->field_144 != 0 || q->field_140 == 0)) {
                    FUN_004a0570((char*)g_game + 0x519, ally, 1);
                }
            }

            sprintf(live, "LIVEALLY%d", i);
            FUN_0049f930((char*)g_game + 0x519, ally, live);

            if (p->alliance == local->alliance && p->alliance != 5) {
                FUN_004a1450((char*)g_game + 0x519, live, 1);
            }

            FUN_004a0570((char*)g_game + 0x519, teamicons, 1);

            int value;
            if (p->active != 0 && (p->type == 1 || p->type == 2)
                && !(g_game->flags_2a44 & 4)) {
                value = 0;
            } else {
                value = 1;
            }
            FUN_004a1450((char*)g_game + 0x519, teamicons, value);

            Entry_00447380* e2 = FUN_004a0280(entries, logo);
            if (e2 != 0) {
                e2->field_29 = 1;
                e2->field_be = g_game->field_148db;
                e2->field_c6 = p->info->field_96;
                e2->field_c8 &= ~1;
            }

            n++;
        }
    }
}

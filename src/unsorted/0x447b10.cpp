// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by space-bunny-free, rewritten by claude-opus-5-5, finished by claude-opus-5-5, finished by GPT-6. Names are provisional.
// Codex GPT-6 retry for #5183 (2026-10-03): current main remains 99.2%,
// 4305 bytes against 4306. MAP ? 1 : MAPNAME regresses to 99.0%; /Gi
// leaves the sole `push ebp` versus `push 1` byte unchanged.
// GPT-6 retry (#5232): rechecked at 99.2%; the MAP call still pushes ebp.
// #5316 Codex retry: re-confirmed 99.2%; the MAP body's constant push remains
// in the ebp live range, as described by the C2 trace below.
// #5341 retry: reversing the f97 MAP-view branch scored 97.0%, so the original
// 99.2% source is restored; the immediate-versus-ebp push remains.
// #5367 retry: re-confirmed 99.2%; the MAP call still pushes ebp instead of 1.
// #5383 retry: re-confirmed 99.2%; the single MAP argument push remains.
// #5397 retry: still 99.2%; the shared constant-1 live range still changes
// only the MAP call from `push 1` to `push ebp`. Issue #4841 adds no new lead
// for this branch, and its proposal-loop results do not suggest a useful
// near-miss experiment beyond the source and C2 probes already recorded.
// claude-opus-5-5 retry (#5412, 2026-10-04): still 99.2%. Traced the regions
// of every joint split (FUN_00438f79) that holds const 1: 10 candidates when
// the loop temp takes ebx, 12 when START's g_game takes ebp, 24 when i takes
// ebp. A block joins the region of its first predecessor that still shares
// a free register with it. In each split the region holding the after-loop
// code and the button chain has exactly one free register left (ebp, ebp,
// then esi), and the MAP tests and body block only eax, ecx and edx, so the
// MAP test, the MAPNAME test and the body always join that region. Split
// points then land only at blocks with a successor in another region: in
// the 12-candidate split `done` is in START's region (its first predecessor
// is START's `goto done`), so every branch end that jumps to `done` gets a
// split point at its end. MAP's body is cut off (push 1, no reload) only
// when every edge into it leaves such a block: MAP alone (the MAP test jumps
// to done) or a materialised condition (one test block feeds both the body
// and done). With MAP || MAPNAME only the MAPNAME test reaches done, so the
// MAP test's edge stays uncut and the body needs a reload (99.0%); the
// do/while here, or two labels at done (START's goto to the second, the
// chain falling into the first, 99.2% without the do/while), funnels the
// branch ends through one empty block and drops even that split point. No
// label placement can cut the MAP test's edge: both its successors join its
// region. So the original's allocation must reach this split in a different
// state; the source differences that would do that are still unknown.
// Also flat: 0x444be0 inlined with every label combination, a do/while
// inserted at each of 323 statement positions (with and without the MAP
// one), the MAP test as `goto` into the body, the body placed before the
// tests or after the return (C2 reorders the blocks back), /Gi on those.
// START's and MAP's FUN_00444ea0 tails jumping to one label are merged by
// C2 only after allocation (97.0%), so sharing them changes nothing here.
// Battle room button handler: per player slot LOGO, PLAYER, SIDE, ALLY,
// TEAMICONS, RES and READY, then PREVMENU, MESSAGE, COMMANDER, LOSTYPE,
// WATCHING, CHEATING, FIXEDLOC, MAPPING, START, GAMEOPEN, RESTRICTIONS and
// MAP/MAPNAME, and finally FUN_004ab0a0 on the gadget.
//
// Best so far (99.2%). The MAP branch's do/while wrapper removes the redundant
// `mov ebp, 1`; the only remaining code difference is `push ebp` instead of the
// original's `push 1` when calling FUN_0049fb10.
// What paid, in the order it was found:
// - <windows.h> and real Player/Game structs: the loop head builds the player
//   pointer from the spilled i*331 the way the original does.
// - Functions of this file with no callers, defined above unannotated and
//   inlined: 0x440c10 (free slot search, in PLAYER; it needs `inline`, MSVC
//   does not inline its two loops on its own), 0x440cd0 (map check, in READY)
//   and 0x446f50 (colour cycle, in TEAMICONS; `player` declared before
//   `colour`, which still matches 0x446f50 out of line). START's team test is
//   the CountAlliance helper of 0x446a50 with the flag byte read once, as in
//   0x4478b0 (reading g_game->bits.bit2 in the loop, as 0x446a50 does, gives
//   the same bytes).
// - The text buffer is 249 or 250 bytes (rounded to 252 in the frame): with
//   the inlined 0x440c10's used[10] that puts used[] above the text, as in the
//   original frame (frame order is references per byte; 251 or 252 bytes puts
//   used[] below). This replaces an older struct that forced the order.
// - g_game+0x2bee is one 1-bit unsigned short bitfield (`dirty`). In the tail
//   MSVC hoists the constant 1 into ebp, which gives the original's
//   `or word ptr [..],bp` next to the plain `or byte ptr [..],1` ones.
// - The slot loop as `while (1) { ...; i++; if (i >= 10) break; }`: the plain
//   for loop gives the tail's registers to the wrong values.
// - The map check's version test is `if (major >= 2) check = 1; else if
//   (major == 1 && minor >= 2) check = 1;`, as 0x448c70 needs too; the one-`if`
//   form swaps g_game and `check` (edx/edi) there.
// - `goto done;` on START's "no map selected" path (93.8% to 99.0%). It emits
//   nothing, but with it the gadget is kept in esi only for the chain of
//   button tests and the final FUN_004ab0a0 reloads it from the stack, as in
//   the original; without it the gadget stays in a register through every
//   branch and MSVC copies the final call into each one (88.7%, 4423 bytes).
//   The old `goto done;` at the end of MESSAGE is not needed with it. Only
//   START's placement works: a goto in GAMEOPEN or RESTRICTIONS undoes it, the
//   other branches change nothing.
//
// STILL DIFFERS:
// - MAP: FUN_0049fb10(gui, 1) pushes the hoisted 1 (`mov ebp,1` on entry,
//   `push ebp`) where the original pushes an immediate 1. A `do { } while (0)`
//   around the f97 if/else (found by the permuter) drops the `mov ebp,1` but
//   still pushes ebp (99.2%, 1 byte short); two permuter runs from this file
//   (about 17,800 candidates) stop there; a fresh `1u` probe also gives the
//   same push-ebp byte. Not moved by: 0x444be0 (the
//   VIEWMAP code, no callers) as an inlined helper, char/bool/short/pointer
//   parameter types, the f97 test as a byte mask or an inline helper, `!f97`
//   with the arms swapped, a block-scope info local, MAP and MAPNAME as two
//   branches, the MAP condition spelled with `!= 0` or `|| 0`, every subset
//   of `goto done;` at the ends of the other branches, inline helpers for the
//   button test and the dirty flag, and `!x` / `^= 1` for the bitfield
//   stores. With the MAP condition written as `MAP ? 1 : MAPNAME` the push is
//   the original's `push 1`, but the condition then materialises the 1.
//
// claude-opus-5-5 retry (#5281, 2026-10-03): still 99.2%. Read with
// tools/c2prio.py plus hooks on C2's FUN_00438f79 (it splits the constants
// into regions and inserts reload markers at region edges): const 1 is one
// candidate for the whole function (every spelling and the inlined 0x444be0
// share it), and the web that gets ebp runs from the after-loop dirty store
// through MAPPING, skips START (no register free there) and reaches GAMEOPEN,
// RESTRICTIONS and MAP through START's false edge. In the original MAP's body
// is outside that web, so its push stays an immediate. What decides it is
// whether the edges into MAP's body get a region marker:
// - MAP alone as the condition (no MAPNAME test) and no do/while: the marker
//   at the end of the MAP test (for its edge to `done`) cuts the body off and
//   the push is the original's `push 1`.
// - MAP || MAPNAME and no do/while: only the MAPNAME test's end gets a
//   marker, the body stays reachable from the MAP test, and C2 reloads ebp
//   at the body (a second `mov ebp,1`, 99.0%).
// - With the do/while (this file) the branch ends no longer reach `done`
//   directly in C2's graph, no markers land in MAP, and the body is in the
//   ebp web (push ebp, 99.2%).
// A condition that materialises its value (a bool, char or int local or
// inline helper, `(bool)(MAP || MAPNAME)`, `(MAP ? 1 : MAPNAME) != 0`)
// also cuts the web and gives `push 1`, but keeps the `mov eax,ebp` and test
// (best 99.1%); MSVC 5 never threads such a test away later (checked on a
// small file). So the original must reach MAP's body from both tests with a
// marker on both edges and no reload; no spelling tried does that (nested
// ifs with `goto done`, `!A && !B`, two branches, a goto into the body, a
// do/while placed anywhere in MAP or around the whole chain with `break`).
// Flat (byte-identical to this file): the 1 as a local, `true`, `1u`,
// `sizeof(char)` or through an inline wrapper; a do/while in RESTRICTIONS;
// `!` in GAMEOPEN; the f97 test as `!= 0`, `== 1` or a byte mask; 0 to 10946
// unused externs at the top, 0 to 512 before the function and 0 to 20000 at
// the end.
//
// Measured with the volatile read diagnostics in build/scratch only: making
// the final FUN_004ab0a0 read the gadget opaquely gives the original's whole
// tail allocation except MAP (99.2%), which is what pointed at the gadget's
// live range through the branch bodies.
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00447b10 {
    char unknown_0[0x94];
    char kind;                          // +0x94
    unsigned char side;                 // +0x95
    unsigned char slot;                 // +0x96
    unsigned char f97_0 : 1;            // +0x97
    unsigned char f97_rest : 7;
    char unknown_98[0x9b - 0x98];
    union {
        unsigned char flags_9b;         // +0x9b
        struct {
            unsigned short low : 4;
            unsigned short started : 1;
            unsigned short ready : 1;
            unsigned short bit6 : 1;
            unsigned short watching : 1;
            unsigned short mapping : 1;
            unsigned short los : 1;
            unsigned short losType : 1;
            unsigned short commander : 2;
            unsigned short cheating : 1;
            unsigned short fixedloc : 1;
            unsigned short closed : 1;
        } b;
    };
    unsigned char f9d_0 : 2;            // +0x9d
    unsigned char f9d_2 : 1;
    unsigned char f9d_rest : 5;
    char unknown_9e[0xa7 - 0x9e];
    unsigned char versionMajor;         // +0xa7
    unsigned char versionMinor;         // +0xa8
    unsigned int mapCrc;                // +0xa9
};

struct Player_00447b10 {                // 0x14b bytes
    int active;                         // +0x00
    int id;                             // +0x04
    unsigned int time;                  // +0x08
    char unknown_c[0x27 - 0xc];
    PlayerInfo_00447b10* info;          // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    char type;                          // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char ally[0xb];            // +0x108
    unsigned char ally2[0xb];           // +0x113
    char unknown_11e[0x13f - 0x11e];
    unsigned char colour;               // +0x13f
    int field_140;                      // +0x140
    short field_144;                    // +0x144
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];

    void FUN_00463c60(int state);
};

struct Entry_00447b10 {
    char unknown_0[0xb6];
    char text[0xcc - 0xb6];             // +0xb6
    char label[0x15b - 0xcc];           // +0xcc
};

struct Layer_00447b10 {
    int unknown_0;
    Entry_00447b10* entries;            // +0x4
    char unknown_8[0x20 - 0x8];
    int field_20;                       // +0x20
};

struct Gadget_00447b10 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);   // +0x8
    char unknown_c[0x18 - 0xc];
    Layer_00447b10* table;              // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                       // +0x60
};

struct Options_00447b10 {
    char unknown_0[0x118];
    int fixedloc;                       // +0x118
};
#pragma pack(pop)

class Class_004358f0 {
public:
    int FUN_004358f0();
};
class Class_00435c40 {
public:
    bool FUN_00435c40();
};
class Class_004373a0 {
public:
    unsigned int FUN_004373a0();
};
class Class_0046df40 {
public:
    char* FUN_0046df40();
};
class Class_004618a0 {
public:
    int FUN_004618a0(int value);
};

#pragma pack(push, 1)
struct Game_00447b10 {
    char unknown_0[0x499];
    int field_499;                      // +0x499
    char unknown_49d[0x519 - 0x49d];
    char gui[0x531 - 0x519];            // +0x519
    Layer_00447b10* table;              // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_00447b10 players[10];        // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Options_00447b10* options;          // +0x29a0
    char unknown_29a4[0x2a30 - 0x29a4];
    Class_0046df40* net;                // +0x2a30
    char unknown_2a34[0x2a3c - 0x2a34];
    unsigned short field_2a3c;          // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43;
    unsigned char flags_2a44;           // +0x2a44
    char unknown_2a45[0x2a9b - 0x2a45];
    void* list;                         // +0x2a9b
    char unknown_2a9f[0x2bc0 - 0x2a9f];
    char state;                         // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned short dirty : 1;           // +0x2bee
    unsigned short dirty_rest : 15;
    char unknown_2bf0[0x37eee - 0x2bf0];
    int field_37eee;                    // +0x37eee
    char unknown_37ef2[0x37f39 - 0x37ef2];
    int sides;                          // +0x37f39
    char unknown_37f3d[0x391e9 - 0x37f3d];
    Class_004358f0* map;                // +0x391e9
    char unknown_391ed[0x39229 - 0x391ed];
    int commander;                      // +0x39229
    int mapping;                        // +0x3922d
    int los;                            // +0x39231
    int losType;                        // +0x39235
};
#pragma pack(pop)

extern Game_00447b10* g_game;
extern int DAT_00506dbc;
extern int DAT_00512994;
extern Class_004618a0 DAT_00513000;

void __cdecl FUN_004d85a0(void* data);
void FUN_00430f00();
void FUN_00444a20();
void __stdcall FUN_00444ba0(void* gadget);
void FUN_00444ea0();
void __stdcall FUN_00446080(int index);
void FUN_00446310();
void FUN_00446a50();
void FUN_00446c70();
void __stdcall FUN_00446e90(Player_00447b10* player);
void FUN_0044c7e0();
void FUN_00450f90();
void FUN_00451180();
int __stdcall FUN_00451220(unsigned char player, int state);
void __stdcall FUN_004526c0(int slot);
void __stdcall FUN_00452960(int a, int b, unsigned char allied, int d);
void __stdcall FUN_00452bd0(Player_00447b10* player);
void __stdcall FUN_00453010(int id, unsigned char msg);
unsigned char FUN_00456850();
int FUN_00457a50();
int FUN_00457af0();
int FUN_00457b40();
int FUN_00457b90();
unsigned int FUN_004b6340();
void __stdcall FUN_00463ca0(char* text, int a, int b, int c);
void __stdcall FUN_00463e50(Player_00447b10* p, char* text, int a, int b);
void __stdcall FUN_0046c620(int sound);
void __stdcall FUN_0047f1a0(char* sound, int b);
int __stdcall FUN_004288d0(char* name, int a, int b, int c);
void __stdcall FUN_0049fb10(char* gui, int value);
int __stdcall FUN_0049fd60(Gadget_00447b10* gadget, char* name);
int __stdcall FUN_0049fdf0(Entry_00447b10* entries, char* name, int type);
Entry_00447b10* __stdcall FUN_004a0010(Entry_00447b10* entries, char* name);
int __stdcall FUN_004a0f30(char* gui, int index);
int __stdcall FUN_004a0f60(Gadget_00447b10* gadget, char* name);
int __stdcall FUN_004a1080(Gadget_00447b10* gadget, char* name, int value);
void __stdcall FUN_004a1110(char* gui, char* name, int value);
void __stdcall FUN_004a5f40(Gadget_00447b10* gadget, int value);
void __stdcall FUN_004a7190(char* gui, int index);
void __stdcall FUN_004a81e0(char* gui, int value);
Gadget_00447b10* __stdcall FUN_004aa8f0(char* gui, char* name, int flags);
void __stdcall FUN_004ab0a0(void* gadget);
void __stdcall FUN_004abd90(void* gadget, char* text, int a, int b, int c);
char* __stdcall FUN_004c5740(char* text);

static inline int IsPlaying_00447b10(Player_00447b10* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_00447b10(Player_00447b10* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int IsLocalHuman_00447b10(Player_00447b10* p)
{
    return p->active != 0 && p->type == 1;
}

static inline int IsRemoteHuman_00447b10(Player_00447b10* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 1;
}

static inline int IsLocal_00447b10(Player_00447b10* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

static inline int CountAlliance_00447b10(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    unsigned char f = (g_game->flags_2a44 >> 2) & 1;
    for (int j = 0; j < 10; j++) {
        Player_00447b10* q = &g_game->players[j];
        if (f) {
            if (q->colour == alliance && IsPlaying_00447b10(q) && IsCounted_00447b10(q))
                count++;
        } else {
            if (q->colour == alliance && IsPlaying_00447b10(q))
                count++;
        }
    }
    return count;
}

// The free slot search at 0x440c10, which has no callers. MSVC inlines it only
// when it is declared inline (it has two loops).
inline int FUN_00440c10()
{
    int used[10];
    memset(used, 0, sizeof(used));
    for (int i = 0; i < 10; i++) {
        Player_00447b10* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->field_146 != 10)
            used[p->info->slot < 9 ? p->info->slot : 9] = 1;
    }
    int result = 0;
    for (int j = 0; j < 10; j++) {
        if (!used[j]) {
            result = j;
            break;
        }
    }
    return result;
}

// The map check at 0x440cd0, which has no callers: /Ob2 inlined it.
int FUN_00440cd0()
{
    if (!g_game->map->FUN_004358f0()) {
        return 0;
    }
    unsigned char me = FUN_00456850();
    PlayerInfo_00447b10* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players[me].info;
        if (data->versionMajor >= 2)
            check = 1;
        else if (data->versionMajor == 1 && data->versionMinor >= 2)
            check = 1;
    }
    if (!check) {
        return 1;
    }
    if (((Class_004373a0*)g_game->map)->FUN_004373a0() != data->mapCrc)
        return 0;
    return 1;
}

// The colour cycle at 0x446f50, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00446f50(int index)
{
    Player_00447b10* player = &g_game->players[index];
    int colour = g_game->players[index].colour;
    FUN_00446e90(player);
    player->colour = (colour + 1) % 6;
    FUN_00452bd0(player);
    FUN_00446c70();
    FUN_00446a50();
}

// FUNCTION: 0x447b10
void __stdcall FUN_00447b10(Gadget_00447b10* gadget)
{
    char text[250];
    Entry_00447b10* entries = gadget->table->entries;

    if (gadget->field_60 == -1) {
        FUN_004d85a0(g_game->list);
        g_game->list = 0;
        DAT_00512994 = 0;
        FUN_00446c70();
        return;
    }

    int lp = g_game->localPlayer;
    Player_00447b10* me = &g_game->players[lp];
    int canAdd = FUN_00457a50();
    int i = 0;
    while (1) {
        Player_00447b10* p = &g_game->players[i];

        sprintf(text, "LOGO%d", i);
        if (FUN_0049fd60(gadget, text) && IsLocal_00447b10(p)) {
            FUN_0047f1a0("Multi", 0);
            FUN_004526c0(p->info->slot + 1);
            g_game->dirty = 1;
            FUN_00450f90();
        }

        sprintf(text, "PLAYER%d", i);
        if (FUN_0049fd60(gadget, text) && i != lp) {
            FUN_0047f1a0("Multi", 0);
            char type = p->type;
            if (type == 0 && canAdd) {
                p->FUN_00463c60(4);
                p->id = -1;
                g_game->field_499--;
            } else if (type != 4 && type != 0) {
                if (p->active != 0 && type == 2 && FUN_004b6340() - p->time > 30) {
                    FUN_00453010(p->id, 1);
                    p->FUN_00463c60(0);
                } else if (canAdd && p->active != 0 && p->type == 3) {
                    FUN_00446080(i);
                }
            } else {
                if (type == 4) {
                    p->FUN_00463c60(0);
                    g_game->field_499++;
                    FUN_00451180();
                }
                if (g_game->players[FUN_00456850()].info->b.closed) {
                    FUN_004abd90(g_game->gui, FUN_004c5740("Can't add another player when game is closed."), 500, 1, 1);
                    p->FUN_00463c60(0);
                    g_game->dirty = 1;
                    break;
                }
                if (g_game->players[FUN_00456850()].info->b.commander != 2 && !FUN_00457b90()) {
                    FUN_00451220(i, 2);
                    p->info->slot = FUN_00440c10();
                }
            }
            g_game->dirty = 1;
            FUN_00451180();
            FUN_00450f90();
        }

        sprintf(text, "SIDE%d", i);
        if (FUN_0049fd60(gadget, text)) {
            FUN_0047f1a0("Multi", 0);
            if (p->active != 0 && p->info->b.bit6) {
                p->info->b.bit6 = 0;
                p->info->side = 0;
            } else {
                p->info->side++;
                if (p->info->side >= g_game->sides) {
                    p->info->side = 0;
                    if (g_game->players[FUN_00456850()].info->b.watching
                        && p->active != 0 && p->type == 1) {
                        p->info->b.bit6 = 1;
                    } else {
                        FUN_004a1080(gadget, text, 0);
                        FUN_004a5f40(gadget, gadget->field_60);
                    }
                }
            }
            g_game->dirty = 1;
            FUN_0046c620(4);
            FUN_00450f90();
        }

        sprintf(text, "ALLY%d", i);
        if (FUN_0049fd60(gadget, text)) {
            me->ally[i] ^= 1;
            FUN_00452960(me->id, p->id, me->ally[i], 0);
            char same;
            if (me->colour == 5)
                same = 0;
            else
                same = me->colour == p->colour;
            if (same) {
                FUN_00446e90(me);
                me->colour = 5;
                FUN_00452bd0(me);
            }
            if (me->ally2[i] << 1 == 3 | me->ally[i])
                FUN_0047f1a0("Ally", 0);
            else
                FUN_0047f1a0("Multi", 0);
            sprintf(text, " %s %s",
                    FUN_004c5740(me->ally[i] ? "allied with" : "broke alliance with"),
                    g_game->players[i].name);
            FUN_00463e50(me, text, 4, 0);
            g_game->dirty = 1;
            FUN_00450f90();
        }

        sprintf(text, "TEAMICONS%d", i);
        if (FUN_0049fd60(gadget, text)) {
            FUN_0047f1a0("Ally", 0);
            FUN_00446f50(i);
            FUN_00452bd0(p);
        }

        sprintf(text, "RES%d", i);
        if (FUN_0049fd60(gadget, text) && IsLocalHuman_00447b10(p)) {
            FUN_0047f1a0("Multi", 0);
            FUN_00446310();
            FUN_004ab0a0(gadget);
            g_game->dirty = 1;
            return;
        }

        sprintf(text, "READY%d", i);
        if (FUN_0049fd60(gadget, text) && IsLocalHuman_00447b10(p)) {
            FUN_0047f1a0("Multi", 0);
            if (FUN_00440cd0()) {
                p->info->b.ready = FUN_004a0f30(g_game->gui, FUN_0049fdf0(entries, text, 1));
                if (p->info->f97_0) {
                    strcpy(entries->label, "START");
                    g_game->table->field_20 = FUN_0049fdf0(entries, "START", 1);
                }
                for (int j = 0; j < 10; j++) {
                    Player_00447b10* q = &g_game->players[j];
                    if (IsLocal_00447b10(q))
                        q->info->b.ready = g_game->players[g_game->localPlayer].info->b.ready;
                }
                g_game->dirty = 1;
                FUN_00450f90();
            } else {
                FUN_004a1110(g_game->gui, text, 0);
            }
        }
        i++;
        if (i >= 10)
            break;
    }
    if (i != g_game->field_2a3c)
        g_game->dirty = 1;

    if (FUN_0049fd60(gadget, "PREVMENU")) {
        FUN_0047f1a0("Previous", 0);
        for (int j = 0; j < 10; j++) {
            Player_00447b10* q = &g_game->players[j];
            if (IsLocal_00447b10(q))
                FUN_00453010(q->id, 2);
        }
        g_game->state = 3;
        return;
    }
    if (FUN_0049fd60(gadget, "MESSAGE")) {
        Entry_00447b10* box = FUN_004a0010(entries, "MESSAGE");
        char* msg = box->text;
        if (strlen(msg) != 0) {
            if (_strcmpi(msg, "+syncerr") == 0) {
                char* s = g_game->net->FUN_0046df40();
                if (s)
                    FUN_00463ca0(s, 4, 0, 10);
            } else {
                FUN_00463e50(me, msg, 4, 0);
                if (DAT_00506dbc)
                    DAT_00513000.FUN_004618a0(1);
            }
            g_game->dirty = 1;
            strcpy(msg, "");
        }
        FUN_004a7190(g_game->gui, FUN_0049fdf0(g_game->table->entries, "MESSAGE", 3));
    } else if (FUN_0049fd60(gadget, "COMMANDER")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.commander++;
        if (me->info->b.commander > 2)
            me->info->b.commander = 0;
        FUN_00450f90();
        FUN_00451180();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "LOSTYPE")) {
        FUN_0047f1a0("Multi", 0);
        if (!me->info->b.los) {
            me->info->b.los = 1;
            me->info->b.losType = 1;
        } else if (me->info->b.losType == 1) {
            me->info->b.losType = 0;
        } else {
            me->info->b.los = 0;
        }
        FUN_00450f90();
        FUN_00451180();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "WATCHING")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.watching = !me->info->b.watching;
        if (!me->info->b.watching && me->active != 0 && me->info->b.bit6)
            me->info->b.bit6 = 0;
        FUN_00450f90();
        FUN_00451180();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "CHEATING")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.cheating = !me->info->b.cheating;
        FUN_00450f90();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "FIXEDLOC")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.fixedloc = !me->info->b.fixedloc;
        FUN_00450f90();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "MAPPING")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.mapping = FUN_004a0f60(gadget, "MAPPING") == 0;
        FUN_00450f90();
        FUN_00451180();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "START")) {
        int count = 0;
        FUN_0047f1a0("BigButton", 0);
        for (int j = 0; j < 10; j++) {
            Player_00447b10* q = &g_game->players[j];
            if ((IsLocalHuman_00447b10(q) || IsRemoteHuman_00447b10(q)) && q->info->f9d_2)
                count++;
        }
        if (count < 1 || (count < 2 && FUN_00457af0() > 3) || (count < 3 && FUN_00457af0() > 6)) {
            FUN_004ab0a0(g_game->gui);
            FUN_004abd90(gadget, FUN_004c5740("There are not enough game CDs present to play"), 200, 1, 1);
            return;
        }
        int total = FUN_00457b40() + FUN_00457af0();
        for (int t = 0; t < 5; t++) {
            if (CountAlliance_00447b10(t) == total) {
                FUN_004ab0a0(g_game->gui);
                FUN_004abd90(gadget, FUN_004c5740("Can not start game with all players on the same team."), 200, 1, 1);
                return;
            }
        }
        if (!((Class_00435c40*)g_game->map)->FUN_00435c40()) {
            FUN_0047f1a0("Multi", 0);
            FUN_00444ea0();
            goto done;
        }
        if (!me->info->b.watching) {
            for (int j = 0; j < 10; j++) {
                Player_00447b10* q = &g_game->players[j];
                if (q->active != 0 && q->type == 3 && (q->info->flags_9b & 0x40))
                    FUN_00453010(q->id, 9);
            }
        }
        g_game->state = 0x11;
        me->info->b.started = 1;
        FUN_00451180();
        g_game->los = me->info->b.los;
        g_game->losType = me->info->b.losType;
        g_game->commander = me->info->b.commander;
        g_game->options->fixedloc = me->info->b.fixedloc;
        g_game->mapping = me->info->b.mapping;
        FUN_00430f00();
        g_game->field_37eee = 2;
        return;
    } else if (FUN_0049fd60(gadget, "GAMEOPEN")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.closed = FUN_004a0f60(gadget, "GAMEOPEN") == 0;
        FUN_00450f90();
        FUN_00451180();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "RESTRICTIONS")) {
        FUN_0047f1a0("Options", 0);
        FUN_0044c7e0();
        FUN_004ab0a0(gadget);
    } else if (FUN_0049fd60(gadget, "MAP") || FUN_0049fd60(gadget, "MAPNAME")) {
        FUN_0047f1a0("Multi", 0);
        do {
        if (me->info->f97_0) {
            FUN_00444ea0();
        } else {
            Gadget_00447b10* view = FUN_004aa8f0(g_game->gui, "VIEWMAP.GUI", 0x900);
            view->handler = FUN_00444ba0;
            FUN_004288d0("DVIEWMAP", 0, 0, 0);
            FUN_00444a20();
            FUN_0049fb10(g_game->gui, 1);
            FUN_004a81e0(g_game->gui, 0x40);
        }
        } while (0);
    }
done:
    FUN_004ab0a0(gadget);
}

// Decompiled by Claude Sonnet 5.5, finished by DeepSeek V4.1 Flash and GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash pass 6 (52.9%, 4360 vs 4420 bytes, current best): two case-3 changes on top
// of pass 5. (1) `Def* tdef` is declared in a NESTED block that opens at the enemy path (`{` plus
// `Def* tdef = target->def;` immediately before the `(target->f110 & 3) != 2` test) and closes just
// before the shared flag_28/break tail, which moved the tdef load to 0x43f1f1 like the original and
// put node in esi, matching `mov esi,[ebp+0x10]` at 0x43f177 (51.6 -> 51.8). (2) deleting that
// local and writing `target->def->` at the 5 use sites instead is better still: MSVC then loads
// edx = target->def only at the first use, after the `& 3` guard and after the node bit test,
// exactly as the original does (51.8 -> 52.9). The two `goto reached`/`goto after` are inside the
// nested block, so they still resolve. Prologue still differs (ours: mode ecx, def edx, `dec ecx`;
// original: mode edx, def esi, `lea ecx,[edx-1]`), as do the friendly path's scratch picks
// (node->f111 in edx vs eax; def->f1ee chain in eax/ecx vs ecx/edx) and the bottom reach compare
// (ours cmp ecx,eax / jg, original cmp eax,ecx / jl).
// Also tested on this base: the bottom reach test written directly as
// `if (target->def->f170 + target->f70 < g_game->threshold) goto after;` (39.7: MSVC folds the
// redundant test away, the pass-5 commuted form must stay), swapping the reach sum's operand
// order (52.9, byte-identical to pass 6), dropping the flags110 local for direct `unit->f110bits`
// reads and making it a 1-element array (both byte-identical to pass 5).
// deepseek-v4.1-flash pass 5 (51.6%, 4332 vs 4420 bytes, current best): the case-3 guard's
// second reach test is a redundant re-test in the original (0x43f23c: cmp eax,ecx / jl 0x43f27a)
// that MSVC folds away in every spelling that shares a syntactic tree with the first test
// (base 51.3%, and all of: same-polarity duplicate, if/else goto-into-else, inline expression
// re-evaluation, two-locals recompute, all folded the compare to jmp). It survives only when
// the bottom test is a COMMUTED negation of the outer one: outer `reach >= g_game->threshold`
// (goto reached) plus bottom `if (g_game->threshold <= tdef->f170 + target->f70) goto reached;
// goto after;`. That keeps the compare (cmp ecx,eax / jg) and lifts 51.3 -> 51.6; it also makes
// the threshold load and the mov edi,0x10000 constant hoist match the original byte for byte.
// Still differs from 0x43f21b on: node is edx here vs esi in the original (the same node/def
// register rotation as every earlier pass), an extra `mov ebx,[esp+0x1c]` reload before the
// bottom compare, and the bottom compare is the commuted form (cmp ecx,eax / jg 0x43f281 vs
// the original's cmp eax,ecx / jl 0x43f27a). vI/vJ (second local for the bottom test) do not
// compile (MSVC C2360/C2361: goto skips the local's initialisation); vK (thr local) folds
// again at 51.3. To also get cmp eax,ecx / jl the bottom tree must stay reach-first while not
// being the outer test's exact negation, which every tested spelling collapses.
// deepseek-v4.1-flash pass (51.3%, 4356 vs 4420 bytes): the whole of case 3 onward was
// shifted by one allocator choice: ours gave esi to the target->def temporary and put the
// node pointer in ecx, the original gives esi to node and keeps tdef in edx. Declaring
// `Def* tdef = target->def;` early, right after `Node* node = unit->f10;` inside the
// flag_31 block (it is otherwise only declared at the reach computation) changes the
// allocator: tdef takes esi and def moves to edx, scoring 51.3% against 50.4%. It does
// break the previously byte-exact prologue/guard region (mode moves edx -> ecx, def
// esi -> edx), so the prologue now differs too, but the net alignment is better. Every
// other spelling tried left the code bytes identical at 50.4%: removing the tdef local
// entirely, routing the two loads through static inline getters, swapping the reach
// operand order, char/bool friendly/enemy, and every header set (headers.py tops out at
// 50.5% with <memory.h>). Declaring tdef at the top of case 3 or before the f245 test
// scores 47.7%. Node still lands in ecx in this version; the remaining diffs are the
// node/tdef/intermediate scratch picks and two constant materialisations.
// deepseek-v4.1-flash pass 2 (no improvement, kept 51.3%): re-tried every node/tdef lever on
// top of the 51.3% version and all scored 47 to 51.3 percent: swapping the two declarations,
// splitting/merging tdef's declaration, moving tdef next to its first use (50.4), declaring it
// without an initialiser, `const Def* tdef`, making def an array/struct local to force it to
// memory, adding throwaway live locals (node2, node2=f2c), converting every `unit->def->` in
// case 1 only (48.4) and in the whole switch (48.3) to the `def` local, and reordering the
// friendly/enemy declarations (51.1). The 51.3% file is the best of the two spellings; the
// cause is still that MSVC colours node in ecx and leaves a live def in edx instead of
// spending def's esi cache on node.
// deepseek-v4.1 pass 4 (50.4%): tried Pick as `cond ? vtol : ground` (43.0%, 4472 bytes) and as
// `name = ground; if (bit) name = vtol;` (45.4%, 4444 bytes); both are worse than the current
// `name = vtol; if (!bit) name = ground;`, so it is restored. Restructuring case 2 to the
// original's early return (`if (!target) return Pick(...,"MOVE_GROUND")` before the target block,
// which the original does at 0x43f873: test edi,edi / jne target code) drops to 48.9% (4340 bytes),
// and replacing every `unit->def->` in the switch with the `def` local drops to 48.0% (4332 bytes);
// both reverted. Still differs at 0x43f177: the
// original has def only in memory ([esp+0x20]) there and reuses esi for the node pointer, ours
// keeps def live in esi and gives the node ecx; later ours swaps node/tdef (esi vs edx too), so
// case 3's whole body and everything after it stays shifted. In case 1 the original reads def
// straight from [esp+0x20] while ours reloads unit from [esp+0x1c] then does `unit->def`, the same
// memory-local theme. The original Pick is a select:
// `mov edx,[def+0x241]; mov eax,VTOL; shr edx,0xb; test dl,1; jne <keep>`.
// deepseek-v4.1-flash pass 3 (50.4%, 4324 vs 4420 bytes): the original's VTOL tests are
// BITFIELD reads, not `(x >> 11) & 1`. A micro-test (build/scratch/0x43f0e0/t.cpp) shows
// `(f241 >> 11) & 1` folding to `test ah,8`, while a bitfield read gives the original's
// `shr ecx,0xb / test cl,1`. Converted IsVtol and the f245 shift checks to bitfields
// (Flags245 union) and rewrote Pick as `name = vtol; if (!bit) name = ground;`. The
// original has 27 `shr ..,0xb` sites; we still fold the Pick ternaries to `test`, so the
// Pick sites remain the main open difference in case 3/9/etc.
// deepseek-v4.1 pass 2 (50.0%): keeping the `def = unit->def;` store but writing unit->def inside
// the case bodies makes the allocator give unit ebp and target edi, so the prologue and the
// whole 0x43f0e0..0x43f1d4 prologue/guard region now match the original byte for byte (score
// 47.5 -> 50.0; 4304 bytes vs the original 4420). The first still-differing instruction is at
// 0x43f177: the original reuses esi (def) for the node pointer (`mov esi,[ebp+0x10]`) and
// reloads def from [esp+0x20], ours keeps def pinned in esi and puts node in ecx; a couple of
// extra `mov dl,2` / `xor ebx,ebx` materialisations follow from that. The original treats the
// local as a memory variable with an opportunistic esi cache; ours still colours it in esi.
// Removing the local entirely (unit->def at all 46 sites) reaches the same prologue but moves
// target to ebx and friendly to edi (33.1%), so the local must stay.
// Previous pass notes (prologue swap) kept in build/scratch/0x43f0e0/.
// iostream improves the corrected implementation.
// deepseek-v4.1 pass: verified against the binary that the source order of the cases is
// already right. The jump table at 0x4401ec maps cases 1..14 to
// 0x43f9e9, 0x43f845, 0x43f154, 0x43f7e8, 0x43f735, 0x43f701, 0x43f4c7, 0x43f46c,
// 0x43f3b9, 0x43f82c, 0x43f813, 0x43f4f7, 0x43f6d1, 0x43f7a0, i.e. bodies are emitted in
// order 3,9,8,7,12,13,6,5,14,4,11,10,2,1, exactly the order this file uses.
// The case-3 body still differs in scratch-register picks (node/def/intermediate) and in two
// constant materialisations; the case order and the guard region above are confirmed correct.
#include <iostream>
#include <windows.h>

#pragma pack(push, 1)
class Class_00438760 {
  public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() { index = 0; }
};

union Flags110_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 31;
        unsigned int flag_31 : 1;
    };
};

union Flags241_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 11;
        unsigned int flag_11 : 1;
        unsigned int bits_12 : 16;
        unsigned int flag_28 : 1;
        unsigned int bits_29 : 3;
    };
};

union Flags111_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 8;
        unsigned int flag_8 : 1;
        unsigned int bits_9 : 8;
        unsigned int flag_17 : 1;
        unsigned int bits_18 : 14;
    };
};

union Flags245_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 4;
        unsigned int flag_4 : 1;
        unsigned int flag_5 : 1;
        unsigned int flag_6 : 1;
        unsigned int flag_7 : 1;
        unsigned int flag_8 : 1;
        unsigned int flag_9 : 1;
        unsigned int flag_10 : 1;
        unsigned int flag_11 : 1;
        unsigned int flag_12 : 1;
        unsigned int bits_13 : 1;
        unsigned int flag_14 : 1;
        unsigned int bits_15 : 17;
    };
};

struct Game_0043f0e0 {
    char unknown_0[0x2a42];
    unsigned char localPlayer;    // +0x2a42
    unsigned char localPlayerBit; // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidth; // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int unitCount; // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    char* units;                // +0x1426f
    unsigned short* visibility; // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char threshold; // +0x1427f
    char unknown_14280[0x37efa - 0x14280];
    int flag37efa; // +0x37efa
};

struct Node_0043f0e0 {
    char unknown_0[0x111];
    union {
        unsigned int f111; // +0x111
        Flags111_0043f0e0 f111bits;
    };
};

struct Def_0043f0e0 {
    char unknown_0[0x146];
    char unknown_146[0x156 - 0x146];
    int f156; // +0x156
    char unknown_15a[0x170 - 0x15a];
    short f170; // +0x170
    char unknown_172[0x1ee - 0x172];
    Node_0043f0e0* f1ee; // +0x1ee
    char unknown_1f2[0x1fa - 0x1f2];
    unsigned int f1fa; // +0x1fa
    char unknown_1fe[0x241 - 0x1fe];
    union {
        unsigned int f241; // +0x241
        Flags241_0043f0e0 f241bits;
    };
    union {
        unsigned int f245; // +0x245
        Flags245_0043f0e0 f245bits;
    };
};

struct Player_0043f0e0 {
    char unknown_0[0x80];
    unsigned int width;  // +0x80
    unsigned int height; // +0x84
    char unknown_88[0x108 - 0x88];
    char allied[1]; // +0x108
    char unknown_109[0x146 - 0x109];
    unsigned char index; // +0x146
};

struct Unit_0043f0e0 {
    int moving; // +0x0
    char unknown_4[0x10 - 0x4];
    Node_0043f0e0* f10; // +0x10
    char unknown_14[0x2c - 0x14];
    Node_0043f0e0* f2c; // +0x2c
    char unknown_30[0x3b - 0x30];
    unsigned char f3b; // +0x3b
    char unknown_3c[0x70 - 0x3c];
    short f70; // +0x70
    char unknown_72[0x86 - 0x72];
    Unit_0043f0e0* f86; // +0x86
    char unknown_8a[0x92 - 0x8a];
    Def_0043f0e0* def;       // +0x92
    Player_0043f0e0* player; // +0x96
    char unknown_9a[0xfb - 0x9a];
    int ffb; // +0xfb
    unsigned char unknown_ff[1];
    char unknown_100[0x104 - 0x100];
    float f104; // +0x104
    short f108; // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int f110; // +0x110
        Flags110_0043f0e0 f110bits;
    };
};

struct Pos_0043f0e0 {
    short xf, x;
    short yf, y;
    short zf, z;
};

struct Cell_0043f0e0 {
    char unknown_0[8];
    unsigned short feature; // +0x8
    unsigned char offsetY;  // +0xa
    unsigned char offsetX;  // +0xb
    char unknown_c;
};

struct Thing_0043f0e0 {
    char unknown_0[0xfe];
    unsigned char ffe; // +0xfe
};
#pragma pack(pop)

extern Game_0043f0e0* g_game;

Cell_0043f0e0* __stdcall FUN_004815a0(Pos_0043f0e0* pos);
class Class_004899b0 {
  public:
    int FUN_004899b0(Unit_0043f0e0* other);
};
class Class_00489a70 {
  public:
    int FUN_00489a90(Unit_0043f0e0* other);
};
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit_0043f0e0* unit,
                                      Unit_0043f0e0* target, Pos_0043f0e0* pos);

static inline int IsVtol(Def_0043f0e0* def) { return def->f241bits.flag_11; }

static inline Class_00438760 Pick(Def_0043f0e0* def, const char* vtol, const char* ground) {
    const char* name = vtol;
    if (!def->f241bits.flag_11)
        name = ground;
    return Class_00438760(name);
}

static inline int Visible(Unit_0043f0e0* unit, Pos_0043f0e0* pos) {
    Player_0043f0e0* p = unit->player;
    int x = pos->x >> 5;
    int y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->width && (unsigned int)y < p->height &&
           ((1 << g_game->localPlayerBit) & g_game->visibility[p->width * y + x]) != 0;
}

static inline Thing_0043f0e0* Lookup(Pos_0043f0e0* pos) {
    Cell_0043f0e0* cell = FUN_004815a0(pos);
    if (!cell)
        return 0;
    unsigned short id = cell->feature;
    if (id >= 0xfffb) {
        if (id != 0xfffe)
            return 0;
        id = (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
        if (id >= 0xfffb)
            return 0;
    } else if ((int)id >= g_game->unitCount) {
        return 0;
    }
    return (Thing_0043f0e0*)(g_game->units + (id << 8));
}

static inline int Marked(Thing_0043f0e0* t) { return t && (t->ffe & 0x80); }

// FUNCTION: 0x43f0e0
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit_0043f0e0* unit,
                                      Unit_0043f0e0* target, Pos_0043f0e0* pos) {
    Def_0043f0e0* def;
    int friendly = 0;
    int enemy = 0;
    if (target) {
        if (!(target->f110 & 0x10000000))
            goto none;
        if (unit->player->allied[target->player->index] != 0)
            friendly = 1;
        else
            enemy = 1;
    }
    def = unit->def;
    switch (mode) {
    case 3: {
        if (!(def->f245 & 0x10))
            break;
        Flags110_0043f0e0 flags;
        flags.raw = unit->f110;
        if (flags.flag_31) {
            Node_0043f0e0* node = unit->f10;
            if (!enemy) {
                if (node->f111bits.flag_17)
                    break;
                if (!(def->f241 & 0x800))
                    return Class_00438760("SUPPRESS");
                if (def->f1ee->f111bits.flag_8)
                    return Class_00438760("AIRSTRIKE");
                return Class_00438760("AIRTOGROUND");
            }
            {
            if ((target->f110 & 3) != 2) {
                if (node->f111 & 0x20000)
                    break;
            }
            if (target->def->f170 + target->f70 >= g_game->threshold)
                goto reached;
            if (!(node->f111 & 0x10000)) {
                if (!(unit->f3b & 2))
                    break;
                if (!(unit->f2c->f111 & 0x10000))
                    break;
            }
            if (g_game->threshold <= target->def->f170 + target->f70)
                goto reached;
            goto after;
        reached:
            if (def->f241 & 0x1000) {
                if (node->f111 & 0x10000)
                    break;
                if ((unit->f3b & 2) && (unit->f2c->f111 & 0x10000))
                    return Class_00438760();
            }
        after:
            unsigned int f = def->f241;
            if (def->f241bits.flag_11) {
                unsigned int air = def->f1ee->f111 & 0x100;
                if (air && !(target->def->f241 & 0x800))
                    return Class_00438760("AIRSTRIKE");
                if (!air && (target->def->f241 & 0x800))
                    return Class_00438760("AIRTOAIR");
                unsigned int tv = target->def->f241 & 0x800;
                if (!tv && !(f & 0x8000000))
                    return Class_00438760("AIRTOGROUND");
                if (!tv && (f & 0x8000000))
                    return Class_00438760("AIRTOGROUNDHOVER");
                break;
            }
            if (unit->moving != 0)
                return Class_00438760("ATTACK_CHASE");
            if (flags.raw & 0x20000000)
                return Class_00438760("ATTACK_NOMOVE");
            }
        }
        if (def->f241bits.flag_28)
            return Class_00438760("ATTACK_KAMIKAZE");
        break;
    }
    case 9:
        if (unit->def->f245 & 0x40) {
            if (unit->moving == 0)
                return Class_00438760("QPATROL");
            if (unit->def->f245bits.flag_9) {
                if (unit->def->f241bits.flag_11)
                    return Class_00438760("VTOL_REPAIRPATROL");
                return Class_00438760("REPAIRPATROL");
            }
            if (unit->def->f241bits.flag_11)
                return Class_00438760("VTOL_PATROL");
            return Class_00438760("PATROL");
        }
        break;
    case 8:
        if (!((Class_004899b0*)unit)->FUN_004899b0(target))
            break;
        if (target->f104 == 0.0f)
            return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
        return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
    case 7:
        if (!(unit->def->f245 & 0x20) || !friendly)
            break;
        return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
    case 12: {
        if (!(unit->def->f245 & 0x400))
            break;
        Thing_0043f0e0* t = Lookup(pos);
        if (pos) {
            if ((unit->def->f245 & 0x800) && Visible(unit, pos) && Marked(t))
                return Class_00438760("RESURRECT");
            if (pos && Visible(unit, pos) && Marked(t))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
        }
        if (!target)
            break;
        return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
    }
    case 13:
        if ((unit->def->f245 & 0x1000) && target && unit->player != target->player)
            return Class_00438760("CAPTURE");
        break;
    case 6:
        if (!target || !((Class_00489a70*)unit)->FUN_00489a90(target))
            break;
        return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
    case 5:
        if ((unit->def->f245 & 0x100) && (unit->def->f241 & 0x800) && target && (target->def->f241 & 0x200))
            return Class_00438760("VTOL_LANDING");
        if (unit->def->f245bits.flag_8)
            return Pick(def, "VTOL_UNLOAD", "GROUND_UNLOAD");
        break;
    case 14:
        if (unit->def->f156 == 0 || unit->moving == 0)
            break;
        return Pick(def, "VTOL_MOBILEBUILD", "MOBILEBUILD");
    case 4:
        if (unit->def->f245bits.flag_14)
            return Class_00438760("ATTACKSPECIAL");
        break;
    case 11:
        return Class_00438760("TELEPORT");
    case 10:
        return Class_00438760("STOP");
    case 2:
        if (!(unit->def->f245 & 0x80))
            break;
        if (unit->moving == 0)
            return Class_00438760("QMOVE");
        if (target) {
            if ((unit->def->f245 & 0x1000) && enemy)
                return Class_00438760("CAPTURE");
            if (!((unit->def->f245 & 0x400) && enemy)) {
                if (friendly) {
                    if (((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                        return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
                    if (((Class_004899b0*)unit)->FUN_004899b0(target) &&
                        (unsigned int)target->f108 < target->def->f1fa)
                        return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
                }
                if ((unit->def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
                    return Class_00438760("VTOL_LANDING");
                if (((Class_00489a70*)unit)->FUN_00489a90(target))
                    return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
                if ((unit->def->f245 & 0x20) && friendly)
                    return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
                return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
            }
            return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
        }
        return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
    case 1: {
        if (g_game->flag37efa == 1) {
            if ((unit->def->f245 & 0x10) && enemy)
                return FUN_0043f0e0(3, unit, target, pos);
            if ((unit->def->f245 & 0x400) && enemy)
                return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target))
                return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
            if ((unit->def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
                return Class_00438760("VTOL_LANDING");
            if (target && ((Class_00489a70*)unit)->FUN_00489a90(target))
                return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
            if ((unit->def->f245 & 0x20) && friendly)
                return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
            if ((unit->def->f245 & 0x800) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Class_00438760("RESURRECT");
            if ((unit->def->f245 & 0x400) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
            if (!(unit->def->f245 & 0x80) || unit->moving == 0)
                break;
        } else {
            if ((unit->def->f245 & 0x10) && enemy)
                return FUN_0043f0e0(3, unit, target, pos);
            if ((unit->def->f245 & 0x400) && enemy)
                return FUN_0043f0e0(0xc, unit, target, pos);
            if (target && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                return FUN_0043f0e0(8, unit, target, pos);
            if (target && target->unknown_ff[0] == g_game->localPlayer && (target->f110 & 0x20) &&
                target->f104 == 0.0f && target->ffb == 0 &&
                (!target->f86 || (target->f86->f110 & 0x40000000)))
                break;
            if ((unit->def->f245 & 0x800) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Class_00438760("RESURRECT");
            if ((unit->def->f245 & 0x400) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
            if (!(unit->def->f245 & 0x80) || unit->moving == 0)
                break;
        }
        return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
    }
    }
none:
    return Class_00438760();
}

// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// STILL PARTIAL: 85.9% (775 bytes vs the original 774). The remaining diff is
// one allocator state, not semantics: the original keeps the 16-bit ownerId in
// the [esp+0x18] slot (16-bit load into cx, DWORD store, `and edx, 0xffff`, a
// 16-bit compare in the scan) with projCount hoisted into edi, and found in
// esi; this file keeps ownerId in edi, spills found to [esp+0x18] and re-reads
// projCount in the loop latch. Declaring ownerId `unsigned short` produces the
// original's instructions but grows the frame to 0x14 and moves unit to edi
// (73.1%). See the notes below the description for every variant tried.
// Correction to old notes: after the push at 0x49d54e, [esp+0x14] at
// 0x49d54f refers to the entry slot at base+0x10, not ownerId at base+0x18.
// The current FUN_0049c9c0(entry, ...) call is correct.
// Creates or updates a projectile for a remote event. The per-team record at
// g_game+0x2cf3 (0x115 bytes, 0x100 of them) holds the weapon flags at +0x111;
// the event gives a team byte, an owning unit id, a per-unit entry index and a
// position. Flag bit 5 spawns a projectile; bits 1, 4, 0/20, 8 dispatch to
// 0x49cde0, 0x49cc20, 0x49c9c0, or a spawn aimed from the owning unit.
//
// Sonnet 5.5 retry (#1097), first pass: 82.9%. `ev->unitId == 0 ? 0 : ...`
// gave +1.2 points (the original tests then computes).
//
// Not matched yet: best 85.9%, own code 775 bytes vs 774 (deepseek-v4.1).
//
// Sonnet 5.5 pass 2, the ONE remaining difference, read off the frame layout:
// the four locals are [esp+0x10]=entry, [esp+0x14]=plain cursor, [esp+0x18]=?, 
// [esp+0x1c]=def. [esp+0x18] is written exactly once, at 0x49d395, with the
// value just loaded by `mov cx, word ptr [eax + 0x1f]` (ev->ownerId), and is read
// twice: as a WORD at 0x49d3fd by the scan loop, and as a DWORD at 0x49d54f by
// the FUN_0049c9c0 call. So local [esp+0x18] is `ownerId`, NOT `found`, and
// FUN_0049c9c0's FIRST argument is `ownerId`, not `entry` (Ghidra's pseudo-C says
// piVar1 there, but piVar1 lives at [esp+0x10], and the b1 branch does read
// [esp+0x10] at 0x49d448 for its own first argument). `found` is the ESI value:
// `xor esi,esi` at the join 0x49d428 and `mov esi, [esp+0x14]` in the found block
// at 0x49d567. So the original splits ownerId(live to the b0/b20 call) into the
// slot and found(live to the b4 call) into ESI; mine has them the other way
// round, ownerId in EDI and found in a slot, which is why my latch re-reads
// projCount instead of hoisting it.
//
// Passing ownerId as FUN_0049c9c0's first argument was tried and is WORSE
// (72.6%): MSVC still keeps ownerId in EDI, and it rotates the locals (entry to
// [esp+0x14], cursor to [esp+0x10]). Also tried and worse: ownerId declared
// `unsigned short` (73.1% either way, 782 bytes; it does produce the original's
// `cmp word ptr [edx+0xa8], bx` but rotates unit into EDI and projCount into
// EBP); `unsigned short` helper parameter (61.4%); an explicit if/else or a
// ternary for the owner lookup (both 66.0% and 792 bytes, the smallest the
// allocator has produced is the unconditional `owner = 0` then conditional
// assignment used here); the ternary `ev->unitId == 0 ? 0 : ...` replaced by an
// if/else with a separate unitId local (81.7%); a pointer-walk loop in the scan
// helper (66.7%); returning from inside the scan loop instead of a saved r
// (79.5%); dropping the `int n` local (no change); factoring the free-slot
// allocation into a static inline helper (no change, 82.9%).
//
// What the source does, in the order the disassembly runs:
//   b5  : take a free projectile slot, FUN_0049c740(proj, def, ev+1, 0,
//         teamColor, 0), copy ev->pos into proj->pos, done.
//   else: unit = unitId ? &units[unitId] : 0, bail if null or !(flags & 0x10000000).
//         entry = &unit->entries[entryIndex]; entry->f_18 = ev->f_1d;
//         entry->f_16 = ev->f_1b; owner = ownerId ? &units[ownerId] : 0.
//         if (ev->b0) found = the first projectile whose player is not the
//         local player, whose aim equals ev->pos and whose owner->f_a8 equals
//         ownerId; otherwise found stays null.
//         b1 -> FUN_0049cde0, b4 -> FUN_0049cc20 (passes found), b0 or b20 ->
//         FUN_0049c9c0, b8 -> a new projectile aimed back from the unit.
//
// Facts that were wrong or missing before, each worth points:
//   - the loop's owner test reads unit+0xa8, NOT the +0x66 the b8 branch
//     reads (`cmp word ptr [edx+0xa8], bx` at 0x49d402). Naming that field
//     f_a8 instead of reusing f_66 was worth 9 points on its own.
//   - the b8 tail assigns f_36 before f_3a (0x49d508 then 0x49d505), and the
//     16-bit argument of FUN_004b70ef/FUN_004b7123 comes FIRST: declare them
//     `int __cdecl f(short angle, int distance)` as 0x406300.cpp does and call
//     `f(unit->f_66, unit->type->param)`. That is the real (angle, distance)
//     order, matches the original's `push eax` then `push ecx`, and leaves the
//     colour un widened in ecx (82.9 -> 85.9). The old `(int, unsigned short)`
//     declaration had the two arguments the wrong way round and forced a
//     `mov dx, cx` widening copy in both call sites.
//   - the position compares are x, z, y, not x, y, z: the original reads aim
//     at +0x28, +0x30, +0x2c against ev+0xd, +0x15, +0x11.
//   - the scan is a `static inline` search helper returning a pointer, not a
//     loop written out here. That is what grows the frame from 3 dwords to
//     the original's 4, and what puts the +0x2c-biased induction variable
//     (the walking register aims at aim.y) and the plain cursor in their own
//     slots: the same "induction variable biased to a middle field, kept in a
//     stack slot beside the plain iterator" shape as 0x475470, which comes
//     from the element being read through a reference and the position
//     passed onward by reference to a further inline helper.
//   - the two hoisted addresses the b5 branch needs across its call must be
//     named locals, or ev stays live across FUN_0049c740, takes a callee-saved
//     register and the whole function's allocation rotates:
//         Vec3* pos = &ev->pos;  void* arg = (char*)ev + 1;
//
// What still differs, all one allocator state rather than independent bugs:
//   - the caller's `int ownerId` lives in edi here; in the original it is a
//     stack slot at [esp+0x18] (loaded as a word at 0x49d388, stored as a
//     DWORD at 0x49d395, reloaded as a word into bx at 0x49d3fd). Because
//     edi is taken, my loop keeps g_game->projCount in ecx and re-reads it in
//     the latch instead of hoisting it into edi as the original does
//     (`cmp esi, edi`, `mov edi, [ebx+0x141f3]` once before the loop). Tried
//     and flat or worse: taking &ownerId, an array holding it, an
//     unsigned short ownerId, giving the helper its own ownerId copy, passing
//     the whole event to the helper, and every parameter order.
//   - the original re-lays-out the unit and owner lookups the other way round
//     (test, jne to the computation, the null store in the fall-through, jmp
//     to the join); mine inverts both branches. Same code, different block
//     placement.
//   - in the b8 tail the original loads the type pointer into edx and the
//     parameter into eax, so it can `push ecx` for the 16-bit colour
//     un widened; mine loads the parameter into ecx and has to materialise
//     the colour in edx first (`mov dx, cx`). The original's `push ecx`
//     passes a value whose upper 16 bits are whatever was in ecx, which is
//     teamColor's upper half, see the report.
// The b5 branch, the two hoisted call sites and the b8 stores all match
// instruction for instruction; the only differences left are the two named
// above and the branch targets, which move with them.
//
// deepseek-v4.1 pass 3 (85.9%): fixed the FUN_004b70ef/FUN_004b7123 call
// (argument order, see above), which removed the `mov dx, cx` copies and both
// register shuffles in the b8 tail and took 83.0 -> 85.9.
//
// Everything still differing is one allocator state, not source semantics:
//   - the original's ownerId is a 16-bit value that lives in the [esp+0x18]
//     slot (`mov cx, [eax+0x1f]` reusing the entry pointer in ecx, DWORD store
//     at 0x49d395, `mov bx, [esp+0x18]` + `cmp word ptr [edx+0xa8], bx` in the
//     scan, `and edx, 0xffff` before the 0x118 imul). Here it is a 32-bit edi
//     (`mov di, [eax+0x1f]`, `and` elided, 32-bit compare) and [esp+0x18] holds
//     `found` instead.
//   - the original hoists projCount into edi (`cmp esi, edi` at 0x49d420);
//     mine re-reads [ebx+0x141f3] in the latch, because edi holds ownerId.
//   - consequently my `owner = 0` store is hoisted to the top of the entry
//     block (`xor edi, edi` + `mov [esp+0x28], esi`) where the original stores
//     it only in the ownerId == 0 arm.
// `unsigned short ownerId` (vs int) reproduces the first bullet's instructions
// exactly but costs the frame (5 slots, 0x14) and moves unit from ebp to edi:
// 73.1 / 782, and 73.3 / 776 with the found ternary. An inlined plain loop
// instead of the helper is 42.9 / 764, the helper with the owner lookup as an
// if/else is 66.0 / 792, and `found` as an if/else rather than a ternary is the
// same 85.9. What is needed is for MSVC to keep ownerId in the [esp+0x18] slot
// while leaving unit in ebp and found in esi.
#pragma pack(push, 1)

struct Vec3_0049d270 {
    int x;
    int y;
    int z;
};

struct DefFlags_0049d270 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 2;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 2;
    unsigned int b8 : 1;
    unsigned int b9 : 11;
    unsigned int b20 : 1;
    unsigned int b21 : 11;
};

struct Type_0049d270 {
    char unknown_0[0x20];
    int param;                        // +0x20
};

struct Def_0049d270 {                 // 0x115 bytes
    char unknown_0[0x111];
    DefFlags_0049d270 flags;          // +0x111
};

struct Entry_0049d270 {               // 0x1c bytes
    char unknown_0[0xc];
    Type_0049d270* type;              // +0xc
    char unknown_10[0x16 - 0x10];
    short f_16;                       // +0x16
    short f_18;                       // +0x18
    char unknown_1a[0x1c - 0x1a];
};

struct UnitFlags_0049d270 {
    unsigned int unknown_0 : 28;
    unsigned int b28 : 1;
    unsigned int b29 : 1;
    unsigned int b30 : 1;
    unsigned int b31 : 1;
};

struct Unit_0049d270 {                // 0x118 bytes
    Type_0049d270* type;              // +0
    Entry_0049d270 entries[1];        // +4
    char unknown_20[0x66 - 0x20];
    unsigned short f_66;              // +0x66
    char unknown_68[0xa8 - 0x68];
    unsigned short f_a8;              // +0xa8
    char unknown_aa[0x110 - 0xaa];
    UnitFlags_0049d270 flags;         // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Proj_0049d270 {                // 0x6b bytes
    Type_0049d270* type;              // +0
    char unknown_4[0x1c - 0x4];
    Vec3_0049d270 pos;                // +0x1c
    Vec3_0049d270 aim;                // +0x28
    char unknown_34[0x36 - 0x34];
    unsigned short f_36;              // +0x36
    char unknown_38[0x3a - 0x38];
    int f_3a;                         // +0x3a
    char unknown_3e[0x4e - 0x3e];
    int f_4e;                         // +0x4e
    Unit_0049d270* owner;             // +0x52
    char unknown_56[0x66 - 0x56];
    char player;                      // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;             // +0x69
};

struct Event_0049d270 {
    char unknown_0[0xd];
    Vec3_0049d270 pos;                // +0xd
    unsigned char team;               // +0x19
    unsigned char b0 : 1;             // +0x1a
    unsigned char unknown_1a : 7;
    unsigned short f_1b;              // +0x1b
    unsigned short f_1d;              // +0x1d
    unsigned short ownerId;           // +0x1f
    unsigned short unitId;            // +0x21
    unsigned char entryIndex;         // +0x23
};

struct Game_0049d270 {
    char unknown_0[0x2a42];
    char localPlayer;                 // +0x2a42
    char unknown_2a43[0x2cf3 - 0x2a43];
    Def_0049d270 defs[0x100];         // +0x2cf3
    int projCount;                    // +0x141f3
    Proj_0049d270* projs;             // +0x141f7
    char unknown_141fb[0x14357 - 0x141fb];
    Unit_0049d270* units;             // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    int teamColor;                    // +0x38a47
};
#pragma pack(pop)

extern Game_0049d270* g_game;

static inline int SamePos_0049d270(Vec3_0049d270& a, Vec3_0049d270& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// The scan is an inlined search helper, not a loop written out here: that is
// what puts the ownerId comparison's `mov bx, word ptr [esp + 0x18]` (the
// helper's own parameter slot) and the walking cursor in their own slots, and
// grows the frame from 3 dwords to the original's 4.
static inline Proj_0049d270* Find_0049d270(Proj_0049d270* p, int n, int lp,
                                           Vec3_0049d270& pos, int ownerId)
{
    Proj_0049d270* r = 0;
    for (int i = 0; i < n; i++) {
        Proj_0049d270& q = p[i];
        if (q.player != lp
            && SamePos_0049d270(q.aim, pos)
            && q.owner->f_a8 == ownerId) {
            r = &q;
            break;
        }
    }
    return r;
}

void __stdcall FUN_0049c740(void*, void*, void*, int, int, void*);
void __stdcall FUN_0049c9c0(void*, void*, void*, void*, void*);
void __stdcall FUN_0049cc20(void*, void*, void*, void*, void*, void*);
void __stdcall FUN_0049cde0(void*, void*, void*, void*, void*);
int __cdecl FUN_004b70ef(short angle, int distance);
int __cdecl FUN_004b7123(short angle, int distance);

// FUNCTION: 0x49d270
void __stdcall FUN_0049d270(int arg1, Event_0049d270* ev)
{
    unsigned char team = ev->team;
    Def_0049d270* def = &g_game->defs[team];
    if (def->flags.b5) {
        Vec3_0049d270* pos = &ev->pos;
        void* arg = (void*)((char*)ev + 1);
        Proj_0049d270* proj = 0;
        if (g_game->projCount < 300) {
            proj = &g_game->projs[g_game->projCount++];
            proj->flags &= 0xfffd;
            proj->f_4e = 0;
        }
        if (!proj)
            return;
        FUN_0049c740(proj, def, arg, 0, g_game->teamColor, 0);
        proj->pos = *pos;
        return;
    }
    Unit_0049d270* unit = ev->unitId == 0 ? 0 : &g_game->units[ev->unitId];
    if (!unit)
        return;
    if (!unit->flags.b28)
        return;
    Entry_0049d270* entry = &unit->entries[ev->entryIndex];
    entry->f_18 = ev->f_1d;
    entry->f_16 = ev->f_1b;
    int ownerId = ev->ownerId;
    Unit_0049d270* owner = 0;
    if (ownerId) owner = &g_game->units[ownerId];
    Proj_0049d270* found = ev->b0
        ? Find_0049d270(g_game->projs, g_game->projCount,
                        g_game->localPlayer, ev->pos, ownerId)
        : 0;
    if (def->flags.b1) {
        FUN_0049cde0(entry, unit, (void*)((char*)ev + 1), &ev->pos, owner);
        return;
    }
    if (def->flags.b4) {
        FUN_0049cc20(entry, unit, (void*)((char*)ev + 1), &ev->pos, owner, found);
        return;
    }
    if (def->flags.b0 || def->flags.b20) {
        FUN_0049c9c0(entry, unit, (void*)((char*)ev + 1), &ev->pos, owner);
        return;
    }
    if (def->flags.b8) {
        Proj_0049d270* proj = 0;
        if (g_game->projCount < 300) {
            proj = &g_game->projs[g_game->projCount++];
            proj->flags &= 0xfffd;
            proj->f_4e = 0;
        }
        if (!proj)
            return;
        FUN_0049c740(proj, entry->type, (void*)((char*)ev + 1), 0, g_game->teamColor, unit);
        proj->f_36 = unit->f_66;
        proj->f_3a = 0;
        proj->pos.y = 0;
        proj->pos.x = -FUN_004b70ef(proj->f_36, unit->type->param);
        proj->pos.z = -FUN_004b7123(proj->f_36, unit->type->param);
        return;
    }
}

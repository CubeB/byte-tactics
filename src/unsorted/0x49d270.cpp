// Decompiled by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5. Names are provisional.
// STILL PARTIAL: 90.7% (774 bytes, same size as the original). Creates or
// updates a projectile for a remote event. The per-team record at
// g_game+0x2cf3 (0x115 bytes, 0x100 of them) holds the weapon flags at +0x111;
// the event gives a team byte, an owning unit id, a per-unit entry index and a
// position. Flag bit 5 spawns a projectile; bits 1, 4, 0/20, 8 dispatch to
// 0x49cde0, 0x49cc20, 0x49c9c0, or a spawn aimed from the owning unit.
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
// Facts that are settled (each was worth points):
//   - the loop's owner test reads unit+0xa8 (f_a8), not the +0x66 the b8 branch reads.
//   - FUN_004b70ef/FUN_004b7123 take (short angle, int distance), angle first.
//   - the position compares are x, z, y (aim +0x28, +0x30, +0x2c vs ev +0xd, +0x15, +0x11).
//   - the scan is a static inline helper that RETURNS &q from inside the loop and
//     `return 0` at the end (an early-return helper: 82.5% alone, the found
//     block `mov esi,[esp+0x14]; mov ebx,g_game; jmp` then matches). The
//     `found` result is the helper's return, ternary `ev->b0 ? Find(...) : 0`.
//   - `int n = g_game->projCount;` declared at the top of the non-b5 path (before
//     the unit lookup) and passed to the helper: 82.5 -> 90.7%. As an argument
//     expression or declared inside the b0 block, MSVC substitutes the load and
//     re-reads projCount in the loop latch.
//   - hoisted addresses (`pos`, `arg`) in the b5 branch must be named locals.
//
// What still differs (one allocator state, Sonnet 5.5 pass, ~60 scoring runs):
//   The original has n in EDI loaded INSIDE the b0 block (`test byte [eax+0x1a],1;
//   je; mov edi,[ebx+0x141f3]; mov ecx,[ebx+0x141f7]; test edi,edi`), the plain
//   cursor in the [esp+0x14] slot, and the 16-bit ownerId spilled to [esp+0x18]
//   (`mov cx,[eax+0x1f]; cmp cx,si; mov [esp+0x18],ecx`, later `mov bx,[esp+0x18];
//   cmp word [edx+0xa8],bx`). Here (int ownerId, n at the top) ownerId takes EDI
//   and n is spilled to a slot, so the latch re-reads n and the guard differs.
//   Findings about what moves the allocator:
//   - Making n a real variable (declared in a different basic block from its
//     uses) is what hands EDI to n and spills the cursor. Declared before the
//     `Unit* owner` lookup with `unsigned short ownerId` and an owner ternary
//     (`ownerId == 0 ? 0 : &units[ownerId]`) it reproduces the original's
//     registers (unit EBP, n EDI, ownerId slot [esp+0x18], cursor slot [esp+0x14])
//     and the owner null arm layout exactly, 80.3%, but the load of n is then
//     before the owner branch and the b8 tail reuses EDI for
//     `g_game->projCount < 300` instead of reloading it (CSE), which costs more
//     than it gains. Declared between the unit checks and the entry stores, the
//     stores kill the CSE and the tail matches, but n takes EBP and unit EDI
//     (83.4%).
//   - Declared inside the b0 block (any form: inline loop, index-form helper,
//     do-while, `register`, `unsigned`, `const`) n is a real variable but loses
//     the register to the cursor / unit, and the latch re-reads it (75 to 78%).
//   - `unsigned short ownerId` alone reproduces the 16-bit spill but moves unit
//     to EDI and the cursor into EBP (frame shrinks to 0xc) unless n is a
//     variable defined before the owner lookup.
//   - No ownerId local (helper reads ev->ownerId): the spill is reproduced
//     (the CSE temp goes to [esp+0x18]) but the cursor takes EBP (75.8%).
//   - Helper taking the Game pointer, helper returning through a reference
//     (15 to 17%), cursor as the found variable, address-of tricks: all worse.
// deepseek-v4.1-flash (issue 3314 retry, 10 min): one more shape tried, no gain.
// `unsigned short ownerId` local plus `unsigned short ownerId` helper parameter
// (to reach the original's `mov cx` / `and edx,0xffff` / 16-bit helper slot)
// collapses to 766 bytes / 81.5%, so the int ownerId in EDI form (90.7%) stays
// the best. The remaining diff is unchanged: n spilled to [esp+0x18] and
// re-read in the loop latch instead of staying in EDI, and the owner f_a8
// compare loading into a register instead of `cmp word [edx+0xa8], bx`.
// So the missing piece is a source form where n is defined at the top of the b0
// block yet stays a register variable ahead of the cursor, while ownerId is a
// ushort that spills.
// deepseek-v4.1-flash (issue 3685 retry, 10 min): body kept at 90.7% / 774
// bytes (same size as the original). Two more shapes tried, both worse:
// `unsigned short ownerId` local with the int helper param (81.5%, 766 bytes,
// the ushort starves n and moves unit/cursor), and `int n;` declared at the
// top but assigned inside the b0 block (82.3%, 770 bytes, the block-local
// definition still loses EDI). The diff is unchanged: n spilled to
// [esp+0x18] and re-read in the latch instead of staying in EDI, and the
// owner f_a8 compare loading into a register instead of
// `cmp word [edx+0xa8], bx`.
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
    for (int i = 0; i < n; i++) {
        Proj_0049d270& q = p[i];
        if (q.player != lp
            && SamePos_0049d270(q.aim, pos)
            && q.owner->f_a8 == ownerId)
            return &q;
    }
    return 0;
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
    int n = g_game->projCount;
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
        ? Find_0049d270(g_game->projs, n,
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
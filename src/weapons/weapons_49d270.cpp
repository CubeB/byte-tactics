// Decompiled by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// Creates or updates a projectile for a remote fire event (the packet type 0xd
// that FUN_0049d580 sends). The per-team record at g_game+0x2cf3 (0x115 bytes,
// 0x100 of them) holds the weapon flags at +0x111; the event gives a team byte,
// two unit ids (unitId, the firing unit, and ownerId), a weapon slot index and
// two positions. Flag bit 5 spawns a projectile at the event position; otherwise
// the weapon slot's angles are updated and bits 1, 4, 0/20 and 8 dispatch to
// FUN_0049cde0, FUN_0049cc20 (given the matching live projectile, if any),
// FUN_0049c9c0, or a spawn aimed from the firing unit.
//
// MATCH (Claude Opus 5.5, issue 4785). The projectile scan is FUN_0049d1e0
// inlined (it has no callers in the exe and sits just before this function).
// Earlier passes stopped at 91.7% with a hand-written scan helper taking the
// count as an `unsigned short` parameter: that was the only way they found to
// keep the count in a register, at the price of an `and edi, 0xffff`. In the
// original the count is the loop-invariant `g_game->projCount` that MSVC hoists
// out of FUN_0049d1e0's loop by itself.
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

struct Unit {                         // 0x118 bytes
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
    Unit* owner;                      // +0x52
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

struct Game {
    char unknown_0[0x2a42];
    char localPlayer;                 // +0x2a42
    char unknown_2a43[0x2cf3 - 0x2a43];
    Def_0049d270 defs[0x100];         // +0x2cf3
    int projCount;                    // +0x141f3
    Proj_0049d270* projs;             // +0x141f7
    char unknown_141fb[0x14357 - 0x141fb];
    Unit* units;                      // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    int teamColor;                    // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

static inline int SamePos_0049d270(Vec3_0049d270& a, Vec3_0049d270& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// FUN_0049d1e0 (matched in its own file), defined here unannotated so that
// /Ob2 inlines it as it did in the original. It has no callers in the exe.
// Written with the positive `if (ev->b0) { ... } return 0;` test, which also
// matches 0x49d1e0 on its own; the `if (!ev->b0) return 0;` spelling in
// 0x49d1e0.cpp compiles to the same standalone bytes but, inlined here, gives
// the cursor a register and spills `unit` (73.4%).
Proj_0049d270* __stdcall FUN_0049d1e0(Event_0049d270* ev)
{
    if (ev->b0) {
        char me = g_game->localPlayer;
        Proj_0049d270* proj = g_game->projs;
        for (int i = 0; i < g_game->projCount; i++, proj++) {
            if (proj->player != me && SamePos_0049d270(proj->aim, ev->pos) && proj->owner->f_a8 == ev->ownerId)
                return proj;
        }
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
    Unit* unit = ev->unitId == 0 ? 0 : &g_game->units[ev->unitId];
    if (!unit)
        return;
    if (!unit->flags.b28)
        return;
    Entry_0049d270* entry = &unit->entries[ev->entryIndex];
    entry->f_18 = ev->f_1d;
    entry->f_16 = ev->f_1b;
    unsigned short ownerId = ev->ownerId;
    Unit* owner = ownerId == 0 ? 0 : &g_game->units[ownerId];
    Proj_0049d270* found = FUN_0049d1e0(ev);
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

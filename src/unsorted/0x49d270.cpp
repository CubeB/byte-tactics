// Decompiled by space-bunny-free. Names are provisional.
// Creates or updates a projectile for a remote event. The per-team record at
// g_game+0x2cf3 (0x115 bytes, 0x100 of them) holds the weapon flags at +0x111;
// the event gives a team byte, an owning unit id, a per-unit entry index and a
// position. Flag bit 5 spawns a projectile; bits 1, 4, 0/20, 8 dispatch to
// 0x49cde0, 0x49cc20, 0x49c9c0, or a spawn aimed from the owning unit.
//
// Not matched yet (55.9% by difflib, own instruction count 780 vs 774 bytes).
// The prologue matches once the two addresses the b5 branch needs across its
// call are hoisted into named locals:
//     Vec3* pos = &ev->pos;  void* arg = (char*)ev + 1;
// Without them ev stays live across FUN_0049c740, MSVC gives it a callee-saved
// register and the whole function's allocation rotates (that alone was 27.5%).
// Still different:
//   - the scan loop is rotated (mine hoists the +0x30 body pointer and jumps to
//     the latch, the original is a plain bottom-tested for);
//   - ev->b0 is materialised into a local instead of `test byte [eax+0x1a],1`;
//   - the b5 branch reads the projs array pointer at [ev+1+0x141f7], one byte
//     above the g_game+0x141f7 every other access uses (see the report: this
//     looks like a genuine miscompile in the original).
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
    char unknown_68[0x110 - 0x68];
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
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

void __stdcall FUN_0049c740(void*, void*, void*, int, int, void*);
void __stdcall FUN_0049c9c0(void*, void*, void*, void*, void*);
void __stdcall FUN_0049cc20(void*, void*, void*, void*, void*, void*);
void __stdcall FUN_0049cde0(void*, void*, void*, void*, void*);
int __cdecl FUN_004b70ef(int, int);
int __cdecl FUN_004b7123(int, int);

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
    Unit_0049d270* unit = 0;
    if (ev->unitId)
        unit = &g_game->units[ev->unitId];
    if (!unit)
        return;
    if (!unit->flags.b28)
        return;
    Entry_0049d270* entry = &unit->entries[ev->entryIndex];
    entry->f_18 = ev->f_1d;
    entry->f_16 = ev->f_1b;
    Unit_0049d270* owner = 0;
    if (ev->ownerId)
        owner = &g_game->units[ev->ownerId];
    Proj_0049d270* found = 0;
    if (ev->b0) {
        int count = g_game->projCount;
        for (int i = 0; i < count; i++) {
            Proj_0049d270& proj = g_game->projs[i];
            if (proj.player != g_game->localPlayer
                && SamePos_0049d270(proj.aim, ev->pos)
                && proj.owner->f_66 == ev->ownerId) {
                found = &g_game->projs[i];
                break;
            }
        }
    }
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
        proj->f_3a = 0;
        proj->f_36 = unit->f_66;
        proj->pos.y = 0;
        proj->pos.x = -FUN_004b70ef(unit->type->param, proj->f_36);
        proj->pos.z = -FUN_004b7123(unit->type->param, proj->f_36);
        return;
    }
}

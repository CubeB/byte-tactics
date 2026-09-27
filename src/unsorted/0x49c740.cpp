// Decompiled by space-bunny-free. Names are provisional.
// Initialises one entry of the 300-entry weapon array (g_game+0x141f7):
// copies the muzzle position into two fields, optionally a third position,
// clears flag bits, and takes the firing unit's colour, its tracked
// projectile slot, the barrel the shot came out of and the frame number.
//
// Still differs (best 87.5%): the original emits the second flag clear as
//   and word ptr [esi+0x69], 0xfffe      <- first clear, in place
//   mov ax, word ptr [esi+0x69]          <- reload, no GVN forwarding
//   ...
//   and eax, 0xffcf                     <- mask in a register
//   ...
//   mov word ptr [esi+0x69], ax         <- store sunk to the end of the run
// i.e. the second clear reloads the word instead of being folded into the
// first one. MSVC 5 always folds two read-modify-writes to the same word in
// one block (plain short, bitfield, 16-bit bitfield, union member, cast to a
// second struct type, and a local holding the value were all tried and all
// fold), and the only forms that stop the fold (a by-reference helper, a
// block boundary) change the first clear or the layout instead. The reload
// also pins ax across the rest of the run, which is why the g_game pointer
// ends up in edx rather than eax and why +0x4a is stored before +0x4e.

struct Vec3_0049c740 {
    int x;
    int y;
    int z;
};

struct Unit_0049c740;

struct Shot_0049c740 {
    char unknown_0[0xf4];
    unsigned short sound;              // +0xf4
};

struct Slot_0049c740 {
    Shot_0049c740* shot;               // +0x0
    char unknown_4[0x18];              // the three slots are 0x1c apart
};

#pragma pack(push, 1)
struct Unit_0049c740 {
    char unknown_0[0x10];
    Slot_0049c740 slots[3];            // +0x10
    char unknown_64[0xb0 - 0x64];
    int field_b0;                      // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char field_ff;            // +0xff, the owner's player colour
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Proj_0049c740 {
    Shot_0049c740* shot;               // +0x00
    Vec3_0049c740 pos;                 // +0x04
    Vec3_0049c740 pos2;                // +0x10
    char unknown_1c[0x28 - 0x1c];
    Vec3_0049c740 target;              // +0x28
    char unknown_34[0x42 - 0x34];
    int field_42;                      // +0x42
    char unknown_46[0x4a - 0x46];
    int field_4a;                      // +0x4a, the current frame number
    int field_4e;                      // +0x4e
    Unit_0049c740* owner;              // +0x52, the unit that fired it
    int field_56;                      // +0x56
    char unknown_5a[0x60 - 0x5a];
    short active;                      // +0x60
    short piece;                       // +0x62, which barrel it came from
    char unknown_64[0x66 - 0x64];
    unsigned char player;              // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;              // +0x69
    // Written as an inline method the load of u->field_ff comes before the
    // store of owner, as the original has it; in the enclosing function it
    // sinks below it.
    void SetOwner(Unit_0049c740* u) { player = u->field_ff; owner = u; }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Game_0049c740 {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Proj_0049c740* projectiles;        // +0x141f7
    char unknown_141fb[0x142f3 - 0x141fb];
    Unit_0049c740* trackedUnit;        // +0x142f3
    Proj_0049c740* trackedProj;        // +0x142f7
    char unknown_142fb[0x38a47 - 0x142fb];
    int field_38a47;                   // +0x38a47, the current frame number
};
#pragma pack(pop)

extern Game_0049c740* g_game;

// Taking the word by reference is what keeps MSVC from folding the two flag
// clears into one `and word ptr [esi+0x69], 0xffce`, which is the shape the
// first clear has in the original.
static inline void Clear45(unsigned short& f) { f = f & ~0x30; }

int __stdcall FUN_0043e1e0(Unit_0049c740* unit, unsigned char weapon);
void __stdcall FUN_0047f300(int sound, Vec3_0049c740* pos, int param_3);

// FUNCTION: 0x49c740
void __stdcall FUN_0049c740(Proj_0049c740* proj, Shot_0049c740* shot, Vec3_0049c740* pos,
                           Vec3_0049c740* aim, int field_5, Unit_0049c740* unit)
{
    unsigned char i;
    proj->shot = shot;
    proj->pos = *pos;
    proj->pos2 = *pos;
    if (aim)
        proj->target = *aim;
    proj->field_42 = field_5;
    proj->flags &= ~1;
    proj->active = 0;
    Clear45(proj->flags);
    proj->field_56 = 0;
    proj->field_4a = g_game->field_38a47;
    proj->field_4e = 0;
    if (unit) {
        proj->SetOwner(unit);
        if ((unit->flags & 0x20000000) && g_game->trackedUnit == unit)
            g_game->trackedProj = proj;
        for (i = 0; i < 3; i++)
            if (unit->slots[i].shot == shot)
                break;
        proj->piece = (short)FUN_0043e1e0(unit, i);
        unit->field_b0 = g_game->field_38a47 + 0x258;
    } else {
        proj->player = 0xa;
        proj->owner = 0;
    }
    FUN_0047f300(shot->sound, pos, 0);
}

// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// Initialises one entry of the 300-entry projectile array (g_game+0x141f7):
// copies the muzzle position into two fields, optionally a third position,
// clears flag bits, and takes the firing unit's colour, its tracked
// projectile slot, the barrel the shot came out of and the frame number.
//
// The one shape that matters here: the two flag clears must not fold into a
// single `and word ptr [esi+0x69], 0xffce`. The original does
//   and word ptr [esi+0x69], 0xfffe     <- the first clear, stored in place
//   mov ax, word ptr [esi+0x69]          <- a 16-bit local re-reads the word
//   ...
//   and eax, 0xffcf                     <- the mask, at 32 bits
//   ...
//   mov word ptr [esi+0x69], ax         <- the store sinks below field_4e
// so the second clear goes through an `unsigned short` local taken from the
// field after the first clear, and the store of that local is the last of the
// run. A local alone is not enough: taken as `int` it stays 16-bit folded,
// and without the later stores to field_4a, field_56 and field_4e the reload
// folds back to one in-place mask. The 32-bit `and eax` with a 16-bit store
// is what an `unsigned short` local gives; a plain `&=` on the field is
// emitted as an in-place `and word ptr` and can never produce it.
// The frame number store (field_4a) has to be read before the mask, which
// puts g_game into edx rather than eax.

#pragma pack(push, 1)
struct Vec3_0049c740 {
    int x;
    int y;
    int z;
};

struct Unit;

struct Shot_0049c740 {
    char unknown_0[0xf4];
    unsigned short sound;              // +0xf4
};

struct Slot_0049c740 {
    Shot_0049c740* shot;               // +0x0
    char unknown_4[0x18];              // the three slots are 0x1c apart
};

struct Unit {
    char unknown_0[0x10];
    Slot_0049c740 slots[3];            // +0x10
    char unknown_64[0xb0 - 0x64];
    int field_b0;                      // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char field_ff;            // +0xff, the owner's player colour
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};

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
    Unit* owner;                       // +0x52, the unit that fired it
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
    void SetOwner(Unit* u) { player = u->field_ff; owner = u; }
};

struct Game_0049c740 {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Proj_0049c740* projectiles;        // +0x141f7
    char unknown_141fb[0x142f3 - 0x141fb];
    Unit* trackedUnit;                 // +0x142f3
    Proj_0049c740* trackedProj;        // +0x142f7
    char unknown_142fb[0x38a47 - 0x142fb];
    int field_38a47;                   // +0x38a47, the current frame number
};
#pragma pack(pop)

extern Game_0049c740* g_game;

int __stdcall FUN_0043e1e0(Unit* unit, unsigned char weapon);
void __stdcall FUN_0047f300(int sound, Vec3_0049c740* pos, int param_3);

// FUNCTION: 0x49c740
void __stdcall FUN_0049c740(Proj_0049c740* proj, Shot_0049c740* shot, Vec3_0049c740* pos,
                           Vec3_0049c740* aim, int field_5, Unit* unit)
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
    unsigned short f = proj->flags;
    proj->field_4a = g_game->field_38a47;
    proj->field_56 = 0;
    proj->field_4e = 0;
    proj->flags = f & ~0x30;
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

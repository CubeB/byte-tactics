// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// Aim a unit's gun at a point and fire it: work out the two aim angles (a
// ballistic solve when weapon flag bit 1 is set, CalcAimAngles when bit 0 is),
// check them, add a random spread, then fire and tell the network.
//
// The last diff was the failure path's `or byte ptr [edi + 0xbb], 0x10`.
// That is a 1-bit store, not an `unsigned char` `|=`: declaring +0xba as an
// `unsigned short` bitfield (bit 12 lands on bit 4 of byte 0xbb) gives the
// memory read-modify-write and leaves eax holding the zero `ok` already put
// there, so the `return 0` costs nothing. A plain `unsigned char f_bb |= 0x10`
// loads through al and needs a fresh `xor eax, eax` (4 extra instructions).
//
// What earlier passes got wrong, all corrected here:
//  - the spread divisor is f_b8 / 12, not / 3: 0x2aaaaaab with `sar edx, 1` is
//    the signed magic for 12 (this was the "signed magic for 3" mystery).
//  - `def` is only used for the two flag tests, speed, pitch and the pointer
//    passed to CalcAimAngles; the spread reads unit->f_c->f_104 afresh. That
//    made def spill to [esp+0x10] as in the original (77.6 to 87.1).
//  - CalcAimAngles's weapon parameter is an unsigned char (the argument is built
//    with `shr al, 2; and al, 3` and pushed whole).
//  - the spread block uses `range >> 1` inline, not a `half` local (89.7 to 93.2).
//  - the two fire calls take the f_1b group first; heading and pitch are
//    `short`; the tail re-reads unit->f_c instead of using def.
#include <math.h>

#pragma pack(push, 1)
struct Vec3_0049d580 {
    int x;
    int y;
    int z;
};

struct Player_0049d580 {
    char unknown_0[4];
    int id;                           // +0x4
};

struct UnitDef_0049d580 {
    char unknown_0[0x1fa];
    unsigned int divisor;             // +0x1fa
};

union Flags_0049d580 {
    struct {
        unsigned int f0 : 1;          // bit 0
        unsigned int f1 : 1;          // bit 1
        unsigned int f2_29 : 28;
        unsigned int f30 : 1;         // bit 30
        unsigned int f31 : 1;
    } b;
    unsigned int value;               // +0x111
};

struct Weapon_0049d580 {
    char unknown_0[0x68];
    int speed;                        // +0x68
    char unknown_6c[0xc8 - 0x6c];
    float pitch;                      // +0xc8
    char unknown_cc[0x104 - 0xcc];
    short f_104;                      // +0x104
    char unknown_106[0x10a - 0x106];
    unsigned char team;               // +0x10a
    char unknown_10b[0x111 - 0x10b];
    Flags_0049d580 flags;             // +0x111
};

struct Unit {
    char unknown_0[8];
    int f_8;
    Weapon_0049d580* f_c;
    char unknown_10[0x16 - 0x10];
    short f_16;                       // the aim heading
    short f_18;                       // the aim pitch
    char unknown_1a;
    unsigned char f_1b;               // bit 0 = needs aiming, bits 2 and 3 = weapon
    char unknown_1c[0x66 - 0x1c];
    short f_66;                       // the unit's own heading
    char unknown_68[0x92 - 0x68];
    UnitDef_0049d580* f_92;           // the unit definition
    Player_0049d580* f_96;            // the owner
    char unknown_9a[0xa8 - 0x9a];
    short f_a8;
    char unknown_aa[0xb8 - 0xaa];
    unsigned short f_b8;              // the aiming inaccuracy
    union {                           // +0xba, bit 12 is the "cannot aim" flag
        unsigned short value;
        struct {
            unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1;
            unsigned short b4 : 1, b5 : 1, b6 : 1, b7 : 1;
            unsigned short b8 : 1, b9 : 1, b10 : 1, b11 : 1;
            unsigned short b12 : 1, b13 : 1, b14 : 1, b15 : 1;
        } f;
    } f_bb;
    char unknown_bc[0x108 - 0xbc];
    short f_108;
};

struct Packet_0049d580 {
    unsigned char type;               // +0x00
    Vec3_0049d580 a;                  // +0x01, the gun's world position
    Vec3_0049d580 b;                  // +0x0d, the aimed at point
    unsigned char team;               // +0x19
    unsigned char unknown_1a;         // +0x1a, never assigned
    short heading;                    // +0x1b
    short pitch;                      // +0x1d
    short target_a8;                  // +0x1f
    short unit_a8;                    // +0x21
    unsigned char weapon;             // +0x23
};

struct Game {
    char unknown_0[0x2a44];
    unsigned char flags;              // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall GetAimFromPosition(Unit* obj, Vec3_0049d580* out, unsigned char weapon);
void __stdcall GetWeaponPiecePosition(Unit* obj, Vec3_0049d580* out, unsigned char weapon, int piece);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall SolveLaunchAngle(int dx, int dy, int dz, int speed, float pitch);
int __stdcall CalcAimAngles(Unit* unit, Weapon_0049d580* target, short* out_heading,
                           short* out_pitch, unsigned char weapon, Vec3_0049d580* point);
int __stdcall AimWithinTolerance(Unit* unit, Unit* aim, short angle1, short angle2);
int __stdcall FireLineOfSightProjectile(Unit* fire, Unit* unit, Vec3_0049d580* p3,
                           Vec3_0049d580* point, Unit* target);
int __stdcall FireBallisticProjectile(Unit* shot, Unit* unit, Vec3_0049d580* pos,
                           Vec3_0049d580* aim, Unit* target);
int __stdcall RandomInt(int range);
int __stdcall BroadcastPacket(int player, void* data, int size);

// FUNCTION: 0x49d580
int __stdcall FireTurretWeapon(Unit* fire, Unit* unit,
                           Unit* target, Vec3_0049d580* point)
{
    if ((unit->f_1b & 1) && unit->f_8) {
        Weapon_0049d580* def = unit->f_c;
        short heading;
        short pitch;
        int ok;
        if (def->flags.b.f1) {
            Vec3_0049d580 p;
            GetAimFromPosition(fire, &p, unit->f_1b >> 2 & 3);
            int dx = p.x - point->x;
            int dy = p.y - point->y;
            int dz = p.z - point->z;
            heading = FUN_004b715a(dx, dz) - fire->f_66;
            pitch = SolveLaunchAngle(dx, dy, dz, def->speed, def->pitch);
            ok = (unsigned short)pitch != 0x8000;
        } else if (def->flags.b.f0) {
            ok = CalcAimAngles(fire, def, (short*)&heading, (short*)&pitch,
                              unit->f_1b >> 2 & 3, point);
        } else {
            ok = 0;
        }
        if (!ok) {
            unit->f_1b &= 0xfe;
            fire->f_bb.f.b12 = 1;
            return 0;
        }
        if (!AimWithinTolerance(fire, unit, heading, pitch)) {
            unit->f_1b &= 0xfe;
            return 0;
        }
        Vec3_0049d580 gunpos;
        GetWeaponPiecePosition(fire, &gunpos, unit->f_1b >> 2 & 3, -1);
        unit->f_16 += fire->f_66;
        short spread = unit->f_c->f_104 - (short)((fire->f_108 << 11) / fire->f_92->divisor) + 0x800;
        int parts = fire->f_b8 / 12;
        if (parts > 1)
            spread = (unsigned short)spread / parts;
        if (spread) {
            unsigned short range = spread;
            unit->f_16 += (short)(RandomInt(range) - (range >> 1));
            unit->f_18 += (short)(RandomInt(range) - (range >> 1));
        }
        int fired = 0;
        if ((unit->f_c->flags.value & 1) || (unit->f_c->flags.value & 0x100000))
            fired = FireLineOfSightProjectile(unit, fire, &gunpos, point, target);
        else if (unit->f_c->flags.b.f1)
            fired = FireBallisticProjectile(unit, fire, &gunpos, point, target);
        if (!fired)
            return 0;
        unit->f_8 = 0;
        unit->f_1b &= 0xfe;
        if (g_game->flags & 1) {
            Packet_0049d580 msg;
            msg.type = 0xd;
            msg.a = gunpos;
            msg.b = *point;
            msg.team = unit->f_c->team;
            msg.weapon = unit->f_1b >> 2 & 3;
            if (fire == 0)
                msg.unit_a8 = 0;
            else
                msg.unit_a8 = fire->f_a8;
            msg.target_a8 = target ? target->f_a8 : 0;
            msg.heading = unit->f_16;
            msg.pitch = unit->f_18;
            msg.unknown_1a = msg.unknown_1a ^ ((unit->f_c->flags.value >> 30 ^ msg.unknown_1a) & 1);
            BroadcastPacket(fire->f_96->id, &msg, 0x24);
        }
        return 1;
    }
    return 0;
}

// SUSPECTED ORIGINAL BUG (kept from an earlier pass, still worth reporting):
// the ballistic path (weapon flag bit 1) stores its heading into argument one's
// home slot, the shooter's pointer, and that slot is what the call to AimWithinTolerance
// at 0x49d681 then reads as its angle1 (`mov edx, dword ptr [esp + 0x58]`). The edi
// register still holds the real shooter pointer, so the first angle checked is the
// low 16 bits of a pointer. Worse, the third argument, the target unit, is read back
// at 0x49d745 from that same clobbered slot, so on this path the `target->f_a8` at
// 0x49d812 and the target handed to FireLineOfSightProjectile at 0x49d77d can both be a pointer's
// low half. `test al, 1` at 0x49d58e means the path is only taken when f_1b bit 0 is
// set, so this is the laser (flags bit 1) aiming path.


// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH. Stopped at the wall clock limit: 49.2%, 731 of 761 bytes.
// Every call, argument order, callee, branch direction, packet field offset and
// stack slot of the original is reproduced; what differs is the register
// allocation, which then permutes about forty otherwise identical instructions.
// The whole gap is ONE allocator decision, named first in the "still differs"
// note at the end of this file. Read that before trying anything else.
//
// Aim a unit's gun at a point and fire it: work out the two aim angles, either
// with the ballistic solver (weapon flag bit 1) or with FUN_0049d910 (flag
// bit 0), check them with the "close enough" helper, add the inaccuracy spread
// to the aim angles, fire through FUN_0049c9c0 or FUN_0049cde0 (flags bit 0,
// bit 20 and bit 1 again) and finally tell the other players with a type 0xd
// packet.
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

struct Unit_0049d580 {
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
    char unknown_ba;
    unsigned char f_bb;
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

struct Game_0049d580 {
    char unknown_0[0x2a44];
    unsigned char flags;              // +0x2a44
};
#pragma pack(pop)

extern Game_0049d580* g_game;

void __stdcall FUN_0043e2e0(Unit_0049d580* obj, Vec3_0049d580* out, unsigned char weapon);
void __stdcall FUN_0043e240(Unit_0049d580* obj, Vec3_0049d580* out, unsigned char weapon, int piece);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall FUN_0049a890(int dx, int dy, int dz, int speed, float pitch);
int __stdcall FUN_0049d910(Unit_0049d580* unit, Weapon_0049d580* target, short* out_heading,
                           short* out_pitch, int weapon, Vec3_0049d580* point);
int __stdcall FUN_0049d880(Unit_0049d580* unit, Unit_0049d580* aim, short angle1, short angle2);
int __stdcall FUN_0049c9c0(Unit_0049d580* fire, Unit_0049d580* unit, Vec3_0049d580* p3,
                           Vec3_0049d580* point, Unit_0049d580* target);
int __stdcall FUN_0049cde0(Unit_0049d580* shot, Unit_0049d580* unit, Vec3_0049d580* pos,
                           Vec3_0049d580* aim, Unit_0049d580* target);
int __stdcall FUN_004b6c30(int range);
int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x49d580
int __stdcall FUN_0049d580(Unit_0049d580* unit, int pitch, Unit_0049d580* target,
                           Vec3_0049d580* point)
{
    if ((unit->f_1b & 1) && unit->f_8) {
        Weapon_0049d580* def = unit->f_c;
        Unit_0049d580* u = unit;
        short heading;
        int ok;
        if (def->flags.b.f1) {
            Vec3_0049d580 p;
            FUN_0043e2e0(u, &p, unit->f_1b >> 2 & 3);
            int dx = p.x - point->x;
            int dy = p.y - point->y;
            int dz = p.z - point->z;
            int heading = FUN_004b715a(dx, dz) - u->f_66;
            pitch = FUN_0049a890(dx, dy, dz, def->speed, def->pitch);
            ok = (unsigned short)pitch != 0x8000;
        } else if (def->flags.value & 1) {
            ok = FUN_0049d910(u, def, &heading, (short*)&pitch, unit->f_1b >> 2 & 3, point);
        } else {
            ok = 0;
        }
        if (!ok) {
            unit->f_1b &= 0xfe;
            u->f_bb |= 0x10;
            return 0;
        }
        if (!FUN_0049d880(u, unit, heading, pitch)) {
            unit->f_1b &= 0xfe;
            return 0;
        }
        Vec3_0049d580 gunpos;
        FUN_0043e240(u, &gunpos, unit->f_1b >> 2 & 3, -1);
        unit->f_16 += u->f_66;
        short spread = def->f_104 - (short)((u->f_108 << 11) / u->f_92->divisor) + 0x800;
        int parts = u->f_b8 / 3;
        if (parts > 1)
            spread = (unsigned short)spread / parts;
        if (spread) {
            unsigned short range = spread;
            unsigned short half = range >> 1;
            unit->f_16 += (short)(FUN_004b6c30(range) - half);
            unit->f_18 += (short)(FUN_004b6c30(range) - half);
        }
        int fired = 0;
        if ((def->flags.value & 1) || (def->flags.value & 0x100000))
            fired = FUN_0049c9c0(u, unit, &gunpos, point, target);
        else if (def->flags.b.f1)
            fired = FUN_0049cde0(u, unit, &gunpos, point, target);
        if (!fired)
            return 0;
        unit->f_8 = 0;
        unit->f_1b &= 0xfe;
        if (g_game->flags & 1) {
            Packet_0049d580 msg;
            msg.type = 0xd;
            msg.a = gunpos;
            msg.b = *point;
            msg.team = def->team;
            msg.weapon = unit->f_1b >> 2 & 3;
            if (unit == 0)
                msg.unit_a8 = 0;
            else
                msg.unit_a8 = u->f_a8;
            msg.target_a8 = target ? target->f_a8 : 0;
            msg.heading = unit->f_16;
            msg.pitch = unit->f_18;
            msg.unknown_1a = msg.unknown_1a ^ ((def->flags.value >> 30 ^ msg.unknown_1a) & 1);
            FUN_00451df0(u->f_96->id, &msg, 0x24);
        }
        return 1;
    }
    return 0;
}

// STILL DIFFERS, and what to try next.
//
// ROOT CAUSE OF THE WHOLE 30 BYTE GAP: the unit pointer's register allocation.
// The original keeps TWO live copies of the first argument, both reloaded from
// the same argument slot (0x49d586 into esi, 0x49d5a4 into edi): esi for
// f_1b, f_8, f_c, f_16, f_18, as the `aim` argument of FUN_0049d880 and as the
// `unit` argument of FUN_0049c9c0/FUN_0049cde0; edi for f_66, f_bb, f_92, f_b8,
// f_108, f_a8, f_96 and as the `fire`/`shot` argument of those two calls and of
// FUN_0043e2e0, FUN_0043e240 and FUN_0049d880. That fills esi and edi, and then
// dx takes ebp and dz takes ebx in the ballistic path, so `def` and `dy` have
// nowhere but memory: the original's frame is 0x44 (17 dwords) and stores `def`
// at [E0+0x10] (0x49d5a8), reloading it at 0x49d5fe after FUN_004b715a.
//
// This build has one copy, so `def` takes ebp, dx takes edi, dz takes ebx, the
// frame is 0x40 (16 dwords) and every [esp+arg] reference in the function is
// then 4 or 8 bytes out, which is what the diff is full of. So the target to
// hit is "two live copies of the argument", and everything else follows.
//
// The copy is written here as `Unit_0049d580* u = unit;` used for the second
// set of accesses, and it is still folded away by MSVC 5's copy propagation:
// identical output whether `u` is declared at the top of the function or inside
// the if. NOT REACHED, and the thing to try next:
//  - a second type for the object, so the call arguments differ in type:
//    `Fire_0049d580* f = (Fire_0049d580*)unit;` with FUN_0049c9c0 declared
//    `int __stdcall (Fire_*, Unit_*, ...)` and the two calls written
//    `FUN_0049c9c0(f, unit, ...)` (tried, also propagated);
//  - the same copy through an int, `Unit* u = (Unit*)(int)unit;` (tried, also
//    propagated);
//  - anything that forces a load rather than a copy, e.g. keeping the pointer
//    in an aggregate whose address is taken;
//  - splitting the object into a base class and a derived one and letting the
//    calls that want the `fire`/`aim` argument take an implicit upcast of the
//    same variable, so the two call arguments are different IR values. Every
//    one of these five produced byte identical code to the version without a
//    second pointer, so copy propagation is not what stops it, and the
//    original's two registers must come from somewhere else, most likely from
//    the ORIGINAL having two source variables that are not copies of each
//    other at all (one being, say, a field of a containing object) and the
//    argument slot just happening to be where MSVC put the second one.
//
// TWO SMALLER THINGS, both confirmed from the disassembly:
//  - the divide by 3 of f_b8: the original emits the general signed sequence
//    `mov eax, 0x2aaaaaab; imul edx; sar edx, 1` while any spelling here emits
//    the non negative one, `mov eax, 0x55555556; imul edx`, so something in the
//    original's types leaves the dividend's sign unknown. Tried and rejected:
//    f_b8 as `unsigned short` (mine, zero extended 16 bit load but the short
//    magic), as `short` (moves the load to `movsx` and still takes the short
//    magic, 729 bytes) and as an `int` (that shifts f_bb and f_108, so it is
//    simply wrong at 0xbb and 0x108).
//  - the two angle slots. With E0 = esp just after the four prologue pushes, the
//    original keeps the pitch at E0+0x5c, the dead second argument, written as
//    a dword by the ballistic path and as 16 bits by FUN_0049d910 through a
//    `short*`, and the heading at E0+0x58, the dead first argument, which holds
//    the unit pointer before the FUN_0049d910 call. This build keeps the pitch
//    in the second argument (right) and puts the heading in a locals slot
//    (wrong). Since one address is written as a dword and read as a dword and
//    the other only ever as 16 bits, the pair may need a union, or an `int`
//    parameter cast to `short*` at the call; not reached.
//
// SUSPECTED ORIGINAL BUG, worth reporting: the ballistic path (weapon flag
// bit 1) stores its heading to E0+0x60, the third argument slot, and never
// reads it back; the call to FUN_0049d880 at 0x49d681 reads its angle1 from
// E0+0x58, which that path never writes, so on that path the first angle
// checked is whatever is in the dead first argument slot, i.e. the low 16 bits
// of the unit pointer (0x49d679 `mov edx, dword ptr [esp+0x58]`, against
// 0x49d5fa `mov dword ptr [esp+0x58], eax` with one push outstanding, i.e.
// E0+0x60). The third argument, the target unit, is then read at 0x49d745 from
// that clobbered slot, so the `target->f_a8` at 0x49d812 and the target passed
// to FUN_0049c9c0 can both be garbage on that path. `test al, 1` at 0x49d58e
// means that path is only taken when f_1b bit 0 is set, so this is the laser
// (flags bit 1) aiming path. A second candidate in the same block: the
// dividend of the divide by 3 has stale bits 16 to 31 (only `mov dx`, so the
// `xor edx, edx` half of `xor edx, edx / mov dx` is what makes it unsigned),
// yet the original uses the signed magic, so any f_b8 above 0x7fff (the
// inaccuracy field is 16 bits) would be divided wrongly.
//
// TRIED, all worse or neutral, none of these are worth repeating:
//  - `unit->f_c->...` in the tail instead of the `def` local. That is the
//    reading the disassembly supports (the original re-reads [esi+0xc] at
//    0x49d742, 0x49d7e5 and 0x49d830, so `def` really is dead by then), but
//    it scores 742 bytes against this file's 731 and still does not reach the
//    frame size, so the `def` spelling is kept;
//  - the `short heading` local in the outer scope versus declared inside the
//    FUN_0049d910 branch;
//  - `int` for the pitch parameter with a `(short*)&pitch` cast for the
//    FUN_0049d910 out parameter, versus a `short` parameter with an
//    `(unsigned short) != 0x8000` test for the ballistic return (the dword
//    store at 0x49d622 needs the int spelling);
//  - the flags as a plain `unsigned int` with `& 1`, `& 0x100000` and `>> 30`
//    (loses the `shr edx,1 / test dl,1` for bit 1, so the bitfield union is
//    required);
//  - logical or for the flag tests at 0x49d751 (needed, the original
//    branches);
//  - the two different spellings of the two packet ternaries copied from the
//    matching sibling 0x49d9c0 (if/else for unit_a8, ternary for
//    target_a8): both are in this file and both match the original's shape.

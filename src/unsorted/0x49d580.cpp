// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH. Stopped at the wall clock limit: 50.4 percent, 775 of 761 bytes.
// CORRECTION TO THE EARLIER NOTES IN THIS FILE: the original does NOT keep two
// copies of one argument. It has TWO unit pointer ARGUMENTS, and the register the
// earlier notes read as a copy is the other parameter. With S = esp on entry,
// after "sub esp,0x44" and the three pushes ebx/ebp/esi esp = S-0x50, so the load
// at 0x49d586 "mov esi, [esp+0x58]" reads S+8, the SECOND argument; and after the
// fourth push edi esp = S-0x54, so the load at 0x49d5a4 "mov edi, [esp+0x58]"
// reads S+4, the FIRST argument. Same test on every other reference: [esp+0x64]
// is S+0x10 = argument four (the point), [esp+0x60] is S+0xc = argument three
// (the target).
//
// So: esi (argument two) is the gun, it holds f_1b, f_8, f_c, f_16 and f_18 and is
// passed as the aim argument of FUN_0049d880 and as argument four of the two fire
// calls; edi (argument one) is the shooter, it holds f_66, f_92, f_108, f_b8,
// f_bb, f_a8 and f_96 and is passed as the fire/shot argument. A two parameter
// signature reproduces that exactly, and it is worth about one point over the old
// single argument spelling.
//
// Aim a unit's gun at a point and fire it: work out the two aim angles, either
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
int __stdcall FUN_0049d580(Unit_0049d580* unit, Unit_0049d580* fire,
                           Unit_0049d580* target, Vec3_0049d580* point)
{
    if ((unit->f_1b & 1) && unit->f_8) {
        Weapon_0049d580* def = unit->f_c;
        int heading;
        int pitch;
        int ok;
        if (def->flags.b.f1) {
            Vec3_0049d580 p;
            FUN_0043e2e0(fire, &p, unit->f_1b >> 2 & 3);
            int dx = p.x - point->x;
            int dy = p.y - point->y;
            int dz = p.z - point->z;
            heading = FUN_004b715a(dx, dz) - fire->f_66;
            pitch = FUN_0049a890(dx, dy, dz, def->speed, def->pitch);
            ok = (unsigned short)pitch != 0x8000;
        } else if (def->flags.value & 1) {
            ok = FUN_0049d910(fire, def, (short*)&heading, (short*)&pitch,
                              unit->f_1b >> 2 & 3, point);
        } else {
            ok = 0;
        }
        if (!ok) {
            unit->f_1b &= 0xfe;
            fire->f_bb |= 0x10;
            return 0;
        }
        if (!FUN_0049d880(fire, unit, heading, pitch)) {
            unit->f_1b &= 0xfe;
            return 0;
        }
        Vec3_0049d580 gunpos;
        FUN_0043e240(fire, &gunpos, unit->f_1b >> 2 & 3, -1);
        unit->f_16 += fire->f_66;
        short spread = def->f_104 - (short)((fire->f_108 << 11) / fire->f_92->divisor) + 0x800;
        int parts = fire->f_b8 / 3;
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
            fired = FUN_0049c9c0(fire, unit, &gunpos, point, target);
        else if (def->flags.b.f1)
            fired = FUN_0049cde0(fire, unit, &gunpos, point, target);
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
            if (fire == 0)
                msg.unit_a8 = 0;
            else
                msg.unit_a8 = fire->f_a8;
            msg.target_a8 = target ? target->f_a8 : 0;
            msg.heading = unit->f_16;
            msg.pitch = unit->f_18;
            msg.unknown_1a = msg.unknown_1a ^ ((def->flags.value >> 30 ^ msg.unknown_1a) & 1);
            FUN_00451df0(fire->f_96->id, &msg, 0x24);
        }
        return 1;
    }
    return 0;
}

// STILL DIFFERS, and what to try next.
//
// THE WHOLE REMAINING GAP IS STILL ONE ALLOCATION, but it is NOT the one the
// previous version of this note described. The old note said the original keeps
// two live COPIES of the first argument and that a copy would fix the frame
// size. That is wrong: the two loads read two different argument slots (see the
// header), so the original has two parameters, which is what this file now has.
// Every trick for manufacturing a second copy of one pointer (a second type, a
// cast through an int, a base/derived split, a local declared at the top or
// inside the if) produced byte identical code, and it is still byte identical
// code now that the second pointer is a genuine second parameter, which is the
// proof that none of them was ever the problem.
//
// WHAT IS STILL WRONG, precisely: this build hands esi to the f_66/f_92/f_bb
// group (the "fire" parameter) and edi to the f_1b/f_8/f_16 group (the "unit"
// parameter). The original is the other way round: esi is the f_1b group,
// loaded from argument two at 0x49d586, and edi is the f_66 group, loaded from
// argument one at 0x49d5a4. The swap is worth about forty instructions because
// every field access and every push of one of the two pointers moves.
//
// This build ranked the two parameters the same way whichever order they are
// declared in, so the order in the signature is not the lever, the USES are:
// the f_66 group is passed to six callees (FUN_0043e2e0, FUN_0043e240,
// FUN_0049d910, FUN_0049d880 and the two fire calls) and the f_1b group to
// three (FUN_0049d880 and the two fire calls), and MSVC gives esi to whichever
// pointer is live across the most calls. Both variants were measured:
//   build/scratch/0x49d580/v1.cpp  (fire first, unit second)  43.8 percent
//   build/scratch/0x49d580/v3.cpp  (unit first, fire second)  50.4 percent
// so with the roles fixed the argument order is worth about six points, and
// the two builds differ only in the two load displacements. The next thing to
// try is therefore to get the f_1b group live across more callees than the
// f_66 group, or the f_66 group across fewer, in a way that folds away: the
// agent guide's "register priority" entry is the lever, and a throwaway extra
// use in a scratch copy is the cheap way to confirm which side wins.
//
// A SECOND, INDEPENDENT SYMPTOM OF THE SAME ROTATION: the original's frame is
// 0x44 (17 dwords) and it spills the weapon definition pointer to [esp+0x10]
// at 0x49d5a8, reloading it at 0x49d5fe after FUN_0043e2e0. This build keeps
// the definition in ebp and only stores it into the argument home slot it is
// about to reuse, and its frame is 0x40 (16 dwords), so every frame offset in
// the function is four bytes out. With the two pointers in the right registers
// the four long lived values would be esi, edi, ebp and ebx (the two pointers,
// dx and dz) and the definition would have to spill, which is the original's
// shape; that is why this is probably the same single cause and not two.
//
// TWO SMALLER THINGS, both confirmed from the disassembly:
//  - the divide by 3 of f_b8: the original emits the general signed sequence
//    `mov eax, 0x2aaaaaab; imul edx; sar edx, 1` while any spelling here emits
//    the non negative one, `mov eax, 0x55555556; imul edx`, so something in the
//    original's types leaves the dividend's sign unknown. Tried and rejected:
//    f_b8 as `unsigned short` (mine, zero extended 16 bit load but the short
//    magic), as `short` (moves the load to `movsx` and still takes the short
//    magic) and as an `int` (that shifts f_bb and f_108, so it is simply wrong
//    at 0xbb and 0x108). The dividend is loaded with `xor edx, edx / mov dx`,
//    so the dividend itself is never negative; the signed magic is MSVC's
//    choice, and something in the surrounding code has to stop it proving that;
//  - the 16 bit subtraction of the ballistic heading: the original does
//    `sub ax, word ptr [edi + 0x66]` and then a FULL dword store of eax to the
//    heading slot (0x49d5f3 into 0x49d5fa), while a plain `int` heading emits
//    `movsx edx, word ptr [...]` plus a 32 bit `sub eax, edx`. A `short`
//    heading would give the 16 bit sub but a word store, so this wants an
//    aggregate: a union of an `int` and a `short`, or the heading written
//    through a `short` temporary into a four byte local.
//
// THE TWO ANGLE SLOTS ARE THE DEAD PARAMETER HOMES, which is now provable and
// settles the question the old note left open. With E = esp just after the
// fourth push, the original keeps the heading at E+0x58 (argument one's home,
// S+4) and the pitch at E+0x5c (argument two's home, S+8): the ballistic path
// stores the heading at 0x49d5fa (`mov [esp+0x58], eax`, E+0x58) and the pitch
// at 0x49d622 (`mov [esp+0x5c], eax`, E+0x5c), and the FUN_0049d910 path
// passes the very same two addresses as its out parameters (`lea edx,
// [esp+0x5c]` for the pitch, `lea eax, [esp+0x60]` with two pushes
// outstanding, which is also E+0x58, for the heading). So the two angles are
// plain locals whose storage MSVC overlays on the two now dead parameter
// homes; no union and no cast is needed, the only requirement is that both
// locals have their address taken and that the compiler sees the parameters
// as dead by then. In this build the same overlay happens one slot lower.
//
// SUSPECTED ORIGINAL BUG, worth reporting: the ballistic path (weapon flag
// bit 1) stores its heading into argument one's home slot, the shooter's
// pointer, and that slot is what the call to FUN_0049d880 at 0x49d681 then
// reads as its angle1 (`mov edx, dword ptr [esp+0x58]`, E+0x58). The edi
// register still holds the real shooter pointer, so the first angle checked is
// the low 16 bits of a pointer. Worse, the third argument, the target unit, is
// read back at 0x49d745 from that same clobbered slot, so on this path the
// `target->f_a8` at 0x49d812 and the target handed to FUN_0049c9c0 at 0x49d77d
// can both be a pointer's low half. `test al, 1` at 0x49d58e means the path is
// only taken when f_1b bit 0 is set, so this is the laser (flags bit 1) aiming
// path. A second candidate in the same block: the dividend of the divide by 3
// is loaded with only `mov dx` (0x49d6e0), yet the signed magic is used, so any
// f_b8 above 0x7fff (the inaccuracy field is 16 bits) would divide wrongly.
//
// TRIED, all worse or neutral, none of these are worth repeating:
//  - `unit->f_c->...` in the tail instead of the `def` local. That is the
//    reading the disassembly supports (the original re-reads [esi+0xc] at
//    0x49d742, 0x49d7e5 and 0x49d830, so `def` really is dead by then), but
//    it scored 742 bytes against 731 and still did not reach the frame size;
//  - the `short heading` local in the outer scope versus declared inside the
//    FUN_0049d910 branch;
//  - `int` for the pitch parameter with a `(short*)&pitch` cast for the
//    FUN_0049d910 out parameter, versus a `short` parameter with an
//    `(unsigned short) != 0x8000` test for the ballistic return;
//  - the flags as a plain `unsigned int` with `& 1`, `& 0x100000` and `>> 30`
//    (loses the `shr edx,1 / test dl,1` for bit 1, so the bitfield union is
//    required);
//  - logical or for the flag tests at 0x49d751 (needed, the original branches);
//  - the two different spellings of the two packet ternaries copied from the
//    matching sibling 0x49d9c0 (if/else for unit_a8, ternary for target_a8):
//    both are in this file and both match the original's shape;
//  - a single pointer with a `Unit_0049d580* u = unit;` copy, in every spelling
//    the old note lists, and the two parameter signature in both argument
//    orders. Compare the two scratch builds above: the roles are right in both,
//    only the argument order moves the score.

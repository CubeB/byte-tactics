// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH. Best so far: 77.6 percent, 768 of 761 bytes (7 over). Re-measured
// with check.py after every change below; the numbers in this header are current.
//
// WHAT THIS PASS CHANGED (50.4 -> 77.6), all established from the disassembly:
//  - the two fire calls take the f_1b group FIRST:
//        FUN_0049c9c0(unit, fire, &gunpos, point, target)
//        FUN_0049cde0(unit, fire, &gunpos, point, target)
//    The original pushes esi (f_1b) last, so it is arg one. (0x49d76d, 0x49d77c)
//  - the tail dispatch flags, the packet team byte and the packet >>30 flag all
//    RE-READ unit->f_c instead of using the `def` local, so `def` is dead well
//    before the packet. This is the single biggest win (72.4 -> 77.6). (0x49d742,
//    0x49d7d6, 0x49d830)
//  - heading and pitch are `short`, not `int`. This is what gives the original's
//    16-bit `sub ax, word ptr [edi+0x66]` instead of a movsx plus a 32-bit sub.
//    (0x49d5f3)
//  - the two parameters are (fire, unit) - the f_66 group is argument one. Only
//    correct ON TOP of the tail re-read above; on its own it scored worse.
//
// THE EARLIER NOTES IN THIS FILE WERE RIGHT that the two pointers are the two
// arguments (esi = arg two = the f_1b/gun group, edi = arg one = the f_66/shooter
// group), and right that argument order alone is not the lever. What the earlier
// notes got wrong is the claim that nothing structural was missing: re-reading
// unit->f_c in the tail is worth 27 points on its own.
//
// WHAT STILL DIFFERS, one coupled MSVC allocation state, not four separate bugs:
//  - `def` (unit->f_c). The original loads it into ecx and SPILLS it to a real
//    local at [esp+0x10] (0x49d5a1, 0x49d5a8), reloading it once at 0x49d5fe.
//    This build keeps it in ebx and spills it into a dead argument home instead,
//    so every frame offset in that region is four bytes out. The spill is forced
//    only when def's live range ends before the spread: dropping the spread's
//    `def->` DOES produce the correct [esp+0x10] spill (verified) but lands def
//    in edx rather than ecx and scores 75.2, so the two effects pull apart.
//  - that same choice rotates the two angle slots. The original keeps heading at
//    E+0x58 and pitch at E+0x5c (the two dead argument homes, E = esp after the
//    fourth push: S+4 and S+8), confirmed by the d910 out-params at 0x49d637 and
//    0x49d642. This build puts one angle in a real local and the other in an
//    argument home.
//  - the flag read. The original does `mov edx,[ecx+0x111]; shr edx,1; test dl,1`
//    - def already in ecx, so the flags go straight into the shift register. Every
//    spelling here loads them into ecx first and copies, because def never lands
//    in ecx: it either stays in ebx or (when spilled) lands in edx, and the shift
//    register then conflicts. The bl is the f_8 test reusing ecx: the original
//    reloads def into that same ecx, nothing here does.
//  - the divide by 3 of f_b8: the original emits the general signed sequence
//    `mov eax,0x2aaaaaab; imul edx; sar edx,1` plus the round-toward-zero fixup,
//    any spelling here emits the non-negative `mov eax,0x55555556; imul edx`. The
//    dividend is loaded with `xor edx,edx / mov dx`, so it is never negative, yet
//    MSVC still chose the signed magic. f_b8 as unsigned short, short, int, a cast
//    to int, and a signed local were all tried; unsigned short is the only one
//    that keeps the 16-bit load the original has, and it is the one kept.
//  - `fire->f_bb |= 0x10` is `or byte ptr [edi+0xbb],0x10`, a pure memory
//    read-modify-write, because the result is dead. Spelling it as
//    `= (unsigned char)(x | 0x10)` or `= x | 0x10` still round-trips through al.
//
// TRIED, flat or worse, do not repeat: swapping the parameter order on its own
// (43.8); putting the fire arguments back (72.4 vs 77.6); re-reading only the
// team byte (63.7) or only the >>30 flag without the dispatch flags; f_b8 as
// short, int, unsigned short local, (int) cast, (int)(short) cast; the divisor
// spelled (int)3; parts as short/unsigned short; spread as int or unsigned short
// or an extra (short) cast; the packet fields reordered (type/weapon before the
// vector copies, or the two vector copies swapped); both ternaries collapsed into
// one spelling; the two units as an array; heading declared before pitch; p read
// through three int temporaries; a `const` def; a local holding def->flags.value.
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
int __stdcall FUN_0049d580(Unit_0049d580* fire, Unit_0049d580* unit,
                           Unit_0049d580* target, Vec3_0049d580* point)
{
    if ((unit->f_1b & 1) && unit->f_8) {
        Weapon_0049d580* def = unit->f_c;
        short heading;
        short pitch;
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
        if ((unit->f_c->flags.value & 1) || (unit->f_c->flags.value & 0x100000))
            fired = FUN_0049c9c0(unit, fire, &gunpos, point, target);
        else if (unit->f_c->flags.b.f1)
            fired = FUN_0049cde0(unit, fire, &gunpos, point, target);
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
            FUN_00451df0(fire->f_96->id, &msg, 0x24);
        }
        return 1;
    }
    return 0;
}

// SUSPECTED ORIGINAL BUG (kept from an earlier pass, still worth reporting):
// the ballistic path (weapon flag bit 1) stores its heading into argument one's
// home slot, the shooter's pointer, and that slot is what the call to FUN_0049d880
// at 0x49d681 then reads as its angle1 (`mov edx, dword ptr [esp + 0x58]`). The edi
// register still holds the real shooter pointer, so the first angle checked is the
// low 16 bits of a pointer. Worse, the third argument, the target unit, is read back
// at 0x49d745 from that same clobbered slot, so on this path the `target->f_a8` at
// 0x49d812 and the target handed to FUN_0049c9c0 at 0x49d77d can both be a pointer's
// low half. `test al, 1` at 0x49d58e means the path is only taken when f_1b bit 0 is
// set, so this is the laser (flags bit 1) aiming path. A second candidate in the same
// block: the dividend of the divide by 3 is loaded with only `mov dx` (0x49d6e0),
// yet the signed magic is used, so any f_b8 above 0x7fff would divide wrongly.


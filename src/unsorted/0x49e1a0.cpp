// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free. Names are provisional.
// STATUS: partial, 84.7% (ours 969 bytes against the original 969, no MATCH
// yet). Up from 60.7%. The frame, the whole aim block, the cooldown
// arithmetic, both script plus sound call pairs and the flag word update all
// match now. What is left is a handful of register choices and one branch.
//
// What it is: the per-unit update of the three weapon slots. unit->entries[3] sits
// at +0x04, 0x1c bytes per entry, walked with esi += 0x1c (0x49e54c) and a byte
// counter compared with 3 (0x49e54f). Per slot: decay the cooldown (word at
// esi-7, entry+0x18), ask FUN_0048a1e0 for the slot's world position, aim the slot
// (independent-aim path FUN_0043e2e0 + FUN_0049e570 + FUN_004b715a +
// FUN_0049a890, or the shared path FUN_0049d910), fire the script trigger
// 0x4b0a70 plus the 0x456200 call, then when the cooldown is zero check the ammo
// count (entry+0x1e) or store metal and energy ([edi+0xec]+0x8c and +0x98)
// against the target's cost (target+0xc0 and +0xc4), call the vtable fire
// function at target+0x60, and either decrement the ammo count (bit 28 of
// target+0x111) or recompute the cooldown from unit+0xb8, unit+0x108 and
// unit->type+0x1fa. Finally it sets a bit in the flag word at unit+0xba, calls
// 0x4012a0 to spend metal and energy when bit 0x10000000 of target+0x111 is
// clear, or sets 0x10 in unit+0xbb when the range or cost check failed.
//
// Frame: __stdcall, ret 4, one stack argument. "sub esp,0x30" plus ebx, ebp, esi
// and edi, so locals occupy [esp+0x10, esp+0x40) and the argument home slot
// [esp+0x44] is reused as scratch once edi holds the unit. Slots: +0x10 the loop
// index (dword), +0x14 the heading (dword), +0x18 the byte loop counter, +0x1c
// dz, +0x20 dx and +0x24 dy (dwords), +0x28 the slot position Vec3, +0x34 the
// aim Vec3 out of FUN_0043e2e0, and +0x44 the aim angle (written at 0x49e29c and
// 0x49e2ce). 12 dwords of locals in total.
//
// What the previous attempt had wrong, and what fixed it. The 60.7% draft read
// its target pointer once into a single local named "attached", so MSVC had one
// register for it and burned ebp as a zero register. The original keeps TWO
// live copies: ebx from the loop head (0x49e1bd) and a second load into ebp
// (0x49e21b) inside the aim block. Declaring a second local for the aim block
// alone, "Target* t = e->attached;" just inside the "if (!(e->flags & 1))"
// block, and leaving the outer f60 and b19 tests on the first copy, is what
// moved 62.9% to 72.7%. Three more changes took it to 84.7%:
//   - target+0x111 is a BITFIELD, not a plain unsigned int. Declaring
//     b0:1, b1:1, b2_3:2, b4:1, b5_18:14, b19:1, b20_25:6, b26:1, b27:1, b28:1,
//     b29_31:3 makes MSVC emit "mov edx,eax / shr edx,0x13 / test dl,1" instead
//     of "test ecx,0x80000" (worth 1.3 points on its own). Every bit is a
//     bitfield EXCEPT bit 26, which is the one place the original reads the
//     word raw ("shr eax,0x1a / and al,1"), so that one use goes through the
//     f_111_raw() alias (worth a further 0.2). Bitfields cannot be reached
//     through a union member in MSVC 5 (error C2039), so the alias is a
//     method returning "*(unsigned int*)&f_111".
//   - the FUN_0049d910 out parameters are unsigned short* and the weapon
//     parameter is unsigned char, not int. The byte width is what produces
//     "shr al,2 / and al,3" at 0x49e2ae instead of a 32-bit shift (1.6 points).
//   - DAT_00509688[(e->flags >> 2) & 3] is hoisted into a local "char* nm" in
//     the first script pair only, which is what lets MSVC tail-merge the
//     FUN_00456200 call the way the original does with the jmp at 0x49e33b
//     (5.7 points, and the exact 969-byte size came with it). Hoisting it in
//     the second pair as well scores 82.1%, so only the first one is hoisted.
//
// Ranges that now match instruction for instruction: the prologue
// 0x49e1a0-0x49e1ed; 0x49e22b-0x49e2a0 the whole aim and angle block including
// the "cmp cx,0x8000" and "setne al"; 0x49e2e8-0x49e33b the first script plus
// sound pair and its jump into the shared tail; 0x49e35b-0x49e3a6 the second
// pair; 0x49e393-0x49e3ab the shared tail; 0x49e3ed-0x49e419 the float metal
// and energy compare; 0x49e468-0x49e4ee the cooldown arithmetic including both
// magic-multiply idioms; 0x49e4f2-0x49e50b the flag word update; and the
// epilogue at 0x49e55f-0x49e566. Only jump displacements differ inside those.
//
// What still differs, all of it register choice rather than shape (61 of 310
// instructions):
//  1. The loop tail. The original reloads the index into ebx at 0x49e545 and
//     increments it there (0x49e54b, 0x49e555); we keep it in edx. ebx holds
//     the target at the loop head, so the tail needs a third copy. Four
//     declaration orders were tried and none moved it; the extra live range
//     the original wants is not expressible through a source reorder.
//  2. The b4 test. The original reaches it with "mov edx,eax / shr edx,4 /
//     test dl,1" reusing the eax that still holds target+0x111 from the b19
//     test at 0x49e1fd; we reload. Our b4 is a bitfield, and a bitfield read
//     cannot reuse the parent word that a different bit's read left in eax.
//  3. The b0 branch's jump target (0x49e2ce) and the two "mov ecx,[esp+0x4c]"
//     style argument reloads at 0x49e185-0x49e193, which read through a
//     different outstanding-push depth than ours.
//  4. The FUN_004012a0 argument registers: ecx and edx in the original at
//     0x49e51f-0x49e52d, edx and eax in ours. Declaring the parameters int
//     instead of float, and the flag word as a bare unsigned short, both cost
//     3.5 points, so the float form is right and only the register pick is
//     off.
//  5. The second script pair's name register (edx versus eax) and the
//     "lea ecx" versus "lea eax" for &e->name at 0x49e1d9.
//
// Dead ends recorded so nobody repeats them: a weapon-index local (unsigned
// char w) drops to 55.7% because MSVC then keeps it live across both pairs;
// declaring int can without an initialiser, or as a boolean assignment, costs
// 8 points (the "xor edx,edx / mov edx,1" pair at 0x49e3db and 0x49e419 is what
// the original does, and it wants the block form); reading target+0x111
// through e->attached everywhere instead of a cached local drops to 48.1%;
// short rather than unsigned short locals cost 0.3; the 0x10000000 tests as
// raw shifts rather than bitfields cost 1.6.
//
// More dead ends measured by deepseek-v4.1-flash (deepseek-v4.1-flash):
// writing the bit-26 test as the bitfield member attached->f_111.b26 emits the
// exact original shape (mov ecx/ebx+0x111, shr ecx,0x1a, and cl,1, neg cl,
// sbb ecx,ecx, and ecx,0x400, add ecx,0x400, or [edi+0xba],cx) but picks ecx
// where the original uses eax, and the file scores 86.8 versus 86.9, so the
// f_111_raw() alias stays. Factory-materialising the b4 test into a local does
// NOT turn "test al,0x10" into "mov edx,eax / shr edx,4 / test dl,1" (both
// variants are 86.8). Hoisting a raw "unsigned int f = attached->f_111_raw()"
// for the b19/b4 pair and spelling them as (f>>19)&1 and (f>>4)&1 explodes the
// frame to 979 bytes and drops to 74.2%: the extra live range costs more than
// the two shifts win. So the bitfield-plus-alias split stays the best variant.
//
// The calling convention was checked first and is NOT the cause. The function
// returns with "ret 4" and has one stack argument, so __stdcall with one
// argument is correct and was already in place. All 11 callees agree with
// their ctx.py ret values.
//
// ---- NOTES CARRIED OVER from the earlier 73.5% version (LongCat 2.5 Preview
// Free), kept verbatim below the new notes. Read them as history: this file is
// 84.7% and exact in size, so the scores, byte counts and 'STILL OPEN' items
// below describe the OLDER file, and some of them are now fixed or superseded.
// The facts about what the original does (the eager s->def load, the
// &s->target argument, the /5 divide, the field widths) are still the useful part.
//
//
// GAVE UP at 73.5% (993 of 969 bytes; ours is 24 bytes LONGER than the
// original, so the tail is over-long as well as the frame being permuted).
// Two workers contributed; the winner is a merge, not one agent's file.
//
// THE KEY FACT, which is worth more than the score. The three prologue
// differences all came from ONE cause: the original never materialises a zero in
// a register, which leaves ebx free to hold `s->def` across the flags test.
// Fixing that took 33.3% -> 73.5%, and it needed TWO loads, not one:
//
//   original:  mov byte ptr [esp+0x18], 0
//              mov dword ptr [esp+0x10], 0
//              mov al, byte ptr [esi]        ; esi = &s->flags
//              mov ebx, dword ptr [esi-0xf]  ; s->def, EAGERLY
//              test al, 2
//   earlier:   xor ebx, ebx
//              mov byte ptr [esp+0x14], bl
//              test byte ptr [esi], 2        ; s->def loaded after the test
//
// A single hoisted `def` was NOT enough (36.0% -> 33.1%, worse). What worked was
// hoisting `def` for the loop body AND a second `d = s->def` declared inside the
// `flags.bits.b19` branch for the b1/b0 sub-branches. The second copy takes ebp,
// which is what stopped MSVC inventing a zero register; the `xor ebp,ebp`,
// `cmp ax,bp` and `cmp [ebx+0x60],ebp` forms all disappear at once.
//
// esi is &s->flags, not s: [esi-0xf] is s+0x1b-0xf = s+0xc, which is `def`.
//
// A whole call was missing, and finding it was worth 31.4% -> 73.5% on its own:
//   def->fire(unit, &s->target, FUN_0048a190(unit, n), &aimPoint)
// must be spelled `&s->target`, NOT `&s`. Passing `&s` re-anchors the loop and
// collapses the allocation to 31.4%.
//
// Other measured corrections: the reload divide is `/5` (magic 0x66666667), not
// `/30`; `field_108 * 100` is `* 20`; `s->name` is passed as `&s->name`;
// `heading` is a `short`; the field_14 test is `if (s->field_14 > 0)`; the
// `0x400 + (b26 ? 0x400 : 0)` needs a named `int` temp or MSVC narrows it to 16
// bits; the `FUN_0049aa80` branch is inverted with `int fl2` hoisted to the loop
// body (68.1% vs 65.6%); and the +0xba/+0xbb overlapping ORs need a two-byte
// union, `field_66`/`field_b8`/`field_14` are unsigned short, and
// `unknown_f0[0x18]` (not `[0x108-0xec]`) so `field_108` lands on +0x108.
//
// STILL OPEN, in the order I would attack them:
// 1. The frame-slot permutation, which is the main blocker. Ours is
//    heading@0x10, i@0x14, n@0x18; the original needs n@0x10, heading@0x14,
//    i@0x18. PROVEN INVARIANT to declaration order (all six permutations give
//    identical offsets), variable names, scope (function vs for-init vs loop
//    body), type (int/short/unsigned short/char), initialisers, and moving the
//    zeroing out of the for-init. The tail (dz,dx,dy,aimPoint,muzzle,pitch) is
//    reverse declaration order and only this trio is rotated, so this is
//    IR-determined. A `Frame_0049e1a0` struct pins the layout but destroys the
//    `lea esi, [edi+0x1f]` pointer walk, so it is not the answer as written.
//    Since it is not a declaration-side lever, attack it from the CODE side: a
//    structural change elsewhere may flip it. Worth trying a single
//    `FUN_00456200` call site after the if/else (see 4), and a `static inline`
//    helper for `(s->flags >> 2) & 3`.
// 2. `ok` in the b1 branch: original keeps it in eax, ours in ecx. A branchy
//    `int ok = 0; if (...) ok = 1;` reproduces the original's SHAPE exactly
//    (verified) but scores 0.5% lower because of the register choice.
// 3. The metal/energy `ok` is branchy in the original (`xor edx,edx` ... `mov
//    edx,1` ... `test edx,edx`); ours if-converts to `setne`.
// 4. The two `FUN_00456200` tails: the original shares
//    `mov al,[esi]; push 2; shr; and; mov ecx,[DAT]; push ecx; push edi; call`
//    between both branches, ours duplicates the last three pushes.
// 5. `or byte [esi],1` (original, a direct memory RMW) vs `mov cl,[esi]; or
//    cl,1; mov [esi],cl`; `or byte [edi+0xbb],0x10` sits mid-block instead of
//    last before the loop tail; `test al,0x10` vs `mov edx,eax; shr edx,4;
//    test dl,1` for b4.
//
// REJECTED BY MEASUREMENT, do not repeat:
//   - a single hoisted `def` alone: 36.0% -> 33.1% (worse)
//   - splitting the comma-separated for-init into two statements: no change
//   - removing the `= 0` initialisers (already absent): no change
//   - all six permutations of the n/heading/i declarations: identical offsets
//   - a Frame struct to pin the layout: correct offsets, breaks the pointer walk
#include <string.h>

struct Vec3_0049e1a0 {
    int x;
    int y;
    int z;
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct Store_0049e1a0 {
    char unknown_0[0x8c];
    float metal;                        // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                       // +0x98
};
#pragma pack(pop)

#pragma pack(push, 1)
class Class_004012a0 {
public:
    char unknown_0[0x4];
    float x0;
    char unknown_8[0x1c - 0x8];
    float y0;
    char unknown_20[0x30 - 0x20];
    Store_0049e1a0* store;              // +0x30
    int FUN_004012a0(float dx, float dy);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Point_0049e1a0 {
    short x;
    short z;
};

struct Unit_0049e1a0;

// The unit a slot points at. Its class vtable sits at +0x60.
struct Target_0049e1a0 {
    char unknown_00[0x60];
    int (__stdcall *f60)(Unit_0049e1a0*, Point_0049e1a0*, Unit_0049e1a0*, Vec3_0049e1a0*);
    char unknown_64[0x68 - 0x64];
    int f_68;
    char unknown_6c[0xc0 - 0x6c];
    float f_c0;
    float f_c4;
    float f_c8;
    char unknown_cc[0xe4 - 0xcc];
    unsigned short f_e4;
    char unknown_e6[0x111 - 0xe6];
    struct { unsigned int b0:1, b1:1, b2_3:2, b4:1, b5_18:14, b19:1,
                              b20_25:6, b26:1, b27:1, b28:1, b29_31:3; } f_111;
    unsigned int& f_111_raw() { return *(unsigned int*)&f_111; }
};

struct Entry_0049e1a0 {                // 0x1c bytes
    Point_0049e1a0 point;              // +0x00
    char* name;                        // +0x04
    int f_8;                           // +0x08
    Target_0049e1a0* attached;         // +0x0c
    char unknown_10[0x14 - 0x10];
    unsigned short f_14;               // +0x14, the slot's tick counter
    short f_16;                        // +0x16
    short f_18;                        // +0x18
    unsigned char f_1a;                // +0x1a
    unsigned char flags;               // +0x1b
};

struct UnitType_0049e1a0 {
    char unknown_0[0x1fa];
    unsigned int f_1fa;
};

struct Unit_0049e1a0 {
    char unknown_00[0x4];
    Entry_0049e1a0 entries[3];         // +0x04
    char unknown_58[0x66 - 0x58];
    short heading;                     // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0049e1a0 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0049e1a0* type;           // +0x92
    char unknown_96[0x9a - 0x96];
    Class_004b0a70* script;            // +0x9a
    char unknown_9e[0xb8 - 0x9e];
    unsigned short f_b8;               // +0xb8
    union {                             // +0xba
        unsigned short w;
        unsigned char b[2];
    } f_ba;
    Class_004012a0 f_bc;               // +0xbc
    char unknown_f0[0x108 - 0xf0];
    short f_108;                       // +0x108
};
#pragma pack(pop)

extern char* DAT_00509688[4];

int __stdcall FUN_0048a1e0(Unit_0049e1a0* unit, Vec3_0049e1a0* pos, int index);
Unit_0049e1a0* __stdcall FUN_0048a190(Unit_0049e1a0* obj, int index);
void __stdcall FUN_0043e2e0(Unit_0049e1a0* unit, Vec3_0049e1a0* out, unsigned char weapon);
void __stdcall FUN_0049e570(Vec3_0049e1a0* a, Vec3_0049e1a0* b, int* dx, int* dy, int* dz);
short __cdecl FUN_004b715a(int x, int z);
unsigned short __stdcall FUN_0049a890(int a, int b, int c, int d, float e);
int __stdcall FUN_0049d910(Unit_0049e1a0* unit, Target_0049e1a0* target,
                           unsigned short* out_heading, unsigned short* out_pitch,
                           unsigned char weapon, Vec3_0049e1a0* point);
int __stdcall FUN_0049aa80(Unit_0049e1a0* unit, Vec3_0049e1a0* a2,
                           Vec3_0049e1a0* a3, unsigned char a4);
int __stdcall FUN_00456200(Unit_0049e1a0* obj, char* name, char field_5,
                           int field_6, int field_a, unsigned short field_e,
                           unsigned short field_12);
void __stdcall FUN_0041c150(Unit_0049e1a0* unit);

// FUNCTION: 0x49e1a0
void __stdcall FUN_0049e1a0(Unit_0049e1a0* unit)
{
    unsigned short heading;
    unsigned char i;
    int dz;
    int dx;
    int dy;
    Vec3_0049e1a0 pos;
    Vec3_0049e1a0 aim;

    for (i = 0; i < 3; i++) {
        Entry_0049e1a0* e = &unit->entries[i];
        unsigned char fl = e->flags;
        Target_0049e1a0* attached = e->attached;
        if (!(fl & 2))
            continue;
        if (e->f_14 > 0)
            e->f_14--;
        if (!FUN_0048a1e0(unit, &pos, i)) {
            e->flags &= 0xfe;
            continue;
        }
        if (attached->f60 == 0)
            continue;
        if (attached->f_111.b19) {
            if (!(e->flags & 1)) {
                Target_0049e1a0* t = e->attached;
                unsigned short angle;
                int ok;
                if (t->f_111.b1) {
                    FUN_0043e2e0(unit, &aim, (unsigned char)((e->flags >> 2) & 3));
                    FUN_0049e570(&aim, &pos, &dx, &dy, &dz);
                    heading = (unsigned short)(FUN_004b715a(dx, dz) - unit->heading);
                    angle = FUN_0049a890(dx, dy, dz, t->f_68, t->f_c8);
                    ok = (angle != 0x8000);
                } else if (t->f_111.b0) {
                    ok = FUN_0049d910(unit, t, &heading, &angle,
                                      (unsigned char)(e->flags >> 2 & 3), &pos);
                } else {
                    ok = 0;
                }
                if (ok) {
                    e->f_18 = angle;
                    e->f_16 = heading;
                    e->f_8 = 0;
                    char* nm = DAT_00509688[(e->flags >> 2) & 3];
                    unit->script->FUN_004b0a70(nm, &e->name, 0, 2,
                                               heading, angle, 0, 0);
                    FUN_00456200(unit, nm, 2, heading, angle, 0, 0);
                    e->flags |= 1;
                }
            }
        } else {
            if (attached->f_111.b4
                && (!attached->f_111.b28 || e->f_1a)
                && !(e->flags & 1)) {
                e->f_8 = 0;
                unit->script->FUN_004b0a70(DAT_00509688[(e->flags >> 2) & 3],
                                           &e->name, 0, 2, 0, 0, 0, 0);
                FUN_00456200(unit, DAT_00509688[(e->flags >> 2) & 3], 2, 0, 0, 0, 0);
                e->flags |= 1;
            }
        }
        if (e->f_14 != 0)
            continue;
        if (FUN_0049aa80(unit, &unit->pos, &pos, i)) {
            int can = 0;
            if (attached->f_111.b28) {
                if (e->f_1a)
                    can = 1;
            } else {
                if (unit->f_bc.store->metal >= attached->f_c0
                    && unit->f_bc.store->energy >= attached->f_c4)
                    can = 1;
            }
            if (can == 0)
                continue;
            Unit_0049e1a0* fired = FUN_0048a190(unit, i);
            if (attached->f60(unit, &e->point, fired, &pos) == 0)
                continue;
            if (attached->f_111.b28) {
                e->f_1a--;
                FUN_0041c150(unit);
            } else {
                int n = unit->f_b8 / 5;
                if (n > 5)
                    n = 5;
                int q = unit->f_108 * 20 / unit->type->f_1fa;
                int pct = 100 - n * 6;
                e->f_14 = (short)((120 - q) * (pct * attached->f_e4 / 100) / 100);
            }
            unit->f_ba.w |= ((attached->f_111_raw() >> 26) & 1) ? 0x800 : 0x400;
            if (!attached->f_111.b28)
                unit->f_bc.FUN_004012a0(attached->f_c0, attached->f_c4);
        } else {
            unit->f_ba.b[1] |= 0x10;
        }
    }
}

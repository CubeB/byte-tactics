// Decompiled by LongCat 2.5 Preview Free, finished by LongCat 2.5 Preview Free.
// Names are provisional.
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
#pragma pack(push, 1)

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Unit_0049e1a0;
struct Def_0049e1a0;

struct Store_0049e1a0 {
    char unknown_0[0x8c];
    float metal;                       // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                      // +0x98
};

class Class_004012a0 {
public:
    char unknown_0[4];
    float x0;                          // +0x04
    char unknown_8[0x1c - 8];
    float y0;                          // +0x1c
    char unknown_20[0x30 - 0x20];
    Store_0049e1a0* store;             // +0x30
    int FUN_004012a0(float dx, float dy);
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

union Flags_0049e1a0 {
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2_3 : 2;
        unsigned int b4 : 1;
        unsigned int b5_7 : 3;
        unsigned int b8 : 1;
        unsigned int b9_18 : 10;
        unsigned int b19 : 1;
        unsigned int b20_25 : 6;
        unsigned int b26 : 1;
        unsigned int b27 : 1;
        unsigned int b28 : 1;
        unsigned int top : 3;
    } bits;
    unsigned int all;
};

struct Def_0049e1a0 {
    char unknown_0[0x60];
    int (__stdcall* fire)(Unit_0049e1a0*, void*, void*, void*);   // +0x60
    char unknown_64[4];
    int field_68;                      // +0x68
    char unknown_6c[0xc0 - 0x6c];
    float cost_metal;                  // +0xc0
    float cost_energy;                 // +0xc4
    int field_c8;                      // +0xc8
    char unknown_cc[0xe4 - 0xcc];
    unsigned short field_e4;           // +0xe4
    char unknown_e6[0x111 - 0xe6];
    Flags_0049e1a0 flags;              // +0x111
};

// The three weapon slots, walked by the loop. The same bytes are described in
// 0x49e070 anchored 0xc later (its `attached` is this `def`); there the loop
// keeps the flags byte as its induction variable, which is why the offsets
// here are written as they are.
struct Slot_0049e1a0 {
    void* target;                      // +0x00
    char* name;                        // +0x04
    int field_8;                       // +0x08
    Def_0049e1a0* def;                 // +0x0c
    int field_10;                      // +0x10
    unsigned short field_14;           // +0x14
    short field_16;                    // +0x16
    short field_18;                    // +0x18
    unsigned char field_1a;            // +0x1a
    unsigned char flags;               // +0x1b
};

struct Type_0049e1a0 {
    char unknown_0[0x1fa];
    unsigned int field_1fa;            // +0x1fa
};

struct Unit_0049e1a0 {
    char unknown_0[4];
    Slot_0049e1a0 slots[3];            // +0x04
    char unknown_58[0x66 - 0x58];
    unsigned short field_66;           // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    Type_0049e1a0* type;               // +0x92
    char unknown_96[4];
    Class_004b0a70* script;            // +0x9a
    char unknown_9e[0xb8 - 0x9e];
    unsigned short field_b8;           // +0xb8
    // The two flags the code sets overlap: the short OR is done on 0xba..0xbb
    // and the byte OR on 0xbb alone, so they are one 2 byte cell.
    union Ba_0049e1a0 {
        unsigned short field_ba;       // +0xba
        struct {
            unsigned char field_ba_lo; // +0xba
            unsigned char field_bb;    // +0xbb
        } bytes;
    } ba;                              // +0xba
    Class_004012a0 energy;             // +0xbc
    char unknown_f0[0x18];             // +0xf0
    short field_108;                   // +0x108
};
#pragma pack(pop)

extern char* DAT_00509688[4];

int __stdcall FUN_0048a1e0(Unit_0049e1a0* unit, Vec3* pos, int index);
void __stdcall FUN_0043e2e0(Unit_0049e1a0* unit, Vec3* out, unsigned char weapon);
void __stdcall FUN_0049e570(Vec3* a, Vec3* b, int* dx, int* dy, int* dz);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall FUN_0049a890(int dx, int dy, int dz, int a, int b);
int __stdcall FUN_0049d910(Unit_0049e1a0* unit, void* target,
                          short* out_heading, short* out_pitch, int weapon,
                          Vec3* point);
int __stdcall FUN_0049aa80(Unit_0049e1a0* a1, Vec3* a2, Vec3* a3, unsigned char a4);
Unit_0049e1a0* __stdcall FUN_0048a190(Unit_0049e1a0* obj, int index);
void __stdcall FUN_0041c150(void* u);
int __stdcall FUN_00456200(Unit_0049e1a0* obj, char* name, char field_5,
                           int field_6, int field_a, int field_e, int field_12);

// FUNCTION: 0x49e1a0
void __stdcall FUN_0049e1a0(Unit_0049e1a0* unit)
{
    // Declaration order sets the frame slots: the original's locals run
    // n +0x10, heading +0x14, i +0x18, dz +0x1c, dx +0x20, dy +0x24,
    // aimPoint +0x28, muzzle +0x34, with pitch in the dead argument slot
    // at +0x44 (MSVC puts the last local above the return address once the
    // 0x30-byte frame is full).
    int pitch;
    Vec3 muzzle;
    Vec3 aimPoint;
    int dy;
    int dx;
    int dz;
    unsigned char i = 0;
    short heading;
    int n = 0;

    for (i = 0, n = 0; i < 3; i++, n++) {
        Slot_0049e1a0* s = &unit->slots[n];
        Def_0049e1a0* def = s->def;
        int fl2;
        if (!(s->flags & 2)) {
            continue;
        }
        if (s->field_14 > 0) {
            s->field_14--;
        }
        if (!FUN_0048a1e0(unit, &aimPoint, n)) {
            s->flags &= 0xfe;
            continue;
        }
        if (def->fire == 0) {
            continue;
        }
        if (def->flags.bits.b19) {
            int ok;
            if (s->flags & 1) {
                continue;
            }
            Def_0049e1a0* d = s->def;
            if (d->flags.bits.b1) {
                FUN_0043e2e0(unit, &muzzle, (unsigned char)((s->flags >> 2) & 3));
                FUN_0049e570(&muzzle, &aimPoint, &dx, &dy, &dz);
                heading = FUN_004b715a(dx, dz) - unit->field_66;
                pitch = FUN_0049a890(dx, dy, dz, d->field_68, d->field_c8);
                ok = ((short)pitch != (short)0x8000);
            } else if (d->flags.bits.b0) {
                ok = FUN_0049d910(unit, d, (short*)&heading, (short*)&pitch,
                                  (s->flags >> 2) & 3, &aimPoint);
            } else {
                ok = 0;
            }
            if (!ok) {
                continue;
            }
            s->field_18 = (short)pitch;
            s->field_16 = (short)heading;
            s->field_8 = 0;
            unit->script->FUN_004b0a70(DAT_00509688[(s->flags >> 2) & 3], &s->name,
                                      0, 2, (unsigned short)heading,
                                      (unsigned short)pitch, 0, 0);
            FUN_00456200(unit, DAT_00509688[(s->flags >> 2) & 3], 2,
                         (unsigned short)heading, (unsigned short)pitch, 0, 0);
        } else {
            if (!def->flags.bits.b4) {
                continue;
            }
            if ((def->flags.all & 0x10000000) && s->field_1a == 0) {
                continue;
            }
            if (s->flags & 1) {
                continue;
            }
            s->field_8 = 0;
            unit->script->FUN_004b0a70(DAT_00509688[(s->flags >> 2) & 3], &s->name,
                                      0, 2, 0, 0, 0, 0);
            FUN_00456200(unit, DAT_00509688[(s->flags >> 2) & 3], 2, 0, 0, 0, 0);
        }
        s->flags |= 1;

        if (s->field_14 != 0) {
            continue;
        }
        if (FUN_0049aa80(unit, &unit->pos, &aimPoint, i)) {
    int ok;
    if (def->flags.bits.b28) {
        ok = (s->field_1a != 0);
    } else {
        ok = (unit->energy.store->metal >= def->cost_metal
              && unit->energy.store->energy >= def->cost_energy);
    }
    if (!ok) {
        continue;
    }
    if (def->fire(unit, &s->target, FUN_0048a190(unit, n), &aimPoint) == 0) {
        continue;
    }
    if (def->flags.bits.b28) {
        s->field_1a--;
        FUN_0041c150(unit);
    } else {
        int shots = unit->field_b8;
        shots = shots / 5;
        if (shots > 5) {
            shots = 5;
        }
        int health = unit->field_108 * 20 / unit->type->field_1fa;
        int rate = (100 - 6 * shots) * (int)def->field_e4 / 100;
        s->field_14 = (short)((120 - health) * rate / 100);
    }
        } else {
            unit->ba.bytes.field_bb |= 0x10;
        }
        fl2 = 0x400 + (def->flags.bits.b26 ? 0x400 : 0);
        unit->ba.field_ba |= fl2;
        if (!(def->flags.all & 0x10000000)) {
            unit->energy.FUN_004012a0(def->cost_metal, def->cost_energy);
        }
    }
}

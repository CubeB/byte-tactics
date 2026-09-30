// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by GPT-6, finished
// by space-bunny-free. Names are provisional. PARTIAL 81.4%, 972 of 969 bytes (was 984).
// GPT-6.1-sol refinement: six checks kept 81.4%. Reusing the full flag word
// and changing the bit-26 expression did not improve the best source. No MATCH.
// Remaining diff hunks, by address:
//  - 0x49e1c2, 0x49e1ed, 0x49e1f7, 0x49e215, 0x49e2d4, 0x49e3ae, 0x49e420, 0x49e441, 0x49e51d,
//    0x49e538: the loop-jump target 0x49e541 reads as 0x49e544, only the 3 extra bytes.
//  - 0x49e33d: the bit-4 flag test. The original keeps the shift (`mov edx, eax; shr edx, 4;
//    test dl, 1`), a mask test (`test al, 0x10`) is what a 1-bit field in a boolean context folds
//    to. Bit 19 folds to a shift in both builds because its mask needs more than a byte.
//  - 0x49e370: the bit-4 path's script call. The original passes the name in edx with
//    `lea ecx, [esi-0x17]` and loads unit->script after it; ours reuses eax for the name, takes
//    edx for the lea and hoists the ecx load above the pushes. A local for unit->script does not
//    move it.
//  - 0x49e38b: the sound call is not tail-merged into the bit-19 path's tail at 0x49e393, so the
//    second DAT_00509688 lookup, the FUN_00456200 call and `e->flags |= 1` are emitted twice.
//    MSVC 5 merges identical block ends, so the two copies must be textually equal: in the
//    original the post-script lookup is `mov al,[esi]; push 2; shr eax,2; and eax,3;
//    mov ecx,[eax*4+DAT]` in both paths, in ours it is that in path A and `mov cl,[esi]; push 0;
//    shr ecx,2; and ecx,3; mov edx,[ecx*4+DAT]` in path B. A static inline helper for the call
//    plus the flag store (the four zero pushes then being its arguments) compiles to the same two
//    blocks, so the fix has to make the allocator choose eax in path B, not just the source.
//  - 0x49e3b9: FUN_0049aa80's arguments. Original (i, &pos, &unit->pos) = (edx, eax, ecx), ours
//    = (eax, ecx, edx): the same one-step rotation of the temp registers.
//  - 0x49e4f2: the bit-26 flag OR. The original is `shr eax,0x1a; and al,1; neg al; sbb eax,eax;
//    and eax,0x400; add eax,0x400; or word [edi+0xba],ax` (a shift extract plus a branchless
//    0/1 -> 0x400). `b26 ? 0x800 : 0x400` gives exactly that, in ecx; `b26 * 0x400 + 0x400` (this
//    file) gives a 12 byte shorter `inc ecx; shl ecx,0xa` and scores better; `b26 ? 0x400 : 0`
//    + 0x400 gives `mov dl,cl; inc edx; shl edx,0xa`. The raw-dword accessor folds the extract to
//    a 0x4000000 mask.
//  - 0x49e51f: FUN_004012a0's arguments, original (edx, ecx) loaded as ecx = +0xc4, edx = +0xc0,
//    ours as edx = +0xc4, eax = +0xc0: the same rotation again.
// The rotation in the last three hunks follows the last register the previous block pushed: with
// the duplicate sound tail in place, path B's block ends on `push eax` and its next temp takes
// ecx, so fixing the merge is probably what fixes all three.
#include <string.h>

struct Vec3_0049e1a0 {
    int x;
    int y;
    int z;
};

class Class_004b0a70 {
  public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6,
                     int param_7, int param_8);
};

#pragma pack(push, 1)
struct Store_0049e1a0 {
    char unknown_0[0x8c];
    float metal; // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy; // +0x98
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
    Store_0049e1a0* store; // +0x30
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
    int(__stdcall* f60)(Unit_0049e1a0*, Point_0049e1a0*, Unit_0049e1a0*, Vec3_0049e1a0*);
    char unknown_64[0x68 - 0x64];
    int f_68;
    char unknown_6c[0xc0 - 0x6c];
    float f_c0;
    float f_c4;
    float f_c8;
    char unknown_cc[0xe4 - 0xcc];
    unsigned short f_e4;
    char unknown_e6[0x111 - 0xe6];
    struct {
        unsigned int b0 : 1, b1 : 1, b2_3 : 2, b4 : 1, b5_18 : 14, b19 : 1, b20_25 : 6, b26 : 1,
            b27 : 1, b28 : 1, b29_31 : 3;
    } f_111;
    unsigned int& f_111_raw() { return *(unsigned int*)&f_111; }
};

struct Entry_0049e1a0 {        // 0x1c bytes
    Point_0049e1a0 point;      // +0x00
    char* name;                // +0x04
    int f_8;                   // +0x08
    Target_0049e1a0* attached; // +0x0c
    char unknown_10[0x14 - 0x10];
    unsigned short f_14; // +0x14, the slot's tick counter
    short f_16;          // +0x16
    short f_18;          // +0x18
    unsigned char f_1a;  // +0x1a
    unsigned char flags; // +0x1b
};

struct UnitType_0049e1a0 {
    char unknown_0[0x1fa];
    unsigned int f_1fa;
};

struct Unit_0049e1a0 {
    char unknown_00[0x4];
    Entry_0049e1a0 entries[3]; // +0x04
    char unknown_58[0x66 - 0x58];
    short heading; // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0049e1a0 pos; // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0049e1a0* type; // +0x92
    char unknown_96[0x9a - 0x96];
    Class_004b0a70* script; // +0x9a
    char unknown_9e[0xb8 - 0x9e];
    unsigned short f_b8; // +0xb8
    union {              // +0xba
        unsigned short w;
        unsigned char b[2];
    } f_ba;
    Class_004012a0 f_bc; // +0xbc
    char unknown_f0[0x108 - 0xf0];
    short f_108; // +0x108
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
int __stdcall FUN_0049aa80(Unit_0049e1a0* unit, Vec3_0049e1a0* a2, Vec3_0049e1a0* a3,
                           unsigned char a4);
int __stdcall FUN_00456200(Unit_0049e1a0* obj, char* name, char field_5, int field_6, int field_a,
                           unsigned short field_e, unsigned short field_12);
void __stdcall FUN_0041c150(Unit_0049e1a0* unit);

// FUNCTION: 0x49e1a0
void __stdcall FUN_0049e1a0(Unit_0049e1a0* unit) {
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
                    ok = FUN_0049d910(unit, t, &heading, &angle, (unsigned char)(e->flags >> 2 & 3),
                                      &pos);
                } else {
                    ok = 0;
                }
                if (ok) {
                    e->f_18 = angle;
                    e->f_16 = heading;
                    e->f_8 = 0;
                    char* nm = DAT_00509688[(e->flags >> 2) & 3];
                    unit->script->FUN_004b0a70(nm, &e->name, 0, 2, heading, angle, 0, 0);
                    FUN_00456200(unit, DAT_00509688[(e->flags >> 2) & 3], 2, heading, angle, 0, 0);
                    e->flags |= 1;
                }
            }
        } else {
            if (attached->f_111.b4 && (!attached->f_111.b28 || e->f_1a) && !(e->flags & 1)) {
                e->f_8 = 0;
                unit->script->FUN_004b0a70(DAT_00509688[(e->flags >> 2) & 3], &e->name, 0, 2, 0, 0,
                                           0, 0);
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
                if (unit->f_bc.store->metal >= attached->f_c0 &&
                    unit->f_bc.store->energy >= attached->f_c4)
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
            unit->f_ba.w |= 0x400 * (attached->f_111.b26 + 1);
            if (!attached->f_111.b28)
                unit->f_bc.FUN_004012a0(attached->f_c0, attached->f_c4);
        } else {
            unit->f_ba.b[1] |= 0x10;
        }
    }
}

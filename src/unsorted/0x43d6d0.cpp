// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
//
// mimo-v2.6-pro, 2026-10-01 (retry pass 2): still 69.5%. This pass mapped the
// frame with /Fa listings (build/scratch/0x43d6d0/*.asm, name offsets: slot
// from full-push esp = name + 56 in the 0x28 frame) and tested:
//   - `Vec3 t = FUN_0043e060(...); Vec3 v; v = t;` and `const Vec3& t =
//     FUN(...); v = t;`: both still group the copy (load-load-load-store) and
//     fold t to [esp+N]; the interleaved load-store copy needs the copy source
//     to be the CALL RESULT assigned straight to v (`Vec3 v; v =
//     FUN_0043e060(...);`) which reproduces `mov edi,[eax]; mov [esp+N],edi;
//     mov ecx,[eax+4]; ...` and `cmp ecx,eax` exactly, but then the sret temp
//     lands BELOW v (temp@-24 merged with pp, v@-12) and the frame shrinks to
//     0x20: 68.6% (scratch vA). Copying through a pointer variable (`Vec3* pp;
//     pp = &t; v = *pp;`, even as one pp reused for the later `pp = &p1`) is
//     copy-propagated back to [esp+N]: identical to base (vK).
//   - `Point draft = u->draft;` as a real local plus ONE cell and ONE draft
//     variable reassigned in the clamp block (`draft = u->draft; cell =
//     u->cell;` instead of draft2/c) puts cell in the dead parameter slot
//     (+8, name +8) as the original does, and draft gets a real home, but ny
//     then takes a frame slot and the frame grows to 0x2c, shifting every
//     offset: 61.0% (scratch v1). Base's layout (frame 0x28, cell@0x10,
//     m@0x14, ny@0x3c) scores higher only because the frame size is right.
//   - Original slot map (name offsets): m@-40, draft@-36, pp/o@-32,
//     v/pos@-24, sret temp@-12, ny@-8 (inside the dead sret temp's range,
//     written as `mov [esp+0x30],eax` between the cmp and the pos stores),
//     cell + MakeFixed temp in the dead parameter slot (+8). Ours: cell@-40,
//     m@-36, pp@-32, v@-24, t@-12, ny@+8. Getting m/draft to -40/-36 needs
//     draft allocated before m while ny gets no frame home (it merged into
//     the dead 12-byte temp range at -8 in the original, which no spelling
//     reached: ny took +8 in base, a real frame slot in v1).
// What still differs (same list as before): grouped vs interleaved first-block
// copy with v/temp slots reversed; m/draft/cell slot contents; the seaLevel
// load register order; the second block's instruction interleaving; the
// field_20 clamp block's half/edi register swap and the tail `or [esi+0x110]`.
//
// mimo-v2.6-pro, 2026-10-01: 69.5% (original 920 bytes, ours 921), up from
// 59.8. This pass solved both long-standing levers together:
//   1. The clamp's address-select (`lea; jmp; spill; lea; mov ecx,[eax]`) is
//      MSVC 5's class-type ternary: a MAX macro over class objects whose
//      second operand is a prvalue, `v.y = MAXM(v.y, MakeFixed(x))`, with
//      `operator>` on the Fixed union. The macro's double evaluation CSEs
//      into one register value, the compare reads it from the register
//      (`cmp ecx,eax` shape), and the false arm materialises it lazily into
//      the dead parameter slot (`mov [esp+0x3c],eax; lea eax,[esp+0x3c]`)
//      while the true arm does `lea eax,[esp+0x24]; jmp`. Every int
//      spelling (lvalue ternary, reference max, const-ref binding) hoists
//      the true-arm lea above the branch and spills lim eagerly, as before.
//   2. The lim chain `(x * 0xffff + s) << 16` stays unfolded as
//      `shl/sub/add/shl` when it feeds the clamp compare through the same
//      MakeFixed bitfield-union trick as 0x4853b0 (union {int value; struct
//      {unsigned frac:16; int whole:16} parts;} with frac = 0). The fold to
//      `(s - x) << 16` happens for plain ints and even for MakeFixed when
//      only an int compare consumes it, but NOT when the Fixed object is an
//      operand of the class-type ternary.
//   3. Frame is now 0x28 (matching) because `const Point& draft =
//      Point(u->draft.x, u->draft.y);` copy-propagates away completely and
//      only one 4-byte named local (cell) is left at slot 0x00.
//
// WHAT STILL DIFFERS:
// a. Slot contents: the original has m@0x00, draft@0x04 (a real dword copy
//    `mov eax,[esi+0x7e]; mov [esp+0x14],eax` then word reads from the slot),
//    o/&p1@0x08, v/pos@0x10, temp@0x1c, and cell in the dead parameter slot
//    [esp+0x3c]. Ours has cell@0x00, m@0x04, o/&p1@0x08, v/pos@0x10,
//    temp@0x1c, with draft copy-propagated away (direct `movsx eax,word ptr
//    [esi+0x7e]` reads) and the ny spill in the parameter slot (the original
//    spills ny into the dead temp slot [esp+0x30]). Getting draft back as a
//    named local while pushing cell into the dead parameter slot is the one
//    remaining frame question: named `Point draft = u->draft;` plus cell as a
//    named local or a `const Point& cell = Point(...)` temporary all grow the
//    frame to 0x2c (the constructed temporary lands in a frame slot, not the
//    parameter slot), and `const Point& cell = MakeCell(a,b)` (inline
//    returning Point by value, sret) scores 63.4 / 65.3.
// b. The copy after FUN_0043e060: the original interleaves load-store pairs
//    through the returned pointer (`mov edi,[eax]; mov [esp+0x20],edi; mov
//    ecx,[eax+4]; ...`) keeping y in ecx for the compare; ours reads the
//    fixed temp slots grouped (load-load-load-store-store-store) and keeps y
//    in edi (`cmp edi,eax`). `Vec3 v; v = FUN_0043e060(...);` (build/scratch
//    /0x43d6d0/f1.cpp) DOES produce the interleaved copy and `cmp ecx,eax`,
//    but stack slots follow creation order (the later-created value gets the
//    lower address), so the sret temp lands BELOW v (temp@0x08, v@0x14) and
//    the frame shrinks to 0x20: 68.6%. A user-declared copy constructor
//    (g1/g2) makes the copy go through a real copy-ctor call: 62.0 / 62.8.
//    Untested: some spelling that creates the temp AFTER v while still
//    copying through the returned pointer.
// c. The seaLevel load: the original hoists `mov ebx,[g_game]` before the
//    draft load and reuses edx for the zero-extended byte
//    (`xor edx,edx; mov ebx,[g_game]; mov dl,[eax+0x22c]` ... `mov
//    dl,[ebx+0x1427f]; add eax,edx`); ours loads g_game after the draft
//    computation into edx and seaLevel into ecx.
// d. Second block instruction order: the original interleaves the p1/pos
//    loads with the adds (`mov edi,[ebp+8]; mov eax,[esi+0x6a]; mov
//    ebx,[ebp+0xc]; lea ecx,[ebp+8]; add edi,eax; ...`) and loads p1.z
//    through the &p1 register; ours groups them differently (nx=edi matches,
//    but ny is ecx vs the original's eax, and the adds order differs).
// e. The field_20 clamp block: the original keeps half in ebx and the negated
//    FUN_004b70ef result in edi; ours swaps them (half in edi, result in
//    ebx), and the original writes `or dword ptr [esi+0x110],0x10000` in
//    this tail vs our load-or-store.
//
// Ideas not yet tried: declaring Point cell at function scope; a second Point
// copied from a pointer after draft to trigger the 0x421eb0 first-frame /
// later-parameter-slot pattern; spilling ny into the dead temp slot by
// keeping the Vec3 sret temp live longer; a MAX macro spelled on a struct
// wrapper around y only.
#include <string.h>` changes codegen: 52.2 / 166 / 893.
//   - declaration order alone moves nothing: `Point draft;` declared early and
//     assigned late, or `Vec3 pos;` first, compile byte-identically; inline
//     accessors for the two `mode` reads, `memcpy(&v, &t, sizeof(Vec3))` and
//     three field assignments for the copy are byte-identical too; putting the
//     null path first (source inverted) collapses to 39.1.
//   - making `pp` a real variable fails: `(Vec3*)((char*)this + 8)`, a
//     function-scope `Vec3* pp;` assigned in the null path, `Vec3& pp = p1;`,
//     a fresh `Vec3* qq = &p1;` inside the flags block and `(void)&pp;` are
//     all byte-identical, MSVC rematerialises this+8. Splitting either path
//     into an in-class inline member helper is byte-identical too.
// What is left, in order of size:
//   a. `this` is in edi (original `mov ebp,ecx` at 0x43d6db), and the whole
//      second block follows: original nx=edi, nz=ebx, ours nx=ebx, nz=ebp.
//   b. the first block's copy is load-load-load-store-store-store (inlined user
//      copy ctor) where the original interweaves load-store pairs, reusing edi
//      for t.x straight into the argument build.
//   c. small-local slots: original m@[esp+0x10], draft@[esp+0x14], a dead
//      `&p1` store at [esp+0x18] (dereferenced only for p1.z) and cell in the
//      parameter home slot [esp+0x3c]; ours cell@0x10, m@0x14, draft@0x18 and
//      the ny spill at [esp+0x3c]. Note the original's pp=&p1 is NOT copy
//      propagated and keeps a home, while ours is.

#include <string.h>

#pragma pack(push, 1)

struct FP_0043d6d0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_0043d6d0 {
    int value;
    FP_0043d6d0 parts;
};

static inline Fixed_0043d6d0 MakeFixed_0043d6d0(int i)
{
    Fixed_0043d6d0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

inline int operator>(const Fixed_0043d6d0& a, const Fixed_0043d6d0& b) { return a.value > b.value; }

#define MAXM_0043d6d0(a, b) ((a) > (b) ? (a) : (b))

struct Vec3 {
    Fixed_0043d6d0 x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) { x.value = a; y.value = b; z.value = c; }
};

struct Point {
    short x, y;
    Point() {}
    Point(int a, int b) : x(a), y(b) {}
};

struct Short3 {
    short x, y, z;
};

struct Game_0043d6d0 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;                         // +0x1427f
    char unknown_14280[0x38a47 - 0x14280];
    int field_38a47;                                // +0x38a47
};

struct UnitType_0043d6d0 {
    char unknown_0[0x192];
    int range;                                      // +0x192
    char unknown_196[0x22c - 0x196];
    unsigned char draft;                            // +0x22c
    char unknown_22d[0x241 - 0x22d];
    unsigned int low : 19;                          // +0x241
    unsigned int b19 : 1;
    unsigned int rest : 12;
};

struct Target_0043d6d0 {
    int field_0;                                    // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                             // +0x73
};

struct TargetData_0043d6d0 {
    char unknown_0[8];
    Vec3 p1;                                        // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                                   // +0x20
};

struct Path_0043d6d0 {
    TargetData_0043d6d0* field_0;                   // +0x0
};

struct Unit_0043d6d0 {
    char unknown_0[0x64];
    Short3 f64;                                     // +0x64
    Vec3 pos;                                       // +0x6a
    Point cell;                                     // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point draft;                                    // +0x7e
    char unknown_82[0x86 - 0x82];
    Path_0043d6d0* obj;                             // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0043d6d0* type;                        // +0x92
    Target_0043d6d0* target;                        // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short a8;                                       // +0xa8
    char unknown_aa[0xf9 - 0xaa];
    signed char index;                              // +0xf9
    char unknown_fa[0x110 - 0xfa];
    unsigned int flags;                             // +0x110
};

#pragma pack(pop)

extern Game_0043d6d0* g_game;

Vec3 __stdcall FUN_0043e060(Path_0043d6d0* obj, int index);
Short3 __stdcall FUN_0043e180(Path_0043d6d0* obj, int index);
void __stdcall FUN_0048a9f0(Unit_0043d6d0* unit, Vec3 pos, int mode);
int __stdcall FUN_0047db70(UnitType_0043d6d0* type, short a8, Point cell, int mode);
void __stdcall FUN_0047d0e0(Unit_0043d6d0* unit);
void __stdcall FUN_0047cc30(Unit_0043d6d0* unit);
void __stdcall FUN_004827b0(Unit_0043d6d0* unit);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

#pragma pack(push, 1)
class Class_0043d6d0 {
public:
    char unknown_0[8];
    Vec3 p1;                                        // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                                   // +0x20
    char unknown_24[0x2a - 0x24];
    int field_2a;                                   // +0x2a
    unsigned char mode : 2;                         // +0x2e
    unsigned char flag : 1;                         // bit 2
    unsigned char unknown_2f : 5;

    void FUN_0043d6d0(Unit_0043d6d0* unit);
};
#pragma pack(pop)

static inline int& MaxRef_0043d6d0(int& a, int& b) { return a > b ? a : b; }

// FUNCTION: 0x43d6d0
void Class_0043d6d0::FUN_0043d6d0(Unit_0043d6d0* u)
{
    if (u->obj != 0) {
        Vec3 t = FUN_0043e060(u->obj, u->index);
        Vec3 v;
        v.x = t.x;
        v.y = t.y;
        v.z = t.z;
        if (u->type->b19) {
            v.y = MAXM_0043d6d0(v.y, MakeFixed_0043d6d0(u->type->draft * 0xffff + g_game->seaLevel));
        }
        FUN_0048a9f0(u, v, mode);
        Short3 o = FUN_0043e180(u->obj, u->index);
        u->f64 = o;
        if (u->obj->field_0 != 0) {
            field_20 = u->obj->field_0->field_20;
            p1 = u->obj->field_0->p1;
        } else {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            p1 = zero;
        }
        u->flags &= 0xfffeffff;
        return;
    }

    Vec3* pp = &p1;
    int nx = pp->x.value + u->pos.x.value;
    int ny = pp->y.value + u->pos.y.value;
    int nz = pp->z.value + u->pos.z.value;
    int m = mode;
    Vec3 pos;
    pos.x.value = nx;
    pos.y.value = ny;
    pos.z.value = nz;
    if (nx == u->pos.x.value && nz == u->pos.z.value && ny == u->pos.y.value && m == (int)(u->flags & 3))
        return;

    field_2a = g_game->field_38a47;
    const Point& draft = Point(u->draft.x, u->draft.y);
    Point cell;
    cell.x = (nx - (draft.x << 19) + 0x80000) >> 20;
    cell.y = (nz - (draft.y << 19) + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == (int)(u->flags & 3)) {
        u->pos.x.value = nx;
        u->pos.y.value = ny;
        u->pos.z.value = nz;
        u->flags |= 0x10000;
        return;
    }

    if (u->target->field_0 != 0 && (u->target->type == 1 || u->target->type == 2)) {
        flag = FUN_0047db70(u->type, u->a8, cell, m) == 0;
    }

    if (flag) {
        Point draft2 = u->draft;
        Point c = u->cell;
        int cx = (draft2.x + c.x * 2) << 19;
        int cz = (draft2.y + c.y * 2) << 19;
        if (nx > cx + 0x7ffff)
            pos.x.value = cx + 0x7ffff;
        else if (nx < cx - 0x7ffff)
            pos.x.value = cx - 0x7ffff;
        if (nz > cz + 0x7ffff)
            pos.z.value = cz + 0x7ffff;
        else if (nz < cz - 0x7ffff)
            pos.z.value = cz - 0x7ffff;
        int half = u->type->range / 2;
        if (field_20 > half) {
            field_20 = half;
            unsigned short angle = u->f64.y;
            Vec3 vec;
            vec.x.value = -FUN_004b70ef(angle, half);
            vec.y.value = 0;
            vec.z.value = -FUN_004b7123(angle, half);
            *pp = vec;
        }
        u->pos.x.value = pos.x.value;
        u->pos.y.value = pos.y.value;
        u->pos.z.value = pos.z.value;
        u->flags |= 0x10000;
        return;
    }

    FUN_0047d0e0(u);
    u->pos.x.value = nx;
    u->pos.y.value = ny;
    u->pos.z.value = nz;
    u->cell = cell;
    u->flags = (u->flags & 0xfffffffc) | (m & 3);
    FUN_0047cc30(u);
    u->flags |= 0x10000;
    FUN_004827b0(u);
}

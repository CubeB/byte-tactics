// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash retry, 2026-10-01. State: 59.0% (original 920 bytes,
// ours 889); no improvement, the version below is the best. Also tried this
// pass: the reversed clamp ternary `v.y = lim > v.y ? lim : v.y;` (that flips
// the original cmp v.y,lim / jle operand order), which regresses 59.0 to 58.0
// at the same 889 bytes, so the direct store stays. Also tried binding the
// call to a named reference before the copy (`Vec3 v; const Vec3& t =
// FUN_0043e060(...); v = t;`): 54.9% / 881 bytes, so MSVC still routes the
// sret buffer through its own choice and the direct-init form stays best.
// The frame is 0x20
// against the original's 0x28, and the first call has no return temporary.
// Tried this pass, all <= 59.0%: a user copy constructor (init-list, body,
// pointer-cast body, declared-only), a destructor, both together, a const
// return type, direct-init `Vec3 v(f())`, a single-use static helper, a
// distinct return struct with a converting ctor or converting operator=,
// assignment `Vec3 v; v = f();` (plain, with ctor, with operator=), two-step
// and const-reference forms. MSVC 5 elides the copy in every one and the frame
// stays 0x20. The distinct-return-struct form does create a real temporary
// (frame 0x24) but places it 12 bytes below v, the opposite of the original.
// Clamp forms: reference-returning inline max alone 51.8, as an assignment
// 58.3, `*(cond ? &v.y : &lim)` 58.3, so the direct store still wins.
//
// deepseek-v4.1-flash, 2026-09-30. State: 59.0% (check.py: original 920 bytes,
// ours 889). Best so far; up from the 58.5% this file held at the start of the
// pass. Two changes, both copy-construction:
//   - `Vec3 t = FUN_0043e060(...); Vec3 v = t;` replaced by the direct
//     `Vec3 v = FUN_0043e060(...)`. The two-step made MSVC emit the user copy
//     constructor call and an extra 4-byte frame slot (frame 0x2c); the direct
//     form drops that and lands at frame 0x20, keeping `this` in ebp.
//   - the user copy constructor was removed from Vec3 (now a POD), which makes
//     the flag path's `*pp = vec;` emit the original's three straight stores
//     instead of a ctor call, and shortens the diff by 7 lines for the same
//     59.0%.
//
// WHAT STILL DIFFERS: frame is 0x20, the original's is 0x28, so every esp+N
// below the args is 8 lower than the original's. The original keeps a separate
// 12-byte temporary for the FUN_0043e060 return at [esp+0x30] and copies it
// into `v` at [esp+0x20] with interleaved load-store pairs (edi/ecx/eax); ours
// passes &v straight to the callee, so the six-instruction copy is absent
// (this is the bulk of the 889-vs-920 byte gap). The b19 sea-level clamp is a
// direct store in ours, an address select in the original (`lea eax,[esp+0x24]`
// / spill lim / `mov ecx,[eax]`). Tried and worse or equal this pass: a
// reference-returning inline max for the clamp (56.4), POD on top of the old
// two-step (54.9), and `Vec3 t = f(); Vec3 v; v = t;` / `Vec3 v(t);` (58.5).
// headers.py: all 128 sets tie at 59.0.
//
// deepseek-v4.1, 2026-09-30. State: 58.5% (check.py: original 920 bytes, ours
// 900). Up from the 53.1% this file held before.
//
// THE LEVER (this is the whole story of this pass): `this` is now in ebp, as
// the original has it. It flipped when the flag path stopped writing the three
// trig results straight into `pp->x/y/z` and materialised them in a local Vec3
// first:
//     Vec3 vec;
//     vec.x = -FUN_004b70ef(angle, half);
//     vec.y = 0;
//     vec.z = -FUN_004b7123(angle, half);
//     *pp = vec;
// Writing `pp->x = -FUN_004b70ef(...); pp->y = 0; pp->z = ...;` (53.1%) let
// MSVC rematerialise pp as this+8 and kept writing [edi+8]/[edi+0xc]/[edi+0x10]
// with this in edi; the three stores through pp above make the pointer's live
// range cross both __cdecl trig calls, and the allocator then homes `this` to
// ebp and leaves edi free, which is exactly the original: ebp = this,
// edi = nx/v.x, ebx = nz, ny spilled.
// Also tried this pass, all <= 58.5%: the same block with a constructor
// `Vec3 vec(-sin, 0, -cos); *pp = vec;` (58.1 / 900), with `p1 = vec;` instead
// of `*pp` (58.1), with `vec.z`/`vec.y` swapped (58.5, byte-identical), with
// vec declared at function scope (58.5, byte-identical), `ny` declared before
// `nx` (58.5), `pos.y` used instead of the `ny` local in the early-out test
// (58.5), POD Vec3 with the user copy constructor removed (54.9 / 881, the
// frame drops to 0x20 and ebp goes back to nz), a reference-returning inline
// max for the sea level clamp, alone (56.4 / 914) and on top of this version
// (56.4 / 914), direct field stores for the null path's `p1 = zero;`
// (55.7 / 889), the sums read without `pp` (`p1.x + u->pos.x`, pp declared
// after, 53.0 / 891), `Vec3 pos(nx, ny, nz);` (53.1, identical), an explicit
// `v.x = t.x; v.y = t.y; v.z = t.z;` copy (53.1, byte-identical), a named
// `TargetData_0043d6d0* td = u->obj->field_0;` local (51.5 / 881).
//
// WHAT STILL DIFFERS (these are now offsets, not allocation):
// a. Frame is 0x2c, the original's is 0x28. The extra dword is the homes of
//    `ny` (ours [esp+0x1c] inside the frame) and of the FUN_0043e180 sret
//    buffer (ours [esp+0x20], original [esp+0x1c]): the original spills ny into
//    the middle of the FUN_0043e060 sret buffer ([esp+0x30]) and keeps `cell`
//    in the parameter home slot [esp+0x3c] (ours has cell at [esp+0x10] and the
//    flag path's cell at [esp+0x40], the same slot shifted by the 4 extra
//    bytes). Every esp+N below 0x3c is 4 higher than the original's.
// b. The by-value Vec3 argument of FUN_0048a9f0: ours still calls the user copy
//    constructor (ecx = esp, source = the address of v) where the original
//    emits three stores, and ours builds the argument from v's memory while
//    the original passes v.x in edi. Removing the copy constructor (which is
//    what forces the separate sret temp and the 0x28 frame without the vec
//    trick) collapses the frame to 0x20 and loses the ebp for `this`.
// c. The b19 sea level clamp: the original selects the ADDRESS of each arm
//    (`lea eax,[esp+0x24]` / `mov [esp+0x3c],eax; lea eax,[esp+0x3c]` /
//    `mov ecx,[eax]`) and so never stores back to v.y; ours is a direct
//    `cmp ...; jg; mov [esp+0x28],eax` store.
// d. The FUN_0043e060 return copy: ours loads into ecx/eax/edi and stores
//    v.z last; the original loads into edi/ecx/eax (v.x into edi first) and
//    stores v.z last from eax.
//
// NOT MATCHED (52.2%). The logic and the call sequence are believed correct;
// what still differs is register allocation and one stack slot.
//
// 1. The original keeps a separate 12-byte temporary for the FUN_0043e060
//    return at [esp+0x2c] and then copies it to `v` at [esp+0x20] (frame
//    sub esp,0x28). MSVC 5 elides that copy here and passes &v straight to the
//    callee (frame sub esp,0x1c). Tried and did not force the temporary:
//    Vec3 v = f(); Vec3 v; v = f(); Vec3 t = f(); Vec3 v = t; a one-element
//    Vec3 array, a const-returning callee, a user copy ctor / dtor / operator=,
//    an explicit &v pointer, an inline wrapper returning Vec3, and direct
//    initialisation Vec3 v(f()). Every one coalesced.
// 2. The original puts `this` in ebp (mov ebp,ecx) and then nx in edi, nz in
//    ebx, spilling ny to [esp+0x30]. Ours puts `this` in edi, nx in ebx, nz in
//    ebp. Same object family as 0x43cc20 (which also uses ebp for this), so the
//    choice is legitimate in this source file, but no local declaration order,
//    branch inversion, self alias, inline GridCell/Max helper or header set
//    (headers.py: 52.2% best, 5 sets tie) moved it.
//
// The final `u->flags` update also differs: the original emits a plain
// and/or (and eax,3; and ecx,0xfffffffc; or eax,ecx) where ours emits the
// xor pair a ^ ((a^b)&3) that MSVC 5 uses for a 2-bit bitfield write. Writing
// the assignment as a union bitfield (u->bits.mode = m) or as the
// (m & 3) | (flags & ~3) expression both still give the xor pair.
//
// deepseek-v4.1-flash re-checked 2026-09-29 and confirms 52.2% as the best.
// Newly tried, all scored worse or equal and did not change the `this` = edi
// allocation or the missing 0xc return temporary:
//   - Vec3 v; v = f();                         (50.3)
//   - Vec3 v = (Vec3)f();  Vec3 v = Vec3(f()); (50.3)
//   - Vec3 t = f(); Vec3 v(t);                 (48.3)
//   - FUN_0043e060 declared returning a distinct Pos struct with an inlining
//     converting ctor Vec3(const Pos&): this DOES separate the return buffer
//     from v (frame sub esp,0x20) but the offsets and allocation shift and the
//     score falls to 49.1.
//   - Vec3& pp = p1; instead of Vec3* pp = &p1; (52.2, byte-identical)
//   - u->flags &= ~3; u->flags |= m & 3;       (51.0)
//   - int nf = (u->flags & ~3) | (m & 3); u->flags = nf; (51.8)
// The single remaining cause is still the ownership of ebp by `this`, which
// the 12-byte return temporary very likely forced in the original.
//
// deepseek-v4.1-flash, second pass (same session): the assignment idiom does
// NOT help either. Both
//   Vec3 v; v = FUN_0043e060(u->obj, u->index);
//   Vec3 v; Vec3* p = &v; *p = FUN_0043e060(u->obj, u->index);
// still let MSVC 5 copy-propagate v into the hidden return buffer (frame
// sub esp,0x1c, buffer at [esp+0x20], 50.3%). The only construct that
// separates the buffer is a distinct 12-byte return type with an inlined
// converting constructor (guide: 0x4dbd00), which lands at frame 0x20 (49.1%)
// rather than the original 0x28. 52.2% remains the best.
//
// space-bunny-free pass (52.2% kept, 1 check.py run): no improvement, but two
// new facts about the b19 sea-level clamp, which is the one place the first
// hunk differs in code shape rather than in slot offsets:
// 3. The original does NOT store the clamped y back into `v`. The clamp ends
//    with the value in ecx, and ecx is then written straight into the by-value
//    Vec3 argument of FUN_0048a9f0 (`mov dword ptr [edx+4], ecx`). Ours emits
//    a real store to v.y, so ours materialises a memory location the original
//    never touches.
// 4. The clamp itself is an "address select": `cmp ecx, eax; jle L; lea eax,
//    [esp+0x24]; jmp M; L: mov [esp+0x3c], eax; lea eax, [esp+0x3c]; M: mov
//    ecx, [eax]`. That is MSVC 5 taking the ADDRESS of each arm of a ternary
//    and loading through the selected one, which only happens when both arms
//    are lvalues, i.e. the source is the `?:` of a max() MACRO whose false arm
//    is a value in a register (it has to be spilled to a dead arg slot first).
//    Writing it as a fresh local (`int lim = ...; v.y = v.y > lim ? v.y : lim`)
//    gives the direct store instead. Tried the macro form with the limit
//    expression duplicated (so CSE gives one computation plus a temp):
//    `v.y = (v.y > (LIM)) ? v.y : (LIM);` -> 51.8%, worse, still a direct store.
//    So the arms must come from something else than a duplicated expression.
// 5. The frame is 40 bytes = 4 scalar dwords (mode, &p1, Point draft, and one
//    more) + the 12-byte local `v` + a separate 12-byte sret buffer for
//    FUN_0043e060. The second call's sret buffer (FUN_0043e180) sits at
//    [esp+0x1c], overlapping v, so only the FIRST call keeps a buffer of its
//    own: the first result is copied out of it and it is never reused, while v
//    is dead by the second call.
//
// deepseek-v4.1 pass (this file, 53.1% checked): the frame size, the slot of
// `v` ([esp+0x20]) and the slot of the FUN_0043e060 return temporary
// ([esp+0x2c]) all MATCH the original in this version, so note 1 above is
// stale here: the temporary is present and it comes from the user copy
// constructor on Vec3. What really differs is the register allocation and the
// small-local slots. Measured this pass (check.py % / differing instructions
// out of 294 / bytes):
//   - the file as it stands:  53.1 / 168 / 889
//   - POD Vec3 (no copy ctor): 47.4 / 184 / 895. It does fix two real things,
//     the copy becomes interleaved load-store pairs like the original and the
//     by-value argument stops calling the ctor (the original builds it with
//     three stores), but the frame then shrinks to 0x20 and the two 12-byte
//     objects swap (temp [esp+0x18], v [esp+0x24]). Same for `Vec3 v = t;`,
//     `Vec3 v; v = t;` and `Vec3 v = FUN_0043e060(...)`.
//   - a reference-returning inline max for the clamp DOES reproduce the
//     original's address select and drops the store to v.y, but alone it is
//     52.0 / 179, and with `int m = mode;` declared first 51.9 / 177.
//   - `int m = mode;` moved above `Vec3* pp = &p1;` (the m store and the mode
//     read are then emitted at the top of the block): 53.0 / 166 / 892. Lowest
//     instruction edit distance of the whole pass, but the mode read position
//     is wrong and its m/ny slots move (m 0x14 -> 0x3c, ny 0x3c -> 0x10).
//   - all six declaration orders of the three sums: 53.1, 53.1, 53.8, 53.4,
//     53.1, 53.1, but the emitted order never reaches the original's
//     nx, ny, nz, and the 53.8 one (ny, nx, nz) differs in 169 instructions
//     against the file's 168, so it is difflib re-alignment, not progress.
//   - headers.py: all 128 header sets and all 768 --cpp sets tie or lose; the
//     best is 53.1 (the file's own <string.h> set). No TU-state lever here.
//   - 19 different counts of inert `extern int dummyN;` declarations before the
//     include (0..250): every one is byte-identical to the file, so the
//     allocator state is not reachable through the TU symbol table either.
//   - removing `#include <string.h>` changes codegen: 52.2 / 166 / 893.
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

struct Vec3 {
    int x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Point {
    short x, y;
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

// FUNCTION: 0x43d6d0
void Class_0043d6d0::FUN_0043d6d0(Unit_0043d6d0* u)
{
    if (u->obj != 0) {
        Vec3 v = FUN_0043e060(u->obj, u->index);
        if (u->type->b19) {
            int lim = (u->type->draft * 0xffff + g_game->seaLevel) << 16;
            v.y = v.y > lim ? v.y : lim;
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
    int nx = pp->x + u->pos.x;
    int ny = pp->y + u->pos.y;
    int nz = pp->z + u->pos.z;
    int m = mode;
    Vec3 pos;
    pos.x = nx;
    pos.y = ny;
    pos.z = nz;
    if (nx == u->pos.x && nz == u->pos.z && ny == u->pos.y && m == (int)(u->flags & 3))
        return;

    field_2a = g_game->field_38a47;
    Point draft = u->draft;
    Point cell;
    cell.x = (nx - (draft.x << 19) + 0x80000) >> 20;
    cell.y = (nz - (draft.y << 19) + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == (int)(u->flags & 3)) {
        u->pos.x = nx;
        u->pos.y = ny;
        u->pos.z = nz;
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
            pos.x = cx + 0x7ffff;
        else if (nx < cx - 0x7ffff)
            pos.x = cx - 0x7ffff;
        if (nz > cz + 0x7ffff)
            pos.z = cz + 0x7ffff;
        else if (nz < cz - 0x7ffff)
            pos.z = cz - 0x7ffff;
        int half = u->type->range / 2;
        if (field_20 > half) {
            field_20 = half;
            unsigned short angle = u->f64.y;
            Vec3 vec;
            vec.x = -FUN_004b70ef(angle, half);
            vec.y = 0;
            vec.z = -FUN_004b7123(angle, half);
            *pp = vec;
        }
        u->pos.x = pos.x;
        u->pos.y = pos.y;
        u->pos.z = pos.z;
        u->flags |= 0x10000;
        return;
    }

    FUN_0047d0e0(u);
    u->pos.x = nx;
    u->pos.y = ny;
    u->pos.z = nz;
    u->cell = cell;
    u->flags = (u->flags & 0xfffffffc) | (m & 3);
    FUN_0047cc30(u);
    u->flags |= 0x10000;
    FUN_004827b0(u);
}

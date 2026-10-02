// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by Space Bunny Free. Names are provisional.
//
// Space Bunny Free, 2026-10-02: 73.9% (original 920 bytes, ours 922), up from
// 70.9. ONE change is kept: the clamp's test re-spells the expression instead
// of naming it, `if (field_20 > (u->type->range / 2))` with `int half =
// u->type->range / 2;` kept only for the two uses inside. VC5 then emits the
// division a second time at the compare, which is what puts `half` in ebx (as
// the original has it) and makes the whole cdq/sub/sar/cmp block match
// byte for byte. Re-measured at this baseline: naming it (`> half`) 70.9;
// moving the `Vec3 vec;` declaration above `unsigned short angle;` on its own
// 70.9; both together the same 73.9; declaring `half` after the test 73.9;
// `>= (u->type->range / 2) + 1` 72.4; `(int)(u->type->range / 2)` 73.9. So it
// is the re-spelled test, not the declaration move, that is the lever.
// Found by cleaning up tools/permute.py's best (72.6%, from the 70.9% file,
// with a do/while(0) pair, an `inl0` helper and other junk); each mutation I
// re-measured alone: only this `temp_inline` move is worth anything (the
// cosmetic ones, `if (u->obj)` and braces around the else arm, were not
// re-measured).
// The rest are 0.0 or worse at 73.9%: z,y,x declaration order (73.3), the
// split `int ny = u->pos.y.value + pp->y.value, nx; nx = ...;` (72.6, and 179
// diff lines against this file's 171, though its shape-only ratio is the
// better 80.1% against 78.7%), `int nz/nx/ny` split with a separate
// `Vec3* pp; pp = &p1;` (69.5), `(int)(u->flags & 3) == m` in either test
// (73.9 and 73.6), `goto skip1/skip0` restructures (73.9), merged declarations
// `Point draft2 = u->draft, c = u->cell;` (73.9), swapped `+` operands inside
// the cell and clamp expressions (73.9), `(3 & m) | (u->flags & 0xfffffffc)`
// and `(u->flags & 0xfffffffc) | (3 & m)` (73.9), `2 == u->target->type`
// (73.9), swapped z-clamp compares (73.9), `Vec3 v, t = FUN(...)` (73.9).
// New negative results, all at 73.9% unless stated:
//   - The clamp's operand cannot be forced back to memory. `pos.z.value` in
//     the two z compares, `pos.x.value` in the two x compares, both, through a
//     `pos.Z()` accessor and through a `Vec3* pz` are byte-identical: VC5
//     forwards the tracked store. Storing the clamp through
//     `MakeFixed_0043d6d0(...)` or `pos.z.parts.whole/frac` (to make the store
//     opaque, technique 8) costs 6 to 8 points (941/945/951 bytes).
//   - The final flags combine cannot be made to emit `and ecx,0xfffffffc; or
//     eax,ecx`: `(a & 0xfffffffc) + (m & 3)`, the reversed operands, `& ~3`,
//     `3 & m | ...`, an int temporary, and dropping the mask (919 bytes,
//     73.7%) all leave VC5's double-`xor` form.
//   - The seaLevel load's register (g_game in ebx, zero-extended byte in edx)
//     is not reachable by operand order: seaLevel first, an explicit int cast,
//     65535 instead of 0xffff, or swapping the MAXM operands are all 73.9 or
//     70.5 (`(g_game->seaLevel << 16) + ...` collapses to 68.5/901 bytes).
//   - The three sums' declaration order re-measured here: nx,nz,ny 73.9
//     (kept), nz,nx,ny 73.3, ny,nx,nz and nx,ny,nz 72.6, nz,ny,nx 72.6,
//     ny,nz,nx 72.2. The three `pos.*` stores' order makes no difference at
//     all (all six 73.9), and `int m = mode;` has to stay AFTER `Vec3 pos;`
//     (m before the sums or before pos = 71.8).
//   - THE CELL SLOT, the best lead left: a by-value `Point` parameter of an
//     inlined static helper DOES get a home in the dead incoming-argument slot
//     [esp+0x3c] that the original uses for cell (`CanPlace_0043d6d0(u, cell,
//     m)` -> the store and both reads land there, 71.9%). But the copy into it
//     is one 4-byte `mov [esp+0x3c],eax` where the original writes the two
//     fields separately, and the named local cell keeps frame+0x10, so adding
//     the `Point draft` local the original also has pushes the frame to 0x2c
//     (61.7%). Moving the whole cell computation and tail into an inlined
//     member function taking Point by value (so the fields would be written
//     individually into the parameter home) costs more than it wins: 60.8%,
//     928 bytes, the argument setup is 8 instructions.
//   - The first block's copy: `Vec3 v; v = FUN_0043e060(...)` is still the only
//     spelling that produces the original's interleaved load/store copy through
//     the returned pointer (`mov edi,[eax]; mov [esp+0x20],edi; ...`), but it
//     puts v at [esp+0x24] with a 0x20 frame (70.0%). The two-object spelling
//     kept here has the original's slots (sret temp [esp+0x2c], v and pos both
//     at [esp+0x20]..[esp+0x28]) and only reads the temp's slots instead,
//     which costs the extra `mov ecx,edi`. A user-defined operator= (47.1%) or
//     copy constructor (65.2%), a const reference to the call result, a
//     `*(Vec3*)&t` type pun, and a pointer variable are all worse or
//     byte-identical: VC5 elides the sret temporary into t's home, which is
//     why the copy reads fixed slots.
//   - Frame accounting (both are exactly 10 dwords, confirmed with a slot map
//     that tracks esp through every push and each callee's `ret N`): the
//     original's are m at esp+0x10, draft at 0x14, the dead `&p1` store at
//     0x18, a hole at 0x1c, pos at 0x20..0x2b and the sret temp at
//     0x2c..0x37, with cell in the dead argument slot at 0x3c and ny reusing
//     the dead temp's y slot at 0x30; ours are cell at 0x10, m at 0x14, &p1 at
//     0x18, the same hole, v and pos both at 0x20..0x2b, the temp at
//     0x2c..0x37, and ny in the dead argument slot at 0x3c. So the only slot
//     difference is which object owns 0x10/0x14 and that the original's ny
//     lands inside the dead temp, which needs the temp to be anonymous.
// What still differs: the first block's grouped copy plus the seaLevel
// register choice; the second block's interleaving of the three adds (ours
// nz,nx,ny and p1.y read through &p1, the original nx,ny,nz and p1.z through
// &p1); the m/draft/cell slot contents and the missing store of pos.y; the
// clamp's z-operand reload (`mov edx,[esp+0x28]` vs `cmp ebx,eax`) and its
// tail's load-or-store against the original's in-place `or [esi+0x110],
// 0x10000`; the final flags `xor` pair.
//
// space-bunny-free, 2026-10-02: 70.9% (original 920 bytes, ours 922), up from
// 69.5. The only change kept is the DECLARATION ORDER of the three sums in the
// second block: `nx`, then `nz`, then `ny` scores 70.9; the previous nx, ny, nz
// scores 69.5, nz, ny, nx 69.5, nz, nx, ny 70.5, ny, nx, nz 69.5. So the y sum
// must be declared LAST of the three even though the emitted code still
// computes it before nx. The frame layout is unchanged (cell@0, m@4, pp@8,
// v/pos@0x10, the FUN_0043e060 sret temp@0x1c, ny in the dead parameter slot
// at 0x2c), and it is identical for every permutation.
// New measurements this pass, all with a slot map derived from objdump of our
// own object (annotate [esp+N] as frame offset N-0x10 after tracking the
// pushes and each callee's `ret`; the /Fa name offsets in this file's older
// notes do NOT map that way):
//   - Layout, restated from the exe's own offsets (F = esp offset - 0x10 at
//     the post-prologue esp): original has m@F0, draft@F4, &p1@F8, v/pos@F0x10,
//     the FUN_0043e060 sret temp@F0x1c, ny@F0x20 (INSIDE the dead temp's 12
//     bytes, temp+4) and cell in the dead parameter slot at F0x2c. Ours has
//     cell@F0, m@F4, the same three blocks, and ny in the parameter slot.
//     That is a 4-byte shortfall: the original has only THREE 4-byte locals at
//     the bottom of the frame, we have four, and the original's fourth (ny) sits
//     in a hole inside the first block's temp. Everything else follows from
//     that one slot.
//   - WHY ny has to be in a hole (the best lead I have left): the original's
//     FUN_0043e180 sret temp (6 bytes) sits at F0x8, sharing &p1's slot and
//     running into F0xd, so the fourth 4-byte slot at F0xc is only 2 bytes wide
//     and cannot hold ny; the next free 4-byte hole is the dead
//     FUN_0043e060 temp at F0x1c, and ny lands at its +4. So the original's
//     allocator put the Short3 temp AFTER &p1 in the bottom group, which is
//     what pushes ny out. With four 4-byte locals in the bottom group (our
//     case) the same temp instead goes to the fourth slot (F0xc in the draft
//     variant) and ny keeps a bottom slot, growing the frame to 0x2c. Nothing I
//     tried (declaration order of cell/m/draft/ny, their position in the
//     function, an extra brace level around the whole second block, a
//     Point-prvalue `Point cell(a, b)`) changes which side of that the
//     allocator takes.
//   - Adding the missing `Point draft` local (with the clamp block reusing
//     `cell`/`draft` instead of separate draft2/c, as the exe's second
//     `mov [esp+0x14],eax` suggests) DOES flip the allocator: cell then goes to
//     the dead parameter slot (F0x30) exactly as the original does, and draft
//     gets a real home, but the frame grows to 0x2c and every offset shifts by
//     4: 61.7%. So the draft local is real, and the remaining problem is only
//     where ny lands.
//   - That variant is insensitive to declaration order and to scope: declaring
//     `Point draft` before or after `int m`, declaring cell before draft,
//     wrapping the whole second block in an extra `{ }`, and reordering nx/nz/ny
//     around it all give byte-identical code (frame 0x2c, layout
//     draft@0, ny@4, m@8, pp@0xc, v@0x14, temp@0x20). Nothing I tried makes
//     the allocator put a 4-byte local in the hole inside the dead 12-byte
//     temp; that is the one lever left and I could not find it.
//   - Dropping nx/nz as variables (assigning pos.x.value/pos.z.value directly
//     and keeping only a named ny) collapses the shape badly: 44.6%. Repeating
//     the y expression instead of naming it: 38.2%.
//   - The first block's copy: `Vec3 v; v = FUN_0043e060(...)` (v = the call
//     result, no named temp) gives the original's interleaved
//     load/store-through-eax copy AND `cmp ecx,eax` with y live in ecx, but the
//     sret temp lands BELOW v (F0, v@F8) and the frame shrinks to 0x20: 66.9%.
//     `Vec3 t = FUN(...); Vec3 v; v = t;`, and the same with `v` declared
//     before `t`, both fold t back to a constant [esp+N] and give the old
//     grouped load-load-load-store copy: 69.5% each, byte-identical. The three
//     field assignments (`v.x = t.x; ...`) are also identical to `v = t`, so
//     the copy shape is not reachable with a frame-local source object: it needs
//     the call result pointer itself, and that spelling breaks the frame.
//   - `(u->flags & ~3) | (m & 3)`, `(u->flags & 0xfffffffc) | m` and
//     `... + (m & 3)` instead of `(u->flags & 0xfffffffc) | (m & 3)`: 69.5 /
//     69.3 / 69.5, none reaching the original's `and eax,3; and ecx,~3;
//     or eax,ecx`. VC5 always rewrites the last one to the double `xor` form
//     (69.5%), which is what costs that hunk.
//   - `u->pos.y.value = pos.y.value` instead of `= ny` in the early-return
//     arm is byte-identical (the compiler forwards it), so the reload of
//     pos.y in the original's clamp tail cannot be reached from that source.
// What still differs: the frame's m/draft/cell/ny slot contents (one 4-byte
// local too many at the bottom, and the missing store of pos.y from the ny
// slot); the first block's grouped copy and the seaLevel load's register
// choice (g_game in ebx in the original, edx here); the second block's
// interleaving of the three adds with the u->pos loads; the clamp block's
// half/result register swap and the tail's load-or-store against the
// original's in-place `or dword ptr [esi+0x110],0x10000`.
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
    int nz = pp->z.value + u->pos.z.value;
    int ny = pp->y.value + u->pos.y.value;
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
        if (field_20 > (u->type->range / 2)) {
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

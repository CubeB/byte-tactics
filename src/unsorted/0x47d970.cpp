// Decompiled by Space Bunny Free, finished by Claude Sonnet 5.5 and deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// deepseek-v4.1 pass (#2438): swept the inert-declaration state that fixed the
// sibling 0x47d820 (16 to 80 unused `extern int` lines in front of the first
// `#pragma pack`). For this function that lever reaches only the y sum:
//   N = 0..58 and 200..300     98.0, x loads [ecx+0x7e], y loads [ecx+0x80]
//   N = 59..180, 340..420, 600, 900   96.0, only y flips to
//                              `mov dx,[ecx+0x78] / add dx,[ecx+0x80]`
// The x sum is `mov ax,[ecx+0x7e] / add ax,[ecx+0x76]` in every one of those
// 51 states, and no state reaches MATCH. Also tried and unchanged at 98.0
// (all free-scored scratch, none committed): two distinct field types for pos
// and size (same layout, different leaf symbols), a wrapper struct holding
// both, and dead statements before the sums (a read of field_a8, a Point copy
// of size, an int local). Source operand order and `+=` were re-confirmed
// dead. So the declaration state moves the y add but nothing reaches the x
// add, and the body below stays the best version.
// space-bunny-free pass (#1856): settled the residual with a new instrument
// instead of more spellings. A scratch TU holding a dozen copies of this body
// (one per spelling, all in one object) compiles in a single pass, and parsing
// the .obj with tools/coff.py shows what each spelling really produces: the
// register operand of a mem+mem 16-bit add is chosen by front-end state, not by
// the source. In that one TU the first three copies put the LOWER displacement in
// the register for BOTH sums, the next six the HIGHER one for both, and no
// spelling (member access, short* index, `+=`, an explicit cast, the Point copy
// first or between the sums) ever split the two sums. The original splits them:
// x takes the lower, y the higher. The choice is per add node, so a mixed pair is
// not reachable from the source. Free-scored (check.py --sym) as well: each sum
// reversed, both reversed, and the copy moved to both positions, all 98.0% with
// the same two-instruction residual. Body unchanged from the previous passes.
// deepseek-v4.1-flash second pass (#1609): attacked only the x sum's register
// operand with levers not tried before. All scored as scratch (no committed body
// change): the seven operand orders of the two sums; a `Point&`/`Point*` bound to
// pos and to size; `short&` and `short*` bound to the individual fields; short
// and int locals holding pos.x; a static inline helper returning pos.x; static
// inline add helpers taking the object; and Point member accessors (SumX/SumY)
// called with the receiver swapped so the x sum is `pos.SumX(size)` and the y sum
// is `size.SumY(pos)`. Every one is 98.0 with the same two-instruction residual,
// except the `Point&`-to-size forms which compile x to the higher offset and flip
// y to the lower offset (96.0). So neither operand order nor any access path in
// the source can move this x operand: MSVC canonicalises the mem+mem add to the
// higher displacement regardless. This confirms the earlier conclusion, the
// residual is front-end state and the body below stays the best version.
//
// deepseek-v4.1-flash pass (#1182): re-ran the free-scored sweep (operands
// swapped, augment form, local/pointer/reference aliases of obj, statement
// order, comma declaration), the static-helper form, and tools/headers.py
// (128 sets, best <windows.h> 98.0). Every variant is 98.0 with the same
// two-instruction residual and the total size exactly right, so the file
// below stays the best version; the choice is not reachable from the source.
//
// space-bunny-free pass (#1112): re-derived every stack slot, confirmed the body is
// the right shape and that the one remaining difference is not reachable from the
// source. Details below; the body itself is unchanged from the previous passes.
//
// PARTIAL: 98.0%, 322 of 322 bytes, and the total size is exactly right, so this is
// a two-instruction residual and nothing else: the first pair of 16-bit adds.
//
//   original   mov ax,[ecx+0x76]   add ax,[ecx+0x7e]    x end = pos.x then size.x
//              mov dx,[ecx+0x80]   add dx,[ecx+0x78]    y end = size.y then pos.y
//   ours       mov ax,[ecx+0x7e]   add ax,[ecx+0x76]    x end: operands swapped
//              mov dx,[ecx+0x80]   add dx,[ecx+0x78]    y end: correct
//
// The original's two adds pick OPPOSITE operands: the lower offset goes into the
// register for x and the higher one for y. No consistent canonicalizer produces
// that, and this build will not produce it either. Measured this pass, all
// free-scored with score.py, all at exactly 322 bytes:
// 1. Source order is not the lever at all. `pos.x+size.x` and `size.x+pos.x`
//    compile to the same code (verified by diffing the objects), and so do the
//    four combinations of the two sums: reversing x only, y only, both, and
//    `size.y+pos.y` for y (the spelling the original's operand order implies).
//    MSVC picks the operand with the higher displacement for the register and
//    ignores which side of the `+` it was written on. So the source above is the
//    most plausible original, and no spelling of it changes the output.
// 2. Other shapes, all 98.0 with the same residual: one declaration with a comma
//    (`short xend = ..., yend = ...`), `xend` and `yend` declared uninitialised
//    and then assigned, `xend = pos.x; xend += size.x` (and the same for y, and
//    both), the `Point p = obj->pos` copy moved between the two sums, and the two
//    sums swapped (that one is worse, 97.0: it reorders the four instructions).
// 3. What does move it is state, not shape. Deleting `#include <windows.h>` flips
//    BOTH sums to the lower-displacement-first form AND introduces a second
//    residual, `mov bl,[edi+ecx]` where the original has `mov bl,[ecx+edi]`,
//    which is the same base/index SIB swap that 0x47d820 could not shake. So the
//    include is load bearing twice over and stays; the earlier note understated
//    this by crediting it only with the y sum.
// 4. The declaration-state probe that reached a padding MATCH on 0x47d820 does
//    not work here. N unused declarations after the include, N = 4, 12, 20, 32
//    for each of `extern void __cdecl f(void);`, `extern int __cdecl f(int,int);`,
//    `static int v;`, `typedef int t;` and `extern int v;`: 98.0 everywhere except
//    two 96.0s, never MATCH, all at 322 bytes. Combined with the earlier
//    `extern int dummyK` sweep (K = 0 to 200 step 4) and headers.py's 128 sets
//    (best 98.0), there is no padding, header or spelling that reaches MATCH.
//
// Conclusion, same class as the residual on 0x47d820: front-end symbol-hash
// state, and the file below is the correct source. Do not spend a pass on the
// operand order of these two adds. (No padding MATCH was found here to decline.)
//
// Claude Sonnet 5.5 pass (#571) notes, kept because they explain the body: the
// fix that took it from 57.6 to 98.0 percent is to write the mask read with the
// post-increment inside it, `mask[n++] & bit`, instead of `n++` at the bottom of
// the loop. The original increments right after the load (`mov ecx,[n]; mov
// bl,[ecx+edi]; inc ecx; test bl,bl; mov [n],ecx`), and with the increment merged
// MSVC keeps `n` in the dead `flag` argument slot and gives esi to the cell
// pointer, as the original does; with `n++` at the bottom it puts `n` in esi and
// spills `bit`, which cascades through the whole loop. Explicit `(short)` casts
// and `int` locals for the two ends (56.3 percent, worse) were also tried.
//
// Stack map, re-derived this pass from the frame (sub esp,0x14, then four
// pushes, so the saved registers are at esp+0..0xc and the five local dwords at
// esp+0x10..0x1c, with arg1 at esp+0x28 and arg2 at esp+0x2c): S+0 xend, S+4 the
// outer row counter, S+8 first the 4-byte `Point p` copy and then the inner column
// counter, S+0xc first p.y and then the byte stride 13*width, S+0x10 yend. p.x
// stays in edi from the copy's dword load and p.y is re-read from the stack
// half of that copy, which is why `Point p = obj->pos;` must stay a 4-byte copy
// and not two separate `short` locals.
//
// Caller, for whoever names this: the only caller is the thunk 0x47dac0, which
// forwards its own two arguments unchanged (`push arg2; push arg1; call`, so the
// last push is the callee's first parameter) and, when the scan succeeds, clears
// bit 2 of the byte at obj+0x10f, sets it from `flag & 1`, sets
// obj->[0x110] |= 0x8000000, calls 0x47c790(obj) and then 0x440a40 with the
// object's point at +0x76 and its point at +0x7e pushed by value. So arg1 is the
// object being placed and arg2 is a flag, as declared here.
#include <windows.h>
#pragma pack(push, 1)

struct Point_0047d970 {
    short x;
    short y;
};

struct Cell_0047d970 {
    short field_0;                      // +0x0, id of the unit owning the cell
    char unknown_2[0xd - 0x2];
};

struct Unit_0047d970 {
    char unknown_0[0x14e];
    unsigned char* mask;                // +0x14e, one byte per footprint cell
};

struct Obj_0047d970 {
    char unknown_0[0x76];
    Point_0047d970 pos;                 // +0x76
    char unknown_7a[4];
    Point_0047d970 size;                // +0x7e
    char unknown_82[0x92 - 0x82];
    Unit_0047d970* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    short field_a8;                     // +0xa8, the owner's own id
};

struct Game_0047d970 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_0047d970* cells;               // +0x14287
};
#pragma pack(pop)

extern Game_0047d970* g_game;

// Scans the rectangle the object covers, and fails if any cell the object's
// mask marks with `flag ? 2 : 4` belongs to a unit other than this one.
// FUNCTION: 0x47d970
int __stdcall FUN_0047d970(Obj_0047d970* obj, int flag)
{
    short xend = obj->pos.x + obj->size.x;
    short yend = obj->pos.y + obj->size.y;
    Point_0047d970 p = obj->pos;
    if (p.x < 1 || p.y < 1 || xend >= g_game->width || yend >= g_game->height)
        return 0;
    int width = g_game->width;
    unsigned char bit = flag ? 2 : 4;
    int n = 0;
    for (int y = p.y; y < yend; y++) {
        Cell_0047d970* c = g_game->cells + y * width;
        for (int x = p.x; x < xend; x++) {
            if ((obj->unit->mask[n++] & bit) && c[x].field_0 != 0 && c[x].field_0 != obj->field_a8)
                return 0;
        }
    }
    return 1;
}

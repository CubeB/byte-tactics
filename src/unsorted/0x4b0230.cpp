// Decompiled by space-bunny-free, finished by Sonnet 5.5 (partial), deepseek-v4.1-flash (partial), verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 3105): confirmed the documented
// compiler-state tie: the 3x3 tiling block keeps index in edx and height in eax
// in the original vs esi/edx here. New levers all failed: dummy extern-int sweep
// 0..400 flat at 82.9; moving `h = sub->height` after the if/else bloats to
// 63-72; for-loop forms 63.7-78.0; arm order x0-then-y0 gives 82.5. Best
// variant retained.
// #2928 retry by GPT-6.1-sol: one real check.py run reconfirmed 82.9% (631/631), no MATCH. Prior notes record exhaustive source-order and declaration-order probes plus all headers.py combinations; no untested, low-risk source change was apparent, so the established best is retained.
// #2316 retry by OpenCode / GPT-6.1-sol: checkall scored 82.9% (631/631 bytes), no MATCH. Moving the width calculation after the origin branch scored 79.9%; a single-use width helper stayed at 82.9%. The remaining mismatch is register and stack-slot allocation in the 3x3 tiling block, including the loop latch. Best source retained.
// #1700 retry by Codex / GPT-6.1-sol: checkall reconfirmed 82.9% (631/631 bytes), no MATCH.
// Both direct checks and all 128 headers.py combinations left the 3x3 tiling mismatch unchanged.
// Draws a list box's frame. FUN_004a15c0 gives the entry's rectangle; when no
// bitmap arrives, the "Listbox" piece is looked up in the object's GAF and, if
// found, the rectangle is grown by 3 on every side. The destination is the
// surface at entries+0xbc. When FUN_004a18c0 finds a background cell for the
// entry, the area is tiled with it through FUN_004c6b70 and only the bevel
// (FUN_004b0160) is drawn; with no cell and no bitmap the rectangle is filled
// (FUN_004bf6f0) and bevelled; with a bitmap set of more than one child the
// set is laid out as a 3x3 border around the rectangle, rows 0/3/6 and columns
// 0/1/2 of the set, stepping by the first child's width and height, the last
// row and column pinned to the far edges; with one child or less it is blitted
// at the origin. The colours are the object's bytes at +0x8b2 (dark), +0x8c3
// (light) and +0x8c6 (fill).
//
// NOT MATCHED: check.py 82.9%, 631 of the original's 631 bytes. The prologue, the tile loop
// and the fill/single-child arms are byte-identical to the original (every
// register agrees up to the start of the 3x3 block); what still differs is the
// 3x3 block only: its pre-loop schedule and the register roles of the three
// temporaries there. The original keeps height in eax, h in ecx and the y
// counter in edx (the counter lives in the dead `index` argument slot,
// [esp+0x3c], y0 in the dead `obj` slot [esp+0x38]); this file has y and
// height rotated. Only the final `y` register rotation and the order of the
// loads before the `index != 0` test remain (about 40 instructions, all one
// cause).
//
// Second pass (space-bunny-free, #1152): still 82.9%, one real check.py run,
// everything else scored free with `check.py --sym`. The diff is ONE allocator
// decision in the 3x3 block, confirmed by reading the original's registers:
// the original computes BOTH extents AFTER the x0/y0 if/else, so rect.x0 stays
// live in esi and rect.y0 in edx across the branch; with ecx=h, ebx=w and the
// dead 0 in eax already occupying four scratch registers, height=y1-y0+1 is
// forced into eax and the y counter into edx (edx is free once y0 dies). The
// counter MUST be in edx, because the row expression clobbers edx (setge/dec/
// and/add 6), which is why the original compares against the reloaded
// [esp+0x3c] and reloads it into edx afterwards. This file computes width
// early, so x0 dies before the branch, edx goes free and the pair comes out
// rotated (height in edx, counter in eax). Statement orders scored here
// (body = the five statements before the y loop, W=width, H=height, I=index=0):
//   sub w h / W / if / I / H  82.9  (kept)
//   sub w h / if / W / I / H  79.9, and with H then I 81.2
//   sub w h / if / W / H / I  73.4,  H / W / I 76.7
//   sub h w / W / if / I / H  77.2
//   if written as `index == 0` first  78.0,  x0 assigned before y0 78.0
//   H/W before the if/else 18-75 (chaotic, e.g. h ends up in edx)
// So the "keep x0 live" source shape is right but only reachable with the
// extents in the order H then W, which costs 10 points elsewhere; the tie is
// still open. Declaration order of the 18 locals was re-tested on the kept
// body: moving height/ypos/width around changes nothing (82.9) except moving
// x/width, which costs 10 (73.0), so the memory-slot layout (row, x0, height,
// ypos, h, rect) is robust and is not the lever. Note the original's slot
// order is row, x0, ypos, height, h, so height and ypos are transposed
// relative to this file, and the original reloads BOTH the counter and the
// height at the latch while this file reloads only the height.
//
// What moved the score (Sonnet 5.5, #1076), for whoever continues:
//  * The surface pointer is re-read as obj->holder->entries + 0xbc AFTER the
//    first call while `entries` stays a local (the original does exactly that:
//    the reload chain [esi+0x18] -> [eax+4] -> [ecx+0xbc]). 46% to 51%.
//  * The if/else tree is `bmp != 0 { count > 1 {3x3} else {blit} } else {fill}`,
//    which puts the fill arm last as in the original, and the single-child arm
//    is `p = FUN_004b7f30(bmp, 0); FUN_004b7f90(surface, p, 0, 0)` (the call
//    comes first, not nested in the argument list). 51% to 57%.
//  * `row = (y < height - h + 1) ? 3 : 6;` gives the original's setge/dec/
//    and 0xfffffffd/add 6 (the `>= ? 6 : 3` spelling gives setl/and 3/add 3).
//  * DECLARATION ORDER: all locals declared uninitialised at the top in the
//    order below and assigned later. That alone flipped the ebx/edi roles of
//    `bmp` and `entries`/`cell` in the prologue (a random search over orders
//    found it; the same tie was not movable by any statement rewrite, and an
//    extra `cell->step_x` reference in the tile loop flipped it too, which is
//    the same weight tie). 74% to 82%. A random search over declaration order
//    plus the order of the six statements before the y loop tops out at 83%.
//
// Third pass (deepseek-v4.1-flash, #1369): confirmed the tie is NOT reachable
// by statement/declaration order. Restructuring so BOTH extents are computed
// after the if/else does put rect.x0 in esi (the missing half), but the
// allocator then loads hv (sub->height) BEFORE index and gives hv edx, while
// the original gives index edx and hv ecx, so it drops to 76.7% (both extents
// after the branch, H then W) or 74.6% (index test before h, which gets index
// into edx but hoists y0 into edi and keeps sub in eax). Writing the x0/y0
// select as ternaries scores 67.8%, a separate loop-counter local is identical
// to reusing index, an `int cond = index;` copy is folded away, and swapping
// the if arm order costs 1.7 points. The root cause is the emitted order of
// the `index` and `hv` loads (index, hv, 0 in the original; hv, index, 0 when
// both extents follow the branch), which no source reordering tested here
// changes. Kept the 82.9% version.
// deepseek-v4.1 (#1892): re-checked, still 82.9% (631/631). New evidence: the register
// pair x0/y0 flips because the early x1 load that computing width before the branch forces
// occupies edi. Source shapes scored this pass: w h / if / height width index=0 76.7
// (633 bytes), if before w h 74.6, w / if / h 74.6 (in the last one the index load does
// land in edx as in the original, but h recycles edx after the compare, so h ends in edx
// instead of ecx and the rest of the block stays rotated). Swapping the declarations of
// height and ypos does not move their frame slots, so the 0x18/0x1c transposition is not
// declaration order. Nothing beat the version below.
// deepseek-v4.1 (#1892, second pass): exhaustively confirmed the tie. All 96
// combinations of (w/h order) x (the 24 statement orders of width, height and
// index=0 around the if/else) x (both arm orders) were compiled and scored; the
// ceiling is 82.9 with `w h / width / if / index=0 / height`, the arm order
// y0,x0 and no variant reached the original's pre-loop schedule. The shapes that
// DO put x0 in esi (both extents after the branch) always pay for it: h lands in
// edx and index in eax, because the width subtraction that otherwise keeps eax
// busy moves below the branch, so the scheduler reuses eax (freed after the two
// 16-bit loads) for the index load and pushes h and the compare's zero down a
// register. Probes for the 0x18/0x1c ypos/height transposition (an early
// `ypos = 0;` or `height = 0;` folded in front of the block, swapped
// declarations) leave the slots exactly where they are, so the frame order is
// not declaration order nor first-use order either; both halves of the mismatch
// (3x3 registers and the two slots) stay unexplained by any source shape tested.
// #2678 retry by deepseek-v4.1-flash: re-confirmed 82.9% (631/631). Corrected
// scratch generator (an earlier one silently dropped the FUN_004b7f30 call, so
// its low scores were invalid) and re-ran the statement orders. Both extents
// after the origin branch score 76.7 (H then W), 81.2 (index=0 before H,W or H,
// index=0, W) and 79.9 (W then H); hoisting x0/y0 before the branch and testing
// `index == 0` scores 77.8 to 79.9. None reach the original's index-in-edx,
// h-in-ecx, height-in-eax allocation. Best stays this 82.9% body.
//
struct Rect_004b0230 {
    int x0;                          // +0x0
    int y0;                          // +0x4
    int x1;                          // +0x8
    int y1;                          // +0xc
};

struct Pic_004b0230 {
    unsigned short width;            // +0x0
    unsigned short height;           // +0x2
    char unknown_4[0x28 - 0x4];
};

struct Bits_004b0230 {
    unsigned short count;            // +0x0
    char unknown_2[0x28 - 0x2];
    Pic_004b0230* child[1];          // +0x28
};

struct Gaf_004b0230 {
    char unknown_0[0x14];
};

struct Cell_004b0230 {
    int step_x;                      // +0x0
    int step_y;                      // +0x4
};

struct Surface_004b0230 {
    int tiles_x;                     // +0x0
    int tiles_y;                     // +0x4
    char unknown_8[0xbc - 0x8];
};

struct Holder_004b0230 {
    char unknown_0[4];
    char* entries;                   // +0x4
};

struct Object_004b0230 {
    char unknown_0[4];
    Gaf_004b0230* gaf;               // +0x4
    char unknown_8[0x18 - 0x8];
    Holder_004b0230* holder;         // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char dark;              // +0x8b2
    char unknown_8b3[0x8c3 - 0x8b3];
    unsigned char light;             // +0x8c3
    char unknown_8c4[2];
    unsigned char fill;              // +0x8c6
};

void __stdcall FUN_004a15c0(char* entries, int index, Rect_004b0230* out);
int __stdcall FUN_004a18c0(char* entries, int index);
Bits_004b0230* __stdcall FUN_004b8d40(Gaf_004b0230* gaf, const char* name);
void __stdcall FUN_004c6b70(Surface_004b0230* dst, Cell_004b0230* cell, int x, int y);
Pic_004b0230* __stdcall FUN_004b7f30(Bits_004b0230* bits, int index);
void __stdcall FUN_004b7f90(Surface_004b0230* dst, Pic_004b0230* pic, int x, int y);
void __stdcall FUN_004b0160(Surface_004b0230* surface, Rect_004b0230* rect, int dark, int light, int fill);
int __stdcall FUN_004bf6f0(Surface_004b0230* surface, Rect_004b0230* rect, int colour);

// FUNCTION: 0x4b0230
void __stdcall FUN_004b0230(Object_004b0230* obj, int index, Bits_004b0230* bmp)
{
    char* entries;
    int col;
    Surface_004b0230* surface;
    int height;
    int x;
    int width;
    int x0;
    Rect_004b0230 rect;
    Pic_004b0230* tile;
    int w;
    int row;
    int ypos;
    int y;
    Pic_004b0230* p;
    int y0;
    Cell_004b0230* cell;
    int h;
    Pic_004b0230* sub;
    entries = obj->holder->entries;
    FUN_004a15c0(entries, index, &rect);

    if (bmp == 0) {
        if (obj->gaf == 0)
            return;
        bmp = FUN_004b8d40(obj->gaf, "Listbox");
        if (bmp == 0)
            return;
        rect.x0 -= 3;
        rect.y0 -= 3;
        rect.x1 += 3;
        rect.y1 += 3;
    }

    surface = *(Surface_004b0230**)(obj->holder->entries + 0xbc);
    cell = (Cell_004b0230*)FUN_004a18c0(entries, index);

    if (cell != 0) {
        for (x = 0; x < surface->tiles_x; x += cell->step_x) {
            for (y = 0; y < surface->tiles_y; y += cell->step_y) {
                FUN_004c6b70(surface, cell, x, y);
            }
        }
        FUN_004b0160(surface, &rect, obj->dark, obj->light, obj->fill);
    } else if (bmp != 0) {
        if (bmp->count > 1) {
            sub = FUN_004b7f30(bmp, 0);
            w = sub->width;
            h = sub->height;
            width = rect.x1 - rect.x0 + 1;
            if (index != 0) {
                y0 = rect.y0;
                x0 = rect.x0;
            } else {
                y0 = 0;
                x0 = 0;
            }
            index = 0;
            height = rect.y1 - rect.y0 + 1;
            while (index < height) {
                if (index != 0)
                    row = (index < height - h + 1) ? 3 : 6;
                else
                    row = 0;
                if (index + h > height)
                    index = height - h;
                ypos = y0 + index;
                for (x = 0; x < width; x += w) {
                    if (x + w >= width) {
                        x = width - w;
                        col = 2;
                    } else {
                        col = (x != 0) ? 1 : 0;
                    }
                    tile = FUN_004b7f30(bmp, row + col);
                    FUN_004b7f90(surface, tile, x0 + x, ypos);
                }
                index += h;
            }
        } else {
            p = FUN_004b7f30(bmp, 0);
            FUN_004b7f90(surface, p, 0, 0);
        }
    } else {
        FUN_004bf6f0(surface, &rect, obj->fill);
        FUN_004b0160(surface, &rect, obj->dark, obj->light, obj->fill);
    }
}

// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, deepseek-v4.1-flash, GPT-6, GPT-6.1-sol, deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// mimo-v2.6-pro retry: best 80.6% (was 80.2%). Fix: the j1 = 1 store now sits
// after the inner-loop guard exactly as in the original. The spelling is
// `short j = 0; if (0 < num) { int j1 = 1; for (; j < num; j++, j1++) {...} }`
// (j initialised outside the guard, j1 inside it, empty for-init): MSVC CSEs
// the guard with the loop test (single `test ax, ax / jle`), keeps j's store
// before the branch and sinks j1's store after it. `if (j < num)` as the
// guard CSEs the same way; `if (num > 0)` does not (75.5%, a second test).
// The earlier 69.2% note came from guarding the whole loop including j = 0;
// only j1's initialiser belongs inside the guard. Still differs: (1) x is
// stored at 0x20 and y at 0x1c, the reverse of the original (x=0x1c, y=0x20);
// the slot pair is bound to the initializer value (params->field_4[0] vs [1]),
// not to name, declaration order, assignment order or load order (verified
// again: y-first swaps the two `movsx`/store instructions and keeps the
// slots). Fold-away pairs do not flip it either: `x += 1; x -= 1;` and
// `unsigned ix = x; x = ix;` at the declarations fold away but leave both the
// slots and the esi/ebp register assignment untouched (80.2%); the same pairs
// inside the outer or inner loop do NOT fold away (820-838 bytes, 58.9-70.8%).
// int xy[2] / P32 struct aggregates with copies out (xy[0] = field_4[0]; ...
// int x = xy[0];) coalesce completely to the identical bytes, so the pair is
// not an aggregate aliasing win. (2) The bestDiff*j1 product loads j1 first
// (original loads bestDiff first): source operand order (j1 * bestDiff), a
// static inline Mul(a, b) helper, `bestDiff * j1 < d0 * bestIdx` (78.8%) and
// a named prod temp all compile byte-identically to the swapped form; the
// load choice is a tiling/scheduling tie no spelling has moved. (3) Branch 2
// differs in register allocation (ours: y reloaded into edi, halfH into ecx,
// nx in eax, loop counter edi, n hoisted to slot 0x44; original: y into ecx,
// halfH edx, nx edi, counter eax, n = limitX - nx rematerialised in esi each
// outer iteration), limitX reuses slot 0x34 (j) in ours vs 0x18 (i) in the
// original, limitY 0x30 (grid) vs 0x34 (j), and the original computes dst
// before src through the `add reg, 0x7c` grid accessor shape. Moving dst
// before src collapses the whole function to 38.9-39.0% because the global
// register allocation reshuffles, so the src-first form was kept.
// Remaining diff is 54 lines; the `jae 0x482597` vs `0x48259c` targets are
// just our 5 extra bytes (822 vs 817) and fix themselves when (3) matches.
// mimo-v2.6-pro retry: best 80.2% (was 79.1%). Fix: both limitX/limitY
// ternaries spelled in the negated form `(x + frame->width >= halfW) ? halfW - x
// : frame->width` flips the arm layout to the original's `jl`-to-second-arm with
// the false arm falling through, and fixes the limitX/limitY codegen
// instruction for instruction apart from slot/register numbers. Still differs:
// (1) x is stored at 0x20 and y at 0x1c, the reverse of the original (x=0x1c,
// y=0x20); the slot pair is bound to the initializer value (params->field_4[0]
// vs [1]), not to name, declaration order, assignment order, load order, ctor
// init or split declarations: swapping declarations swaps the loads/stores but
// field_4[0] keeps 0x20 in every spelling tried (also struct/array aggregates
// which move the pair to the top of the frame instead). (2) The `mov [esp+0x10],
// 1` (j1 = 1) sits before the inner-loop `jle` in ours and after it in the
// original; guarding with `if (0 < num)` drops the score to 69.2%. (3) The
// bestDiff*j1 product loads j1 first (original loads bestDiff first): source
// operand order, named temp and j1*bestDiff all still load [esp+0x10] first.
// (4) Branch 2 differs in register allocation (ours: y reloaded into edi,
// halfH into ecx, nx in eax, loop counter edi, n hoisted to slot 0x44;
// original: y into ecx, halfH edx, nx edi, counter eax, n = limitX - nx
// rematerialised in esi each outer iteration), limitX reuses slot 0x34 (j) in
// ours vs 0x18 (i) in the original, limitY 0x30 (grid) vs 0x34 (j), and the
// original computes dst before src through the `add reg, 0x7c` grid accessor
// shape. Moving dst before src (raw or at() form) collapses the whole function
// to 38.9-39.0% because the global register allocation reshuffles, so the
// src-first form was kept.
// Partial: 79.1%, 822 bytes versus 817. The big win over the previous 66.9%
// attempt: the inner loop's seemingly dead x2/y2 stores are really the live
// update of the int e1/e2 locals. Writing `int e1; int e2;` then
// `e1 += x; e2 += y; short x16 = (short)e1; short y16 = (short)e2;` (instead
// of separate int x2/y2 locals) is what the original does, and it moved every
// branch-1 stack slot except x/y onto the original's offsets (e1=0x24,
// e2=0x14, i=0x18, j=0x34, etc.).
// Still differs: x sits at 0x20 and y at 0x1c, the reverse of the original
// (x=0x1c, y=0x20); no declaration order tried (x first, y first, uninitialised
// then assigned, one combined declaration) moved the slot. Branch 2 still
// differs over ~70 lines: the original reuses slot 0x18 for limitX and 0x34
// for limitY, computes dst before src, and runs the scan as a do/while whose
// entry jumps straight into the body. Rewriting branch 2 with dst first (or
// with if/else limits, or with a do/while) collapsed the whole function to
// 39.0% because it reshuffled the global register allocation, so the src-first
// ternary form was kept. The imul operand order in the bestDiff/bestIdx
// comparison is also reversed.
// The LOD selection block now matches
// the original instruction for instruction: writing the clamp as a ternary
// directly in the `if` condition (instead of a separate Lod_ helper call or
// `int v = ...; int lod = ...;` statements) makes MSVC fold the clamp into
// `sets cl; dec ecx; and ecx, eax` before the FUN_00433520 call. Dropping the
// redundant `if (count <= 0) return;` and `if (num <= 0) continue;` guards
// also removed extra tests. What still differs: stack slot numbering for y,
// i, grid, lod, table, ref and bestDiff (ours 4 to 8 bytes lower than the
// original), the inner-loop e1/e2 result slots and the dead int stores of
// x2/y2 that the original keeps, and the branch-2 limitX/limitY branch
// layout (ours emits `jge` where the original emits `jl`). The distance
// counter advances even for out-of-bounds points. Grid accessors recover the
// receiver-relative addressing.
// GPT-6.1-sol retry in #1932: four checks kept 61.0%; the reversed coordinate
// declaration tied, while do-while conversions scored 59.1%. Neighbor coordinates
// need a signed 16-bit cast before an unsigned bounds check (movsx AX then cmp).
// GPT-6.1-sol refinement: explicit signed-16 locals also tied at 61.0%; the
// `>> 5` form fell to 58.1% and shared max-LOD temporaries fell to 60.0%.
// The best source was retained. No MATCH was reached.
// deepseek-v4.1-flash retry in #2989: nineteen more x/y spellings (split and
// combined declarations, declaration before halfW/halfH, uninitialised then
// assigned, comma declarators, pointer deref, y-declared-first, casts) all
// compile byte-identically to the 79.1% version, so the x=0x20/y=0x1c slot
// pair is immovable from source. Restructuring branch 2 (limits as if/else,
// dst before src, src re-association) still collapses to 39.0%.
// deepseek-v4.1-flash retry in #2819: tried swapping the x/y declaration
// order (each load order), combined and split declarations, renaming both
// coordinates, declaring them before/after halfW/halfH, a dummy local
// between them, short coordinates (collapsed to 40%), and every combination
// of common headers (headers.py, 128 sets). The x slot stays 0x20 and y stays
// 0x1c in every variant: the mapping is tied to which params->field_4[] index
// each value comes from, not to name, declaration order or type. Branch-2
// reorderings (limitY first 76.9%, nx/ny first 72.6%, dst before src 39.0%)
// all score worse, so the current src-first form was kept. Best stays 79.1%.
#include <windows.h>

#pragma pack(push, 1)

class Class_00433500 {
public:
    void* FUN_00433500(int n);
};

class Class_00433520 {
public:
    int FUN_00433520();
};

class Class_004335c0 {
public:
    int FUN_004335c0();
};

class Class_4335e0 {
public:
    void* FUN_004335e0(short i);
};

class Class_004339c0 {
public:
    int FUN_004339c0();
};

class Class_004339e0 {
public:
    void FUN_004339e0(short i, int* a, int* b);
};

extern char DAT_0051e6a0[];

struct Grid_482270 {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int field_c;                       // +0xc
    unsigned char& at(int x, int y) { return cells[y * width + x]; }
};

struct Player_482270 {
    char unknown_0[0x7c];
    Grid_482270 grid;                  // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char playerIndex;         // +0x146
};

struct Params_482270 {
    void* field_0;                     // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    char unknown_10[0xc];              // +0x10
};

struct Game_482270 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14281 - 0x1423b];
    unsigned short bit0 : 1;           // +0x14281
    unsigned short bit1 : 1;
    unsigned short flag2 : 1;
    unsigned short flag3 : 1;
    unsigned short rest : 12;
    char unknown_14283[0x1428f - 0x14283];
    Grid_482270 grid1;                 // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned short f0 : 1;             // +0x142f1 bit 0
    unsigned short f1 : 1;
    unsigned short flagA : 1;          // bit 2, mask 4
    unsigned short f3 : 5;
    unsigned short fhi : 8;
    char unknown_142f3[0x1485b - 0x142f3];
    void* losTable;                    // +0x1485b
};

struct Frame_482270 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[4];
    unsigned char mask;                // +0x8
    char unknown_9[0x10 - 9];
    unsigned char* data;               // +0x10
};

#pragma pack(pop)

extern Game_482270* g_game;

Frame_482270* __stdcall FUN_004b7f30(unsigned short* table, int index);

inline int Lod_482270(Params_482270* params)
{
    int v = params->field_8 / 32;
    return v < 0 ? 0 : v;
}

// FUNCTION: 0x482270
void __stdcall FUN_00482270(Params_482270* params)
{
    if (((Player_482270*)params->field_0)->playerIndex == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flagA = 1;
    }
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    int x = params->field_4[0];
    int y = params->field_4[1];
    if (g_game->flag2 == 1) {
        Grid_482270* grid = &g_game->grid1;
        if ((unsigned)x >= grid->width)
            return;
        if ((unsigned)y >= grid->height)
            return;
        int idx;
        if (((params->field_8 / 32 < 0) ? 0 : params->field_8 / 32)
                < (short)((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1)
            idx = (params->field_8 / 32 < 0) ? 0 : params->field_8 / 32;
        else
            idx = (short)((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1;
        void* table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(idx);
        short count = ((Class_004335c0*)table)->FUN_004335c0();
        short i = 0;
        ((Player_482270*)params->field_0)->grid.at(x, y)++;
        int ref = *params->field_c;
        for (i = 0; i < count; i++) {
            void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
            short num = ((Class_004339c0*)line)->FUN_004339c0();
            int bestDiff = -1;
            int bestIdx = 0;
            short j = 0;
            if (0 < num) {
            int j1 = 1;
            for (; j < num; j++, j1++) {
                int e1;
                int e2;
                ((Class_004339e0*)line)->FUN_004339e0(j, &e1, &e2);
                e1 += x;
                e2 += y;
                short x16 = (short)e1;
                short y16 = (short)e2;
                if ((unsigned)x16 >= grid->width)
                    continue;
                if ((unsigned)y16 >= grid->height)
                    continue;
                unsigned char* cell =
                    grid->cells + (y16 * grid->width + x16) * 2;
                int d1 = cell[1] - ref;
                int d0 = cell[0] - ref;
                if (d0 * bestIdx > bestDiff * j1) {
                    ((Player_482270*)params->field_0)->grid.at(x16, y16)++;
                    if (d1 * bestIdx > bestDiff * j1) {
                        bestIdx = j1;
                        bestDiff = d1;
                    }
                }
            }
            }
        }
    } else {
        int ref = *params->field_c;
        Frame_482270* frame =
            FUN_004b7f30((unsigned short*)g_game->losTable, ref);
        int limitX = (x + frame->width >= halfW) ? halfW - x : frame->width;
        int limitY = (y + frame->height >= halfH) ? halfH - y : frame->height;
        int nx = x < 0 ? -x : 0;
        int ny = y < 0 ? -y : 0;
        if (ny >= limitY)
            return;
        if (nx >= limitX)
            return;
        for (int i = ny; i < limitY; i++) {
            unsigned char* src = frame->data + i * frame->width + nx;
            unsigned char* dst =
                ((Player_482270*)params->field_0)->grid.cells + (y + i) * ((Player_482270*)params->field_0)->grid.width + nx + x;
            int n = limitX - nx;
            do {
                if (*src != frame->mask)
                    (*dst)++;
                dst++;
                src++;
            } while (--n);
        }
    }
}

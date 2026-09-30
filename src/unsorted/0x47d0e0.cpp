// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL, 80.3% (507 bytes vs 505). One instruction reverts to a register
// form instead of the original's memory-operand form, and everything else
// matches instruction for instruction.
//
// Two source shapes were worth real points. First, the inner mask loop must
// read the footprint byte into a named local before the cell-owner test:
//     unsigned char m = obj->unit->mask[index];
//     index++;
//     if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
//     if (m & 1) cell->field_c &= 0xfd;
// Naming `m` (rather than testing `obj->unit->mask[index] & 1` inline) is what
// lets MSVC keep g_game in ebx: it moved the whole body from 36.4% to 79.0%
// because obj finally landed in esi, size in edi and g_game in ebx, and every
// downstream difference (the bx compare temp, ebp as the row-advance scratch,
// the outer counter spilled to [esp+0x1c], ebp as the case 2 zero) followed
// from that one allocation. This is the guide's "one shared upstream cause".
// Second, the mask load and index++ must come BEFORE the cell-owner compare
// in the source; the reverse order (the reviewer-obvious spelling) scored
// 65.8%.
//
// GPT-6.1-sol pass: confirmed 80.3% (507 vs 505 bytes). A signed/unsigned
// game-width declaration and explicit cellIndex multiply/add statements both
// compile to the same register/register multiply and two-byte-longer block.
// Restored the original best source. Remaining difference is the multiply
// operand scheduling described below; all loop bodies otherwise match.
//
// What is left is one block: the original computes the cell index as
//     movsx eax, word ptr [esi+0x78]
//     imul  eax, dword ptr [ebx+0x14233]
// so the multiply folds g_game->width into the imul; ours loads the width into
// eax first, puts pos.y in ecx, and needs a separate `mov eax, [ebx+0x14233]`
// plus `imul eax, ecx`. That is 2 bytes over and shifts every jump target.
// It is the same class as the 0x47d820 base/index swap: nothing in the source
// moves it. Tried and flat at 80.3% or worse: all six declaration orders of
// size/cell/index, the index declared uninitialised and assigned, size
// assigned after the cell pointer, a `row`/`idx` intermediate, a `cells + a*b`
// pointer-arithmetic form, `width * y` and `x + y * width` operand orders, and
// `short px/py` locals. The condition order and every loop spelling were also
// swept earlier, before the mask-local fix, and none of them is the lever.
//
// DeepSeek V4.1 Flash retried the block with isolated one-function scratch
// files (scored with --sym, no check.py budget): swap of the multiply
// operands, `x + y*width` and `x + width*y`, int/short cy/cx locals, a
// base-pointer local, a second Game* pointer, an inlined GetCell(obj, g_game)
// helper, an inlined CellIndex(obj, width) helper, and size/index declared
// before and after the cell pointer. All stay at 80.3% except the declaration
// reorders, which drop to 79.0%. tools/headers.py tried all 128 header sets:
// closest is 80.3%, so no header set changes the multiply. The multiply's
// destination register (pos.y, so `imul eax, [width]`) and the early cells
// load are a single scheduling choice that no source shape here reaches.
//
// deepseek-v4.1-flash second pass (also 80.3%, no new lever helped): in
// isolation, MSVC 5 emits the memory-operand form `imul eax, [width]` only
// when the other operand is a *zero-extended byte* (it keeps the byte in eax
// via `xor eax,eax; mov al,[..]`, as 0x47db70 proves at 0x47dcb9). For a
// sign-extended `short` it always sign-extends into a scratch register and
// uses the reg,reg form (`movsx esi,[y]; mov eax,[width]; imul eax,esi`),
// whatever the source order, casts, int/short/Point locals, or inlined
// PosY/Row/Idx/Mul helpers. So the original's `movsx eax,[esi+0x78];
// imul eax,[ebx+0x14233]` is an allocator outcome for a short operand, not a
// source-shape difference, and nothing in the source reaches it.
//
// deepseek-v4.1 third pass (still 80.3%, 507 vs 505, 24 more check.py runs;
// all variants below compiled to a byte-identical 507-byte stream, so MSVC 5
// canonicalizes the whole block and none of them is a lever):
//   - multiply operand swap `g_game->width * obj->pos.y`, `obj->pos.x +
//     g_game->width * obj->pos.y`, `(int)g_game->width * obj->pos.y`,
//     `(unsigned)g_game->width * obj->pos.y` (Z2), `(unsigned short)` y cast;
//   - named intermediates: `int idx = y * width;` + `idx + x`,
//     `int px = obj->pos.x;` + `... + px`, `int row`/`int cy`, `unsigned idx`;
//   - pointer forms: `&g_game->cells[row]` then `cell += x`; `&cells[x]` then
//     `cell += y * width`; `cells + y * width + x`; `cells + (y * width + x)`;
//     `Cell* base = g_game->cells;` then `&base[...]`;
//     `(Cell*)((int)cells + idx * 0xd)`; `int* pw = &g_game->width` and
//     `short* py = &obj->pos.y` (the optimizer folds both pointers back);
//   - prologue reorders: `int index = 0;` before the cell statement (80.3),
//     size before cell (79.0), cell before both (79.0);
//   - `#include <windows.h>`: identical;
//   - a second use of a live `y` local (`pad.y = y - 1;`) makes the local live
//     across the loops and drops to 64.5% (int) / 64.3% (short), so the
//     original's y is definitely not a surviving local.
// New evidence from the MATCHed neighbour 0x47db70: its original folds the
// width into imul only because its LHS comes from the zero-extend byte idiom
// (`xor eax,eax; mov al,[..]`), which leaves the value in eax before the
// multiply. A sign-extended `movsx` operand never takes that path here, and
// 0x47d820 (99.1%) shows the same reg,reg form for a `short y` local. The
// remaining 2 bytes are the scheduler choosing eax for the width load first
// (right-to-left at the multiply node); no source shape tried reaches it.
//
// deepseek-v4.1-flash final pass (still 80.3%, 507 vs 505): this retry added
// more structural spellings, all scored with --sym so no check.py budget was
// spent: the index as a named int with `idx *= width` and `idx = idx * width`,
// a `Cell* cell = &cells[x]; cell += y * width` split, `int& w = g_game->width`
// then `y * w`, `int* wp = &g_game->width` then `y * *wp`, a pointer-add form
// `&cells[y * width] + x`, a `long row` temporary, and moving `Point size`
// after the cell statement (with an uninitialised-then-assigned spelling).
// Every one produced a byte-identical 507-byte stream to the source already in
// this file: the multiply block canonicalises, so the 2-byte gap is a fixed
// MSVC 5 allocator choice for a sign-extended short operand. Nothing left to
// try; this stays a partial.
#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell_0047db20 {
    short field_0;
    short field_2;
    char unknown_4[0xc - 0x4];
    unsigned char field_c;
};

struct Unit_0047db20 {
    char unknown_0[0x14e];
    unsigned char* mask;
};

union Flags_0047db20 {
    struct {
        unsigned int unknown_0 : 26;
        unsigned int flag26 : 1;
        unsigned int unknown_1 : 5;
    } bits;
    int all;
};

struct Obj_0047db20 {
    char unknown_0[0x76];
    Point pos;
    char unknown_7a[4];
    Point size;
    int field_82;
    char unknown_86[0x92 - 0x86];
    Unit_0047db20* unit;
    char unknown_96[0xa8 - 0x96];
    short field_a8;
    char unknown_aa[0x110 - 0xaa];
    Flags_0047db20 flags;
};

struct Game_0047db20 {
    char unknown_0[0x14233];
    int width;
    char unknown_14237[0x14287 - 0x14237];
    Cell_0047db20* cells;
    char unknown_1428b[0x142b7 - 0x1428b];
    int field_142b7;
};

class Class_0047db20 {
public:
    virtual void FUN_0047ed30();
};
#pragma pack(pop)

extern Game_0047db20* g_game;
extern Class_0047db20 DAT_004fd660[];

void __stdcall FUN_00483210(Point pos, Point size);
void __stdcall FUN_0047e5c0(Point pos, Point size, Class_0047db20* visitor);
void __stdcall FUN_00440a70(Obj_0047db20* obj);

// FUNCTION: 0x47d0e0
void __stdcall FUN_0047d0e0(Obj_0047db20* obj)
{
    if (obj->field_82 != g_game->field_142b7) {
        Point size = obj->size;
        int index = 0;
        Cell_0047db20* cell = &g_game->cells[obj->pos.y * g_game->width + obj->pos.x];
        if (obj->flags.all & 0x20000000) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    unsigned char m = obj->unit->mask[index];
                    index++;
                    if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
                    if (m & 1) cell->field_c &= 0xfd;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            Point grown;
            grown.x = size.x + 2;
            grown.y = size.y + 2;
            Point pad;
            pad.x = obj->pos.x - 1;
            pad.y = obj->pos.y - 1;
            FUN_00483210(pad, grown);
        } else if ((obj->flags.all & 3) == 1) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        } else if ((obj->flags.all & 3) == 2) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    if (cell->field_2 == obj->field_a8) cell->field_2 = 0;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        }
    }
    obj->flags.all &= ~0x08000000;
    if (obj->flags.bits.flag26) {
        obj->flags.all &= ~0x04000000;
        Class_0047db20 visitor;
        FUN_0047e5c0(obj->pos, obj->size, &visitor);
    }
    FUN_00440a70(obj);
}

// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// DeepSeek V4.1 Flash second pass (still 96.1%, no code change): two 3-minute
// permuter runs (2224 candidates on the file below, 3385 seeded from the
// obj-first width-pointer variant) found nothing. Confirmed by hand that the
// obj-first guard written `if (obj->field_82 != g_game->field_142b7)` on top of
// the width pointer reproduces the original's guard bytes AND keeps pos.y in
// eax, giving 507 bytes / 82.8% whose index block differs from the original by
// exactly the missing fold. About 60 more index spellings seeded from that
// 82.8% candidate (short/int/unsigned locals for y, size via a pointer, the
// width through `int*`/`const int*`/`int&` to the local and to the field,
// element and cast forms, `Point`/`Point*` copies, named row/index temporaries,
// a live dummy use) all emit `mov ecx,[width]; imul eax,ecx`, never
// `imul eax,[width]`. So the cross obj-first + fold is unreachable from source
// in this file, and the 2-byte residual stands.
// PARTIAL, 96.1%, 505 bytes vs 505: the width-pointer form below is the whole
// improvement over the 93.5% that was here when this pass started.
// claude-sonnet-5-5 pass (still 93.5%): the ORIGINAL guard is the obj-first one
// (`mov ecx,[g+0x142b7]; mov eax,[obj+0x82]; cmp eax,ecx`), so the original is the
// old guard plus the folded `imul eax,[width]` (-2 bytes); we have either old guard
// without the fold (507) or the swapped guard without it (505). Isolation harness:
// MSVC folds `imul reg,[mem]` for a short operand only when the right operand needs
// no address register ([abs global], [esp+k]) or the left operand is heavier than the
// right (`(pos.y + pos.x) * g->width` folds). `short * g->width` never folds, whatever
// the base register (param, CSE temp, local), cast, constant wrapper or spelling
// (14 more index spellings and 2 permuter runs of 10 min: no change).
//
// PARTIAL, 93.5% (505 bytes vs 505). The size now matches the original exactly
// and everything from 0x47d12d on is byte identical, including every jump
// target; only the eight-instruction index block at the top of the body still
// differs, and it is the same multiply-block problem the earlier passes
// recorded. See the last entry for the new lever.
//
// WHAT THE EARLIER PASSES RECORDED (all of it still true unless noted):
// One instruction reverted to a register form instead of the original's
// memory-operand form, and everything else matched instruction for instruction.
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
//
// deepseek-v4.1-flash retry #3087 (still 80.3%%, 507 vs 505, no check.py spent
// on scratch): swept the 0x47d820 inert-declaration lever, 0/2/4/6/8/12/16/20/
// 24/28/32/40/64 unused `extern int` lines before the first `#pragma pack`,
// every count identical. Six more address-expression shapes (int idx with
// idx += x, raw pointer add, x-first sum, a Point* local, int/short y locals)
// all canonicalise to the same 507-byte stream. The gap is the imul operand:
// original `movsx eax,[esi+0x78]; imul eax,[ebx+0x14233]` (505); ours loads
// the width into eax and uses `imul eax,ecx` (507). No source shape reaches
// the memory-operand form for a sign-extended short.
//
// Space Bunny Free pass: 80.3% -> 93.5%, and the blocker moved. The lever is
// the ORDER OF THE OPERANDS OF THE FIRST COMPARISON, not the index expression.
// Writing the guard as
//     if (g_game->field_142b7 != obj->field_82)
// instead of
//     if (obj->field_82 != g_game->field_142b7)
// takes ours from 507 to exactly 505 bytes, so every jump target from the
// index block onwards now matches the original and the whole tail of the
// function is byte identical. The two are the same test; only the order in
// which MSVC loads the two fields changes, and that is enough to move the
// register allocator. Ours goes from
//     mov ecx,[ebx+0x142b7] ... mov eax,[esi+0x82] cmp eax,ecx
// to
//     mov eax,[ebx+0x142b7] ... cmp eax,[esi+0x82]
// which is 2 bytes shorter and exactly makes up the 2 bytes the index block
// was over. `(a ^ b) != 0` scores the same 93.5% with the same compare, so
// the win is the size, not the particular spelling.
// Swept at the new 93.5% baseline and all flat, so the multiply block is now
// the only thing left and none of the earlier index spellings reach it:
//   - all six orders of size / index / cell: 505 bytes at 92.2% with the cell
//     statement first and 93.5% for the other five under the new guard, and
//     507 at 78.96-80.26% under the old guard, so the new guard and putting
//     the cell statement first are two separate effects;
//   - index expression, all byte-identical at 93.5%: `width * pos.y`,
//     `pos.x + pos.y * width`, a separate `int idx`, a separate `int row`,
//     `int`/`short` `pos.x`/`pos.y` locals, `(int)` casts on the operands,
//     `g_game` through a local `Game*`, `cells` through a local pointer,
//     pointer arithmetic, an inlined `static inline` cell helper taking
//     (game, x, y) or (game, Point), a `Game::cellAt` member, `n` computed
//     in two statements, `n *= width` then `n += x`, `Point& size`,
//     `Point& p`, `Obj& o`, `short py`/`px` locals: all 93.5%, same block;
//   - prologue: `!(a == b)`, `a != b ? 1 : 0`, `(a - b) != 0`, `a > b ||
//     a < b`, `int a`/`int b` locals, a `bool` local, an inlined
//     `Changed(obj)` helper, a `Game::stamp()` member and a `Stamp(obj)`
//     helper: `(a ^ b) != 0` is the only other 93.5%, `bool ne` drops to
//     77.6% and the helpers to 78.2%.
// The remaining residual is the same eight instructions, and the register the
// multiply lands in is still the difference:
//   original: movsx eax,[esi+0x78] / imul eax,[ebx+0x14233]
//   ours:     movsx ecx,[esi+0x78] / mov eax,[ebx+0x14233] / imul eax,ecx
// with `edi` picking up `obj->size` two instructions earlier than the original
// does. So the fold still needs eax to be free at the multiply and the size
// load to be scheduled after it; the new guard freed the register, the
// scheduling did not follow.
// Further search at the new 93.5% baseline, about 90 more scratch variants and
// a minimal isolation harness in build/scratch/0x47d0e0, all still 93.5% with
// the same eight-instruction block, so nothing else moves it:
//   - the address form: `&cells[...]`, `cells + ...`, `cells + (...)`,
//     `cells; cell += ...`, a row pointer then `+ x`, a row pointer then
//     `&row[x]`, `&cells[row]` then `+= x`, `cells + x` then `+= y*width`,
//     a `char*` scale by 13, and a `cells - (...)` pointer subtraction:
//     all byte-identical at 93.5%;
//   - inlined helpers, ten shapes: `CellIndex(obj)`, `CellIndexXY(game,x,y)`,
//     `RowOf(game,y)`, `MulY(y,w)`, a `Game::rowOf` member, a
//     `Game::cellAt` member, three separate one-line accessors for the
//     operands, the `c - (y * w + x)` shape from the matched 0x421e60, and a
//     helper returning `g->cells + y * g->width + x`: all 93.5% with the
//     same block, except the `c - (...)` one (77.7%) and a non-inlinable
//     out-of-line helper (47.0%), both worse;
//   - `int`/`unsigned`/`long` index locals, a `short` y local, `unsigned`
//     y and width, `(int)` casts, a `Point& size`, a `Point& p`, an
//     `Obj& o`, a `Game* game` local, a `Cell* cells` local, the index read
//     into `px` first, the index split over `n = y*w; n += x`, and
//     `n = y; n *= w; n += x`: all 93.5%, same block;
//   - declaration state: all six orders of size / index / cell; `int index;`
//     uninitialised then `index = 0`; `index = 0` written after the cell
//     statement; `size` declared then assigned; `index` named `mapIndex` and
//     copied; `cell -= 0` and `&cells[...] + 0` no-ops: 93.5% except the
//     three orders that put the cell statement first (92.2%);
//   - `g_game->width` redeclared `unsigned int`, `pos.y` copied to an
//     `unsigned` local, and dropping the flags union in favour of a plain
//     `unsigned int fl` read before the cell statement: flat or worse
//     (61.3% for the flags change);
//   - `tools/headers.py` again: all 128 sets are 93.5%.
// The isolation harness settles what the earlier passes guessed at. A
// sign-extended `short` DOES fold: `cell - (cell->y * g_game->width +
// cell->x)` compiles to `movsx eax,[y]; imul eax,[width]` in a small function
// (the matched 0x421e60's own shape), so the rule the earlier passes recorded
// ("a sign-extended short was always loaded into a register first") does not
// hold in general. What decides it is the register the allocator gives the
// sign-extended operand: when it lands in eax the multiply folds the width,
// when it lands anywhere else the width is loaded into eax first and the
// multiply becomes reg,reg. In this function the allocator gives it ecx, and
// `edi` (the `Point size` copy) is scheduled two instructions before the
// multiply rather than after the `cells` load as the original has it. So the
// residual is one allocator decision, and neither the expression's spelling
// nor its order reaches it.
//
// Space Bunny Free pass, baseline reproduced at 93.5% / 505 = 505. The body
// below is unchanged and is still the best measured; this pass bought
// precision about what is missing rather than points. What is new:
//  - THE ORIGINAL'S COMPARE IS THE OBJ-FIRST ONE. `mov eax,[esi+0x82]` is the
//    accumulator load and `mov ecx,[ebx+0x142b7]` the scratch, which is what
//    `a != b` compiles to, so the original's guard is
//    `if (obj->field_82 != g_game->field_142b7)`. Under that guard every
//    instruction of the sixteen-instruction block matches except the multiply,
//    and the block is 63 bytes against the original's 61. Under the game-first
//    guard in the file it is 61 bytes, so the sizes agree and the compare shape
//    does not. So the missing two bytes are exactly the fold: `mov ecx,[y];
//    mov eax,[width]; imul eax,ecx` in place of `movsx eax,[y];
//    imul eax,[width]`, and reaching MATCH needs the obj-first guard PLUS the
//    fold, not either alone. Every earlier pass swept the index block under the
//    game-first guard and the guard under one index block, so that cross was
//    never run;
//  - which guard you get is NOT the expression's doing. Both are reachable from
//    this file: under the obj-first guard, moving `int index = 0;` above
//    `Point size = obj->size;` silently reverts to the game-first compare and
//    back to 505 bytes / 93.5%, and so does hoisting a `Game* game = g_game;`
//    local above the cell statement. The compare form and the block are one
//    allocation decision, not two;
//  - the guard x index cross, 23 variants (width local, const int width,
//    unsigned width, `Game* game`, `Cell* cells`, `short`/`int` y and x locals,
//    a `Point p` copy, a named `int n`, a named row, base pointer add, row
//    pointer plus x, the multiply split over three statements, an explicit
//    cast, a row-base pointer, `const int& w`, two inline helpers): every one
//    is 507 bytes / 80.3% with the same block, so no index spelling folds and
//    the cross closes;
//  - local removal and reshaping under the obj-first guard, 12 variants: no
//    `size` local at all (direct `obj->size` in the loops, 40.4%), no `index`
//    local (a `mask++` pointer walk, 41.6%), neither (41.3%), `size` as two
//    shorts, `size` as one `unsigned int` with shifts (52.2%), `unsigned index`,
//    `index` before `size`, a `Point p` copy first (77.4%), `Game* game` first,
//    `cell` declared then assigned, a mask pointer inside the mask branch
//    (78.1%), and the width through a getter: none is 505 bytes, and the ones
//    that fold nothing cost 20 to 40 points by breaking the loop bodies;
//  - register-pressure perturbations aimed at 0x47de60's stated mechanism
//    (c1 keeps the width as a memory operand only when the scratch it wants for
//    the other multiply operand is not ecx, and here that scratch is ecx):
//    hoisting the mask pointer, the unit pointer, `obj->field_a8`, a live
//    `cell->field_c`, `py` as a short local, `py` and `px` as shorts, and `py`
//    reused after the loops. All flat 507 bytes or 44 to 64%; none folds. So
//    the mechanism does not transfer from that body;
//  - 0x47d820's declaration-state lever is INERT here at every scale: 128
//    runs, `extern int` / `extern void __cdecl f(void)` / `extern int __cdecl
//    f(int,int)` / `typedef` padding at N = 0 to 10000 step 2, 0 to 80, and
//    100 to 10000, both guard forms. Every single one is 93.5% / 505 bytes or
//    80.3% / 507 bytes. Unlike 0x47d820 (16 to 80 flips it) and unlike
//    0x47de60 (~2300 does), no declaration count reaches the fold here;
//  - 62 include sets, the ones 0x47de60's notes name as moving that fold
//    (non-lean <windows.h>, lean <windows.h> plus each of ole2/vfw/richedit/
//    d3dtypes/dinput/ddraw/dsound/dplay/d3d, and stdio/stdlib/string/math/
//    mmsystem/commctrl/shellapi/winsock/rpc and 25 C++ headers) plus 16
//    windows.h pairs: all 507 bytes, none folds;
//  - what the fold actually needs, measured rather than guessed
//    (build/scratch/0x47d0e0/imulrule.py and imulrule2.py, ~40 tiny
//    functions, and cutdown.py, which cuts the real body down and adds pieces
//    back). A level-0 width folds every time: a global `int`, `*int_ptr`,
//    `pw[0]`, a stack slot. A `[reg+disp]` width folds in none of them, at
//    offsets 1, 2, 4, 8, 0x100, 0x1000 and 0x14233, spelled as `g->w`, as
//    `*(int*)((char*)g + k)` and as `pw[k]`, and 0x14233 measured under the
//    same `#pragma pack(1)` as this file so the displacement is exactly the
//    original's. The order of emission there is this function's own, `movsx
//    edx,[y]` then `mov eax,[width]` then `imul eax,edx`. That is only half
//    the story,
//    because the folding ex-sites of this very expression in this very area
//    (0x421e60, 0x4246b0, 0x47db70, 0x47dfc0, 0x47de60, 0x47e2d0) all fold
//    `[reg+disp32]` into `imul`, so the address form is not the discriminator.
//    What differs is which operand c1 makes the accumulator: it puts the width
//    there and materialises it, where the original put `pos.y` there and left
//    the width as the memory operand. In the cut-down body the accumulator is
//    still the width even with the loops deleted and only a single store left,
//    so it is not the pressure of this function's later code either;
//  - the frame-slot widths are right and need no change: [esp+0x10] is a
//    4-byte `Point` (written and read as a dword, and read and written as two
//    words), [esp+0x1c] is a 4-byte slot shared by the `int` row counter, the
//    4-byte `grown` Point and the 4-byte visitor pointer. That is why
//    everything from 0x47d12d on is byte identical.
// Space Bunny Free pass, 93.5% -> 96.1%. THE LEVER IS THE BRIEF'S ITEM 18.
// Passing the width through a pointer to a local that is never modified,
//     int w = g_game->width;
//     int* pw = &w;
//     Cell* cell = &g_game->cells[obj->pos.y * *pw + obj->pos.x];
// is the first thing in nine passes to move this block. A plain `int w` local
// does nothing (80.9%, and the pointer is what does it), because VC5 copies a
// plain local straight back into the register the arithmetic freed, while taking
// its address blocks that propagation. What it changes is the one thing the
// earlier passes could not reach: pos.y now lands in EAX, which is the
// original's accumulator. Every earlier spelling put it in ecx, which is why the
// width had to be materialised there; with eax free at the multiply the width
// load is the only thing in the way. The width local alone, without the
// pointer, is 80.3% and byte identical to the 93.5% file's block, so the
// pointer is the whole of it.
// Two declaration orders then separate: with `int index = 0;` BEFORE the cell
// statement it is 94.2% (9 lines), and with it AFTER the cell statement it is
// 96.1% (6 lines), which is the file below. Moving `Point size = obj->size;`
// after the cell statement as well, or leaving it before, are both 96.1%, so
// the index's position relative to the cell statement is what counts.
// At 96.1% six lines differ and the rest of the function, jump targets
// included, is byte identical:
//   original: mov ecx,[ebx+0x142b7] / mov eax,[esi+0x82] / cmp eax,ecx
//   ours:     mov eax,[ebx+0x142b7] / cmp eax,[esi+0x82]
//   original: movsx eax,[esi+0x78] / imul eax,[ebx+0x14233]
//   ours:     movsx eax,[esi+0x78] / mov ecx,[ebx+0x14233] / imul eax,ecx
//   original: mov ecx,[ebx+0x14287] / mov edi,[esi+0x7e] / lea edx,[eax+eax*2]
//   ours:     mov edi,[esi+0x7e] / mov [esp+0x10],edi / mov ecx,[ebx+0x14287]
//             / lea edx,[eax+eax*2]
// So the compare form and the fold are still one decision and still both
// needed: the obj-first compare shape alone is 82.8% here, so on top of the
// width pointer the game-first compare is right, the reverse of the 93.5%
// file. Swept at this new 96.1% baseline, all flat at 96.1% unless noted:
//   - the guard: the obj stamp in a local, both operands in locals, `==`
//     negated, `!=(a,b)` through pointers, an obj reference, the stamp through
//     a pointer, and `int&` for both sides: 96.1%. A `bool` local is 77.8% and
//     `int* pw = &g_game->width` is 92.2%;
//   - the width: `const int w`, `const int* const pw`, two pointers to w, the
//     multiply written `*pw * pos.y`, the sum written `pos.x + pos.y * *pw`,
//     and a `short y` local read through its own pointer: 96.1%. A second real
//     use of w in the row advance is 59.5% and `w + 0` is 96.1%;
//   - the cell pointer through a pointer to a local, a `Game* game` local, a
//     `Cell* cells` local, a row pointer plus x, a named `int n`, base pointer
//     arithmetic, `long index`, a live `rows` int in the mask case, `size`
//     through a pointer, `pos.x` or `pos.y` through a pointer of its own, and
//     the width local first, before size, or after both: 96.1%. `pos.x` through
//     a pointer is 81.6%, and the three orders that put `index` before the cell
//     statement are 94.2%;
//   - declaration order, all six of size / width / cell / index: 96.1% for the
//     four with the cell statement after the width and `index` last, 94.2% for
//     the two with `index` before the cell statement;
//   - removing `size` entirely is 41.0%, `Game* game` and `obj` as a reference
//     are 96.1%, and every row advance through `*pw` instead of
//     `g_game->width` is 59.5% at 479 bytes, which is shorter only because the
//     whole mask branch changed, so it was not taken.
// Declaration state re-swept at the new 96.1% baseline, since every negative
// above was measured against the old body: 416 runs, eight flavours
// (`extern int`, `extern void __cdecl f(void)`, `extern int __cdecl f(int,int)`,
// `typedef`, a struct definition per line, a union per line, an enum per line and
// a `static int` per line), N = 0 to 10000 including 12, 16, 24, 40, 48, 64, 80,
// 100, 200, 400, 800, 1600, 2400, 3200, 5000 and 10000, and both guard forms.
// Nothing improves on 96.1%: the game-first arm is 96.1% up to N = 40 and 92.2%
// from N = 80 on for every one of the eight flavours (only `typedef` and
// `static int` hold 96.1% to N = 64), and the obj-first arm is 82.8% or 79.0%
// throughout. So there is still no declaration count that reaches the fold here,
// at either scale, which is the same negative as before the fix and now measured
// on both sides of it. `tools/headers.py` was not re-run at 96.1%; the 62 include
// sets swept on the 93.5% body all produced the 93.5% block, so they are the
// wrong lever and the width pointer is the one that moved.
// A permuter run against the 93.5% body (15 min, 2658 candidates, 11 did not
// compile, 14 duplicates) found nothing: 93.5% -> 93.5%. It did not run again at
// 96.1% because the width pointer is not a spelling the permuter generates.
// Conclusion: 96.1% is the file below. The remaining residual is the fold alone,
// and with pos.y now in eax it is a single allocator decision rather than the
// scheduling tangle the earlier passes recorded: we need c1 to leave the width
// load as the imul's memory operand instead of giving it ecx. The pointer form
// is worth trying on the two other partials in this area that fold the same
// multiply, 0x47d2e0 and 0x47de60, since it is what unblocked it here.
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
    if (g_game->field_142b7 != obj->field_82) {
        Point size = obj->size;
        int w = g_game->width;
        int* pw = &w;
        Cell_0047db20* cell = &g_game->cells[obj->pos.y * *pw + obj->pos.x];
        int index = 0;
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
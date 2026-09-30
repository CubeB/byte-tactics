// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Retry #1952: GPT-6.1-sol tested pitch/count declaration order and equivalent
// products. The saved 53.1% (420/423) variant remains best; no MATCH. The
// prologue still puts the 2*w pitch in ebx/esi work registers and clobbers h,
// while the original keeps pitch in edi, count in ebx, and h in esi across the
// allocator call. Loop locals also remain in different registers and stack slots.
// deepseek-v4.1-flash (third pass): declaring double scale = lens; BEFORE the
// count/pitch pair lifts the score from 51.7 to 53.1 percent (best so far). The
// first diff hunk is still the allocator: the original holds w in ebp, h in esi,
// pitch (2*w) in edi and count (2*w*h) in ebx across the alloc call and spills f
// to the arg1 slot; every spelling here instead computes count in esi
// (imul esi,ebp / shl esi,1), takes ebx for pitch and keeps f in edi, reloading h
// from its argument slot. To close it, h must win esi over the count temp: all
// pre-block orderings (scale, count, pitch, f permutations; w*h*2, (w*2)*h,
// h*(w*2)) give 51.7 or 53.1 and never move count out of esi.
// deepseek-v4.1-flash (second pass, same session model): re-confirmed the
// prologue is a genuine plateau, and pinned what does and does not reach the
// allocator here. 24 further spellings of the pitch/count pre-block
// (h*2*w, 2*h*w, h*(2*w), pitch*h, count*=pitch, w*h then shl, += doubling,
// unsigned, separate assignments) all give the identical 420-byte body at
// 51.7 percent; so do 6 loop-head spellings (x/y/base declared at function
// scope, base as unsigned, while loops, x+base vs base+x). The loop spelling
// therefore never reaches the allocator. The only spellings that move count
// into ebx (as the original has it) are the y*w+x index or pitch*h, but each
// then takes pitch into esi and either spills h (reloaded from [esp+0x38]) or
// puts it in ebx, so the four callee-saved slots come out permuted. int hw/hh
// with (short) casts regresses to 35.1 percent, so the short locals are load
// bearing. Defining the real preceding function (0x4b90a0) above this one in
// the scratch file changed nothing (still 51.7), as did <string>, <vector>,
// <map>, <list> and <iostream>.
// deepseek-v4.1-flash: the falloff quotient's SIGN was wrong. The disassembly's
// `fisubr dword [esp+0x40]` computes `half_width - dist`, and `fdiv st,st(1)`
// divides that by `scale`, so the correct expression is
// `g = (hw - dist) / scale` and the value is
// `hw + ((int)(dy / g) + hh) * w + (int)(dx / g) - (base + x)` (hw added
// before the dy term, matching `add ecx,edx`). With the sign fixed the score
// is unchanged at 51.7 percent, which confirms the remaining gap is only the
// register rotation, not the arithmetic. The upstream cause is unchanged: the
// original holds h in esi, pitch (2*w) in edi and count (2*w*h) in ebx across
// the alloc call; every pre-block declaration order tried here either drops h
// into memory (pitch takes esi) or reuses esi for count. Sweeps of the
// pre-block (25 orders), the whole pitch/count/size spelling, and
// tools/headers.py (128 sets, flat 51.7) did not move it.
// PARTIAL, 53.1% (420 of 423 bytes). The 146-instruction
// original is matched instruction for instruction in outline, so what is left
// is register and slot placement, not missing code. About ninety shapes were
// scored with check.py --sym; this is a strong partial and NOT a proven
// plateau, the register-allocation question below is under-explored.
//
// Corrected from the previous attempt's notes, which were wrong in two ways
// worth recording:
//  - hw and hh are ONE halving, `w / 2` and `h / 2`, not `w/2/2`. The
//    disassembly's `cdq; sub eax,edx; ... sar reg,1` is a single signed
//    halving, and reading it as two cost several points. Fixing the semantics
//    alone was 45.6% to 46.3%.
//  - The frame size now MATCHES: both this file and the original are
//    `sub esp, 0x24`, and `w` is in ebp as in the original. The previous
//    attempt's 0x28 frame and its "the value needs one local more than the
//    original's nine" conclusion are both superseded.
//
// What this builds: the "lens" image that 0x420620 asks for with (22, 22, 8).
// A 0x18-byte header followed by a w by h array of 16-bit cells, each either
// 0x7d00 (a hard '}' mark) or a short built from two integer divisions of
// (dist - half_width) by the falloff argument, plus the header's pitch, height
// and half-width/half-height fields.
//
// Derived, with the evidence:
//  - The header field at +0 is written twice, 2*w right after the allocation
//    and w after the null test, and the second store is `shr di, 1` on the
//    16-bit register, so it divides the first value by two rather than
//    reloading the parameter.
//  - The end pointer at +0x14 is `(char*)data + 2*w*h`, byte arithmetic on the
//    doubled width times the height, while the allocation is
//    `2*(2*w*h) + 0x18` bytes. Both come from one shared 2*w*h value, which is
//    why the original computes `lea edi,[ebp+ebp]`, `mov ebx,edi`,
//    `imul ebx,esi` and then `lea eax,[ebx+ebx+0x18]`, and reuses ebx for the
//    end. The loop only writes w*h cells, so the allocation is twice what the
//    cell array needs while the end pointer is exactly right: one of the two
//    is a Cavedog slip.
//  - +0x9, +0xa and +0xb are zeroed as three separate bytes and +0x8 is left
//    alone, and the null test floats up beside the `xor eax,eax` that feeds
//    them, so the source stores the header first and tests afterwards.
//  - hw and hh are `short`: stored truncated to words, sign extended at every
//    use (`movsx ecx,dx` in the preheader, `movsx ebx,word [esp+0x40]` in the
//    loop) but the falloff subtraction is `fisubr dword`, the promoted value
//    in the dead third parameter slot.
//  - The falloff argument becomes a double once at the top and stays on the
//    x87 stack across both loops, popped by the `fstp` at the common exit.
//  - (int)dist keeps the sqrt on the stack (`fld st(0)` before _ftol) so the
//    else branch subtracts half_width from it without reloading.
//  - (w+3)/4 and dy*dy are loop invariant and sink into the `if (w > 0)` body
//    that a for loop lowers to, ahead of the x loop's own preheader; dy2 keeps
//    a copy in ecx that the back edge reloads.
//  - The value is `(a + hh) * w + b + hw - (base + x)`: the compiler forms
//    half_width + (a + half_height) * w first (declaration order), adds b,
//    then subtracts the cell index.
//
// What still differs:
//  - The original computes the doubled width before the height product and
//    keeps it in edi across the call; a named pitch local is the only spelling
//    that stops MSVC folding the doubling into the size, and that declaration
//    also costs the parameters ebp and esi, so here the count is declared
//    first and MSVC emits `imul esi,ebp` / `shl esi,1`.
//  - The original spills the frame pointer to the dead first parameter slot
//    and reloads the height from the second; here the frame stays in a
//    register and the width is re-read from its own parameter slot, so the
//    loop rotates (base in ebx, the x counter in esi), MSVC folds the header
//    offset 0x18 into the index (base starts at 12 with a matching +12 in the
//    value) and every local slot moves.
//  - Frame size 0x28 against the original's 0x24: SUPERSEDED, both are 0x24
//    now. The old conclusion that "the value needs one local more than the
//    original's nine" was wrong; the frame was right all along once the
//    semantics were fixed. What the original is doing instead is holding four
//    values live across the call in the four callee-saved registers (ebp = w,
//    esi = h, edi = pitch, ebx = count) and spilling f to the dead first
//    parameter slot at [esp+0x38]. That is the single largest remaining hunk
//    and the one lever not yet exhausted: make h genuinely live across the
//    call, by touching it in a way that does not merely reload it, and MSVC
//    should be forced into esi. Getting four registers live would in turn make
//    f spill, which is what the loop's base addressing is waiting for.
//  - The loop body differs in register rotation and indexing. The original
//    uses esi as the running base and edi as x, addressing the cell as
//    `(f + 0x18) + index*2` with base starting at 0; this file folds the 0x18
//    into base (starting at 12, with a matching +12 in the value) and uses a
//    countdown `inc esi; dec ecx; cmp esi,edi` where the original has a plain
//    `inc edi; cmp edi,ebp`. Routing through a `cells` pointer local or
//    through `f->data` both scored far worse (38% to 40%), so the folding is
//    MSVC's own choice for the current source shape, not a spelling error.
//  - The `f->half_width` and `f->half_height` stores repeat the expressions
//    `(short)(w / 2)` and `(short)(h / 2)` rather than using the `hw` and `hh`
//    locals. That duplication is load-bearing: using the locals scores 50.3%
//    against 51.7%. Two other spellings score the same 51.7%, so the
//    duplication is required but its exact form is not pinned down, which
//    suggests the original's shape is something else again that frees the
//    same register.
//
// Tried, with no better result (each scored on check.py from a scratch copy):
// the size as count*2, count+count, (w*2)*h*2, 2*((w*2)*h) and with the
// constant at 0x30; the count initialised as (w*2)*h, w+(w)*h, h*(w*2),
// h*pitch and pitch*h; count declared before and after pitch, both as one
// `int a, b;` declarator and as two assignments; pitch as an int, an unsigned
// and an unsigned short; the end pointer through data, through cells, and as
// short-pointer arithmetic (which folds to f + size and is wrong for the
// original's byte offset); hw and hh as int with (short) casts and as short
// locals; the value as one expression, with a named int, with a named short,
// with a named index, with no index subtraction and with -base - x; a cells
// pointer local; the height store moved before the data and end stores; and
// the threshold inlined in the comparison. Those spellings were a plateau at
// 45.6% and are no longer one: the semantics fix plus the value expression
// above reached 51.7%, which shows the plateau was an artefact of a wrong
// reading rather than a wall. Treat any future plateau here with the same
// suspicion and re-derive from the disassembly before believing it.
#include <math.h>

struct LensFrame_4b91b0 {
    unsigned short pitch;      // +0, the row stride in bytes, then the width
    unsigned short height;     // +2
    short half_width;          // +4
    short half_height;         // +6
    char field_8;              // +8, left alone
    char field_9;              // +9
    char field_a;              // +a
    char field_b;              // +b
    char unknown_c[4];         // +c
    unsigned short* data;      // +0x10
    char* end;                 // +0x14, one past the last cell, in bytes
    unsigned short cells[1];   // +0x18
};

extern void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

// FUNCTION: 0x4b91b0
unsigned char* __stdcall FUN_004b91b0(int w, int h, int lens)
{
    double scale = lens;
    int count = (w * 2) * h;
    int pitch = w * 2;
    LensFrame_4b91b0* f = (LensFrame_4b91b0*)FUN_004d83b0("LensFrame", count * 2 + 0x18);
    f->pitch = (unsigned short)pitch;
    f->data = f->cells;
    f->end = (char*)f->data + count;
    f->height = (unsigned short)h;
    f->half_width = 0;
    f->half_height = 0;
    f->field_9 = 0;
    f->field_a = 0;
    f->field_b = 0;
    if (!f)
        return 0;
    short hw = (short)(w / 2);
    short hh = (short)(h / 2);
    f->pitch = (unsigned short)pitch / 2;
    f->half_width = (short)(w / 2);
    f->half_height = (short)(h / 2);
    int base = 0;
    int y;
    for (y = 0; y < h; y++) {
        int x;
        for (x = 0; x < w; x++) {
            int dy = y - hh;
            int dy2 = dy * dy;
            int thresh = (w + 3) / 4;
            int dx = x - hw;
            int d2 = dx * dx + dy2;
            double dist = sqrt((double)d2);
            if ((int)dist >= thresh) {
                f->cells[base + x] = 0x7d00;
            } else {
                double g = (hw - dist) / scale;
                f->cells[base + x] = (unsigned short)(hw + (((int)((double)dy / g)) + hh) * w + ((int)((double)dx / g)) - (base + x));
            }
        }
        base += w;
    }
    return (unsigned char*)f;
}

// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 45.6% (best of about forty source shapes scored on check.py; the
// 146-instruction original is matched instruction for instruction in outline,
// so what is left is register and slot placement, not missing code).
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
//  - Frame size 0x28 against the original's 0x24: the value needs one local
//    more than the original's nine, and the original's set (hh, y, base, dy2,
//    d2, thresh, dy, and the double that dx then shares) has no spare slot.
//    The named value local is what lifts this file from 39.6% (the same source
//    with the value as one expression) to 45.6%, so it is a stand-in for
//    something in the original that frees a register; the original must be
//    holding the four values live across the call (w, h, pitch, count) in the
//    four callee-saved registers and spilling f, which leaves no register for
//    a ninth value.
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
// the threshold inlined in the comparison. 45.6% is a plateau: the last eight
// variants that differ only in those spellings all score the same.
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
    int count = (w * 2) * h;
    int pitch = w * 2;
    double scale = lens;
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
    short hw = (short)(w / 2 / 2);
    short hh = (short)(h / 2 / 2);
    f->pitch = (unsigned short)pitch / 2;
    f->half_width = hw;
    f->half_height = hh;
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
                double g = (dist - hw) / scale;
                int a = (int)((double)dy / g);
                int b = (int)((double)dx / g);
                int v = (a + hh) * w;
                v = v + b + hw - (base + x);
                f->cells[base + x] = (unsigned short)v;
            }
        }
        base += w;
    }
    return (unsigned char*)f;
}

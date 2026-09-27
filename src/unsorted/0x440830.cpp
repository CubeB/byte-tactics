// Decompiled by space-bunny-free. Names are provisional.
// Writes a rectangle of 2-bit cells into the transposed bitmap whose dword at
// column x of row band (y>>4) holds 16 cells stacked down the column.
//
// Gave up at 71.2%. Everything matches (both clamp chains, the guard, both
// loop bounds, the band reload via `imul`, and the reuse of the incoming
// argument slots for `top` and `bottom`) except the inner loop's register
// allocation and the frame size.
//
// The original has 5 stack homes (left, right, mask, band, masked-old) and a
// frame of 0x14; ours has 3 (left, right, band) and a frame of 0xc. The only
// missing instructions are the mask spill before the loop and the reload
// block at the inner-loop top:
//   mov [esp+0x18], ebp        ; original only
//   jmp  body
//   mov ebp, [esp+0x18]        ; original only, inner-loop latch target
// Ours instead schedules `v << shift` FIRST, so the masked value is never live
// at the same time and needs no home:
//   mov ecx, edi / shl eax, cl / mov ecx,[edx] / and ecx,ebp / or eax,ecx
// The original masks first, spills, then shifts, then ORs into the reloaded
// value (`or ecx, eax`).
//
// Things already tried that made it WORSE, do not repeat:
// - naming `mask` as a local, at either block or function scope: 28.6%. Any
//   named mask changes the frame to 0xc and shifts the whole allocation, so
//   even the first two `movsx` argument loads come out in the wrong registers.
// - naming `mask` + `band` + `keep` together: 32.6%.
// Things tried that changed nothing at all (byte-identical to the 71.2%
// version, so the compiler promoted them straight back to the same code):
// - naming the masked value `keep` as its own statement.
// - naming the shift `(y & 0xf) * 2` as a local and using it in both places.
//
// The mask must therefore stay an inline expression, and its stack home has
// to come from register pressure in the original rather than from being a
// variable. Forcing that pressure is the open problem. The already-matched
// single-cell setter 0x4404c0 uses the same shift/mask idiom.

struct Point_00440830 {
    short x;
    short y;
};

class Class_00440830 {
public:
    int* field_0;                      // +0x0
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned int width;                // +0x10
    unsigned int height;               // +0x14
    unsigned int* data;                // +0x18

    void FUN_00440830(Point_00440830 a, Point_00440830 b);
};

unsigned int __stdcall FUN_0047e1f0(Class_00440830* obj, int x, int y);

// FUNCTION: 0x440830
void Class_00440830::FUN_00440830(Point_00440830 a, Point_00440830 b)
{
    int left = a.x - field_4;
    int top = a.y - field_6;
    int right = a.x + b.x + 1;
    int bottom = a.y + b.y + 1;
    if (left < 0) {
        left = 0;
    }
    if (top < 0) {
        top = 0;
    }
    if (right > width) {
        right = width;
    }
    if (bottom > height) {
        bottom = height;
    }
    if (left < right && top < bottom) {
        for (int y = top; y < bottom; y++) {
            for (int x = left; x < right; x++) {
                unsigned int v = FUN_0047e1f0(this, x, y);
                unsigned int* p = &data[(y >> 4) * width + x];
                *p = (*p & ~(3 << ((y & 0xf) * 2)))
                   | (v << ((y & 0xf) * 2));
            }
        }
    }
}

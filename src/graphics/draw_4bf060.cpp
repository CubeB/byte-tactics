// Decompiled by space-bunny-free. Names are provisional.

// Draws the polyline points[0..count-1] into `surface`, or into the screen
// (locked with LockScreen, unlocked with UnlockScreen) when `surface` is
// null. Every segment is clipped by ClipLine and drawn by BlitLine.
// Returns the lock result on the screen path, so a failed lock returns 0
// without ever unlocking; the caller-surface path returns 1.
//
// The locked branch carries a copy of the single segment drawer DrawLine
// per segment, nested lock and all, which is what /Ob2 leaves behind when the
// source calls it: the callee's own `if (surface == 0)` is the `if (&screen ==
// 0)` below, tested on an address the compiler knows is not null, so the test
// survives as `lea eax, [esp+0x14]; test eax, eax; jne` with the locked block
// as the fall-through. Keeping that test is what puts the second lock call and
// its unlock in the function at all.
//
// check.py prints MATCH (508 bytes). Two source shapes carry the match and
// both are needed:
// - the segment ends walk the array through the local Segment_004bf060. With
//   two plain `Point*` locals (however they are spelled, in either order, or
//   with an index) MSVC 5 folds them into a single induction variable biased
//   to a middle field, and then one callee-saved register is free, so the
//   result stays in ebp: the frame is 0x60, one `add reg, 8` is missing and
//   the function is 464 bytes (32.1 percent). The struct keeps both pointers
//   as induction variables (esi and edi) and spills the result to [esp+0x10],
//   which is what fixes the 0x64 frame and the 25 dwords of locals.
// - the four coordinates are declared in the order x0, y0, x1, y1 (that order
//   picks which dead argument slot each one lands in: x0 at [esp+0x78] and
//   y0 at [esp+0x84] in the locked loop, x0 at [esp+0x10] and y0 at
//   [esp+0x78] in the caller-surface loop) and assigned in the order y1, x1,
//   y0, x0, which is the order the original loads them in. The other 23 orders
//   all score 0.90 to 0.95.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor
};

struct Point_004bf060 {
    int x;                             // +0x0
    int y;                             // +0x4
};

// The two ends of the segment being drawn.
struct Segment_004bf060 {
    Point_004bf060* from;
    Point_004bf060* to;
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl BlitLine(Surface* dst, int x0, int y0, int x1, int y1, int color);

// FUNCTION: 0x4bf060
int __stdcall DrawPolyline(Surface* surface, Point_004bf060* points,
                           int count, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            Segment_004bf060 seg;
            seg.from = points;
            seg.to = points + 1;
            int n = count - 1;
            while (n > 0) {
                int x0, y0, x1, y1;
                y1 = seg.to->y;
                x1 = seg.to->x;
                y0 = seg.from->y;
                x0 = seg.from->x;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
                seg.from++;
                seg.to++;
                n--;
            }
            UnlockScreen(&screen);
        }
    } else {
        Segment_004bf060 seg;
        seg.from = points;
        seg.to = points + 1;
        int n = count - 1;
        while (n > 0) {
            int x0, y0, x1, y1;
            y1 = seg.to->y;
            x1 = seg.to->x;
            y0 = seg.from->y;
            x0 = seg.from->x;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                BlitLine(surface, x0, y0, x1, y1, color);
            seg.from++;
            seg.to++;
            n--;
        }
        result = 1;
    }
    return result;
}

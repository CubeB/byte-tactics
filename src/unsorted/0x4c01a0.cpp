// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Draws a dashed circle: sweeps `angle` from `step` to 0x10000 in steps of
// step = 0x10000 / n and draws the segment from the previous point to the
// current point only when the counter `start` is odd (the caller passes 0 or
// 1, see 0x4670fd). FUN_004b7123 / FUN_004b70ef are the sine and cosine table
// lookups (angle, radius); FUN_004bea20 clips the segment and FUN_004cc7ab
// draws it. When `surface` is null the screen is locked with FUN_004c5e70 and
// unlocked with FUN_004c5fa0.
//
// Still differs: the original keeps the previous x in ebx, the angle in ebp,
// the current x/y in esi/edi and spills the previous y and the counter to the
// stack; MSVC 5 here instead keeps the previous x/y and the counter in
// registers (edi/ebp/ebx), puts the angle in esi and spills the current x/y.
// The source shape (px/py copies into x0/y0, address-taken end point) is
// right; only the register allocator's choice differs, which
// `check.py` shows as the prologue and loop-carried stores.
//
// A third pass pinned the structural shape of the gap, which is worth having
// even though it did not close it. The prologue frames everything:
//
//   original: sub esp,0x40 ; push ebx ; push ebp ; mov edi,[esp+0x5c] ;
//             mov ecx,[esp+0x58] ; mov [esp+0x10],ecx ; lea ebx,[edi+eax]
//   ours:     sub esp,0x3c ; push ebp ; push edi ; lea edi,[ecx+eax]
//
// So the original reserves one dword more of frame (0x40 against 0x3c) and
// spends a fourth callee-saved register: it pushes ebx, ebp and edi before the
// division and pushes esi only *after* the loop guard (`jg`), while we push ebp
// and edi and use ecx and esi instead. The `lea` is the same address in both,
// computed into ebx in the original and into edi here, which is the same
// register-choice permutation as the rest. The original also spills the step
// (`mov ebp, eax` then `mov [esp+0x18], eax`) where we keep the step in ebp
// and spill a different value to [esp+0x10], so the two functions disagree about
// which value owns a frame slot as well as about how many slots there are.
//
// Four shapes of the head were tried and none moved it: the step as a named
// local against a separate named loop limit, the loop bound from a local
// `limit`, hoisting the `surface == 0` test, and taking the previous point as a
// two-field struct. None changes the frame size, so the extra dword is a value
// the original keeps in memory that this source keeps in a register.

struct Surface_004c01a0 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __cdecl FUN_004b7123(int angle, int scale);
int __cdecl FUN_004b70ef(int angle, int scale);
int __stdcall FUN_004c5e70(Surface_004c01a0* out);
int __stdcall FUN_004c5fa0(Surface_004c01a0* s);
int __stdcall FUN_004bea20(Surface_004c01a0* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface_004c01a0* dst, int x0, int y0, int x1, int y1, int color);

// FUNCTION: 0x4c01a0
void __stdcall FUN_004c01a0(Surface_004c01a0* surface, int cx, int cy, int radius,
                            int color, int n, int start)
{
    Surface_004c01a0 screen;
    int px = cx + radius;
    int py = cy;
    int x1, y1;
    int step = 0x10000 / n;
    if (step > 0x10000)
        return;
    int i = start;
    for (int angle = step; angle <= 0x10000; angle += step) {
        x1 = FUN_004b7123(angle, radius) + cx;
        y1 = FUN_004b70ef(angle, radius) + cy;
        if (i & 1) {
            int x0 = px;
            int y0 = py;
            if (surface == 0) {
                if (FUN_004c5e70(&screen)) {
                    if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                    FUN_004c5fa0(&screen);
                }
            } else {
                if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
                    FUN_004cc7ab(surface, x0, y0, x1, y1, color);
            }
        }
        px = x1;
        py = y1;
        i++;
    }
}

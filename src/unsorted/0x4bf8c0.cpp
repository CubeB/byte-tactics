// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Draws the frame (four edges) of the rectangle `r` in `surface`, or on the
// locked screen (FUN_004c5e70 / FUN_004c5fa0) when `surface` is null. Each
// edge is a copy of the segment drawer FUN_004be950: the screen path carries
// four inlined copies (with the drawer's own unreachable null-surface lock),
// the caller-surface path two inlined copies plus two real FUN_004be950 calls.
// The caller-surface path returns r->bottom, the screen path the lock result.
//
// PARTIAL: 99.4 percent (1169 of 1172 bytes). The screen path matches exactly,
// including the register roles (ebx = lock result, esi = r, edi = color) that
// come from declaring the lock result as a block-scoped `int result` in the
// `surface == 0` arm and returning it there. Two hunks remain, both in the
// caller arm and both caused by the original assigning the caller return to
// the same function-wide variable as the screen result (ebx):
//  - the failed lock jumps to 0x4bfd49 (the shared tail after the caller arm)
//    where ours jumps to the screen arm's own epilogue at 0x4bfc54;
//  - the caller arm ends `mov ebx, [esp+0x7c]; pop edi; mov eax, ebx` (the
//    value is the spilled r->bottom, in ebx) where ours ends
//    `mov eax, [esi+0xc]; pop edi; pop esi; pop ebx`.
// Giving the caller arm a function-scoped return variable (so both arms share
// one tail at the end, which fixes hunk 1) rotates the allocator instead:
// result goes to esi, r to edi and color to ebx, costing the screen arm. That
// shape scores 92.9 percent. Scoping the screen result inside the if is what
// fixes the screen arm; the same variable cannot also serve the caller arm
// without flipping the allocation back.

struct Rect_004bf8c0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bf8c0 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor
};

int __stdcall FUN_004c5e70(Surface_004bf8c0* out);
int __stdcall FUN_004c5fa0(Surface_004bf8c0* s);
int __stdcall FUN_004bea20(Surface_004bf8c0* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface_004bf8c0* dst, int x0, int y0, int x1, int y1, int color);
int __stdcall FUN_004be950(Surface_004bf8c0* s, int x0, int y0, int x1, int y1, int color);

// FUNCTION: 0x4bf8c0
int __stdcall FUN_004bf8c0(Surface_004bf8c0* surface, Rect_004bf8c0* r, int color)
{
    if (surface == 0) {
        Surface_004bf8c0 screen;
        int result = FUN_004c5e70(&screen);
        if (result != 0) {
            {
                int x0, y0, x1, y1;
                y1 = r->top;
                x1 = r->right;
                y0 = r->top;
                x0 = r->left;
                if (&screen == 0) {
                    Surface_004bf8c0 inner;
                    if (FUN_004c5e70(&inner)) {
                        if (FUN_004bea20(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        FUN_004c5fa0(&inner);
                    }
                } else {
                    if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1;
                y1 = r->bottom;
                x1 = r->right;
                y0 = r->top;
                x0 = r->right;
                if (&screen == 0) {
                    Surface_004bf8c0 inner;
                    if (FUN_004c5e70(&inner)) {
                        if (FUN_004bea20(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        FUN_004c5fa0(&inner);
                    }
                } else {
                    if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1;
                y1 = r->bottom;
                x1 = r->right;
                y0 = r->bottom;
                x0 = r->left;
                if (&screen == 0) {
                    Surface_004bf8c0 inner;
                    if (FUN_004c5e70(&inner)) {
                        if (FUN_004bea20(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        FUN_004c5fa0(&inner);
                    }
                } else {
                    if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1;
                y1 = r->bottom;
                x1 = r->left;
                y0 = r->top;
                x0 = r->left;
                if (&screen == 0) {
                    Surface_004bf8c0 inner;
                    if (FUN_004c5e70(&inner)) {
                        if (FUN_004bea20(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        FUN_004c5fa0(&inner);
                    }
                } else {
                    if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            FUN_004c5fa0(&screen);
        }
        return result;
    } else {
        {
            int x0, y0, x1, y1;
            y1 = r->top;
            x1 = r->right;
            y0 = r->top;
            x0 = r->left;
            if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
                FUN_004cc7ab(surface, x0, y0, x1, y1, color);
        }
        {
            int x0, y0, x1, y1;
            y1 = r->bottom;
            x1 = r->right;
            y0 = r->top;
            x0 = r->right;
            if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
                FUN_004cc7ab(surface, x0, y0, x1, y1, color);
        }
        FUN_004be950(surface, r->left, r->bottom, r->right, r->bottom, color);
        FUN_004be950(surface, r->left, r->top, r->left, r->bottom, color);
        return r->bottom;
    }
}

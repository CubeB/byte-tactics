// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Draws the outline of `r` (top, right, bottom, left edges in that order) into
// `surface`, or into the screen when `surface` is 0 (locked with
// LockScreen, unlocked with UnlockScreen). Each edge is a segment drawer of
// the same shape as DrawLine (clip with ClipLine, fill with
// FUN_004cc7ab); the screen path carries four hand-inlined copies of it whose
// null-surface lock arm survives because the compiler only tests the address
// of the local `screen`, exactly as in 0x4bf260.
//
// check.py prints MATCH (1172 bytes). The one source shape that carries it:
// each edge's four clipped coordinates must be assigned y1, x1, y0, x0 (the
// order 0x4bf260 documents), with `int x0, y0, x1, y1;` declared first and
// assigned on separate statements. Writing one initialising declaration
// (`int x0 = ...;`) instead compiles to exactly the same size but only 61.7
// percent: it changes which dead argument slot each coordinate lands in and
// the load order of the FUN_004cc7ab arguments.
//
// The surface != 0 path inlines only the first two edges (top, right) and
// emits real DrawLine calls for the last two, i.e. the compiler ran its
// inline expansion budget out after six copies; the two calls pass the
// segment helper's spilled parameter slots, not fresh r-> loads.
//
// Suspected original bug: the surface != 0 path returns an uninitialised
// `result`. Its tail loads [esp+0x7c] (the arg2 slot, last written with
// r->bottom by the right edge's inlined body) into eax, because `r` is dead
// after being copied to esi; the screen path returns the lock result from ebx.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor
};

struct Rect_004bf8c0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface* dst, int x0, int y0, int x1, int y1, int color);
int __stdcall DrawLine(Surface* surface, int x0, int y0, int x1, int y1,
                           int color);

// FUNCTION: 0x4bf8c0
int __stdcall DrawRectangle(Surface* surface, Rect_004bf8c0* r, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            {
                int x0, y0, x1, y1; y1 = r->top; x1 = r->right; y0 = r->top; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->top; x0 = r->right;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->bottom; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->left; y0 = r->top; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                }
            }
            UnlockScreen(&screen);
        }
    } else {
        {
            int x0, y0, x1, y1; y1 = r->top; x1 = r->right; y0 = r->top; x0 = r->left;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                FUN_004cc7ab(surface, x0, y0, x1, y1, color);
        }
        {
            int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->top; x0 = r->right;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                FUN_004cc7ab(surface, x0, y0, x1, y1, color);
        }
        DrawLine(surface, r->left, r->bottom, r->right, r->bottom, color);
        DrawLine(surface, r->left, r->top, r->left, r->bottom, color);
    }
    return result;
}

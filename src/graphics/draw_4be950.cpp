// Decompiled by space-bunny-free. Names are provisional.

// Draws into `surface`, or into the screen (locked with FUN_004c5e70 and
// unlocked with FUN_004c5fa0) when `surface` is null. FUN_004bea20 clips the
// rectangle and FUN_004cc7ab fills it. Returns the lock result on the screen
// path, so a failed lock returns 0 without ever unlocking.

struct Surface_004be950 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall FUN_004c5e70(Surface_004be950* out);
int __stdcall FUN_004c5fa0(Surface_004be950* s);
int __stdcall FUN_004bea20(Surface_004be950* dst, int* a, int* b, int* c, int* d);
void __cdecl FUN_004cc7ab(Surface_004be950* dst, int a, int b, int c, int d, int e);

// FUNCTION: 0x4be950
int __stdcall FUN_004be950(Surface_004be950* surface, int x0, int y0, int x1, int y1,
                           int color)
{
    int ret;
    if (surface == 0) {
        Surface_004be950 screen;
        ret = FUN_004c5e70(&screen);
        if (ret) {
            if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
            FUN_004c5fa0(&screen);
        }
    } else {
        if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
            FUN_004cc7ab(surface, x0, y0, x1, y1, color);
        ret = 1;
    }
    return ret;
}

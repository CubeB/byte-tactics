// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 38.2% (498 of 505 bytes). What still differs, in order of size:
//  1. Register rotation. The original loads the radius parameter (arg4) into ebx
//     before the ebp save and keeps the loop angle in ebp, keeps pos in edi and
//     y2 in esi, so the frame needs no spill. Mine puts the parameter in edi,
//     the angle in esi and creates two extra zero registers, which spills the
//     angle into arg5's slot and reloads both pos and rad at the loop head.
//     Swapping the declaration order of the locals moves the rotation around
//     (see build/scratch/0x438ea0) but never lands it on ebx/ebp/edi/esi.
//  2. Loop guard. The original emits i = 0, the n store and `cmp ecx, ebp; jl`
//     first and only then the 0x10000/n division, the << 16 and index *= 3, i.e.
//     its loop rotation sinks the whole preheader into the guard. Mine keeps the
//     preheader in front of the test and emits the test from the idiv flags.
//  3. Zero stores. The original writes lx = 0 then ly = 0 straight after the
//     fild, before the fmul; mine writes them after the idiv, ly first.
//  4. Point setup. Both inlined copies load pos->x, pos->y, pos->z; the original
//     loads pos->x, pos->z, pos->y and stores the two points in a different
//     order in the second copy (y, x, z against x, y, z).
// What is already right: the 0x2c frame, the 16.16 Pos_00438ea0 layout, every
// frame slot (ly +0x10, lx +0x14, rad +0x18, step +0x1c, n +0x20, p1 +0x24,
// p2 +0x30), the loop counter in the dead arg4 slot, index *= 3 in arg7's slot,
// the __max double call for the terrain height, and both draw calls.
// Draws a ring of n + 1 line segments around a 16.16 map position, where
// n = radius * pi / 4, and the label of the segment number index * 3 under it.
#include <stdlib.h>

// A 16.16 world position: the frac/whole halves share one dword, so the code
// adds whole values through *(int*)&frac and reads the whole part back.
struct Pos_00438ea0 {
    unsigned short x_frac;               // +0x0
    short x;                             // +0x2
    unsigned short y_frac;               // +0x4
    short y;                             // +0x6
    unsigned short z_frac;               // +0x8
    short z;                             // +0xa
};

struct View_00438ea0 {
    char unknown_0[0x2c];
    int scroll_x;                        // +0x2c
    int scroll_y;                        // +0x30
};

extern double DAT_004fd2b0;              // 6.28318530717958
extern double DAT_004fd2b8;              // 0.125

int __cdecl FUN_004b70ef(int angle, int radius);
int __cdecl FUN_004b7123(int angle, int radius);
int __stdcall FUN_00485070(Pos_00438ea0* pos);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);

// FUNCTION: 0x438ea0
void __stdcall FUN_00438ea0(void* surface, View_00438ea0* view, Pos_00438ea0* pos, int radius,
                            int color, const char* text, int index)
{
    int angle = 0;
    if (radius) {
        int n = (int)(radius * DAT_004fd2b0 * DAT_004fd2b8);
        int ly = 0;
        int lx = 0;
        int i = 0;
        int x2, y2;
        int rad = radius << 16;
        int step = 0x10000 / n;
        index *= 3;
        for (; i <= n; i++) {
            Pos_00438ea0 p1;
            *(int*)&p1.x_frac = *(int*)&pos->x_frac - (-FUN_004b70ef(angle, rad));
            *(int*)&p1.y_frac = *(int*)&pos->y_frac;
            *(int*)&p1.z_frac = *(int*)&pos->z_frac - (-FUN_004b7123(angle, rad));
            p1.y = __max(pos->y, FUN_00485070(&p1));
            angle += step;
            Pos_00438ea0 p2;
            *(int*)&p2.x_frac = *(int*)&pos->x_frac - (-FUN_004b70ef(angle, rad));
            *(int*)&p2.y_frac = *(int*)&pos->y_frac;
            *(int*)&p2.z_frac = *(int*)&pos->z_frac - (-FUN_004b7123(angle, rad));
            p2.y = __max(pos->y, FUN_00485070(&p2));
            int x1 = p1.x - view->scroll_x + 0x80;
            int y1 = p1.z - (p1.y >> 1) - view->scroll_y + 0x20;
            x2 = p2.x - view->scroll_x + 0x80;
            y2 = p2.z - (p2.y >> 1) - view->scroll_y + 0x20;
            FUN_004be950(surface, x1, y1, x2, y2, color);
            if (i == index) {
                lx = x2;
                ly = y2;
            }
        }
        if (text) {
            if (lx == 0 && ly == 0) {
                lx = x2;
                ly = y2;
            }
            FUN_004c14f0(surface, text, lx, ly + 4, -1);
        }
    }
}

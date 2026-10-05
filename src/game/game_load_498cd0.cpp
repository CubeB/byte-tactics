// Decompiled by Opus. Names are provisional.
// Converts a point by the view origin at +0x2c/+0x30 (less a 0x80 by 0x20
// border) and passes it on to FUN_00484b50.

struct Point_00498cd0 {
    int x;                             // +0x0
    int y;                             // +0x4
};

struct View_00498cd0 {
    char unknown_0[0x2c];
    int x;                             // +0x2c
    int y;                             // +0x30
};

void __stdcall FUN_00484b50(int x, int y, int param_3);

// FUNCTION: 0x498cd0
void __stdcall FUN_00498cd0(Point_00498cd0* p, View_00498cd0* view, int param_3)
{
    FUN_00484b50(p->x + view->x - 0x80, p->y + view->y - 0x20, param_3);
}

// Decompiled by Opus. Names are provisional.
// Point version of 0x44d310: whether (px, py) lies within the circle around
// (x, y).

class Class_0044d290 {
public:
    char unknown_0[0x8];
    short x;                           // +0x8
    short y;                           // +0xa
    char unknown_c[0x10 - 0xc];
    int radiusSq;                      // +0x10

    int FUN_0044d290(int px, int py);
};

// FUNCTION: 0x44d290
int Class_0044d290::FUN_0044d290(int px, int py)
{
    int dy = py - y;
    int dx = px - x;
    return dx * dx + dy * dy <= radiusSq;
}

// Decompiled by Opus. Names are provisional.
// Maps a (dx, dy) step to one of eight directions, or -1.
// FUNCTION: 0x40d9c0
int __stdcall DirectionFromStep(int dx, int dy)
{
    if (dx > 0) {
        if (dy == dx)
            return 5;
        if (dy == -dx)
            return 7;
        if (dy == 0)
            return 6;
    } else if (dx < 0) {
        if (dy == dx)
            return 1;
        if (dy == -dx)
            return 3;
        if (dy == 0)
            return 2;
    } else {
        if (dy > 0)
            return 4;
        if (dy < 0)
            return 0;
    }
    return -1;
}

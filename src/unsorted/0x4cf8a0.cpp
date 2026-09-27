// Decompiled by Opus. Names are provisional.
// Sound object (see 0x4cf0b0): releases finished buffers, then creates a new
// buffer set in the first free slot and starts it; frees the set again if
// starting fails. Returns 1 on success.
#include <windows.h>
#include <dsound.h>

class Class_004cf0b0 {
public:
    void FUN_004cf0b0();
};

class Class_004cf370 {
public:
    IDirectSoundBuffer** FUN_004cf370(int a, int b, int c, int d, int e);
};

class Class_004cf4d0 {
public:
    void FUN_004cf4d0(IDirectSoundBuffer** set);
};

class Class_004cf570 {
public:
    int FUN_004cf570(IDirectSoundBuffer** set, int a, int b);
};

class Class_004cf8a0 {
public:
    char unknown_0[0x1c4];
    IDirectSoundBuffer** sets[8];           // +0x1c4

    int FUN_004cf8a0(int a, int b, int c, int d, int e, int f, int g);
};

// FUNCTION: 0x4cf8a0
int Class_004cf8a0::FUN_004cf8a0(int a, int b, int c, int d, int e, int f, int g)
{
    ((Class_004cf0b0*)this)->FUN_004cf0b0();
    for (int i = 0; i < 8; i++) {
        if (sets[i] == 0) {
            sets[i] = ((Class_004cf370*)this)->FUN_004cf370(a, b, c, d, e);
            if (sets[i] == 0)
                return 0;
            if (((Class_004cf570*)this)->FUN_004cf570(sets[i], f, g) == 0) {
                ((Class_004cf4d0*)this)->FUN_004cf4d0(sets[i]);
                sets[i] = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

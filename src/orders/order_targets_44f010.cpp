// Decompiled by Haiku. Names are provisional.

struct Class_0044f010 {
    int vtable;           // +0x0
    int unknown_4;        // +0x4
    int unknown_8;        // +0x8
    char unknown_c[0x50]; // +0xc
    int unknown_5c;       // +0x5c
    int unknown_60;       // +0x60
    unsigned char unknown_64; // +0x64

    Class_0044f010(int param_1);
};

extern void* DAT_004fd458[];  // vtable

// FUNCTION: 0x44f010
Class_0044f010::Class_0044f010(int param_1)
{
    unsigned char dl = *(unsigned char*)(((char*)this) + 0x64);
    ((int*)this)[2] = param_1;    // +0x8
    dl = dl & 0xfc;
    dl = dl | 8;
    ((int*)this)[1] = 0;          // +0x4
    ((void**)this)[0] = DAT_004fd458; // vtable
    ((int*)this)[0x17] = 0;       // +0x5c
    *(unsigned char*)(((char*)this) + 0x64) = dl;
    ((int*)this)[0x18] = 0;       // +0x60
}

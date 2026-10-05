// Decompiled by Opus. Names are provisional.

class Class_00409160 {
public:
    char unknown_0[0x10d];
    Class_00409160(unsigned char player);
};

class Class_00409730 {
public:
    void FUN_00409730();
};

extern Class_00409160* DAT_005119c0[];

// FUNCTION: 0x40b320
void __stdcall FUN_0040b320(int player)
{
    Class_00409160*& slot = DAT_005119c0[player];
    slot = new Class_00409160(player);
    ((Class_00409730*)slot)->FUN_00409730();
}

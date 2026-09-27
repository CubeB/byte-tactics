// Decompiled by Haiku. Names are provisional.

extern double FUN_004e1730();

class Class_004e2160 {
public:
    double value;
    char unknown_8[0x40];
    char flag_48;

    void FUN_004e2160();
};

// FUNCTION: 0x4e2160
void Class_004e2160::FUN_004e2160()
{
    if (flag_48 != 0) {
        double fVar1 = FUN_004e1730();
        value = fVar1 - value;
        flag_48 = 0;
    }
}

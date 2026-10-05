// Decompiled by Haiku. Names are provisional.

class Class_004339c0 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int GetLosLineStepCount();
};

// FUNCTION: 0x4339c0
int Class_004339c0::GetLosLineStepCount()
{
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 2;
}

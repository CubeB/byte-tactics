// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fdc18;

class Class_004c3730 {
public:
    char unknown_0[0x10];
    void* field_10;

    void* FUN_004c3730();
};

// FUNCTION: 0x4c3730
void* Class_004c3730::FUN_004c3730()
{
    void* result = field_10;
    if (result == 0) {
        result = &DAT_004fdc18;
    }
    return result;
}

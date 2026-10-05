// Decompiled by Haiku. Names are provisional.

class Pathfinder {
public:
    char unknown_0[0x58];
    void* field_0x58;
    char unknown_1[0x08];
    void* field_0x64;

    void FUN_0040e9a0();
};

class MovementClass {
public:
    void RefreshUnitIfStale(int arg);
};

// FUNCTION: 0x40e9a0
void Pathfinder::FUN_0040e9a0()
{
    ((MovementClass*)field_0x64)->RefreshUnitIfStale((int)field_0x58);
    field_0x58 = 0;
    field_0x64 = 0;
}

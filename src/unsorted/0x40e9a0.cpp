// Decompiled by Haiku. Names are provisional.

class Class_0040e9a0 {
public:
    char unknown_0[0x58];
    void* field_0x58;
    char unknown_1[0x08];
    void* field_0x64;

    void FUN_0040e9a0();
};

class Dummy_00440be0 {
public:
    void FUN_00440be0(int arg);
};

// FUNCTION: 0x40e9a0
void Class_0040e9a0::FUN_0040e9a0()
{
    ((Dummy_00440be0*)field_0x64)->FUN_00440be0((int)field_0x58);
    field_0x58 = 0;
    field_0x64 = 0;
}

// Decompiled by Opus. Names are provisional.
// Looks up an entry and resets it the same way as 0x462470.

class Class_00461630 {
public:
    void* FUN_00461630(int param_1, int param_2);
};

class Class_00462470 {
public:
    int field_0;
    char unknown_4[0x18];
    int field_1c;
    char unknown_20[4];
    int field_24;

    void FUN_00461db0(int a1, int a2, int a3, int a4);
};

class Class_00461820 {
public:
    void FUN_00461820(int id);
};

// FUNCTION: 0x461820
void Class_00461820::FUN_00461820(int id)
{
    Class_00462470* e = (Class_00462470*)((Class_00461630*)this)->FUN_00461630(id, 0);
    if (e != 0) {
        e->FUN_00461db0(-1, 0xc8, 2, 0x64);
        e->field_0 = -1;
        e->field_1c = -1;
        e->field_24 = 0;
    }
}

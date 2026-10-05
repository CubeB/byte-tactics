// Decompiled by Opus. Names are provisional.
// Looks an entry up by name in the 32-entry table that 0x440290.cpp defines
// (the class declarations are copied from there; field_0 holds the name).
#include <string.h>

void __cdecl FUN_004d85a0(int* param_1);

struct MovementClass {
    int* field_0;
    short field_4;
    short field_6;
    short field_8;
    short field_a;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
    unsigned char field_f;
    int field_10;
    int field_14;
    void* field_18;
    int field_1c;

    MovementClass()
    {
        field_0 = 0;
        field_4 = 0;
        field_6 = 0;
        field_8 = 10000;
        field_a = -10000;
        field_c = 0xff;
        field_e = 0xff;
        field_d = 0xff;
        field_f = 0xff;
        field_10 = 0;
        field_14 = 0;
        field_18 = 0;
        field_1c = 0;
    }
};

struct MovementClassTable {
    MovementClass entries[32];

    ~MovementClassTable()
    {
        MovementClass* p = &entries[32];
        int n = 32;
        do {
            --p;
            FUN_004d85a0(p->field_0);
            operator delete(p->field_18);
        } while (--n);
    }

    static MovementClassTable g_movementClasses;
};

// FUNCTION: 0x440420
MovementClass* __stdcall FindMovementClass(char* name)
{
    for (int i = 0; i < 32; i++) {
        if (MovementClassTable::g_movementClasses.entries[i].field_0 != 0
            && _strcmpi((char*)MovementClassTable::g_movementClasses.entries[i].field_0, name) == 0) {
            return &MovementClassTable::g_movementClasses.entries[i];
        }
    }
    return 0;
}

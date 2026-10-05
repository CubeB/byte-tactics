// Decompiled by Opus. Names are provisional.
// A static data member holding 32 entries. 0x440230 is its compiler-generated
// initialiser (the entry constructor inlined as a loop), and this is the
// destructor it registers with atexit. MSVC guards the destructor of a static
// data member with a "$S" flag, which is the byte right after the table.

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

// FUNCTION: 0x440290 _$E3
MovementClassTable MovementClassTable::g_movementClasses;

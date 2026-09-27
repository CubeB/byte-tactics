// Decompiled by Sonnet. Names are provisional.

class Class_004c46c0 {
public:
    char unknown_0[0x21];
    int FUN_004c46c0(const char* name, int def);
};

struct StructA_004ada10 {
    char unknown_0[0xb6];
    short field_b6;
};

struct StructB_004ada10 {
    char unknown_0[4];
    Class_004c46c0* field_4;
};

// FUNCTION: 0x4ada10
void __stdcall FUN_004ada10(StructA_004ada10* a, StructB_004ada10* b)
{
    a->field_b6 = (short)b->field_4->FUN_004c46c0("status", 0);
}

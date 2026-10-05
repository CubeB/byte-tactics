// Decompiled by Sonnet. Names are provisional.

class Class_004c46c0 {
public:
    char unknown_0[0x21];
    int GetFieldInt(const char* name, int def);
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
void __stdcall ReadStatusField(StructA_004ada10* a, StructB_004ada10* b)
{
    a->field_b6 = (short)b->field_4->GetFieldInt("status", 0);
}

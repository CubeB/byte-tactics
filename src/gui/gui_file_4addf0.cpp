// Decompiled by Sonnet. Names are provisional.

class Class_004c46c0 {
public:
    int GetFieldInt(const char* name, int flag);
};

struct Param2_004addf0 {
    char unknown_0[4];
    Class_004c46c0* obj;
};

#pragma pack(push, 2)
struct Struct_004addf0 {
    char unknown_0[0xce];
    int field_ce;
    char unknown_d2[4];
    int field_d6;
    unsigned short field_da;
};
#pragma pack(pop)

// FUNCTION: 0x4addf0
void __stdcall ReadListBoxFields(Struct_004addf0* param1, Param2_004addf0* param2)
{
    param1->field_ce = 0;
    param1->field_d6 = 0;
    param1->field_da = (unsigned short)param2->obj->GetFieldInt("itemheight", 0);
}

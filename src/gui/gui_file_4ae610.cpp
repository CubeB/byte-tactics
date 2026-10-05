// Decompiled by Opus. Names are provisional.

class Class_004c46c0 {
public:
    int GetFieldInt(const char* name, int def);
};

#pragma pack(push, 1)
struct StructA_004ae610 {
    char unknown_0[0xb6];
    int field_b6;                      // +0xb6
};
#pragma pack(pop)

struct StructB_004ae610 {
    char unknown_0[4];
    Class_004c46c0* field_4;           // +0x4
};

// FUNCTION: 0x4ae610
void __stdcall ReadNuttinField(StructA_004ae610* a, StructB_004ae610* b)
{
    a->field_b6 = b->field_4->GetFieldInt("nuttin", 0);
}

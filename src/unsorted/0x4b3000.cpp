// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_004b3000 {
public:
    int field_0;
    unsigned char field_4;

    Class_004b3000* FUN_004b3000(int* param_1, unsigned char* param_2);
};
#pragma pack(pop)

// FUNCTION: 0x4b3000
Class_004b3000* Class_004b3000::FUN_004b3000(int* param_1, unsigned char* param_2)
{
    field_0 = *param_1;
    field_4 = *param_2;
    return this;
}

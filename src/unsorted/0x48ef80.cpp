// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_0048ef80
{
public:
    char unknown_0[0x24];
    short field_24;
    int field_26;

    bool FUN_0048ef80(int param_1);
};
#pragma pack(pop)

// FUNCTION: 0x48ef80
bool Class_0048ef80::FUN_0048ef80(int param_1)
{
    if (*(short*)((char*)param_1 + 0xa6) == field_24) {
        field_26++;
    }
    return field_26 <= 1;
}

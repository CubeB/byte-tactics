// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Class_0047cb40
{
    char unknown_0[0x6];
    int field_6;
    void FUN_0047cb40(int param_1);
};
#pragma pack(pop)

// FUNCTION: 0x47cb40
void Class_0047cb40::FUN_0047cb40(int param_1)
{
    int temp = field_6;
    *(int*)((char*)param_1 + 0x8e) = temp;
    field_6 = param_1;
}

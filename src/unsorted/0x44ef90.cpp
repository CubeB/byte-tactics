// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_44ef90
{
public:
    char unknown_0[0x4];
    void* ptr_at_0x4;

    void FUN_0044ef90(void* param);
};
#pragma pack(pop)

class Class_0044ced0 {
public:
    void FUN_0044ced0(int);
};

// FUNCTION: 0x44ef90
void Class_44ef90::FUN_0044ef90(void* param)
{
    if (ptr_at_0x4 != 0) {
        ((Class_0044ced0*)ptr_at_0x4)->FUN_0044ced0(0x80);
    }
    ptr_at_0x4 = param;
}

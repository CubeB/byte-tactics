// Decompiled by Haiku. Names are provisional.

extern char DAT_00502ae8[];

void FUN_004a0300(int param1, void* param2, char* param3);

struct Class_004ac080
{
public:
    int unknown_0[6];
    int* ptr_18;
    int unknown_1c[17];
    void* ptr_60;
};

// FUNCTION: 0x4ac080
void __stdcall FUN_004ac080(Class_004ac080* obj)
{
    FUN_004a0300(*(int*)(obj->ptr_18 + 1), obj->ptr_60, DAT_00502ae8);
}

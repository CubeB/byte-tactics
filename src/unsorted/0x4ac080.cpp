// Decompiled by Sonnet. Names are provisional.

extern char DAT_00502ae8[];

bool __stdcall FUN_004a0300(int param1, int param2, char* param3);

struct Class_004ac080
{
public:
    int unknown_0[6];
    int* ptr_18;
    int unknown_1c[17];
    int ptr_60;
};

// FUNCTION: 0x4ac080
void __stdcall FUN_004ac080(Class_004ac080* obj)
{
    FUN_004a0300(obj->ptr_18[1], obj->ptr_60, DAT_00502ae8);
}

// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

class Class_46e610
{
public:
    void FUN_0046e610();
};

// FUNCTION: 0x46e610
void Class_46e610::FUN_0046e610()
{
    int local_var;
    void* ptr = *(void**)((char*)this + 0x4);
    local_var = (int)ptr;
    delete ptr;
    *(int*)((char*)this + 0x4) = 0;
    *(int*)((char*)this + 0x8) = 0;
    *(int*)((char*)this + 0xc) = 0;
}

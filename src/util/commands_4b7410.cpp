// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

class Class_004b7410 {
public:
    float FUN_004b7410(int index, float default_val);
};

// FUNCTION: 0x4b7410
float Class_004b7410::FUN_004b7410(int index, float default_val)
{
    if (index < 0 || index >= *(int*)((char*)this + 0xd0)) {
        return default_val;
    }
    char* str = *(char**)((char*)this + index * 4);
    return (float)atof(str);
}

// Decompiled by Haiku. Names are provisional.

struct Class_004c91a0 {
public:
    void* field_0;
    
    void FUN_004c91a0(void** param_1);
};

// FUNCTION: 0x4c91a0
void Class_004c91a0::FUN_004c91a0(void** param_1)
{
    void* ptr = *param_1;
    field_0 = ptr;
    (*(int*)ptr)++;
    ((int*)ptr)[-1]++;
}

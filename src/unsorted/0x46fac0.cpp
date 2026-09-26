// Decompiled by Haiku. Names are provisional.

struct Class_0046fac0;

static inline void* __fastcall Helper(Class_0046fac0* obj, void** out)
{
    void* ptr = *(void**)obj;
    *out = ptr;
    return *(void**)ptr;
}

struct Class_0046fac0 {
public:
    void* field_0;

    void FUN_0046fac0(void** param_1, int unused);
};

// FUNCTION: 0x46fac0
void Class_0046fac0::FUN_0046fac0(void** param_1, int unused)
{
    field_0 = Helper(this, param_1);
}

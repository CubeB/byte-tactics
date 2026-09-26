// Decompiled by Haiku. Names are provisional.

class Dummy {
public:
    void *ptr;
};

// FUNCTION: 0x433da0
void __stdcall FUN_00433da0(void* param_1, int unused)
{
    delete (Dummy*)param_1;
}

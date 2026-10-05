// Decompiled by Haiku. Names are provisional.

class CMemoryCache {
public:
    void FUN_004379a0(void);
};

class Class_00458160 {
public:
    char unknown_0[0x10];
    int field_10;

    Class_00458160* FUN_00458160(void);
};

// FUNCTION: 0x458160
Class_00458160* Class_00458160::FUN_00458160(void)
{
    ((CMemoryCache*)this)->FUN_004379a0();
    field_10 = 0;
    return this;
}

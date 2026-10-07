// Decompiled by Haiku. Names are provisional.

class CMemoryCache {
public:
    void ClearPointers(void);
};

class Class_00458160 {
public:
    char unknown_0[0x10];
    int field_10;

    Class_00458160* Construct(void);
};

// FUNCTION: 0x458160
Class_00458160* Class_00458160::Construct(void)
{
    ((CMemoryCache*)this)->ClearPointers();
    field_10 = 0;
    return this;
}

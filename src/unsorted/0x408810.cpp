// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fc980[];

extern void operator delete(void* p);

class Class_00408810 {
public:
    void** vtable;

    void* FUN_00408810(unsigned char flag);
};

// FUNCTION: 0x408810
void* Class_00408810::FUN_00408810(unsigned char flag)
{
    vtable = DAT_004fc980;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

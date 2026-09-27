// Decompiled by Opus. Names are provisional.
// Scalar deleting destructor of the class whose vtable is at 0x4fc990 (its
// constructor is 0x4079a0); the inlined base destructor leaves only the base
// vtable store. Same shape as 0x407980.cpp and 0x408810.cpp.

extern void* DAT_004fc980[];

void operator delete(void* p);

class Class_004079d0 {
public:
    void** vtable;

    void* FUN_004079d0(unsigned char flag);
};

// FUNCTION: 0x4079d0
void* Class_004079d0::FUN_004079d0(unsigned char flag)
{
    vtable = DAT_004fc980;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

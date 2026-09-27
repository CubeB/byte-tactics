// Decompiled by Opus. Names are provisional.
// Scalar deleting destructor of Class_00407930 (slot 1 of its vtable at
// 0x4fc988); the inlined base destructor leaves only the base vtable store.
// Written with a hand-assigned vtable field like the identical 0x408810.cpp,
// because the base vtable's slot 0 is the free function FUN_00407380.

extern void* DAT_004fc980[];

void operator delete(void* p);

class Class_00407930 {
public:
    void** vtable;

    void* FUN_00407980(unsigned char flag);
};

// FUNCTION: 0x407980
void* Class_00407930::FUN_00407980(unsigned char flag)
{
    vtable = DAT_004fc980;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

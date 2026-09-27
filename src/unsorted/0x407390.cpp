// Decompiled by Sonnet. Names are provisional.
// A "manual vtable" object like src/unsorted/0x408810.cpp and 0x407350.cpp:
// the vtable slot is a plain data member assigned from the shared table
// DAT_004fc980, not a real C++ virtual table.

extern void* DAT_004fc980[];

extern void operator delete(void* p);

class Class_00407390 {
public:
    void** vtable;

    void* FUN_00407390(unsigned char flag);
};

// FUNCTION: 0x407390
void* Class_00407390::FUN_00407390(unsigned char flag)
{
    vtable = DAT_004fc980;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

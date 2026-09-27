// Decompiled by Sonnet. Names are provisional.
// A "manual vtable" object like src/unsorted/0x408810.cpp: the vtable slot
// is a plain data member assigned from the shared table DAT_004fc980, not a
// real C++ virtual table, so it must be named the same as that other file.

extern void* DAT_004fc980[];

class Class_00407350 {
public:
    void** vtable;                       // +0x0
    int field4;                          // +0x4
    int field8;                          // +0x8
    int fieldc;                          // +0xc
    int field10;                         // +0x10

    Class_00407350(int param1, int param2);
};

// FUNCTION: 0x407350
Class_00407350::Class_00407350(int param1, int param2)
{
    field4 = param1;
    field8 = param2;
    fieldc = 0;
    field10 = *(unsigned char*)(param1 + 4);
    vtable = DAT_004fc980;
}

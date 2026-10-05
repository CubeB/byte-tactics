// Decompiled by Sonnet, Haiku and Opus. Names are provisional.
// Class_004b0610: an abstract base class, 0x540 bytes, vtable 0x4fdb00 with
// 21 slots. Slots 0-6 are pure; each is named after the one override that
// fills it, in the vtable of the only derived class (0x4fd698, see
// src/units/units_485e30.cpp). Slots 7-13 are defined in
// src/units/cob_4b1e50.cpp, slots 14-19 below, and slot 20 is the virtual
// destructor (the vtable holds its scalar deleting destructor).

struct Elem_4b0610 {
    int value;         // +0x0
    char pad[0xa0];    // pad to stride 0xa4
};

class Class_004b0610 {
public:
    int field_4;                   // +0x4
    int field_8;                   // +0x8
    char unknown_c[0x10 - 0xc];
    void* ptr10;                   // +0x10
    void* ptr14;                   // +0x14
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];            // +0x1c
    int field_53c;                 // +0x53c

    Class_004b0610();

    virtual void FUN_00480c50(int, int, int) = 0;     // slot 0
    virtual void FUN_00480ce0(int, int, int) = 0;     // slot 1
    virtual void FUN_00480d50(int, int) = 0;          // slot 2
    virtual void FUN_00480db0(int, int) = 0;          // slot 3
    virtual void FUN_00480df0(int, int) = 0;          // slot 4
    virtual int FUN_00480c30(int, int) = 0;           // slot 5
    virtual int FUN_00480cb0(int, int) = 0;           // slot 6
    virtual int FUN_004b1e50(int);                    // slot 7
    virtual int FUN_004b1e60(int);                    // slot 8
    virtual int FUN_004b1e70(int);                    // slot 9
    virtual void FUN_004b1e80(int, int, int);         // slot 10
    virtual void FUN_004b1e90(int);                   // slot 11
    virtual void FUN_004b1ea0(int, int);              // slot 12
    virtual void FUN_004b1eb0(int, unsigned int);     // slot 13
    virtual void FUN_004b0650(unsigned short, int, int); // slot 14
    virtual void FUN_004b0660(unsigned short);        // slot 15
    virtual void FUN_004b0670(int, int);              // slot 16
    virtual int FUN_004b0680(int, int, int, int, int); // slot 17
    virtual int FUN_004b0690(int);                    // slot 18
    virtual int FUN_004b06a0();                       // slot 19
    virtual ~Class_004b0610();                        // slot 20
};

extern int FUN_004b6330();
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4b0610
Class_004b0610::Class_004b0610()
{
    field_8 = 0;
    ptr14 = 0;
    ptr10 = 0;
    for (int i = 0; i < 8; i++) {
        arr[i].value = 0;
    }
    field_53c = 0;
    field_4 = FUN_004b6330();
}

// FUNCTION: 0x4b0650
void Class_004b0610::FUN_004b0650(unsigned short, int, int)
{
}

// FUNCTION: 0x4b0660
void Class_004b0610::FUN_004b0660(unsigned short)
{
}

// FUNCTION: 0x4b0670
void Class_004b0610::FUN_004b0670(int, int)
{
}

// FUNCTION: 0x4b0680
int Class_004b0610::FUN_004b0680(int, int, int, int, int)
{
    return 0;
}

// FUNCTION: 0x4b0690
int Class_004b0610::FUN_004b0690(int)
{
    return 0;
}

// FUNCTION: 0x4b06a0
int Class_004b0610::FUN_004b06a0()
{
    return 0;
}

// The compiler-generated scalar deleting destructor (0x4b06b0, vtable slot
// 20) comes from the destructor definition below, with its body inlined.
// FUNCTION: 0x4b06b0 ??_GClass_004b0610@@UAEPAXI@Z
// FUNCTION: 0x4b06f0
Class_004b0610::~Class_004b0610()
{
    if (ptr14) {
        FUN_004d85a0((int*)ptr14);
    }
    if (ptr10) {
        FUN_004d85a0((int*)ptr10);
    }
}

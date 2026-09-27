// Decompiled by Haiku and Opus. Names are provisional.
// Vtable slots 7-13 of Class_004b0610; the class, its constructor, slots
// 14-19 and its destructor are in src/unsorted/0x4b0610.cpp.

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
    virtual void FUN_004b1eb0(int, int);              // slot 13
    virtual void FUN_004b0650(int, int, int);         // slot 14
    virtual void FUN_004b0660(int);                   // slot 15
    virtual void FUN_004b0670(int, int);              // slot 16
    virtual int FUN_004b0680(int, int, int, int, int); // slot 17
    virtual int FUN_004b0690(int);                    // slot 18
    virtual int FUN_004b06a0();                       // slot 19
    virtual ~Class_004b0610();                        // slot 20
};

// FUNCTION: 0x4b1e50
int Class_004b0610::FUN_004b1e50(int)
{
    return 0;
}

// FUNCTION: 0x4b1e60
int Class_004b0610::FUN_004b1e60(int)
{
    return 0;
}

// FUNCTION: 0x4b1e70
int Class_004b0610::FUN_004b1e70(int)
{
    return 0;
}

// FUNCTION: 0x4b1e80
void Class_004b0610::FUN_004b1e80(int, int, int)
{
}

// FUNCTION: 0x4b1e90
void Class_004b0610::FUN_004b1e90(int)
{
}

// FUNCTION: 0x4b1ea0
void Class_004b0610::FUN_004b1ea0(int, int)
{
}

// FUNCTION: 0x4b1eb0
void Class_004b0610::FUN_004b1eb0(int, int)
{
}

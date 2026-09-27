// Decompiled by Opus. Names are provisional.
// Slot 4 of Class_00485e30 (see 0x485e30.cpp); compare 0x480d50.

#pragma pack(push, 1)
struct Entry_00480df0 {
    short value;                       // +0x0
    unsigned short flag0 : 1;          // +0x2 bit 0
    unsigned short flag1 : 1;          // +0x2 bit 1
    unsigned short flag2 : 1;          // +0x2 bit 2
    char unknown_4[0x36 - 0x4];
};

struct Data_00480df0 {
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    char unknown_8[0x10 - 0x8];
    int field_10;                      // +0x10
    char unknown_14[0x48 - 0x14];
    Entry_00480df0 entries[1];         // +0x48
};
#pragma pack(pop)

class Class_00485e30 {
public:
    char unknown_0[0x540];
    Data_00480df0* data;               // +0x540

    void FUN_00480df0(int index, int flag);
};

// FUNCTION: 0x480df0
void Class_00485e30::FUN_00480df0(int index, int flag)
{
    data->entries[index].flag2 = flag;
    data->field_10 = 0;
}

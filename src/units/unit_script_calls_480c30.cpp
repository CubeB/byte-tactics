// Decompiled by Opus. Names are provisional.
// Returns one of the three ints of an entry in the table at +0x540. The
// original splits the address as `lea [.. + 0x22]` then `[.. + 4]`, so the
// entry array starts at +0x22 with the ints at +4 (0x480c50 reads the same
// ints as +0x16 / +0x10, which folds to the same address there).

#pragma pack(push, 1)
struct Entry_00480c30 {
    int unknown_0;                     // +0x0
    int values[3];                     // +0x4
    char unknown_10[0x36 - 0x10];
};

struct Data_00480c30 {
    char unknown_0[0x22];
    Entry_00480c30 entries[1];         // +0x22
};
#pragma pack(pop)

class Class_00480c30 {
public:
    char unknown_0[0x540];
    Data_00480c30* data;               // +0x540

    int FUN_00480c30(int index, int slot);
};

// FUNCTION: 0x480c30
int Class_00480c30::FUN_00480c30(int index, int slot)
{
    return data->entries[index].values[slot];
}

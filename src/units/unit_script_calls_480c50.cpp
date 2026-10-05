// Decompiled by Opus. Names are provisional.
#include <string.h>   // only for the header state: flips base/index at +0x4a

#pragma pack(push, 1)
struct Entry_00480c50 {
    char unknown_0[0x10];
    int values[3];                     // +0x10
    unsigned short shorts[11];         // +0x1c
    short value;                       // +0x32
    unsigned short flag0 : 1;          // +0x34 bit 0
    unsigned short flag1 : 1;          // +0x34 bit 1
};

struct Data_00480c50 {
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    int dirty;                         // +0x8
    char unknown_c[0x16 - 0xc];
    Entry_00480c50 entries[1];         // +0x16
};
#pragma pack(pop)

class UnitScript {
public:
    char unknown_0[0x540];
    Data_00480c50* data;               // +0x540

    void SetPieceTranslation(int index, int slot, int v);
};

// FUNCTION: 0x480c50
void UnitScript::SetPieceTranslation(int index, int slot, int v)
{
    if (data->entries[index].values[slot] != v) {
        data->entries[index].values[slot] = v;
        data->entries[index].value = 0;
        data->dirty = 1;
        if (data->entries[index].flag1) {
            data->unknown_4 = 0;
        }
    }
}

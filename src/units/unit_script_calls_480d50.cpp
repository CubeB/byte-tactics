// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_00480d50 {
    short value;                       // +0x0
    unsigned short flag0 : 1;          // +0x2 bit 0
    unsigned short flag1 : 1;          // +0x2 bit 1
    char unknown_4[0x36 - 0x4];
};

struct Data_00480d50 {
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    char unknown_8[0x48 - 0x8];
    Entry_00480d50 entries[1];         // +0x48
};
#pragma pack(pop)

class UnitScript {
public:
    char unknown_0[0x540];
    Data_00480d50* data;               // +0x540

    void SetPieceVisible(int index, int flag);
};

// FUNCTION: 0x480d50
void UnitScript::SetPieceVisible(int index, int flag)
{
    if (data->entries[index].flag0 != flag) {
        data->entries[index].flag0 = flag;
        data->entries[index].value = 0;
        if (data->entries[index].flag1) {
            data->unknown_4 = 0;
        }
    }
}

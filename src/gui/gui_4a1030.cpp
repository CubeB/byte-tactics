// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_4a1030 {
    unsigned char type;              // +0x00
    char unknown_1[0x137 - 0x1];
    char f_137;                      // +0x137
    char unknown_138[0x15b - 0x138];
};
#pragma pack(pop)

struct Holder_4a1030 {
    char unknown_0[4];
    Entry_4a1030* entries;           // +0x04
};

struct Class_004a1030 {
    char unknown_0[0x18];
    Holder_4a1030* holder;           // +0x18
};

// FUNCTION: 0x4a1030
int __stdcall SetButtonStage(Class_004a1030* obj, int index, char value)
{
    Entry_4a1030* entries = obj->holder->entries;
    if (entries[index].type == 1) {
        entries[index].f_137 = value;
        return 1;
    }
    return 0;
}

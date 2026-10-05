// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_4a1200 {
    char unknown_0[0x13c];
    unsigned short flag : 1;         // +0x13c bit 0
    unsigned short unknown_13c_1 : 15;
    char unknown_13e[0x15b - 0x13e];
};
#pragma pack(pop)

struct Holder_4a1200 {
    char unknown_0[4];
    Entry_4a1200* entries;           // +0x04
};

#pragma pack(push, 1)
struct Dialog {
    char unknown_0[0x18];
    Holder_4a1200* holder;           // +0x18
    char unknown_1c[0xcca - 0x1c];
    int dirty;                       // +0xcca
};
#pragma pack(pop)

static inline Entry_4a1200* GetEntries(Dialog* obj)
{
    return obj->holder->entries;
}

// FUNCTION: 0x4a1200
void __stdcall FUN_004a1200(Dialog* obj, int index, int value)
{
    GetEntries(obj)[index].flag = value;
    obj->dirty = 1;
}

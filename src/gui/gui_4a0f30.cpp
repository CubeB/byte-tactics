// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_4a0f30 {
    char unknown_0[0x138];
    short f_138;                     // +0x138
    char unknown_13a[0x15b - 0x13a];
};
#pragma pack(pop)

struct Holder_4a0f30 {
    char unknown_0[4];
    Entry_4a0f30* entries;           // +0x04
};

struct Dialog {
    char unknown_0[0x18];
    Holder_4a0f30* holder;           // +0x18
};

static inline Entry_4a0f30* GetEntries(Dialog* obj)
{
    return obj->holder->entries;
}

// FUNCTION: 0x4a0f30
int __stdcall GetGadgetStatus(Dialog* obj, int index)
{
    return GetEntries(obj)[index].f_138;
}

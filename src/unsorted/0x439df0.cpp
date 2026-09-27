// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Entry {
    int value;
    char unknown_4[0x19 - 4];
};
#pragma pack(pop)

struct Sub {
    char unknown_0[4];
    unsigned char index; // +4
};

struct Obj {
    char unknown_0[0x5c];
    Sub* sub; // +0x5c
};

extern Entry* DAT_00512344;

// FUNCTION: 0x439df0
int __stdcall FUN_00439df0(Obj* obj)
{
    if (obj && obj->sub) {
        return DAT_00512344[obj->sub->index].value;
    }
    return DAT_00512344[0].value;
}

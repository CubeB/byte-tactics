// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049fc50 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0xb6 - 0x1];
    char text[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Holder_0049fc50 {
    int unknown_0;
    Entry_0049fc50* entries;           // +0x4
};

struct Object_0049fc50 {
    char unknown_0[0x18];
    Holder_0049fc50* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
    char unknown_68[0x74 - 0x68];
    int length;                        // +0x74
};

// FUNCTION: 0x49fc50
int __stdcall FUN_0049fc50(Object_0049fc50* obj, int index)
{
    if (obj->focus != -1 && obj->holder->entries[obj->focus].type == 3) {
        obj->focus = -1;
    }
    if (obj->focus != -1 && obj->focus != index) {
        return 0;
    }
    obj->focus = index;
    if (index != -1 && obj->holder->entries[index].type == 3) {
        obj->length = strlen(obj->holder->entries[index].text);
    }
    return 1;
}

// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a08f0 {                // 0x15b bytes
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    unsigned char count;               // +0x136
    char unknown_137[0x15b - 0x137];
};
#pragma pack(pop)

struct Holder_004a08f0 {
    char unknown_0[4];
    Entry_004a08f0* entries;           // +0x4
};

struct Object_004a08f0 {
    char unknown_0[0x18];
    Holder_004a08f0* holder;           // +0x18
};

char* __stdcall Translate(char* text);

static inline Entry_004a08f0* GetEntry(Object_004a08f0* obj, int index)
{
    return obj->holder->entries + index;
}

// FUNCTION: 0x4a08f0
void __stdcall FUN_004a08f0(Object_004a08f0* obj, int index)
{
    Entry_004a08f0* entry = GetEntry(obj, index);
    char temp[0x80];
    char* dst = temp;
    char* src = entry->text;
    int i = 0;
    while (i < entry->count) {
        strcpy(dst, Translate(src));
        dst += strlen(dst) + 1;
        src += strlen(src) + 1;
        i++;
    }
    memcpy(entry->text, temp, 0x80);
}

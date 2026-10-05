// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a5d50 {
    unsigned char type;                // +0x00
    char unknown_1;
    char name[0x10];                   // +0x02
    char unknown_12[0x17 - 0x12];
    short height;                      // +0x17
    char unknown_19[0x1b - 0x19];
    int flags;                         // +0x1b
    char unknown_1f[0x15b - 0x1f];
};

struct Data_004a5d50 {
    int unknown_0;                     // +0x00
    Entry_004a5d50* entries;           // +0x04
};

struct Menu_004a5d50 {
    char unknown_0[8];
    int values[3];                     // +0x08
    int current;                       // +0x14
    Data_004a5d50* data;               // +0x18
};

struct Font_004a5d50 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a5d50* font;               // +0x14
};
#pragma pack(pop)

extern Class_0051fba4* DAT_0051fba4;

char* __stdcall FUN_004a0d00(Menu_004a5d50* menu, char* name, char* buf);
void* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int param_1, unsigned char* text);

static inline int Width_004a5d50(char* text)
{
    int w = 0;
    if (DAT_0051fba4->font == 0)
        return GetTextWidth(GetFont(), (unsigned char*)text);
    for (char* p = text; *p; p++) {
        unsigned char c = *p;
        unsigned short* g = (unsigned short*)GetGafFrame(DAT_0051fba4->font->glyphs, c);
        if (g)
            w += *g;
    }
    return w;
}

// FUNCTION: 0x4a5d50
int __stdcall FUN_004a5d50(Menu_004a5d50* menu, int index)
{
    Entry_004a5d50* entries = menu->data->entries;
    char* text = FUN_004a0d00(menu, entries[index].name, 0);
    if (text == 0)
        return 0;
    if (entries[index].type == 5 ||
        (entries[index].type == 1 && (entries[index].flags & 0x8000) != 0))
        menu->current = menu->values[1];
    while (1) {
        int w = Width_004a5d50(text);
        if (w <= entries[index].height - 6) {
            menu->current = menu->values[0];
            return w;
        }
        if (strlen(text) == 0)
            continue;
        text[strlen(text) - 1] = 0;
    }
}

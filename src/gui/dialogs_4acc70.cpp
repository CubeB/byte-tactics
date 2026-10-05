// Decompiled by Opus. Names are provisional.
// Copies three slider values into one palette entry and applies it.

struct Color_004acc70 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char flags;
};

struct Slider_004acc70 {
    char unknown_0[0x140];
    unsigned char value;               // +0x140
};

#pragma pack(push, 1)
struct Class_004acc70 {
    char unknown_0[0x9b2];
    int index;                         // +0x9b2
    char unknown_9b6[0xcb6 - 0x9b6];
    Slider_004acc70* red;              // +0xcb6
    Slider_004acc70* green;            // +0xcba
    Slider_004acc70* blue;             // +0xcbe
};
#pragma pack(pop)

void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);

// FUNCTION: 0x4acc70
void __stdcall ApplySlidersToPaletteEntry(Class_004acc70* obj, Color_004acc70* palette)
{
    Color_004acc70 c;
    c.r = obj->red->value;
    c.g = obj->green->value;
    c.b = obj->blue->value;
    palette[obj->index] = c;
    SetPaletteColors((unsigned char*)&c, obj->index, 1);
}

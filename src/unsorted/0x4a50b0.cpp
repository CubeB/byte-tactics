// Decompiled by Opus. Names are provisional.
// Line height of the current font: the height of the 'I' glyph plus 2, or
// FUN_004c1450's value when no font is loaded.

struct Glyph_004a50b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50b0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a50b0* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void* __stdcall FUN_004b7f30(void* a, int b);
int FUN_004c1450();

// FUNCTION: 0x4a50b0
int FUN_004a50b0()
{
    if (DAT_0051fba4->font == 0) {
        return FUN_004c1450();
    }
    return ((Glyph_004a50b0*)FUN_004b7f30(DAT_0051fba4->font->glyphs, 'I'))->height + 2;
}

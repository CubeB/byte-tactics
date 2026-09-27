// Decompiled by Opus. Names are provisional.
// Width of a string in pixels: the sum of the glyph widths of the current
// font, or FUN_004c1480's measurement when no font is loaded. The loop has to
// index the string (text[i]) with the character in its own local: walking the
// pointer lets MSVC reuse the loop test's load, and without the local the
// character is widened in a register instead of through its stack slot.

struct Font_004a5030 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a5030* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void* __stdcall FUN_004b7f30(void* a, int b);
int FUN_004c1440();
int __stdcall FUN_004c1480(int param_1, unsigned char* text);

// FUNCTION: 0x4a5030
int __stdcall FUN_004a5030(unsigned char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (DAT_0051fba4->font == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    for (int i = 0; text[i]; i++) {
        unsigned char c = text[i];
        unsigned short* glyph = (unsigned short*)FUN_004b7f30(DAT_0051fba4->font->glyphs, c);
        if (glyph)
            width += *glyph;
    }
    return width;
}

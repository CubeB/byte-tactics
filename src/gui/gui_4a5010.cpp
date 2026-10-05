// Decompiled by Opus. Names are provisional.
// The glyph of a character in the current font (see 0x4a5030).

struct Font_004a5010 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a5010* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void* __stdcall GetGafFrame(void* a, int b);

// FUNCTION: 0x4a5010
void* __stdcall FUN_004a5010(unsigned char c)
{
    return GetGafFrame(DAT_0051fba4->font->glyphs, c);
}

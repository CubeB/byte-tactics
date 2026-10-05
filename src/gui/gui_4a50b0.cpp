// Decompiled by Opus. Names are provisional.
// Line height of the current font: the height of the 'I' glyph plus 2, or
// GetFontHeight's value when no font is loaded.

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

extern Class_0051fba4* g_guiContext;

void* __stdcall GetGafFrame(void* a, int b);
int GetFontHeight();

// FUNCTION: 0x4a50b0
int GetFontLineHeight()
{
    if (g_guiContext->font == 0) {
        return GetFontHeight();
    }
    return ((Glyph_004a50b0*)GetGafFrame(g_guiContext->font->glyphs, 'I'))->height + 2;
}

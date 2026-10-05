// Decompiled by Opus. Names are provisional.
// Width of a string in pixels: the sum of the glyph widths of the current
// font, or GetTextWidth's measurement when no font is loaded. The loop has to
// index the string (text[i]) with the character in its own local: walking the
// pointer lets MSVC reuse the loop test's load, and without the local the
// character is widened in a register instead of through its stack slot.

struct Font_004a5030 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog {
    char unknown_0[0x14];
    Font_004a5030* font;               // +0x14
};

extern Dialog* g_guiContext;

void* __stdcall GetGafFrame(void* a, int b);
int GetFont();
int __stdcall GetTextWidth(int param_1, unsigned char* text);

// FUNCTION: 0x4a5030
int __stdcall GetTextPixelWidth(unsigned char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (g_guiContext->font == 0)
        return GetTextWidth(GetFont(), text);
    for (int i = 0; text[i]; i++) {
        unsigned char c = text[i];
        unsigned short* glyph = (unsigned short*)GetGafFrame(g_guiContext->font->glyphs, c);
        if (glyph)
            width += *glyph;
    }
    return width;
}

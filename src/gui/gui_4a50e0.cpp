// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Glyph_004a50e0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50e0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a50e0* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* glyph, int x, int y);
void __stdcall DrawFrameLit(void* surface, void* glyph, int x, int y, int style);
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxWidth);

// FUNCTION: 0x4a50e0
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style)
{
    if (DAT_0051fba4->font == 0) {
        DrawString(surface, text, x, y, -1);
        return;
    }
    unsigned char* s = (unsigned char*)text;
    while (*s) {
        if (*s >= ' ') {
            unsigned char c = *s;
            Glyph_004a50e0* g = (Glyph_004a50e0*)GetGafFrame(DAT_0051fba4->font->glyphs, c);
            if (g) {
                if (maxw != -1 && (int)g->width > maxw)
                    return;
                if (*s != ' ') {
                    if (style == 0)
                        DrawFrame(surface, g, x, y);
                    else
                        DrawFrameLit(surface, g, x, y, style);
                }
                if (maxw != -1) {
                    maxw -= g->width;
                    if (maxw < 0)
                        return;
                }
                x += g->width;
            }
        }
        s++;
    }
}

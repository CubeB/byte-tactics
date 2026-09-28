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

void* __stdcall FUN_004b7f30(void* glyphs, int c);
void __stdcall FUN_004b7f90(void* surface, void* glyph, int x, int y);
void __stdcall FUN_004b8310(void* surface, void* glyph, int x, int y, int style);
void __stdcall FUN_004c14f0(void* surface, char* text, int x, int y, int maxWidth);

// FUNCTION: 0x4a50e0
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style)
{
    if (DAT_0051fba4->font == 0) {
        FUN_004c14f0(surface, text, x, y, -1);
        return;
    }
    unsigned char* s = (unsigned char*)text;
    while (*s) {
        if (*s >= ' ') {
            unsigned char c = *s;
            Glyph_004a50e0* g = (Glyph_004a50e0*)FUN_004b7f30(DAT_0051fba4->font->glyphs, c);
            if (g) {
                if (maxw != -1 && (int)g->width > maxw)
                    return;
                if (*s != ' ') {
                    if (style == 0)
                        FUN_004b7f90(surface, g, x, y);
                    else
                        FUN_004b8310(surface, g, x, y, style);
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

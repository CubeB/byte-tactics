// Decompiled by space-bunny-free. Names are provisional.
// Draws `text` horizontally centred, four times in colour `fore` one pixel to
// each side and once on top in colour `back`: an outlined label. Each group of
// draws first calls the setter FUN_004c13a0 with the getter FUN_004c13f0's value
// (both inlined here), so field_0x210 is copied into field_0x20c each time and
// only the two setters that are not -1 are stored. With a null `dst` the screen
// rect is locked with FUN_004c5e70 for the five draws and unlocked with
// FUN_004c5fa0 afterwards.
// The glyph width has to be read through a `glyphs` byte array declared at
// offset 0 of the font struct, and the font has to arrive as a typed field of a
// typed game struct: reading it as `((unsigned char*)font)[off]` off a font
// pointer loaded from `game + 0x204` gives the SIB byte with the base and index
// registers swapped (`[ecx+edx]` instead of `[edx+ecx]`), and no header set or
// spelling of the cast fixes it. See 0x4c1760.cpp for the same wall.

struct Font_004c1830 {
    unsigned char glyphs[1];            // +0x0, first byte of a glyph is its width
    char unknown_1[2];
    unsigned char first;                // +0x3, first character with a glyph
    unsigned short offsets[1];          // +0x4, glyph offsets from the font start
};

struct Game_004c1830 {
    char unknown_0[0x204];
    Font_004c1830* font;                // +0x204
};

struct Rect_004c1830 {
    int data[12];
};

int FUN_004b6220(void);
int __stdcall FUN_004c5e70(Rect_004c1830* out);
int __stdcall FUN_004c5fa0(Rect_004c1830* buf);
int __stdcall FUN_004c14f0(Rect_004c1830* dst, unsigned char* text, int x,
                           int a, int b);

// FUN_004c13f0, inlined: the current value of field_0x210.
static inline int CurrentColour(void)
{
    return *(int*)((unsigned char*)FUN_004b6220() + 0x210);
}

// FUN_004c13a0, inlined: field_0x208 then field_0x20c, each only if not -1.
static inline void SetColour(int a, int b)
{
    unsigned char* obj = (unsigned char*)FUN_004b6220();
    if (a != -1) {
        *(int*)(obj + 0x208) = a;
    }
    if (b != -1) {
        *(int*)(obj + 0x20c) = b;
    }
}

// Width in pixels of a line of text in a bitmap font (FUN_004c1480, inlined).
static inline int WidthText(Font_004c1830* font, unsigned char* text)
{
    int width = 0;
    if (text && font) {
        for (; *text && *text != '\n'; text++) {
            unsigned char c = *text;
            if (c >= font->first) {
                int d = c - font->first;
                unsigned int off = 0;
                off = font->offsets[(unsigned short)d];
                if (off)
                    width += font->glyphs[off];
            }
        }
    }
    return width;
}

// FUNCTION: 0x4c1830
void __stdcall FUN_004c1830(Rect_004c1830* dst, unsigned char* text, int fore,
                            int back, int y)
{
    Game_004c1830* game = (Game_004c1830*)FUN_004b6220();
    int width = WidthText(game->font, text);
    if (dst == 0) {
        Rect_004c1830 r;
        if (FUN_004c5e70(&r) != 0) {
            int x = (r.data[0] - width) >> 1;
            SetColour(fore, CurrentColour());
            FUN_004c14f0(&r, text, x - 1, y, -1);
            FUN_004c14f0(&r, text, x + 1, y, -1);
            FUN_004c14f0(&r, text, x, y - 1, -1);
            FUN_004c14f0(&r, text, x, y + 1, -1);
            SetColour(back, CurrentColour());
            FUN_004c14f0(&r, text, x, y, -1);
            FUN_004c5fa0(&r);
            return;
        }
    } else {
        int x = (dst->data[0] - width) >> 1;
        SetColour(fore, CurrentColour());
        FUN_004c14f0(dst, text, x - 1, y, -1);
        FUN_004c14f0(dst, text, x + 1, y, -1);
        FUN_004c14f0(dst, text, x, y - 1, -1);
        FUN_004c14f0(dst, text, x, y + 1, -1);
        SetColour(back, CurrentColour());
        FUN_004c14f0(dst, text, x, y, -1);
    }
}

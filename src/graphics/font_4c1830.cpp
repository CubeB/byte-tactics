// Decompiled by space-bunny-free. Names are provisional.
// Draws `text` horizontally centred, four times in colour `fore` one pixel to
// each side and once on top in colour `back`: an outlined label. Each group of
// draws first calls the setter SetTextColors with the getter GetTextKeyColor's value
// (both inlined here), so field_0x210 is copied into field_0x20c each time and
// only the two setters that are not -1 are stored. With a null `dst` the screen
// rect is locked with LockScreen for the five draws and unlocked with
// UnlockScreen afterwards.
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

struct Surface {
    int data[12];
};

int GetDisplay(void);
int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* buf);
int __stdcall DrawString(Surface* dst, unsigned char* text, int x,
                           int a, int b);

// GetTextKeyColor, inlined: the current value of field_0x210.
static inline int CurrentColour(void)
{
    return *(int*)((unsigned char*)GetDisplay() + 0x210);
}

// SetTextColors, inlined: field_0x208 then field_0x20c, each only if not -1.
static inline void SetColour(int a, int b)
{
    unsigned char* obj = (unsigned char*)GetDisplay();
    if (a != -1) {
        *(int*)(obj + 0x208) = a;
    }
    if (b != -1) {
        *(int*)(obj + 0x20c) = b;
    }
}

// Width in pixels of a line of text in a bitmap font (GetTextWidth, inlined).
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
void __stdcall DrawOutlinedString(Surface* dst, unsigned char* text, int fore,
                            int back, int y)
{
    Game_004c1830* game = (Game_004c1830*)GetDisplay();
    int width = WidthText(game->font, text);
    if (dst == 0) {
        Surface r;
        if (LockScreen(&r) != 0) {
            int x = (r.data[0] - width) >> 1;
            SetColour(fore, CurrentColour());
            DrawString(&r, text, x - 1, y, -1);
            DrawString(&r, text, x + 1, y, -1);
            DrawString(&r, text, x, y - 1, -1);
            DrawString(&r, text, x, y + 1, -1);
            SetColour(back, CurrentColour());
            DrawString(&r, text, x, y, -1);
            UnlockScreen(&r);
            return;
        }
    } else {
        int x = (dst->data[0] - width) >> 1;
        SetColour(fore, CurrentColour());
        DrawString(dst, text, x - 1, y, -1);
        DrawString(dst, text, x + 1, y, -1);
        DrawString(dst, text, x, y - 1, -1);
        DrawString(dst, text, x, y + 1, -1);
        SetColour(back, CurrentColour());
        DrawString(dst, text, x, y, -1);
    }
}

// Decompiled by Opus. Names are provisional.
// Width in pixels of a line of text in a bitmap font: the sum of the widths
// (first byte of each glyph) of the characters up to the end of the string
// or the first newline. Characters below the font's first character, or
// without a glyph, count as zero.
// The difference must be computed as an int in its own statement: casting
// it straight to unsigned short makes MSVC do the arithmetic in 16 bits.

struct Font_004c1480 {
    char unknown_0[3];
    unsigned char first;               // +0x3, first character with a glyph
    unsigned short offsets[1];         // +0x4, glyph offsets from the font start
};

// FUNCTION: 0x4c1480
int __stdcall FUN_004c1480(Font_004c1480* font, unsigned char* text)
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
                    width += ((unsigned char*)font)[off];
            }
        }
    }
    return width;
}

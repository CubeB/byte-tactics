// Decompiled by deepseek-v4.1-flash, finished by Claude Sonnet 5.5. Names are provisional.
// Draws `text` horizontally centred: it sums the glyph widths from the font at
// singleton+0x204 (the body of GetTextWidth, inlined) and passes
// (rect.width - width) / 2 to DrawString. With a null `dst` it locks the
// screen rect with LockScreen and unlocks it with UnlockScreen afterwards.
//
// MATCH (201 of 201 bytes, Claude Sonnet 5.5 #694). The SIB base/index swap at
// 0x4c17ba (`mov al, [edx + ecx]` against `[ecx + edx]`) was not compiler state
// and not the glyph access: it comes from how `font` reaches the inlined width
// loop. Written as `int font = GetDisplay(); font = *(int*)(font + 0x204);`
// (the earlier form), or as a Font* local, or as one nested expression, the
// register roles come out swapped (98.8, 51.2 and 48.8 percent). With the
// singleton held as a `char*` local and the font read as
// `*(Font_004c1760**)(single + 0x204)` passed straight into the helper, the
// original's encoding appears. The glyph access itself is unchanged
// (`((unsigned char*)font)[off]`; integer-add, swapped-operand and
// `off[(unsigned char*)font]` spellings all give the same bytes as it).
// So a SIB swap can come from the way an inlined helper's pointer argument is
// produced, not only from the expression that uses it.

struct Font_004c1760 {
    char unknown_0[3];
    unsigned char first;               // +0x3, first character with a glyph
    unsigned short offsets[1];         // +0x4, glyph offsets from the font start
};

struct Surface {
    int data[12];
};

int GetDisplay(void);
int __stdcall LockScreen(Surface* out);
int __stdcall DrawString(Surface* dst, unsigned char* text, int x,
                           int a, int b);
int __stdcall UnlockScreen(Surface* buf);

// Width in pixels of a line of text in a bitmap font (GetTextWidth, inlined).
static inline int WidthText(Font_004c1760* font, unsigned char* text)
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

// FUNCTION: 0x4c1760
void __stdcall DrawStringCentered(int* dst, unsigned char* text, int flag)
{
    char* single = (char*)GetDisplay();
    int width = WidthText(*(Font_004c1760**)(single + 0x204), text);
    if (dst == 0) {
        Surface r;
        if (LockScreen(&r) != 0) {
            DrawString(&r, text, (r.data[0] - width) >> 1, flag, -1);
            UnlockScreen(&r);
        }
    } else {
        DrawString((Surface*)dst, text, (*dst - width) >> 1, flag, -1);
    }
}

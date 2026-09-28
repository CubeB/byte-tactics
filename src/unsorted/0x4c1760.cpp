// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws `text` horizontally centred: it sums the glyph widths from the font at
// singleton+0x204 (the body of FUN_004c1480, inlined) and passes
// (rect.width - width) / 2 to FUN_004c14f0. With a null `dst` it locks the
// screen rect with FUN_004c5e70 and unlocks it with FUN_004c5fa0 afterwards.
//
// STILL DIFFERS (98.8%): the glyph-width byte load at 0x4c17ba is
// `mov al, [edx + ecx]` in the original but `mov al, [ecx + edx]` here: the
// same address with the SIB base and index registers swapped (font is edx and
// the glyph offset ecx in both). The out-of-line sibling 0x4c1480 matches with
// this encoding, and every spelling of the `((unsigned char*)font)[off]`
// access, every header set (tools/headers.py and --cpp) and 0 to 2048 extra
// declarations leave the swap unchanged, so it is likely compiler state from
// the original translation unit.

struct Font_004c1760 {
    char unknown_0[3];
    unsigned char first;               // +0x3, first character with a glyph
    unsigned short offsets[1];         // +0x4, glyph offsets from the font start
};

struct Rect_004c1760 {
    int data[12];
};

int FUN_004b6220(void);
int __stdcall FUN_004c5e70(Rect_004c1760* out);
int __stdcall FUN_004c14f0(Rect_004c1760* dst, unsigned char* text, int x,
                           int a, int b);
int __stdcall FUN_004c5fa0(Rect_004c1760* buf);

// Width in pixels of a line of text in a bitmap font (FUN_004c1480, inlined).
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
void __stdcall FUN_004c1760(int* dst, unsigned char* text, int flag)
{
    int font = FUN_004b6220();
    font = *(int*)(font + 0x204);
    int width = WidthText((Font_004c1760*)font, text);
    if (dst == 0) {
        Rect_004c1760 r;
        if (FUN_004c5e70(&r) != 0) {
            FUN_004c14f0(&r, text, (r.data[0] - width) >> 1, flag, -1);
            FUN_004c5fa0(&r);
        }
    } else {
        FUN_004c14f0((Rect_004c1760*)dst, text, (*dst - width) >> 1, flag, -1);
    }
}

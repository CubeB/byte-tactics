// Decompiled by Opus, Sonnet 5.5, space-bunny-free, muse-spark-1.3-free, deepseek-v4.1-flash and Claude Sonnet 5.5. Names are provisional.

#include <string.h>

#include "surface.h"

struct Font {
    unsigned char glyphs[1];            // +0x0, first byte of a glyph is its width
    char unknown_1[2];
    unsigned char first;                // +0x3, first character with a glyph
    unsigned short offsets[1];          // +0x4, glyph offsets from the font start
};

struct Game {
    char unknown_0[0x204];
    Font* font;                         // +0x204
    int colour1;                        // +0x208
    int colour2;                        // +0x20c
    int colour3;                        // +0x210
};

Game* GetDisplay(void);
int __stdcall RectInsideRect(Rect* a, Rect* b);
int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
void __cdecl BlitText(unsigned char* pixels, int pitch, Font* font, unsigned char* text,
                          int x, int y, int c1, int c2, int c3);
void __stdcall DrawString(Surface* dst, unsigned char* text, int x, int y, int maxWidth);

// Width in pixels of a line of text in a bitmap font: the sum of the widths
// (first byte of each glyph) of the characters up to the end of the string
// or the first newline. Characters below the font's first character, or
// without a glyph, count as zero.
// FUNCTION: 0x4c1480
int __stdcall GetTextWidth(Font* font, unsigned char* text)
{
    int width = 0;
    if (text && font) {
        for (; *text && *text != '\n'; text++) {
            unsigned char c = *text;
            if (c >= font->first) {
                // Own int statement: casting the difference to unsigned short makes it 16-bit.
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

static inline int WidthTextGlyphs(Font* font, unsigned char* text)
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

// Draws `text` (a bitmap font string ended by NUL or newline) at x, y into
// `dst`, or into the locked screen when `dst` is null. When `maxWidth` is not
// -1 and the text is wider, a copy is cut back one character at a time until
// it fits. The text's rectangle is checked against the surface's clip rect
// with RectInsideRect and only then drawn by the glyph blitter BlitText,
// which takes the surface's pixels and pitch, the font, the text, the
// position and the game's three colour fields at +0x208, +0x20c, +0x210.
// FUNCTION: 0x4c14f0
void __stdcall DrawString(Surface* dst, unsigned char* text, int x, int y, int maxWidth)
{
    Game* game = GetDisplay();
    int width = WidthTextGlyphs(game->font, text);
    if (maxWidth != -1 && width > maxWidth) {
        unsigned char buf[0x12c];
        strncpy((char*)buf, (char*)text, 0x12b);
        int len = strlen((char*)buf);
        // Written into the parameter, no local copy: the store lands in its dead slot.
        text = buf;
        unsigned char* end = buf + len - 1;
        while (1) {
            *end = 0;
            if (end == buf)
                break;
            end--;
            width = WidthTextGlyphs(game->font, text);
            if (width <= maxWidth)
                break;
        }
    }
    Rect r;
    r.left = x;
    r.top = y;
    r.right = x + width;
    // From r.top, not y: r is address-taken, so the original reloads it.
    r.bottom = r.top + GetDisplay()->font->glyphs[0];
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen) != 0) {
            Rect clip;
            screen.GetClipRect(&clip);
            if (RectInsideRect(&r, &clip))
                BlitText(screen.pixels, screen.pitch, game->font, text, x, y, game->colour1,
                             game->colour2, game->colour3);
            UnlockScreen(&screen);
        }
    } else {
        Rect clip;
        dst->GetClipRect(&clip);
        if (RectInsideRect(&r, &clip))
            BlitText(dst->pixels, dst->pitch, game->font, text, x, y, game->colour1,
                         game->colour2, game->colour3);
    }
}

// Width in pixels of a line of text in a bitmap font (GetTextWidth, inlined).
static inline int WidthTextCast(Font* font, unsigned char* text)
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

// Draws `text` horizontally centred: it sums the glyph widths from the font at
// singleton+0x204 and passes (rect.width - width) / 2 to DrawString. With a
// null `dst` it locks the screen rect with LockScreen and unlocks it with
// UnlockScreen afterwards.
// FUNCTION: 0x4c1760
void __stdcall DrawStringCentered(Surface* dst, unsigned char* text, int flag)
{
    // Singleton held as char*, font read at +0x204 and passed straight in: sets the SIB order.
    char* single = (char*)GetDisplay();
    int width = WidthTextCast(*(Font**)(single + 0x204), text);
    if (dst == 0) {
        Surface r;
        if (LockScreen(&r) != 0) {
            DrawString(&r, text, (r.width - width) >> 1, flag, -1);
            UnlockScreen(&r);
        }
    } else {
        DrawString(dst, text, (dst->width - width) >> 1, flag, -1);
    }
}

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

// Draws `text` horizontally centred, four times in colour `fore` one pixel to
// each side and once on top in colour `back`: an outlined label. Each group of
// draws first calls the setter SetTextColors with the getter GetTextKeyColor's
// value, so field_0x210 is copied into field_0x20c each time and only the two
// setters that are not -1 are stored. With a null `dst` the screen
// rect is locked with LockScreen for the five draws and unlocked with
// UnlockScreen afterwards.
// FUNCTION: 0x4c1830
void __stdcall DrawOutlinedString(Surface* dst, unsigned char* text, int fore,
                            int back, int y)
{
    // Font arrives as a typed field read through `glyphs`: a cast form swaps base and index.
    Game* game = (Game*)GetDisplay();
    int width = WidthTextGlyphs(game->font, text);
    if (dst == 0) {
        Surface r;
        if (LockScreen(&r) != 0) {
            int x = (r.width - width) >> 1;
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
        int x = (dst->width - width) >> 1;
        SetColour(fore, CurrentColour());
        DrawString(dst, text, x - 1, y, -1);
        DrawString(dst, text, x + 1, y, -1);
        DrawString(dst, text, x, y - 1, -1);
        DrawString(dst, text, x, y + 1, -1);
        SetColour(back, CurrentColour());
        DrawString(dst, text, x, y, -1);
    }
}

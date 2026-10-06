// Decompiled by Opus, Sonnet 5.5, space-bunny-free, muse-spark-1.3-free, deepseek-v4.1-flash and Claude Sonnet 5.5. Names are provisional.

#include <string.h>

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

struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    int unknown_0[2];
    int pitch;                          // +0x8
    unsigned char* pixels;              // +0xc
    int unknown_10[8];                  // 0x30 bytes, the lock descriptor
    Rect* GetClipRect(Rect* out);
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
// The difference must be computed as an int in its own statement: casting
// it straight to unsigned short makes MSVC do the arithmetic in 16 bits.
// FUNCTION: 0x4c1480
int __stdcall GetTextWidth(Font* font, unsigned char* text)
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
//
// MATCH. The two changes that got here from the previous 94.5% (608 of 617
// bytes) version were both needed and neither was enough on its own:
//
//  1. No local text pointer at all. The truncated copy is written straight
//     back into the `text` PARAMETER (`text = buf;`) and the draw passes
//     `text`, which is what the original's `mov [esp+0x19c], edx` at
//     0x4c15a1 and its read-back at 0x4c16be and 0x4c1730 are. Every earlier
//     attempt kept a separate local `unsigned char* t` and scored 608 of 617
//     bytes: that forced a fresh frame slot, an extra `mov [esp+X], ebp` sunk
//     between `cmp eax,-1` and its `je` (0x4c156b), and a 0x188 frame against
//     the original's 0x184. With no local the store lands in the dead
//     parameter slot and the frame is 0x184.
//  2. `r.bottom = r.top + height`, not `r.bottom = y + height`. `r` is
//     address-taken (it is passed to RectInsideRect), so writing the second
//     rectangle field from the first makes MSVC reload it after the
//     GetDisplay() call: that is the original's `mov ecx, [esp+0x1c]` /
//     `add edx, ecx` at 0x4c165b and 0x4c165f, where reading `y` kept it live
//     in a register instead and also left the tail rotating y and dst into
//     edi and ebx rather than ebp and edi.
//
// Point 1 alone (with `r.bottom = y + ...`) is 615 of 617 bytes at 59.6%, and
// point 2 alone is 612 of 617 bytes at 70.3%, so the earlier conclusion that
// no combination of the two existed was wrong: the two are independent.
//
// Dead ends, all of which are still in the history of this file and none of
// which is worth repeating: a local `t` in any form (94.5% at best, 608 of 617
// bytes) and the four spellings that put a local `t` and a parameter store
// together (MSVC forwards the copy into the parameter, so all four collapse to
// the 615-byte form A); declaring `width` first with a separate assignment or
// with uninitialised declarations assigned in order; an extra live local
// holding `width`; a separate `keep` local for the rect; reordering the four
// rect stores including bottom-first; `r.right = width + x`; a brace
// initialiser for the rect; `if (!dst)` and `dst != 0` with the arms swapped;
// passing r.left/r.top to the blitter instead of x/y; a separate `clip`
// variable hoisted out of the screen branch; the height into a local;
// `game->font` instead of a second GetDisplay() call for the height;
// `const unsigned char*` helper parameters; an explicit walker
// `unsigned char* s` with `c = s[1]` or `c = *(++s)`; a separate `p`
// induction variable in the helper; `char* t` instead of `unsigned char* t`
// (byte-identical to the unsigned form); a local `t` aliased against the
// parameter; a sweep of 0 to 400 unused `extern int` declarations, which
// showed the extra slot was not compiler state; and tools/headers.py, which
// found no header set that matched.
// FUNCTION: 0x4c14f0
void __stdcall DrawString(Surface* dst, unsigned char* text, int x, int y, int maxWidth)
{
    Game* game = GetDisplay();
    int width = WidthTextGlyphs(game->font, text);
    if (maxWidth != -1 && width > maxWidth) {
        unsigned char buf[0x12c];
        strncpy((char*)buf, (char*)text, 0x12b);
        int len = strlen((char*)buf);
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
// FUNCTION: 0x4c1760
void __stdcall DrawStringCentered(Surface* dst, unsigned char* text, int flag)
{
    char* single = (char*)GetDisplay();
    int width = WidthTextCast(*(Font**)(single + 0x204), text);
    if (dst == 0) {
        Surface r;
        if (LockScreen(&r) != 0) {
            DrawString(&r, text, (r.unknown_0[0] - width) >> 1, flag, -1);
            UnlockScreen(&r);
        }
    } else {
        DrawString(dst, text, (dst->unknown_0[0] - width) >> 1, flag, -1);
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
// spelling of the cast fixes it. See DrawStringCentered for the same wall.
// FUNCTION: 0x4c1830
void __stdcall DrawOutlinedString(Surface* dst, unsigned char* text, int fore,
                            int back, int y)
{
    Game* game = (Game*)GetDisplay();
    int width = WidthTextGlyphs(game->font, text);
    if (dst == 0) {
        Surface r;
        if (LockScreen(&r) != 0) {
            int x = (r.unknown_0[0] - width) >> 1;
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
        int x = (dst->unknown_0[0] - width) >> 1;
        SetColour(fore, CurrentColour());
        DrawString(dst, text, x - 1, y, -1);
        DrawString(dst, text, x + 1, y, -1);
        DrawString(dst, text, x, y - 1, -1);
        DrawString(dst, text, x, y + 1, -1);
        SetColour(back, CurrentColour());
        DrawString(dst, text, x, y, -1);
    }
}

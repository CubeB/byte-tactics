// Decompiled by Sonnet 5.5, finished by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash. Names are provisional.
// Draws `text` (a bitmap font string ended by NUL or newline) at x, y into
// `dst`, or into the locked screen when `dst` is null. When `maxWidth` is not
// -1 and the text is wider, a copy is cut back one character at a time until
// it fits. The text's rectangle is checked against the surface's clip rect
// with FUN_004b6750 and only then drawn by the glyph blitter BlitText,
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
//     address-taken (it is passed to FUN_004b6750), so writing the second
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
#include <string.h>

struct Font_004c14f0 {
    unsigned char glyphs[1];            // +0x0, first byte of a glyph is its width
    char unknown_1[2];
    unsigned char first;                // +0x3, first character with a glyph
    unsigned short offsets[1];          // +0x4, glyph offsets from the font start
};

struct Game_004c14f0 {
    char unknown_0[0x204];
    Font_004c14f0* font;                // +0x204
    int colour1;                        // +0x208
    int colour2;                        // +0x20c
    int colour3;                        // +0x210
};

struct Rect_004c14f0 {
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

    Rect_004c14f0* GetClipRect(Rect_004c14f0* out);
};

Game_004c14f0* GetDisplay(void);
int __stdcall FUN_004b6750(Rect_004c14f0* a, Rect_004c14f0* b);
int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
void __cdecl BlitText(unsigned char* pixels, int pitch, Font_004c14f0* font, unsigned char* text,
                          int x, int y, int c1, int c2, int c3);

static inline int WidthText(Font_004c14f0* font, unsigned char* text)
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

// FUNCTION: 0x4c14f0
void __stdcall DrawString(Surface* dst, unsigned char* text, int x, int y, int maxWidth)
{
    Game_004c14f0* game = GetDisplay();
    int width = WidthText(game->font, text);
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
            width = WidthText(game->font, text);
            if (width <= maxWidth)
                break;
        }
    }
    Rect_004c14f0 r;
    r.left = x;
    r.top = y;
    r.right = x + width;
    r.bottom = r.top + GetDisplay()->font->glyphs[0];
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen) != 0) {
            Rect_004c14f0 clip;
            screen.GetClipRect(&clip);
            if (FUN_004b6750(&r, &clip))
                BlitText(screen.pixels, screen.pitch, game->font, text, x, y, game->colour1,
                             game->colour2, game->colour3);
            UnlockScreen(&screen);
        }
    } else {
        Rect_004c14f0 clip;
        dst->GetClipRect(&clip);
        if (FUN_004b6750(&r, &clip))
            BlitText(dst->pixels, dst->pitch, game->font, text, x, y, game->colour1,
                         game->colour2, game->colour3);
    }
}

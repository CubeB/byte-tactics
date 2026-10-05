// Decompiled by GPT-5.6-Terra, finished by GPT-6 and deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// MATCH 100% (556 bytes), found by deepseek-v4.1-flash.
//
// The whole residual at 86.8% was the live zero register in edi. The original
// materialises a 32-bit zero at the prologue (`xor edi,edi`) and keeps it
// live: `cmp ebx,edi` for the surface null test, `cmp [esi+0xd2],edi` for the
// showText test, `mov [esp+0x38],edi` for the width initial value and
// `cmp [ecx+0x14],edi` for the first language test. edi is then reused as the
// text pointer and reloaded from the width home after the loop. Writing the
// width measurement inline never forms that register: MSVC emits `test ebx,ebx`
// and immediate-zero stores. Moving the measurement into a `static inline`
// helper (exactly the shape of Measure_004a4d70 in 0x4a4d70) makes the inliner
// hoist the helper's `int width = 0` zero into edi at the function prologue and
// reuse it for every later zero, which also pushes the rect-inset temporary off
// edi onto edx/ecx. One source shape explains every hunk.
//
// Measured dead ends (all scratch, free --sym): inline measurement with width
// declared/initialised at every position, `char *p = 0` or `int zero = 0` at
// function scope (constant-propagated away), reversing every comparison,
// `while`/`for` loop forms and `char *q` loop pointer: all 86.8% or worse.
#include <stdlib.h>
#include <windows.h>

#pragma pack(push, 1)
struct Entry_004a4660 {
    unsigned char type;
    char unknown_01[0x13 - 1];
    short x;
    short y;
    short w;
    short h;
    char unknown_1b[0x1f - 0x1b];
    int color1;
    int color2;
    char unknown_27[0xba - 0x27];
    int number;
    char unknown_be[0xd2 - 0xbe];
    int showText;
    char unknown_d6[0x15b - 0xd6];
};
#pragma pack(pop)

struct Holder_004a4660 {
    char unknown_0[4];
    Entry_004a4660 *entries;
};

struct Class_004a4660 {
    char unknown_0[0xc];
    void *surface;
    int unknown_10;
    void *oldSurface;
    Holder_004a4660 *holder;
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char color1;
    char unknown_8b3[0x8c3 - 0x8b3];
    unsigned char color2;
    char unknown_8c4[2];
    unsigned char color3;
    char unknown_8c7[0xcd2 - 0x8c7];
    void *fallbackSurface;
};

struct Rect_004a4660 { int left, top, right, bottom; };
struct Glyph_004a4660 { unsigned short width, height; };
struct Language_004a4660 { char unknown_0[0xc]; unsigned short *glyphs; };
struct LanguageRoot_004a4660 { char unknown_0[0x14]; Language_004a4660 *language; };

extern LanguageRoot_004a4660 *DAT_0051fba4;
void __stdcall LockScreen(void *);
void __stdcall FUN_004b04b0(void *, Rect_004a4660 *, unsigned int, unsigned int, unsigned int);
void __stdcall FillRectangle(void *, Rect_004a4660 *, int);
int __stdcall FUN_004a50e0(void *, char *, int, int, int, int);
int __stdcall GetGafFrame(unsigned short *, int);
int GetFont();
int __stdcall GetTextWidth(int, char *);
int GetFontHeight();
void __stdcall UnlockScreen(void *);


static inline int Measure_004a4660(char *text)
{
    int width = 0;
    char *p = text;
    if (p == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return GetTextWidth(GetFont(), text);
    char *q = text;
    while (*q != 0) {
        char ch = *q;
        Glyph_004a4660 *glyph = (Glyph_004a4660 *)GetGafFrame(
            DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4660
void __stdcall FUN_004a4660(Class_004a4660 *obj, int index)
{
    Entry_004a4660 *entries = obj->holder->entries;
    Entry_004a4660 *entry = (Entry_004a4660 *)((char *)entries + index * 0x15b);
    obj->oldSurface = obj->surface;

    void *surface = *(void **)((char *)entries + 0xbc);
    if (surface == 0)
        surface = *(void **)((char *)obj + 0xcd2);
    LockScreen(surface);

    Rect_004a4660 rect;
    rect.left = entry->x;
    rect.top = entry->y;
    rect.right = entry->w + entry->x;
    rect.bottom = entry->h + entry->y;
    FUN_004b04b0(surface, &rect, obj->color1, obj->color2, obj->color3);

    rect.left += 2;
    rect.top += 2;
    rect.right -= 2;
    rect.bottom -= 2;
    FillRectangle(surface, &rect, *(int *)((char *)entry + 0x23));

    float scale = (float)*(int *)((char *)entry + 0xba) / *(int *)((char *)entry + 0xb6);
    rect.right = (int)(scale * (entry->w - 4)) + rect.left;
    FillRectangle(surface, &rect, *(int *)((char *)entry + 0x1f));

    if (entry->showText != 0) {
        char text[20];
        _itoa(entry->number, text, 10);
        int width = Measure_004a4660(text);
        int height;
        if (DAT_0051fba4->language == 0) {
            height = GetFontHeight();
        } else {
            Glyph_004a4660 *glyph = (Glyph_004a4660 *)GetGafFrame(
                DAT_0051fba4->language->glyphs, 0x49);
            height = glyph->height + 2;
        }
        FUN_004a50e0(surface, text,
            (entry->w / 2 - width / 2) + entry->x,
            (entry->h / 2 - height / 2) + entry->y, -1, 0);
    }

    obj->oldSurface = *(void **)((char *)obj + 8);
    UnlockScreen(surface);
}

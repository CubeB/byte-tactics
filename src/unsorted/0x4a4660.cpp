// Decompiled by GPT-5.6-Terra, finished by GPT-6 and deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// Partial at 86.8% (562 bytes against 556).
// Retry: the live zero register needed for the original comparisons and width initialization remains unmatched.
//
// space-bunny-free pass (third): the edi zero is a promoted CONSTANT node, not a
// variable, so the "is the zero dynamic" test comes out negative here. With
// `int width = 0;` at function entry MSVC 5 emits `mov dword ptr [esp+0x10],0`
// in the prologue, grows the frame by a dword (sub esp,0x28) and still writes
// `test ebx,ebx` for the surface null test: 68.9%. A local initialised to 0 is
// NOT unified with the literal 0 by the value tracker, so the original cannot be
// comparing against a named zero variable. Also ruled out: the source position
// of the init. The loop reads and writes [esp+0x38] (`mov eax,[esp+0x38]; add
// eax,edx; mov [esp+0x38],eax`), so the `width = 0` store at 0x4a4797 is live
// and has to be at that source position, which means the edi zero has been live
// in a register since the prologue: it is the constant 0 promoted to a register
// with its def hoisted into the entry block. Counting literal-0 uses in both
// versions gives the same seven (surface null, showText, width init, language
// null twice, p != 0, push 0), so "more uses" is not the lever either, which
// matches the guide's unsolved 0x450240 note: a whole-function constant only
// takes the registers the variables leave free.
//
// deepseek-v4.1-flash pass: the whole remaining diff is one allocator decision.
// The original materialises a 32-bit zero in edi at the prologue (`xor edi,edi`)
// and keeps it live: `cmp ebx,edi` for the surface null test, `cmp
// [esi+0xd2],edi` for the showText test, `mov [esp+0x38],edi` for the width
// initial value and `cmp [ecx+0x14],edi` for the first language test. That edi
// is then reused as the text pointer and restored from the width home after the
// loop (`lea edi,[esp+0x20]`, `mov edi,[esp+0x38]`). Ours never forms the zero
// register: it emits `test ebx,ebx`, `mov eax,[esi+0xd2]; test eax,eax` and
// `mov dword ptr [esp+0x38],0`. Because edi is free here, the allocator then
// uses it as the rect-inset temporary (`mov edi,[esp+0x14]; add edi,eax`),
// where the original uses edx/ecx, so the same single decision explains every
// hunk.
//
// Tried and measured (all scratch): width declared at function scope before
// surface 80.0%, right after surface 80.0%, right after FUN_004c5e70 80.0%,
// right before the showText test 81.1%, function-scope `int width;` assigned
// after _itoa 86.8% (identical output), `int width = 0` plus a top-level
// `char *p` 80.0%, and `headers.py 0x4a4660` (all 128 sets, best 86.8%). The
// 0x4a3ef0 notes say a zero local declared right after a call is what makes
// MSVC keep the zero, but none of those placements did here.
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
void __stdcall FUN_004c5e70(void *);
void __stdcall FUN_004b04b0(void *, Rect_004a4660 *, unsigned int, unsigned int, unsigned int);
void __stdcall FUN_004bf6f0(void *, Rect_004a4660 *, int);
int __stdcall FUN_004a50e0(void *, char *, int, int, int, int);
int __stdcall FUN_004b7f30(unsigned short *, int);
int FUN_004c1440();
int __stdcall FUN_004c1480(int, char *);
int FUN_004c1450();
void __stdcall FUN_004c5fa0(void *);

// FUNCTION: 0x4a4660
void __stdcall FUN_004a4660(Class_004a4660 *obj, int index)
{
    Entry_004a4660 *entries = obj->holder->entries;
    Entry_004a4660 *entry = (Entry_004a4660 *)((char *)entries + index * 0x15b);
    obj->oldSurface = obj->surface;

    void *surface = *(void **)((char *)entries + 0xbc);
    if (surface == 0)
        surface = *(void **)((char *)obj + 0xcd2);
    FUN_004c5e70(surface);

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
    FUN_004bf6f0(surface, &rect, *(int *)((char *)entry + 0x23));

    float scale = (float)*(int *)((char *)entry + 0xba) / *(int *)((char *)entry + 0xb6);
    rect.right = (int)(scale * (entry->w - 4)) + rect.left;
    FUN_004bf6f0(surface, &rect, *(int *)((char *)entry + 0x1f));

    if (entry->showText != 0) {
        char text[20];
        _itoa(entry->number, text, 10);
        int width = 0;
        char *p = text;
        if (p != 0) {
            if (DAT_0051fba4->language == 0) {
                width = FUN_004c1480(FUN_004c1440(), text);
            } else {
                while (*p != 0) {
                    char ch = *p;
                    Glyph_004a4660 *glyph = (Glyph_004a4660 *)FUN_004b7f30(
                        DAT_0051fba4->language->glyphs, (unsigned char)ch);
                    if (glyph != 0) width += glyph->width;
                    ++p;
                }
            }
        }
        int height;
        if (DAT_0051fba4->language == 0) {
            height = FUN_004c1450();
        } else {
            Glyph_004a4660 *glyph = (Glyph_004a4660 *)FUN_004b7f30(
                DAT_0051fba4->language->glyphs, 0x49);
            height = glyph->height + 2;
        }
        FUN_004a50e0(surface, text,
            (entry->w / 2 - width / 2) + entry->x,
            (entry->h / 2 - height / 2) + entry->y, -1, 0);
    }

    obj->oldSurface = *(void **)((char *)obj + 8);
    FUN_004c5fa0(surface);
}

// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, gave up: best 21.1% (2160 original bytes, 1839 ours) after 2 scoring
// runs (20.8 baseline, then <windows.h> plus the surface/fallback split below).
// Timebox (900 s) hit; this is a structural transcription, not a finished match.
//
// Second pass by deepseek-v4.1-flash found the original's early block is
// `void* surface = holder->surface; if (!surface) surface = obj->fallback;
//  if (!surface) { if (!(holder->flags10 & 0x80)) FUN_004b0230(...); }
//  else FUN_004c6d20(...);`
// which reproduces the otherwise dead `test eax,eax / je` at 0x4a1bee (eax is
// provably 0 there). It is worth only +0.1%; the match is blocked by the
// register rotation across the whole function (below), not by this block.
//
// Draws one GUI entry (0x15b stride) of a dialog panel. Two mutually exclusive
// renderers:
//   - flags bit 0x10 set (and entry text + field_c0 non zero): the text-line
//     renderer. It picks the entry of type 7 whose tab (+0x28) matches, runs
//     FUN_004b6af0 over the entry text to break lines, measures each line
//     through the language glyph list, applies the alignment bits 1/2/4 of
//     +0x1b, and draws with FUN_004a50e0/FUN_004a51d0.
//   - otherwise, if flags & 0xa0: the cell-grid renderer. It saves the clip
//     rect (FUN_004c6ae0), sets the entry rect as clip (FUN_004c6b10),
//     walks the cell array at +0xc6 (stride 4 when bit 7 of flags is clear,
//     stride 0x18 when set) and blits each cell bitmap (FUN_004c7580).
// The struct shapes (Class/Holder/Entry with the 0x15b stride, +0x13 x,
// +0x15 y, +0x17 w, +0x19 h, +0x1b flags, +0x1f colours, +0x28 tab,
// +0xb6 count, +0xbc surface for entry 0, +0xba/+0xbc/+0xc0/+0xc2/+0xc6/
// +0xd6/+0xda for the current entry) come from the matched siblings 0x4a4d70,
// 0x4a4660, 0x4a4c90 and 0x4a4980; the colour read is the same buggy
// `me->colours[(int)param_1 + 0x8b2]` as those files.
//
// Known remaining differences:
//  - Frame is 0x88 vs the original 0xbc and the callee-saved rotation is off
//    by one: we hold param_1 in esi, me in ebp, entries in ebx; the original
//    holds param_1 in ebp, me in edi and reloads me from [esp+0x50] after
//    calls. The missing 0x34 bytes are the stacked rects the grid path keeps
//    at [esp+0x6c]/[esp+0xac] plus the text-path scratch slots [esp+0x58],
//    [esp+0x5c], [esp+0x60], [esp+0x64], [esp+0x68] which are not modelled
//    as named locals.
//  - The four FUN_004be950 corner blits (0x4a22f1..0x4a23ab) and the two
//    rectangles fed to FUN_004c7580 at 0x4a213c are only approximate.
//  - The selection-highlight arms in both renderers pass &left as the rect;
//    the original builds a separate 4-dword rect on the stack.
//  - The scan loop and the text-line loop follow the disassembly closely and
//    are the place to start; the first ~40 instructions are 1:1 except for
//    the ebp/esi swap noted above.
//
// Third pass (deepseek-v4.1-flash) refined the blocker. param_1 is NOT held in
// ebp for the whole function in the original: at 0x4a1ddd it reloads param_1
// from [esp+0xd0] (the parameter's home slot) for `mov bl,[eax+ebp+0x8b2]`,
// and at 0x4a22f9 it loads it into ebx from the same slot for the corner-blit
// colour. So the original SPILLS param_1 and reloads it; it is not a clean
// ebp/esi rotation, and ebp is reused as entries/q inside the same regions.
// The real blocker is the stack-slot map: the original's frame reaches 0xbc
// with text scratch slots at 0x58/0x5c (xx,xw), 0x60 (font byte), 0x64
// (colour), 0x68 (char), and the grid rects at 0x6c (dst) and 0xac (src),
// plus the clip rect at 0x9c. Register pressure from those live values is
// what forces param_1 and me (edi, reloaded from [esp+0x50]) to spill; ours
// keeps both live in callee-saved registers. Passing `surface` instead of a
// literal 0 to FUN_004b0230 scores the same 21.1%. The grid renderer's
// FUN_004c7580 setup is interleaved in the original (0x4a2137..0x4a21c8) and
// must be reproduced as a single straight store sequence, not as two local
// Rect structs filled before the call.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)

struct Entry_004a1b40 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    unsigned char* colours;             // +0x1f
    char unknown_23[0x28 - 0x23];
    char tab;                           // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                    // +0xb6 (entry 0)
        char text_b6[0xb8 - 0xb6];
    } b6;
    short unknown_b8;
    short field_ba;                     // +0xba
    union {
        void* surface;                  // +0xbc (entry 0)
        short field_bc;                 // +0xbc
    } bc;
    short field_c0;                     // +0xc0
    char* text;                         // +0xc2
    int* cells;                         // +0xc6
    char unknown_ca[0xd6 - 0xca];
    void* field_d6;                     // +0xd6
    short field_da;                     // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Holder_004a1b40 {
    char unknown_00[4];
    Entry_004a1b40* entries;            // +0x04
    char unknown_08[0x10 - 0x08];
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    char unknown_18[0x24 - 0x18];
    void* surface;                      // +0x24
};

struct Class_004a1b40 {
    char unknown_00[0x18];
    Holder_004a1b40* holder;            // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour;               // +0x8b2
    char unknown_8b3[0x8be - 0x8b3];
    unsigned char colour_8be;           // +0x8be
    char unknown_8bf[0xcd2 - 0x8bf];
    void* fallback;                     // +0xcd2
};

struct Glyph_004a1b40 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
};

struct Language_004a1b40 {
    char unknown_0[0xc];
    unsigned short* glyphs;             // +0xc
};

struct LanguageRoot_004a1b40 {
    int current;                        // +0x0
    char unknown_04[0x14 - 0x04];
    Language_004a1b40* language;        // +0x14
};

struct Rect_004a1b40 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Class_004c6ae0 {
    void FUN_004c6ae0(Rect_004a1b40* rect);
};

struct Class_004c6b10 {
    void FUN_004c6b10(int left, int top, int right, int bottom);
};

#pragma pack(pop)

extern LanguageRoot_004a1b40* DAT_0051fba4;

void __stdcall FUN_004b0230(Class_004a1b40* obj, int index, void* bmp);
void __stdcall FUN_004c6d20(void* dst, void* src, Rect_004a1b40* rect, int* pos);
void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
void __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                            int rem, int style);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            unsigned char colour);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a1b40* rect, int id);
void __stdcall FUN_004c7580(void* surface, void* bitmap, Rect_004a1b40* dst,
                            Rect_004a1b40* src);

// FUNCTION: 0x4a1b40
void __stdcall FUN_004a1b40(Class_004a1b40* param_1, int param_2)
{
    int flag = 0;
    int top = 0;

    if (param_1->holder != 0)
        param_1->holder->field_14 = 1;

    Entry_004a1b40* entries = param_1->holder->entries;
    Entry_004a1b40* me = &entries[param_2];

    int h = me->h;
    int left;
    if (me->type == 0) {
        left = 0;
    } else {
        left = me->x;
        top = me->y;
    }
    int right = me->w + left - 1;
    int bottom = me->h + top - 1;

    void* surface = param_1->holder->surface;
    if (surface == 0)
        surface = param_1->fallback;
    if (surface == 0) {
        if (!(param_1->holder->field_10 & 0x80))
            FUN_004b0230(param_1, param_2, 0);
    } else {
        FUN_004c6d20(entries->bc.surface, surface, (Rect_004a1b40*)&left,
                     &left);
    }

    int lh;
    if (DAT_0051fba4->language == 0)
        lh = FUN_004c1450();
    else
        lh = ((Glyph_004a1b40*)FUN_004b7f30(DAT_0051fba4->language->glyphs,
                                            0x49))->height + 2;

    int step = lh + 1;
    if (me->field_da != 0)
        step = me->field_da;

    int yoff = 0;
    unsigned int flags = (unsigned int)me->flags;

    if ((flags & 0x10) != 0 && me->text != 0 && me->field_c0 != 0) {
        // ---- text-line renderer ----
        int i = 1;
        int t = 0;
        for (; i < entries->b6.count + 1; i++) {
            if (entries[i].type == 7) {
                if (t == me->tab) {
                    FUN_004c1420(*(int*)((char*)&entries[i] + 0xd6));
                    break;
                }
                t++;
            }
        }
        if (i == entries->b6.count + 1)
            FUN_004c1420(DAT_0051fba4->current);

        FUN_004c1440();
        int font = FUN_004c13f0();
        char* q = FUN_004b6af0(me->text, me->bc.field_bc);
        int y = me->bc.field_bc;
        int line = 0;

        for (;;) {
            int x1 = left + 2;
            int x2 = x1 + me->w - 2;
            int cy = top + yoff + 2;
            int cy2 = cy + step;

            int w;
            if (q == 0) {
                w = 0;
            } else if (DAT_0051fba4->language == 0) {
                w = FUN_004c1480(FUN_004c1440(), q);
            } else {
                char* pp = q;
                w = 0;
                while (*pp != 0) {
                    unsigned short* g = (unsigned short*)FUN_004b7f30(
                        DAT_0051fba4->language->glyphs, (unsigned char)*pp);
                    if (g != 0)
                        w += *g;
                    pp++;
                }
            }

            unsigned int col = (unsigned int)me->colours[(int)param_1 + 0x8b2];
            if (me->field_d6 != 0) {
                if (*((char*)me->field_d6 + y) == 1)
                    flag = 1;
            } else if (*q == 0x26) {
                if (q[1] == 0x47)
                    flag = 1;
                q += 2;
            }

            int xx;
            int xw;
            if (flags & 1) {
                xx = x1;
                xw = x2 - x1 + 1;
            } else if (flags & 4) {
                xx = x2 - w;
                xw = w;
            } else if (flags & 2) {
                xx = (x1 + x2 - w) / 2;
                if (xx < x1)
                    xx = x1;
                xw = x2 - xx + 1;
            }

            int next = 0;
            if (DAT_0051fba4->language == 0)
                next = FUN_004c1450();
            else
                next = ((Glyph_004a1b40*)FUN_004b7f30(
                            DAT_0051fba4->language->glyphs, 0x49))->height + 2;

            if (me->field_da > next + 6)
                FUN_004a51d0(entries->bc.surface, q, xx, cy, xw, bottom - top,
                             0);
            else
                FUN_004a50e0(entries->bc.surface, q, xx, cy, xw, 0);

            q = FUN_004b6af0(q, 1);

            if (flag) {
                flag = 0;
                FUN_004bf4d0(entries->bc.surface, (Rect_004a1b40*)&left,
                             -0x13);
                FUN_004bf4d0(entries->bc.surface, (Rect_004a1b40*)&left,
                             -0x14);
                FUN_004bf4d0(entries->bc.surface, (Rect_004a1b40*)&left,
                             -0x15);
                FUN_004bf4d0(entries->bc.surface, (Rect_004a1b40*)&left,
                             -0x16);
            } else if ((flags & 0x100) == 0 &&
                       me->field_ba == line + me->bc.field_bc &&
                       me->field_c0 != 0) {
                // both disassembly arms (0x4a1fb8 and 0x4a1fcb) pass the same
                // rect and id; only the branch on holder+0x20 differs.
                FUN_004bf4d0(entries->bc.surface, (Rect_004a1b40*)&left, 0x1e);
            } else {
                FUN_004c13a0((int)col, (int)(font & 0xff));
            }

            yoff += step;
            line++;
            y++;
            h -= step;
            if (h < lh)
                return;
            if (line + me->bc.field_bc >= me->field_c0)
                return;
        }
    } else if ((flags & 0xa0) != 0) {
        // ---- cell-grid renderer ----
        int bp = (int)((flags >> 7) & 1);
        void* surf = entries->bc.surface;
        Rect_004a1b40 clip;
        ((Class_004c6ae0*)surf)->FUN_004c6ae0(&clip);
        ((Class_004c6b10*)surf)->FUN_004c6b10(left, top, right, bottom);

        int row = me->bc.field_bc;
        int* colPtr = 0;
        char* cellPtr = 0;
        if (bp == 0)
            colPtr = &me->cells[row];
        else
            cellPtr = (char*)me->cells + row * 0x18;

        int yy = top + 2;
        int x1 = left + 2;
        int yEnd = yy + step;

        for (;;) {
            void* cell;
            if (yoff == 0) {
                cell = *(void**)(*colPtr + 0x28);
            } else {
                cell = cellPtr;
                cellPtr += 0x18;
            }
            if (cell != 0 && *(int*)((char*)cell + 0x10) != 0) {
                Rect_004a1b40 dst;
                Rect_004a1b40 src;
                dst.left = x1;
                dst.top = yy;
                dst.right = right;
                dst.bottom = yEnd - 1;
                src.left = 0;
                src.top = 0;
                src.right = *(unsigned short*)cell - 1;
                src.bottom = *((unsigned short*)cell + 1) - 1;
                FUN_004c7580(surf, cell, &dst, &src);
            }
            if ((*((unsigned char*)me->field_d6 + row) & 1) == 0) {
                if ((*((unsigned char*)me->field_d6 + row) & 2) != 0) {
                    unsigned char c = param_1->colour_8be;
                    FUN_004be950(surf, x1 + 1, yEnd - 1, right - 2, yy + 1, c);
                    FUN_004be950(surf, x1 + 2, yEnd - 1, right - 1, yy + 1, c);
                    FUN_004be950(surf, x1 + 1, yy + 2, right - 1, yEnd - 2, c);
                    FUN_004be950(surf, x1 + 2, yy + 2, right - 2, yEnd - 2, c);
                }
            } else {
                FUN_004bf4d0(surf, (Rect_004a1b40*)&left, -0x14);
            }

            if ((flags & 0x100) == 0 && me->field_ba == row + me->bc.field_bc) {
                Rect_004a1b40 hl;
                hl.left = x1;
                hl.top = yy;
                hl.right = 0;
                hl.bottom = 0;
                FUN_004bf4d0(surf, &hl, 0x14);
            }

            row++;
            if (yoff == 0)
                colPtr++;
            yy += step;
            yEnd += step;
            if (yy >= bottom)
                break;
            if (row >= me->field_c0)
                break;
        }
        ((Class_004c6b10*)surf)->FUN_004c6b10(clip.left, clip.top, clip.right,
                                              clip.bottom);
    }
}

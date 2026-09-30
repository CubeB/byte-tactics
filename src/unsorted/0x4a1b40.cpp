// Decompiled by deepseek-v4.1. Names are provisional.
// Partial: corner decoration coordinates and register allocation still differ.
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
    int colours;             // +0x1f
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
    char unknown_18b[0x20 - 0x18];
    int field_20;                       // +0x20
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

struct Point_004a1b40 { int x; int y; };
struct Quad_004a1b40 { Point_004a1b40 points[4]; };

struct Class_004c6ae0 {
    void FUN_004c6ae0(Rect_004a1b40* rect);
};

struct Class_004c6b10 {
    void FUN_004c6b10(Rect_004a1b40 rect);
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
void __stdcall FUN_004c7580(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

// Partial (18.6%), not MATCH. Attribution: started by deepseek-v4.1-flash, then GPT-6,
// edited by deepseek-v4.1 (issue 2379).
// Frame size (0xbc), ret 8, the &entries[param_2] lea chain and the overall branch
// structure are right; the differences are prologue register assignment and stack slots.
// Original prologue: sub esp,0xbc / push ebx / push ebp / mov ebp,[esp+0xc8] /
//   xor ebx,ebx / push esi / push edi, i.e. param_1 is loaded into ebp and ebx holds the
//   zero used for flag=0 and for top=0. Ours loads param_1 into another register
//   (eax/ebx depending on the variant) and zeroes into edi, so every [esp+N] drifts.
// Original slot map (verified from the disassembly, all offsets relative to esp after
// the four pushes): 0x10 line/tab counter (both share one slot, disjoint ranges),
// 0x14 flag, 0x18 x1, 0x1c cy, 0x20 x2, 0x24 cy2, 0x28 y, 0x2c q, 0x30 yoff, 0x34 h,
// 0x38 entries, 0x3c lh, 0x40..0x4c bounds (left,top,right,bottom), 0x50 me, 0x54 step,
// 0x60 font byte, 0x64 col, 0x68 glyph char temp. Ours: entries=0x10, flag=0x1c,
// h=0x2c, bounds=0x38..0x44, me=0x48, so the whole frame is reshuffled, not shifted.
// Original keeps me in edi, q in ebp, step in ebx, x1 in esi, x2 in ecx, step=ebx in the
// loop tail; ours uses different roles, so most of the text loop (0x4a1c5c) differs.
// The loop bottom is 0x4a1ff3: yoff+=step, line++ (slot 0x10), y++, h-=step, h<lh exit.
// Tried and rejected: dropping the `holder` local for fresh param_1->holder derefs
// (18.6% -> 17.9%, but the original does reload [ebp+0x18] twice at 0x4a1b53/0x4a1b6c, so
// a `holder` local is closer over the whole function); hoisting the tab counter `t` to
// the top (neutral); removing <windows.h> (18.6% -> 15.6%, include is needed).
// Open question worth solving next: the highlight call at 0x4a1fbe passes esp+0x1c after
// the 0x1e id was pushed, i.e. rowRect+8, and both of its arms (0x4a1fb8/0x4a1fcb) pass
// the same pointer. Either the original really indexes past rowRect or its highlight rect
// is a second rect whose slot overlaps rowRect (mutually exclusive branches).
//
// FUNCTION: 0x4a1b40
void __stdcall FUN_004a1b40(Class_004a1b40* param_1, int param_2)
{
    int t;
    int flag = 0;
    Rect_004a1b40 bounds;
    int& top = bounds.top;
    int& left = bounds.left;
    int& right = bounds.right;
    int& bottom = bounds.bottom;

    Holder_004a1b40* holder = param_1->holder;
    if (holder != 0)
        holder->field_14 = 1;

    Entry_004a1b40* entries = holder->entries;
    Entry_004a1b40* me = &entries[param_2];

    int h = me->h;
    if (me->type == 0) {
        top = 0;
        left = 0;
    } else {
        left = me->x;
        top = me->y;
    }
    right = me->w + left - 1;
    bottom = me->h + top - 1;

    void* surface = holder->surface;
    if (surface == 0)
        surface = param_1->fallback;
    if (surface == 0) {
        if (!(holder->field_10 & 0x80))
            FUN_004b0230(param_1, param_2, 0);
    } else {
        FUN_004c6d20(entries->bc.surface, surface, &bounds,
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
    int xx;
    int xw;
    unsigned int flags = (unsigned int)me->flags;

    if ((flags & 0x10) != 0 && me->text != 0 && me->field_c0 != 0) {
        // ---- text-line renderer ----
        int i = 1;
        t = 0;
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
        unsigned char font = (unsigned char)FUN_004c13f0();
        char* q = FUN_004b6af0(me->text, me->bc.field_bc);
        int y = me->bc.field_bc;
        int line = 0;

        for (;;) {
            int x1 = left + 2;
            int x2 = x1 + me->w - 2;
            int cy = top + yoff + 2;
            int cy2 = cy + step;
            Rect_004a1b40 rowRect = {x1, cy, x2, cy2};

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

            unsigned int col = (unsigned int)*((unsigned char*)param_1 + 0x8b2 + me->colours);
            if (me->field_d6 != 0) {
                if (*((char*)me->field_d6 + y) == 1)
                    flag = 1;
            } else if (*q == 0x26) {
                if (q[1] == 0x47)
                    flag = 1;
                q += 2;
            }

            flags = (unsigned int)me->flags;
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
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x13);
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x14);
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x15);
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x16);
            } else if ((me->flags & 0x100) == 0 &&
                       me->field_ba == line + me->bc.field_bc &&
                       me->field_c0 != 0) {
                // both arms (0x4a1fb8 and 0x4a1fcb) pass the same rect and id,
                // the arm is chosen by holder->field_20 == param_2
                if (param_1->holder->field_20 == param_2)
                    FUN_004bf4d0(entries->bc.surface, &rowRect, 0x1e);
                else
                    FUN_004bf4d0(entries->bc.surface, &rowRect, 0x1e);
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
        ((Class_004c6b10*)surf)->FUN_004c6b10(bounds);

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
            if (bp == 0) {
                cell = *(void**)(*colPtr + 0x28);
            } else {
                cell = cellPtr;
                cellPtr += 0x18;
            }
            if (cell != 0 && *(int*)((char*)cell + 0x10) != 0) {
                Quad_004a1b40 dst = {{{x1, yy}, {right, yy},
                    {right, yEnd - 1}, {x1, yEnd - 1}}};
                int width = *(unsigned short*)cell - 1;
                int height = *((unsigned short*)cell + 1) - 1;
                Quad_004a1b40 src = {{{1, 1}, {width, 1},
                    {width, height}, {1, height}}};
                FUN_004c7580(surf, cell, &dst, &src);
                Rect_004a1b40 rowRect = {x1, yy, right, yEnd - 1};
            if ((*((unsigned char*)me->field_d6 + row) & 1) == 0) {
                if ((*((unsigned char*)me->field_d6 + row) & 2) != 0) {
                    unsigned char c = param_1->colour_8be;
                    FUN_004be950(surf, x1 + 1, yEnd - 1, right - 2, yy + 1, c);
                    FUN_004be950(surf, x1 + 2, yEnd - 1, right - 1, yy + 1, c);
                    FUN_004be950(surf, x1 + 1, yy + 2, right - 1, yEnd - 2, c);
                    FUN_004be950(surf, x1 + 2, yy + 2, right - 2, yEnd - 2, c);
                }
            } else {
                FUN_004bf4d0(surf, &rowRect, -0x14);
            }

            }
            if ((me->flags & 0x100) == 0 && me->field_ba == row) {
                Rect_004a1b40 hl;
                hl.left = x1;
                hl.top = yy;
                hl.right = x1 + *(unsigned short*)cell - 1;
                hl.bottom = yy + *((unsigned short*)cell + 1) - 1;
                FUN_004bf4d0(surf, &hl, 0x14);
            }

            row++;
            if (bp == 0)
                colPtr++;
            yy += step;
            yEnd += step;
            if (yy >= bottom)
                break;
            if (row >= me->field_c0)
                break;
        }
        ((Class_004c6b10*)surf)->FUN_004c6b10(clip);
    }
}

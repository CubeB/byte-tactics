// Decompiled by space-bunny-free, improved by GPT-6.1-sol, finished by GPT-6. Names are provisional.
// PARTIAL 46.6%. Remaining differences: shared draw tails, register allocation and stack locals. Restored selected text cursor, conditional final colour and original 128-byte buffer.
#include <windows.h>
#include <stdio.h>

#pragma pack(push, 1)
struct GafEntry_004a5f40 {
    unsigned short count;              // +0x00
    char unknown_2[2];
};

struct Glyph_004a5f40 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    short xoff;                        // +0x04
    short yoff;                        // +0x06
};

struct Entry_004a5f40 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int flags;                         // +0x1b
    unsigned char* colours;            // +0x1f
    char unknown_23[0x28 - 0x23];
    signed char tab;                   // +0x28
    char unknown_29[0x2f - 0x29];
    GafEntry_004a5f40* gaf;            // +0x2f
    char unknown_33[0xb6 - 0x33];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0x20];               // +0xb6
    } u;
    int language;                      // +0xd6
    char unknown_da[0x136 - 0xda];
    unsigned char field_136;           // +0x136
    unsigned char field_137;           // +0x137
    short field_138;                   // +0x138
    unsigned char field_13a;           // +0x13a
    unsigned char field_13b;           // +0x13b
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x15b - 0x13d];
};

struct Layer_004a5f40 {
    char unknown_0[4];
    Entry_004a5f40* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14
};

struct Language_004a5f40 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct FontRoot_004a5f40 {
    int current;                       // +0x00
    char unknown_4[0x14 - 0x04];
    Language_004a5f40* language;       // +0x14
};

struct Menu_004a5f40 {
    char unknown_00[8];
    int values[3];                     // +0x08
    int current;                       // +0x14
    Layer_004a5f40* layer;             // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour_8b2;          // +0x8b2
    char unknown_8b3;
    unsigned char colour_8b4;          // +0x8b4
    char unknown_8b5[0x8bc - 0x8b5];
    unsigned char colour_8bc;          // +0x8bc
    char unknown_8bd[0x8c3 - 0x8bd];
    unsigned char colour_8c3;          // +0x8c3
    char unknown_8c4;
    unsigned char colour_8c5;          // +0x8c5
    unsigned char colour_8c6;          // +0x8c6
};
#pragma pack(pop)

struct Rect_004a5f40 { int left, top, right, bottom; };

extern FontRoot_004a5f40* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
Glyph_004a5f40* __stdcall FUN_004b7f30(GafEntry_004a5f40* table, int index);
void __stdcall FUN_004b7f90(void* surface, Glyph_004a5f40* glyph, int x, int y);
void __stdcall FUN_004b8310(void* surface, Glyph_004a5f40* glyph, int x, int y, int style);
int __stdcall FUN_004a5d50(Menu_004a5f40* menu, int index);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, unsigned char colour);
void __stdcall FUN_004bfe10(void* surface, Rect_004a5f40* rect);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a5f40* rect, int param);
void __stdcall FUN_004b04b0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FUN_004b04e0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);

static inline int Measure_004a5f40(char* text)
{
    int width = 0;
    char* p = text;
    if (p != 0) {
        if (DAT_0051fba4->language == 0) {
            width = FUN_004c1480(FUN_004c1440(), text);
        } else {
            while (*p != 0) {
                char ch = *p;
                Glyph_004a5f40* glyph = FUN_004b7f30(
                    (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, (unsigned char)ch);
                if (glyph != 0)
                    width += glyph->width;
                ++p;
            }
        }
    }
    return width;
}

static inline int LineHeight_004a5f40()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return FUN_004b7f30(
        (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, 0x49)->height + 2;
}

// FUNCTION: 0x4a5f40
void __stdcall FUN_004a5f40(Menu_004a5f40* menu, int index)
{
    char key1[2];
    char key2[2];
    void* surface;
    int t;
    Entry_004a5f40* me;
    int measured;
    Rect_004a5f40 rect;
    int x;
    char* found;
    int width;
    int border;
    int flagy;
    int textw;
    char* text;
    int pass;
    int saved;
    char buf[0x80];
    int i;
    int y;

    border = 0;
    if (menu->layer != 0)
        menu->layer->field_14 = 1;
    Entry_004a5f40* entries = menu->layer->entries;
    me = &entries[index];
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = me->h + rect.top - 1;
    if (me->flags & 0x8000)
        menu->current = menu->values[1];

    t = 0;
    for (i = 1; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == me->tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries->u.count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    textw = FUN_004a5d50(menu, index);
    surface = *(void**)((char*)entries + 0xbc);
    if (me->gaf == 0) {
        if (me->field_13c & 1) {
            FUN_004b04b0(surface, &rect, menu->colour_8b2, menu->colour_8c5, menu->colour_8c5);
        } else if (me->field_138 != 0) {
            FUN_004b04b0(surface, &rect, menu->colour_8b2, menu->colour_8c3, menu->colour_8c6);
        } else {
            FUN_004b04e0(surface, &rect, menu->colour_8b2, menu->colour_8c3, menu->colour_8c6);
        }
    } else {
        Glyph_004a5f40* glyph;
        if (me->field_13c & 1) {
            if (me->flags & 0x100) {
                glyph = FUN_004b7f30(me->gaf, me->gaf->count - 1);
            } else if (me->field_136 != 0) {
                glyph = FUN_004b7f30(me->gaf, me->field_137);
                border = 1;
            } else if (me->flags & 0x1800) {
                glyph = FUN_004b7f30(me->gaf, me->field_13b);
                border = 1;
            } else {
                int val = me->field_138 + 2;
                if (val >= me->gaf->count - 1)
                    val = me->gaf->count - 1;
                glyph = FUN_004b7f30(me->gaf, val + me->field_13b);
                if (!(me->flags & 0x80))
                    border = 1;
            }
        } else {
            int val;
            if (me->field_138 != 0 && (unsigned short)me->gaf->count > (unsigned short)me->field_136) {
                if (me->field_136 != 0)
                    val = me->gaf->count - 2;
                else
                    val = me->field_13b + me->field_138;
            } else {
                if (me->field_136 != 0)
                    val = me->field_13b;
                else
                    val = me->field_137;
            }
            glyph = FUN_004b7f30(me->gaf, val);
        }
        if (glyph != 0) {
            if (me->colours != 0)
                FUN_004b8310(surface, glyph, glyph->xoff + rect.left,
                             glyph->yoff + rect.top, (int)me->colours);
            else
                FUN_004b7f90(surface, glyph, glyph->xoff + rect.left,
                             glyph->yoff + rect.top);
        }
    }

    t = 0;
    flagy = 0;
    if (me->field_138 != 0) {
        t = 1;
        flagy = 1;
    }
    text = me->u.text;
    pass = 0;
    do {
        if (me->field_138 != 0)
            FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
        else
            FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());

        char* p = text;
        if (me->field_136 != 0 && me->field_137 != 0) {
            for (unsigned int k = me->field_137; k != 0; k--) {
                while (*p != 0)
                    p++;
                p++;
            }
        }

        text = p;
        y = (rect.bottom - LineHeight_004a5f40() - rect.top) / 2 + flagy + rect.top;
        if (me->flags & 0x8000)
            menu->current = menu->values[1];

        if (me->flags & 1) {
            FUN_004a50e0(surface, text, t + rect.left + 3, y,
                         rect.right - rect.left + 1, 0);
        } else if (me->flags & 4) {
            x = rect.right - textw - 3;
            if (x < rect.left)
                x = rect.left;
            FUN_004a50e0(surface, text, x, y, rect.right - rect.left + 1, 0);
        } else if (me->flags & 2) {
            x = (rect.right - textw - rect.left) / 2 + t + rect.left + 1;
            if (me->field_13a == 0 || (me->field_13c & 1)) {
                FUN_004a50e0(surface, text, x, y, rect.right - rect.left + 1, 0);
            } else {
                key1[0] = me->field_13a;
                width = rect.right - rect.left + 1;
                key1[1] = 0;
                strcpy(buf, text);
                found = strstr(buf, key1);
                if (found == 0) {
                    FUN_004a50e0(surface, text, x, y, width, 0);
                } else {
                    FUN_004c1440();
                    strcpy(buf, text);
                    *found = 0;
                    FUN_004a50e0(surface, buf, x, y, width, 0);
                    x += Measure_004a5f40(buf);
                    saved = x;
                    if (me->field_138 != 0)
                        FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
                    else
                        FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                    FUN_004a50e0(surface, key1, x, y, width, 0);
                    measured = Measure_004a5f40(key1);
                    x += measured;
                    if (me->field_138 != 0) {
                        FUN_004be950(surface, saved, LineHeight_004a5f40() + y - 1,
                                     x - 1, LineHeight_004a5f40() + y - 1,
                                     menu->colour_8b2);
                    } else {
                        FUN_004be950(surface, saved, LineHeight_004a5f40() + y - 1,
                                     x - 1, LineHeight_004a5f40() + y - 1,
                                     menu->colour_8b4);
                    }
                    if (me->field_138 != 0)
                        FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
                    else
                        FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                    FUN_004a50e0(surface, found + 1, x, y, width, 0);
                }
            }
        } else if (me->flags & 0x20) {
            x = (rect.right - textw - rect.left) / 2 + t + rect.left + 1;
            measured = flagy - LineHeight_004a5f40() + rect.bottom - 4;
            found = 0;
            if (me->field_13a != 0)
                found = strchr(text, (signed char)me->field_13a);
            if (found == 0) {
                FUN_004a50e0(surface, text, x, measured, rect.right - rect.left + 1, 0);
            } else {
                width = rect.right - rect.left + 1;
                key2[0] = me->field_13a;
                key2[1] = 0;
                FUN_004c1440();
                FUN_004a50e0(surface, text, x, measured, width, 0);
                *found = 0;
                x += Measure_004a5f40(text);
                FUN_004c13a0(menu->colour_8bc, FUN_004c13f0());
                FUN_004a50e0(surface, key2, x, measured, width, 0);
                x += Measure_004a5f40(key2);
                FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                FUN_004a50e0(surface, found + 1, x, measured, width, 0);
            }
        }
    } while (pass--);

    menu->current = menu->values[0];
    if (border) {
        FUN_004bfe10(surface, &rect);
        FUN_004bf4d0(surface, &rect, -0x14);
    }
}

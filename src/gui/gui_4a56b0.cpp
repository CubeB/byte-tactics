// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Space Bunny Free. finished by claude-sonnet-5-5, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, 2026-10-02), from 96.3%. Two changes on top of the previous
// pass, both needed:
//  1. The field_148 branch does not end in its own `obj->field_14 = obj->field_08;
//     return;`: it is the then-arm of an if/else whose else-arm is the field_147
//     underline block, and ONE final `obj->field_14 = obj->field_08;` follows both.
//     MSVC clones that small tail into all three exits after allocating it once,
//     which is why every exit in the original reloads obj into ebx (the
//     field_148 arm has edi busy with entries2). With separate statements the
//     two later exits used edi and x0 stayed in ebx.
//  2. The underline's x positions accumulate in one variable:
//         int x0 = rect.left; x0 += Measure(buf); int x1 = x0; x0 += Measure(pat);
//     With obj now owning ebx, this is the spelling that spills x0 (to the
//     Measure byte slot [esp+0x18]) and keeps x1 in ebp, as the original does;
//     `left + w1` style locals spill x1 instead or grow the frame.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a56b0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int align;                         // +0x1b
    int colours;                       // +0x1f
    int image;                         // +0x23
    char unknown_27[1];
    signed char tab;                   // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xbc - 0xb6];        // +0xb6
    } b6;
    void* surface;                     // +0xbc
    char unknown_c0[0xd6 - 0xc0];
    int language;                      // +0xd6
    char unknown_da[0x147 - 0xda];
    unsigned char field_147;           // +0x147
    unsigned char field_148;           // +0x148
    char unknown_149[0x15b - 0x149];
};

struct Holder_004a56b0 {
    int current;                       // +0x00
    Entry_004a56b0* entries;           // +0x04
};

struct Glyph_004a56b0 { unsigned short width, height; };

struct List_004a56b0 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct LanguageRoot_004a56b0 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a56b0* language;           // +0x14
};

struct Class_004a56b0 {
    char unknown_00[0x08];
    void* field_08;                    // +0x08
    void* field_0c;                    // +0x0c
    char unknown_10[0x14 - 0x10];
    void* field_14;                    // +0x14
    Holder_004a56b0* holder;           // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colours[256];          // +0x8b2, +0x8b3, +0x8b4
};

struct Rect_004a56b0 { int left, top, right, bottom; };
#pragma pack(pop)

extern LanguageRoot_004a56b0* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
void __stdcall FUN_004c14f0(void* surface, char* text, int x, int y, int maxw);
int __stdcall FUN_004bf6f0(void* surface, Rect_004a56b0* rect, int colour);
void __stdcall FUN_004bfe10(void* surface, Rect_004a56b0* rect);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a56b0* rect, int param);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
int __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                           int rem, int style);

static inline int Measure_004a56b0(char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    char* p = text;
    while (*p != 0) {
        char ch = *p;
        Glyph_004a56b0* glyph = (Glyph_004a56b0*)FUN_004b7f30(
            DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

static inline int LineHeight_004a56b0()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return (int)((Glyph_004a56b0*)FUN_004b7f30(
        DAT_0051fba4->language->glyphs, 0x49))->height + 2;
}

// FUNCTION: 0x4a56b0
void __stdcall FUN_004a56b0(Class_004a56b0* obj, int index)
{
    obj->field_14 = obj->field_0c;
    Entry_004a56b0* entries = obj->holder->entries;

    int i = 1;
    int t = 0;
    for (; i < entries[0].b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].b6.count + 1) {
        FUN_004c1420(DAT_0051fba4->current);
        i = -1;
    }


    if (entries[index].x == -1)
        entries[index].x = (short)((entries[0].w - Measure_004a56b0(entries[index].b6.text)) / 2);

    Rect_004a56b0 rect;
    if (entries[index].type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = entries[index].x;
        rect.top = entries[index].y;
    }
    rect.right = entries[index].w + rect.left - 1;
    rect.bottom = entries[index].h + rect.top - 1;

    if (entries[index].image != 0)
        FUN_004bf6f0(entries->surface, &rect, obj->colours[entries[index].image]);
    int nx = rect.left;
    Entry_004a56b0* entry = &entries[index];

    if (entry->align & 4) {
        nx = entry->w + rect.left;
        nx -= Measure_004a56b0(entry->b6.text);
    } else if (entry->align & 2) {
        nx = entry->w / 2 + rect.left;
        int half = Measure_004a56b0(entry->b6.text) / 2;
        nx -= half;
    }

    if (i != -1 && (entries[index].align & 8)) {
        FUN_004c13a0(obj->colours[0], FUN_004c13f0());
        FUN_004c14f0(entries->surface, entries[index].b6.text, nx + 1, rect.top + 3, -1);
    }

    FUN_004c13a0(entries[index].colours, FUN_004c13f0());

    if (i == -1) {
        int lh = LineHeight_004a56b0();
        if (rect.bottom - rect.top > lh * 2)
            FUN_004a51d0(entries->surface, entries[index].b6.text, nx, rect.top,
                         rect.right - rect.left + 1,
                         rect.bottom - rect.top + 1, entries[index].colours);
        else
            FUN_004a50e0(entries->surface, entries[index].b6.text, nx, rect.top,
                         rect.right - rect.left + 1, entries[index].colours);
    } else {
        FUN_004c14f0(entries->surface, entries[index].b6.text, nx, rect.top, -1);
    }

    if (entries[index].field_148 & 1) {
        Entry_004a56b0* entries2 = obj->holder->entries;
        Rect_004a56b0 rect2;
        if (entries2[index].type == 0) {
            rect2.left = 0;
            rect2.top = 0;
        } else {
            rect2.left = entries2[index].x;
            rect2.top = entries2[index].y;
        }
        rect2.right = entries2[index].w + rect2.left - 1;
        rect2.bottom = entries2[index].h + rect2.top - 1;
        FUN_004bfe10(entries2->surface, &rect2);
        FUN_004bf4d0(entries2->surface, &rect2, -0x14);
    } else {
        unsigned char c = entries[index].field_147;
        if (c != 0) {
            char pat[2];
            pat[0] = (char)c;
            pat[1] = 0;
            char buf[0x80];
            strcpy(buf, entries[index].b6.text);
            char* p = strstr(buf, pat);
            if (p != 0) {
                int y = rect.top;
                *p = 0;
                int x0 = rect.left;
                x0 += Measure_004a56b0(buf);
                int x1 = x0;
                x0 += Measure_004a56b0(pat);
                int lh1 = LineHeight_004a56b0();
                int lh2 = LineHeight_004a56b0();
                FUN_004be950(obj->holder->entries->surface, x1, lh2 + y - 1, x0 - 1,
                             lh1 + y - 1, obj->colours[2]);
            }
        }
    }

    obj->field_14 = obj->field_08;
}

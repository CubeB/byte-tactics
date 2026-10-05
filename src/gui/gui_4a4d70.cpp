// Decompiled by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5, finished by space-bunny-free, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, issue 4160), from 99.1%. Draws one list-gadget
// entry: makes the entry's language current, fills or blits its rectangle,
// draws its text, and when the entry has the focus draws the text cursor (a
// vertical line) after the text up to the cursor position.
//
// What it took, in order of weight:
//  1. FUN_004be950's colour parameter is `int`, as in 0x4be950 itself and in
//     the matched siblings 0x4a4c90 and 0x4a56b0. The old `unsigned char`
//     declaration (with an `int colour` local to get the zero extension) was
//     a local optimum at 99.1% that could never fix the cursor call's surface
//     load.
//  2. Every glyph fetch goes through one inline helper, GetGlyph, which wraps
//     FUN_004b7f30 on the current language's glyph table. Used for the line
//     height, it is what keeps the text width in esi (so the zero register
//     appears in the inlined Measure) while the glyph table still loads into
//     ecx; spelling the height fetch out in full gives 87.9% with this
//     prototype.
//  3. The cursor call takes its arguments as expressions (`height +
//     rect.top`, `obj->colours[9]`); only x is a local.
//
// Two old "suspected original bugs" in this file were misreadings and are
// withdrawn: the +0x1f field is an int colour INDEX into the object's colour
// table at +0x8b2 (`obj->colours[entry->colours]`), not a pointer indexed by
// the object's address; and the byte saved, zeroed and restored around the
// width measurement is `text[obj->cursor]`, the character at the cursor, so
// the measured width is that of the text before the cursor.

#pragma pack(push, 1)
struct Entry_004a4d70 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    unsigned char align;               // +0x1b
    char unknown_1c[0x1f - 0x1c];
    int colours;                       // +0x1f, index into Class::colours
    char unknown_23[0x28 - 0x23];
    char tab;                          // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xbc - 0xb6];        // +0xb6
    } b6;
    void* surface;                     // +0xbc
    char unknown_c0[0xd6 - 0xc0];
    int language;                      // +0xd6
    char unknown_da[0x15b - 0xda];
};

struct List_004a4d70 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct Holder_004a4d70 {
    int current;                       // +0x00
    Entry_004a4d70* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a4d70* language;           // +0x14
    char unknown_18[0x24 - 0x18];
    void* surface;                     // +0x24
};

struct Class_004a4d70 {
    char unknown_00[0x18];
    Holder_004a4d70* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
    char unknown_68[0x74 - 0x68];
    int cursor;                        // +0x74
    char unknown_78[0x8b2 - 0x78];
    unsigned char colours[0xcd2 - 0x8b2]; // +0x8b2
    void* fallback;                    // +0xcd2
};

struct Rect_004a4d70 { int left, top, right, bottom; };
struct Glyph_004a4d70 { unsigned short width, height; };

struct LanguageRoot_004a4d70 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a4d70* language;           // +0x14
};
#pragma pack(pop)

extern LanguageRoot_004a4d70* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
int __stdcall FUN_004b0230(Class_004a4d70* obj, int index, void* bmp);
void __stdcall FUN_004c6d20(void* dst, void* src, Rect_004a4d70* rect, int* pos);
int __stdcall FUN_004bf6f0(void* surface, Rect_004a4d70* rect, int colour);
int __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            int colour);

static inline Glyph_004a4d70* GetGlyph_004a4d70(unsigned char c)
{
    return (Glyph_004a4d70*)FUN_004b7f30(DAT_0051fba4->language->glyphs, c);
}

static inline int Measure_004a4d70(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    char* q = text;
    while (*q != 0) {
        char ch = *q;
        Glyph_004a4d70* glyph = GetGlyph_004a4d70(ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4d70
void __stdcall FUN_004a4d70(Class_004a4d70* obj, int index)
{
    Entry_004a4d70* entries = obj->holder->entries;
    int i = 1;
    int t = 0;
    for (; i < entries->b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries->b6.count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    Entry_004a4d70* me = &entries[index];

    Rect_004a4d70 rect;
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = me->h + rect.top - 1;

    if (me->align & 1) {
        FUN_004bf6f0(entries->surface, &rect, obj->colours[0]);
    } else {
        void* surface = obj->holder->surface;
        if (surface == 0)
            surface = obj->fallback;
        if (surface == 0) {
            FUN_004b0230(obj, index, 0);
        } else {
            FUN_004c6d20(entries->surface, surface, &rect, (int*)&rect);
        }
    }

    FUN_004c13a0(obj->colours[me->colours], FUN_004c13f0());
    rect.top += 3;
    // The style is read as entries[index].colours rather than me->colours:
    // sharing one load with the colour read above swaps the SIB registers of
    // that read (see the 0x4a4d70 entry in docs/field-notes.md, Part 5).
    FUN_004a50e0(entries->surface, me->b6.text, rect.left, rect.top,
                 rect.right - rect.left, entries[index].colours);

    if (index == obj->focus) {
        char* at = &me->b6.text[obj->cursor];
        char save = *at;
        *at = 0;
        int w = Measure_004a4d70(me->b6.text);
        *at = save;
        int height;
        if (DAT_0051fba4->language == 0)
            height = FUN_004c1450();
        else
            height = GetGlyph_004a4d70(0x49)->height + 2;
        int x = rect.left + w;
        FUN_004be950(entries->surface, x, rect.top, x, height + rect.top, obj->colours[9]);
    }
}

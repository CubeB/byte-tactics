// Decompiled by space-bunny-free. Names are provisional.
// Sonnet 5.5 retry (#1080): no change to the code. /Gz and /Gr give the same 98.7%. Replacing the SIB operand order (about 150 spellings of the colour byte read, param_1 as int, a local copy of param_1, address-of-field forms) and the surface load (all orders of x, colour, y2 and a surface local) never moved either difference: both look like the same compiler-state effect.
// PARTIAL, 98.7% (661 bytes against 661, six instructions differ). Everything
// from the prologue to the tail of the marker box matches instruction for
// instruction. The six that do not are described at the bottom.
//
// Draws one GUI list entry: finds the language (the entry of type 7 whose tab
// index equals this entry's +0x28) and makes it current, builds the entry's
// rectangle, draws it either as a flat fill through the frame buffer (bit 0 of
// +0x1b set) or as a blit from the object's own bitmap, sets the text colour
// and draws the entry's text, and finally, when the entry is the focused one
// (+0x64), measures its text and draws a marker box just to its right.
//
// The struct shapes (0x15b entry stride, packed, +0x13 x / +0x15 y / +0x17 w /
// +0x19 h / +0x1b align / +0x1f colours / +0x28 tab / +0xb6 union / +0xbc
// surface) are copied from the matched siblings 0x4a4c90, 0x4a53c0, 0x4a4660
// and 0x4a76b0, which walk the same array. Three shapes in here are the
// decisive ones and each is worth several percent:
//
//  1. `me` must be declared AFTER the language loop. Declared before it, the
//     compiler keeps it in a callee-saved register across the loop and spills
//     the loop counter instead, which rewrites the whole loop (31.9% to
//     43.4%). The original reloads param_1 from its stack slot at 0x4a4dfb and
//     0x4a4e19 instead, so its `me` is computed after the loop, twice.
//  2. Measure needs `p` (the pointer it null-tests) and a SECOND cursor `q`
//     (the one it walks). One cursor gives `cmp byte [p],0` as the loop guard;
//     two give the original's `mov al,[p] / mov esi,p / test al,al`
//     (86.5% to 87.7%).
//  3. The marker's y2 and colour must be separate int locals, not folded into
//     the call (87.7% to 98.7%). The colour in particular has to be an int:
//     an `unsigned char` loses the `xor edx,edx` zero extension and drops back
//     to 87.7%.
//
// Suspected original bug: the focused-entry block saves the byte at
// `field_74 + (char*)me + 0xb6`, zeroes it, measures `me + 0xb6` and restores
// the byte. The save and the Measure read two DIFFERENT addresses (the first
// adds the +0x74 pointer, the second does not), so the zeroing never affects
// the string being measured, and `field_74 + me` is a sum of two unrelated
// addresses. Both are reproduced as written; see the `blank` local.
//
// What still differs, six instructions, all register allocation:
//
//  a. The colour byte's SIB byte. The original has
//     `mov dl, byte [ecx + eax*1 + 0x8b2]`, ours `byte [eax + ecx*1 + 0x8b2]`,
//     that is the colour pointer is the SIB base in the original and param_1 is
//     in ours. The identical expression in an isolated function compiles to
//     the original's form, so this is a reassociation inside the full function,
//     not a spelling. It flips to the original's form only when the +0x1f value
//     is used ONCE: writing the FUN_004a50e0 style argument as `(int)(me + 0x1f)`
//     (rather than `(int)me->colours`) gives the right SIB but costs three
//     bytes, 93.3%. A union alias at +0x1f does not break the CSE. Grepping the
//     exe, `8a 94 01 b2 08 00 00` has four hits and the swapped form has none.
//
//  b. Where the surface load lands. The original is
//     `push eax` (y2) / `mov eax,[ebx+0xbc]` / `push ecx` / `push edx` /
//     `push ecx` / `push eax`; ours loads it after two pushes and into edx.
//     Declaring the surface as a local moves it earlier still and costs 7%.

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
    unsigned char* colours;            // +0x1f
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
    void* field_74;                    // +0x74
    char unknown_78[0x8b2 - 0x78];
    unsigned char colour;              // +0x8b2
    char unknown_8b3[0x8bb - 0x8b3];
    unsigned char colour2;             // +0x8bb
    char unknown_8bc[0xcd2 - 0x8bc];
    void* fallback;                    // +0xcd2
};

struct Rect_004a4d70 { int left, top, right, bottom; };
struct Glyph_004a4d70 { unsigned short width, height; };

struct LanguageRoot_004a4d70 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a4d70* language;           // +0x14
};

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
                            unsigned char colour);

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
        Glyph_004a4d70* glyph = (Glyph_004a4d70*)FUN_004b7f30(
            DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4d70
void __stdcall FUN_004a4d70(Class_004a4d70* param_1, int param_2)
{
    Entry_004a4d70* entries = param_1->holder->entries;
    int i = 1;
    int t = 0;
    for (; i < entries->b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[param_2].tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries->b6.count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    Entry_004a4d70* me = &entries[param_2];

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
        FUN_004bf6f0(entries->surface, &rect, param_1->colour);
    } else {
        void* surface = param_1->holder->surface;
        if (surface == 0)
            surface = param_1->fallback;
        if (surface == 0) {
            FUN_004b0230(param_1, param_2, 0);
        } else {
            FUN_004c6d20(entries->surface, surface, &rect, (int*)&rect);
        }
    }

    FUN_004c13a0(me->colours[(int)param_1 + 0x8b2], FUN_004c13f0());
    rect.top += 3;
    FUN_004a50e0(entries->surface, me->b6.text, rect.left, rect.top,
                 rect.right - rect.left, (int)me->colours);

    if (param_2 == param_1->focus) {
        // The original blanks and restores a byte at
        // field_74 + (char*)me + 0xb6, which is not the string it then
        // measures: the Measure argument is me + 0xb6, the entry's own text.
        // Reproduced as written.
        char* blank = (char*)param_1->field_74 + (int)(char*)me + 0xb6;
        char save = *blank;
        *blank = 0;
        int w = Measure_004a4d70(me->b6.text);
        *blank = save;
        int height;
        if (DAT_0051fba4->language == 0)
            height = FUN_004c1450();
        else
            height = ((Glyph_004a4d70*)FUN_004b7f30(
                DAT_0051fba4->language->glyphs, 0x49))->height + 2;
        int x = rect.left + w;
        int colour = param_1->colour2;
        int y2 = height + rect.top;
        FUN_004be950(entries->surface, x, rect.top, x, y2, colour);
    }
}

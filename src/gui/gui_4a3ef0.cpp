// Decompiled by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, issue 4160), from 93.1%. The list gadget's
// scroll-up step, the mirror image of 0x4a99c0: find the entry of type 2
// whose +0x01 byte equals this entry's, then, by the flag bits 0x10, 0x20 and
// 0x80 of that entry, recompute the size of a line (+0x142) and the scroll
// position (+0x136), and refresh the gadget with FUN_004a2580.
//
// The two hunks that were left (the 0x10 arm's denominator registers and the
// 0x20 arm's `e->field_c6` load before `test eax,eax`) both came from two
// inline helpers:
//  - GetGlyph, the one helper for every GetGafFrame glyph fetch (the same
//    helper matched 0x4a4d70), used for the line height in the 0x10 arm. With
//    it the ternary denominator compiles to the original's registers.
//  - LineSize reads the count AND the font pointer before its `count > 0`
//    test, so the pointer load sits before the test, in edx.
//
// The `lines` union is inherited and is still load-bearing: with a plain
// `int lines` (declared anywhere, or a local of the 0x20 arm) the 0x10 arm's
// denominator comes out as `inc eax / cmp edi,eax` (89.3%), and the
// two-statement clamp that fixes that loses the shared ecx zero altogether
// (36.3%), so the union's two zero stores, which emit no code, are
// evidently weighed by MSVC's register choices (probably the same decision
// that keeps the constant 0 in ecx for the found test, the counter `n` and
// the 0x80 arm's `field_da` compare). A plainer source was not found.
//
// Suspected original bug, reproduced: the 0x20 arm divides by the line total
// unguarded, so a `field_c0` of zero or less divides by zero (the jle at
// 0x4a40b2 skips the multiply and 0x4a40d1 does `idiv ecx` with ecx = 0),
// where the 0x80 arm tests both of its divisors first.

#pragma pack(push, 1)
struct Entry_004a3ef0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x17 - 0x02];
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int field_1b;                      // +0x1b (read as a dword here)
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xc0 - 0xb8];
    short field_c0;                    // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    struct Font_004a3ef0** font;       // +0xc6
    char unknown_ca[0xd6 - 0xca];
    int id;                            // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x136 - 0xdc];
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140;                   // +0x140
    short field_142;                   // +0x142
    char unknown_144[0x15b - 0x144];
};
#pragma pack(pop)

struct Glyph_004a3ef0 { unsigned short width, height; };

struct Font_004a3ef0 {
    char unknown_0[0x28];
    Glyph_004a3ef0* glyph;             // +0x28
};

struct List_004a3ef0 {
    char unknown_0[0x0c];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3ef0 {
    int current;                       // +0x00
    Entry_004a3ef0* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3ef0* list;               // +0x14
};

struct Class_004a3ef0 {
    char unknown_0[0x18];
    Holder_004a3ef0* holder;           // +0x18
};

extern Holder_004a3ef0* g_guiContext;

void __stdcall SetFont(int id);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
int GetFontHeight();
void __stdcall FUN_004a2580(Class_004a3ef0* param_1, int param_2);

static inline Glyph_004a3ef0* GetGlyph_004a3ef0(unsigned char c)
{
    return (Glyph_004a3ef0*)GetGafFrame(g_guiContext->list->field_0c, c);
}

static inline int Find_004a3ef0(Entry_004a3ef0* entries, unsigned char kind)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 2 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

static inline int LineSize_004a3ef0(Entry_004a3ef0* e)
{
    int count = e->field_c0;
    Font_004a3ef0** font = e->font;
    int lines = 0;
    if (count > 0)
        lines = (*font)->glyph->height * count;
    return lines;
}

// FUNCTION: 0x4a3ef0
void __stdcall DrawSlider(Class_004a3ef0* param_1, int param_2)
{
    Entry_004a3ef0* entries = param_1->holder->entries;
    Entry_004a3ef0* me = &entries[param_2];
    unsigned char kind = me->kind;
    int found = Find_004a3ef0(entries, kind);
    union { int full; short word; } lines;
    lines.full = 0;
    lines.word = 0;
    if (found != 0) {
        Entry_004a3ef0* e = &entries[found];
        if (e->type == 2) {
            if (e->field_1b & 0x10) {
                int i = 1;
                int n = 0;
                for (; i < entries->count + 1; i++) {
                    if (entries[i].type == 7) {
                        if (n == e->group) {
                            SetFont(entries[i].id);
                            break;
                        }
                        n++;
                    }
                }
                if (i == entries->count + 1) {
                    SetFont(g_guiContext->current);
                }
                int size = (g_guiContext->list == 0) ? GetFontHeight()
                    : GetGlyph_004a3ef0(0x49)->height + 2;
                int numerator = e->field_19 - 2;
                int denominator = (e->field_da > size + 1) ? e->field_da : size + 1;
                int step = numerator / denominator;
                int last = e->field_c0;
                int rows = (int)((float)step / last * (me->field_19 - 3));
                me->field_142 = rows;
                if (me->field_142 < 10) {
                    me->field_142 = 10;
                }
                if (last <= step) {
                    me->field_136 = 0;
                } else {
                    me->field_136 = me->field_19 - me->field_142 - 3;
                }
            } else if (e->field_1b & 0x20) {
                lines.full = LineSize_004a3ef0(e);
                int s = e->field_19 * me->field_19 / lines.full;
                me->field_142 = s;
                if (*(unsigned char*)((char*)me + 0x1b) & 1) {
                    me->field_136 = me->field_17 - s;
                } else {
                    me->field_136 = me->field_19 - s;
                }
            } else if (e->field_1b & 0x80) {
                if (e->field_da != 0 && e->field_c0 != 0) {
                    int s = e->field_19 / e->field_da * me->field_19 / e->field_c0;
                    me->field_142 = s;
                    if (*(unsigned char*)((char*)me + 0x1b) & 1) {
                        me->field_136 = me->field_17 - s;
                    } else {
                        me->field_136 = me->field_19 - s;
                    }
                }
            }
        }
    }
    FUN_004a2580(param_1, param_2);
}

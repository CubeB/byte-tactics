// Decompiled by space-bunny-free, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by Sonnet 5.5. Names are provisional.
// Walks the entry list of a layout object looking for the n-th tab stop
// (entries whose +0x00 byte is 7), sets the language from that entry, computes
// the line height, then lays the entry's text out right aligned (+0x1b bit 2),
// centred (bit 1) or left at its measured width (bit 0), writing the new x,
// width and line height back into the entry. +0xb6 is a union (count on entry
// 0, NUL terminated text elsewhere); +0x1b is a 4-byte field.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a53c0 {
    unsigned char type;                 // +0x00
    char unknown_01[0x12];
    short x;                            // +0x13
    char unknown_15[2];
    short w;                            // +0x17
    short h;                            // +0x19
    int align;                          // +0x1b
    char unknown_1f[0x9];
    signed char tab;                    // +0x28
    char unknown_29[0x8d];
    union {
        short count;                    // +0xb6 on entry 0
        char text[0xd6 - 0xb6];
    } b6;
    int language;                       // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct Holder_004a53c0 {
    char unknown_0[4];
    Entry_004a53c0* entries;
};

struct Dialog {
    char unknown_0[0x18];
    Holder_004a53c0* holder;
};

struct Glyph_004a53c0 { unsigned short width, height; };
struct Language_004a53c0 { char unknown_0[0xc]; unsigned short* glyphs; };
struct LanguageRoot_004a53c0 {
    int language0;                      // +0x0
    char unknown_4[0x10];
    Language_004a53c0* language;        // +0x14
};

extern LanguageRoot_004a53c0* g_guiContext;

void __stdcall SetFont(int param);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
int __stdcall GetGafFrame(unsigned short* glyphs, int c);

static inline int Measure_004a53c0(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    while (*p != 0) {
        char ch = *p;
        Glyph_004a53c0* glyph = (Glyph_004a53c0*)GetGafFrame(g_guiContext->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

// FUNCTION: 0x4a53c0
void __stdcall FUN_004a53c0(Dialog* obj, int index)
{
    Entry_004a53c0* entries = obj->holder->entries;
    Entry_004a53c0* entry = &entries[index];
    int i;
    int t = 0;
    for (i = 1; i < entries[0].b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entry->tab) {
                SetFont(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].b6.count + 1)
        SetFont(g_guiContext->language0);
    int x = !entry->type ? 0 : entry->x;
    // Real variable declared here, assigned in each arm, one shared tail stores it.
    int nx = x;
    int lh;
    if (g_guiContext->language == 0)
        lh = GetFontHeight();
    else
        lh = ((Glyph_004a53c0*)GetGafFrame(g_guiContext->language->glyphs, 0x49))->height + 2;
    if (entry->align & 4) {
        nx = entry->w + x;
        nx -= Measure_004a53c0(entry->b6.text);
    } else if (entry->align & 2) {
        nx = entry->w / 2 + x;
        int half = Measure_004a53c0(entry->b6.text) / 2;
        nx -= half;
        // No named local for the width: it changes how lh is loaded.
        entry->w = (short)(half * 2);
    } else if (entry->align & 1)
        entry->w = (short)Measure_004a53c0(entry->b6.text);
    // Stored via entries[index], not entry: pins the store order of x, w and h.
    entries[index].x = (short)nx;
    entry->h = (short)lh;
}
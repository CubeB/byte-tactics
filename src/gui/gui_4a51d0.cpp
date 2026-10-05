// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Started by Space Bunny Free (partial, 80.1%); finished by deepseek-v4.1-flash.
#include <string.h>

struct Glyph_004a50b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50b0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a50b0* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void* __stdcall FUN_004b7f30(void* glyphs, int c);
int FUN_004c1440();
int __stdcall FUN_004c1480(int a, char* text);
int FUN_004c1450();
void __stdcall FUN_004a50e0(char* dest, char* text, int p3, int x, int maxw, int style);


static inline int LineHeight_004a50b0()
{
    if (DAT_0051fba4->font == 0)
        return FUN_004c1450();
    return (int)((Glyph_004a50b0*)FUN_004b7f30(DAT_0051fba4->font->glyphs, 'I'))->height + 2;
}

static inline int Measure(char* word, int t)
{
    if (word == 0)
        return t;
    if (DAT_0051fba4->font == 0)
        return FUN_004c1480(FUN_004c1440(), word);
    for (char* n = word; *n; n++) {
        unsigned char ch = *n;
        unsigned short* g = (unsigned short*)FUN_004b7f30(DAT_0051fba4->font->glyphs, ch);
        if (g)
            t += *g;
    }
    return t;
}

// FUNCTION: 0x4a51d0
int __stdcall FUN_004a51d0(char* p2, char* text, int p4, int y, int maxw, int rem, int p7)
{
    int last = 0;
    int w;
    int k = 0;
    int i = 0;
    int len = strlen(text);
    while (text[k] != 0) {
        while (i != len && text[i] != ' ' && text[i] != '\r')
            i++;
        char saved = text[i];
        char* word = text + k;
        text[i] = 0;
        w = 0;
        w = Measure(word, w);
        if (w > maxw) {
            text[i] = saved;
            i = last;
            saved = text[i];
            text[i] = 0;
        } else if (saved != '\r' && i != len) {
            last = i;
            text[i] = saved;
            i++;
            continue;
        }
        FUN_004a50e0(p2, word, p4, y, maxw, p7);
        text[i] = saved;
        y += LineHeight_004a50b0() + 2;
        rem -= LineHeight_004a50b0() + 2;
        if (text[i] == 0)
            goto done;
        k = i + 1;
        if (rem <= 0)
            goto done;
        last = k;
        i = k;
    }
done:
    return y;
}

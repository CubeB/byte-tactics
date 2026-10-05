// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by opus. Names are provisional.
// MATCH. Draws one page of the help text (DAT_0051e63c) into the dialog's
// "TextRegion" gadgets, one gadget per line, with "&R", "&Y", "&G" colour
// sections drawn over the line through FUN_004afd80.
//
// What made it match (earlier notes misread the frame: they counted slot
// offsets without the pending argument pushes):
//  * The page scans are the zero-caller neighbour FUN_00476e90 (matched in
//    0x476e90.cpp), defined here unannotated so /Ob2 inlines it twice. This
//    spelling compiles to the same 86 bytes as that file's; inlined it must
//    walk two pointers in step (the test reads one, the body the other) and
//    declare found and count first. With one pointer MSVC carries the byte
//    load across the back edge (79.2%); with p and q declared first the
//    second scan loses its zero register in edx (87.7%).
//  * The table colour goes into its own local, `colour`, which is the fifth
//    argument of FUN_004afd80, not the gadget count. It shares count's slot
//    once count is dead, and `colourState = 0` is then a live store.
//  * The gadget index is count itself, incremented in the loop, so the
//    entry pointer is a strength-reduced induction variable set up after the
//    loop guard (a pointer local set up before the loop puts it first).
//  * `lineStart++; code = *lineStart;` (not `code = lineStart[1]`) gives
//    the entry pointer esi and lineStart ebp, and the closing '&' arm is
//    `colourState = 1; lineStart++;`.
#include <string.h>

#pragma pack(push, 1)

struct Entry_476ef0 {                   // 0x15b bytes
    char unknown_0[0x13];
    short x;                            // +0x13
    short y;                            // +0x15
    short field_17;                     // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colour_1f;                      // +0x1f
    char unknown_23[0x28 - 0x23];
    unsigned char field_28;             // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                    // +0xb6 (entry 0)
        char text[0x80];                // +0xb6
    } u;
    char unknown_136[0x15b - 0x136];
};

struct Holder_476ef0 {
    int unknown_0;
    Entry_476ef0* entries;              // +0x4
};

struct Menu_476ef0 {
    char unknown_0[1];
};

struct Game {
    char unknown_0[0x519];
    Menu_476ef0 menu;                   // +0x519
    char unknown_51a[0x531 - 0x51a];
    Holder_476ef0* dialog;              // +0x531
    char unknown_535[0x37ef2 - 0x535];
    int field_37ef2;          // +0x37ef2
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_0051e63c;
extern int DAT_0051e64c;
extern int DAT_0051e650;
extern int DAT_0051e66c;
extern unsigned char DAT_00507b70[];
extern char DAT_005119b8;

int __stdcall FindGadgetIndex(Entry_476ef0* gadgets, const char* name, int type);
void __stdcall FUN_004afec0(Menu_476ef0* menu);
void __stdcall FUN_004afd20(Menu_476ef0* menu, int value);
void __stdcall SelectFontForEntry(Entry_476ef0* gadgets, int index);
void* __cdecl GetFont();
int __stdcall FontHeight(void* font);
int __stdcall GetTextWidth(void* font, const char* text);
char* __stdcall Translate(const char* key);
void __stdcall AddTextGadget(Holder_476ef0* dialog, const char* name, char* text,
                            int x, int y, int w, int flags);
void __stdcall FUN_004a0bf0(Menu_476ef0* menu, const char* name, char* text,
                            int value);
void __stdcall FUN_004a0c70(Menu_476ef0* menu, const char* name, int value);
void __stdcall FUN_004afd80(Menu_476ef0* menu, char* text, int x, int y,
                            int count, int colour, float a, float b);

char* __stdcall FUN_00476e90(char* start, int lines, int page)
{
    if (!page) return start;
    int found = 0;
    int count = 0;
    char* p = start;
    char* q = start;
    while (*p) {
        char c = *q;
        if (c == (char)0xff) break;
        if (found) break;
        ++p;
        ++q;
        if (c == '\n') {
            ++count;
            if (count == page * lines) found = 1;
        }
    }
    return found ? q : 0;
}

// FUNCTION: 0x476ef0
void DrawHelpPage()
{
    Holder_476ef0* dialog = g_game->dialog;
    Entry_476ef0* gadgets = dialog->entries;
    int count;
    int colourState = 1;
    if (DAT_0051e63c == 0)
        return;

    if (DAT_0051e650 != 0) {
        DAT_0051e66c = gadgets[0].u.count;
        DAT_0051e64c = -1;
        DAT_0051e650 = 0;
    } else {
        gadgets[0].u.count = (short)DAT_0051e66c;
    }

    FUN_004afec0(&g_game->menu);
    FUN_004afd20(&g_game->menu,
                 FindGadgetIndex(gadgets, "TextRegion", 5));
    int idx = FindGadgetIndex(gadgets, "TextRegion", 0xe);
    Entry_476ef0* gp = &gadgets[idx];
    gp->field_28 = g_game->field_37ef2 + 1;
    SelectFontForEntry(gadgets, idx);

    int divisor = FontHeight(GetFont()) + 2;
    int linesPerPage = gp->h / divisor;
    int textX = gp->x + 5;
    int y = gp->y + divisor / 2;
    count = gadgets[0].u.count;
    DAT_0051e64c++;

    char* lineStart = FUN_00476e90(DAT_0051e63c, linesPerPage, DAT_0051e64c);
    if (!lineStart) {
        DAT_0051e64c = 0;
        lineStart = DAT_0051e63c;
    }
    char* nextPage = FUN_00476e90(DAT_0051e63c, linesPerPage, DAT_0051e64c + 1);

    if (nextPage == 0) {
        if (DAT_0051e64c == 0)
            FUN_004a0bf0(&g_game->menu, "MOREBAR", &DAT_005119b8, 0);
        else
            FUN_004a0bf0(&g_game->menu, "MOREBAR",
                         Translate("BACK TO START"), 0);
    } else {
        FUN_004a0bf0(&g_game->menu, "MOREBAR", Translate("MORE..."), 0);
    }
    FUN_004a0c70(&g_game->menu, "MOREBAR",
                 DAT_00507b70[g_game->field_37ef2 * 4 + 1]);

    char buf[0x80];

    for (int i = linesPerPage * DAT_0051e64c;
         i < (DAT_0051e64c + 1) * linesPerPage; i++) {
        AddTextGadget(dialog, "TextRegion", &DAT_005119b8, textX, y, -1, 2);
        count++;
        char* dst = gadgets[count].u.text;
        gadgets[count].flags = 0x411;
        gadgets[count].field_28 = gp->field_28;
        gadgets[count].colour_1f = DAT_00507b70[g_game->field_37ef2 * 4];
        memset(gadgets[count].u.text, 0, 0x80);

        y += divisor;
        while (*lineStart != '\n') {
            char c = *lineStart;
            if (c == 0)
                break;
            if (c == (char)0xff)
                break;
            if (c == '&') {
                if (colourState != 0) {
                    lineStart++;
                    char code = *lineStart;
                    colourState = 0;
                    int sel = code == 'R' ? 3
                            : code == 'Y' ? 2
                            : code == 'G' ? 1 : 3;
                    lineStart++;
                    int colour =
                        DAT_00507b70[g_game->field_37ef2 * 4 + sel];
                    int x = GetTextWidth(GetFont(), gadgets[count].u.text) + textX;
                    int ey = gadgets[count].y;
                    int k = 0;
                    char* q = lineStart;
                    while (k < 0x7f) {
                        if (*q == '&')
                            break;
                        buf[k] = *q;
                        k++;
                        q++;
                    }
                    buf[k] = 0;
                    FUN_004afd80(&g_game->menu, buf, x, ey, colour, 0x5e,
                                 1.0f, 0.25f);
                } else if (*lineStart == '&') {
                    colourState = 1;
                    lineStart++;
                }
            }
            *dst++ = *lineStart++;
        }
        lineStart++;
    }
}

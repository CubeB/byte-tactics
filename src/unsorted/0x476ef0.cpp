// Decompiled by deepseek-v4.1-flash. Names are provisional.
// FIRST PASS / PARTIAL. 30.3% (frame 0xb4 vs 0xac; 1059 vs 1124 bytes).
// Structure follows the disassembly but the following still differ:
//  - local frame is 8 bytes too large and the [esp+0x38]/[esp+0x30] spill
//    slots (gp and its copy) are not reproduced; the original keeps the
//    revived TextRegion gadget pointer in [esp+0x30] on loop back-edges.
//  - [esp+0x20] is one slot used both as the '&' colour state and then
//    overwritten with the palette colour (0x476f15 init 1, 0x477257 = 0,
//    0x477295 = colour); model it as one int to match.
//  - the '&' segment copy writes buf at the line-relative offset
//    (buf - line0 + src), not at buf[0].
//  - the 0x80-byte zero fill of the new entry text is rep stosd (memset).
//  - the MOREBAR/label branch (0x4770e1..0x477144), the two page scans and
//    the main line loop are structurally right but registers/order differ.
// The TextRegion gadget entry is 0x15b bytes; entry 0 holds the count at
// +0xb6 and the new entries hold the text at +0xb6.
#include <string.h>

#pragma pack(push, 1)

struct Entry_476ef0 {                   // 0x15b bytes
    char unknown_0[0x13];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
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
    char unknown_0[0x519];
};

struct Game_476ef0 {
    char unknown_0[0x519];
    Menu_476ef0 menu;                   // +0x519
    char unknown_51a[0x531 - 0x51a];
    Holder_476ef0* dialog;              // +0x531
    char unknown_535[0x37ef2 - 0x535];
    unsigned char field_37ef2;          // +0x37ef2
};
#pragma pack(pop)

extern Game_476ef0* g_game;
extern char* DAT_0051e63c;
extern int DAT_0051e64c;
extern int DAT_0051e650;
extern int DAT_0051e66c;
extern unsigned char DAT_00507b70[];
extern char DAT_005119b8;

int __stdcall FUN_0049fdf0(Entry_476ef0* entries, const char* name, int param_3);
void __stdcall FUN_004afec0(Menu_476ef0* menu);
void __stdcall FUN_004afd20(Menu_476ef0* menu, int value);
void __stdcall FUN_004a1810(Entry_476ef0* entries, int index);
void* FUN_004c1440();
int __stdcall FUN_004c1470(void* font);
int __stdcall FUN_004c1480(void* font, const char* text);
char* __stdcall FUN_004c5740(const char* key);
void __stdcall FUN_004ab1b0(Holder_476ef0* holder, const char* name, char* text,
                            int x, int y, int w, int flags);
void __stdcall FUN_004a0bf0(Menu_476ef0* menu, const char* name, char* text,
                            int value);
void __stdcall FUN_004a0c70(Menu_476ef0* menu, const char* name, int value);
void __stdcall FUN_004afd80(Menu_476ef0* menu, char* text, int x, int y,
                            int a, int b, float c, float d);

// FUNCTION: 0x476ef0
void FUN_00476ef0()
{
    Holder_476ef0* dialog = g_game->dialog;
    Entry_476ef0* gadgets = dialog->entries;
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
                 FUN_0049fdf0(gadgets, "TextRegion", 5));
    int idx = FUN_0049fdf0(gadgets, "TextRegion", 0xe);
    Entry_476ef0* gp = &gadgets[idx];
    gp->field_28 = g_game->field_37ef2 + 1;
    FUN_004a1810(gadgets, idx);

    void* font = FUN_004c1440();
    int divisor = FUN_004c1470(font) + 2;
    int linesPerPage = gp->h / divisor;
    int textX = gp->x + 5;
    int half = divisor / 2;
    int y = gp->y + half;
    int firstCount = gadgets[0].u.count;
    DAT_0051e64c++;

    char* lineStart = 0;
    if (DAT_0051e64c == 0) {
        lineStart = DAT_0051e63c;
    } else {
        char* p = DAT_0051e63c;
        int n = 0;
        while (*p != 0) {
            char ch = *p;
            if (ch == (char)0xff)
                break;
            if (lineStart != 0)
                break;
            p++;
            if (ch == '\n') {
                n++;
                if (n == linesPerPage * DAT_0051e64c)
                    lineStart = (char*)1;
            }
        }
        lineStart = lineStart ? p : 0;
    }
    if (lineStart == 0) {
        DAT_0051e64c = 0;
        lineStart = DAT_0051e63c;
    }

    char* nextPage = 0;
    if (DAT_0051e64c == -1) {
        nextPage = DAT_0051e63c;
    } else {
        char* q = DAT_0051e63c;
        int n = 0;
        if (*q != 0) {
            do {
                char ch = *q;
                if (ch == (char)0xff)
                    break;
                if (nextPage != 0)
                    break;
                q++;
                if (ch == '\n') {
                    n++;
                    if (n == (DAT_0051e64c + 1) * linesPerPage)
                        nextPage = (char*)1;
                    else
                        nextPage = 0;
                }
            } while (*q != 0);
        }
        nextPage = nextPage ? q : 0;
    }

    char* label;
    if (nextPage == 0) {
        if (DAT_0051e64c == 0)
            label = &DAT_005119b8;
        else
            label = FUN_004c5740("BACK TO START");
    } else {
        label = FUN_004c5740("MORE...");
    }
    FUN_004a0bf0(&g_game->menu, "MOREBAR", label, 0);
    FUN_004a0c70(&g_game->menu, "MOREBAR",
                 DAT_00507b70[g_game->field_37ef2 * 4 + 1]);

    int colourState = 1;
    char buf[0x80];
    int i = linesPerPage * DAT_0051e64c;
    Entry_476ef0* e = &gadgets[firstCount];
    for (; i < (DAT_0051e64c + 1) * linesPerPage; i++) {
        FUN_004ab1b0(dialog, "TextRegion", &DAT_005119b8, textX, y, -1, 2);
        e++;
        char* dst = e->u.text;
        e->flags = 0x411;
        e->field_28 = gp->field_28;
        e->colour_1f = DAT_00507b70[g_game->field_37ef2 * 4];
        memset(e->u.text, 0, 0x80);

        char* line0 = lineStart;
        char ch = *lineStart;
        y += divisor;
        if (ch != '\n') {
            do {
                if (ch == 0 || ch == (char)0xff)
                    break;
                if (ch == '&') {
                    if (colourState != 0) {
                        char code = lineStart[1];
                        lineStart++;
                        colourState = 0;
                        int sel;
                        if (code == 'R')
                            sel = 3;
                        else if (code == 'Y')
                            sel = 2;
                        else
                            sel = (code == 'G') ? 1 : 2;
                        lineStart++;
                        int colour =
                            DAT_00507b70[g_game->field_37ef2 * 4 + sel];
                        colourState = colour;
                        int x = FUN_004c1480(font, e->u.text) + textX;
                        short ey = e->y;
                        int n = 0;
                        char* q = lineStart;
                        while (*q != '&') {
                            buf[q - line0] = *q;
                            n++;
                            q++;
                            if (n >= 0x7f)
                                break;
                        }
                        buf[q - line0] = 0;
                        FUN_004afd80(&g_game->menu, buf, x, ey, firstCount,
                                     0x5e, 1.0f, 0.25f);
                    } else {
                        lineStart++;
                        colourState = 1;
                    }
                }
                *dst++ = *lineStart++;
                ch = *lineStart;
            } while (ch != '\n');
        }
        lineStart++;
    }
}

// Decompiled by deepseek-v4.1-flash. Names are provisional.
// TextRegion paged-text renderer (1124 bytes, __cdecl, no arguments).
// Structure follows the disassembly: two page scans, a MOREBAR label choice,
// then a per-line loop that fills one 0x15b-byte gadget entry per text line
// and draws the '&'-escaped coloured runs.  PARTIAL, 50.7%.
// Facts recovered from the disassembly (useful for the next attempt):
//  - the two page scans are exactly FUN_00476e90's body inlined twice; the
//    calls are FUN_00476e90(DAT_0051e63c, linesPerPage, DAT_0051e64c) and
//    FUN_00476e90(DAT_0051e63c, linesPerPage, DAT_0051e64c + 1).  The
//    special cases (param_3 == 0 -> return start) are the `if (DAT_0051e64c
//    == 0)` / `if (DAT_0051e64c == -1)` arms.  Writing them as real calls
//    does not get inlined and scores lower (36.9%).
//  - canonical local slots (E = esp after the four pushes): dv+0x10 scan n /
//    dst ptr, +0x14 scan n / loop i, +0x18 linesPerPage, +0x1c count,
//    +0x20 colourState, +0x24 textX, +0x28 dialog, +0x2c e->y (dead spill),
//    +0x30 gp, +0x34 y, +0x38 divisor, +0x3c..+0xbb buf[0x80].
// Remaining differences:
//  - frame 0xb0 vs 0xac: one extra dword local.  Ours spills dialog to a
//    later slot (original dialog is E+0x28, right after textX), and ours
//    spills linesPerPage to E+0x10 and y to E+0x18 whereas the original
//    keeps y in ebx for the whole function and linesPerPage at E+0x18.
//  - the inner '&' run-copy and the second page scan differ in block order
//    and register roles (original keeps the run base ebx = buf - line0).
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

int __stdcall FUN_0049fdf0(Entry_476ef0* gadgets, const char* name, int type);
void __stdcall FUN_004afec0(Menu_476ef0* menu);
void __stdcall FUN_004afd20(Menu_476ef0* menu, int value);
void __stdcall FUN_004a1810(Entry_476ef0* gadgets, int index);
void* __cdecl FUN_004c1440();
int __stdcall FUN_004c1470(void* font);
int __stdcall FUN_004c1480(void* font, const char* text);
char* __stdcall FUN_004c5740(const char* key);
void __stdcall FUN_004ab1b0(Holder_476ef0* dialog, const char* name, char* text,
                            int x, int y, int w, int flags);
void __stdcall FUN_004a0bf0(Menu_476ef0* menu, const char* name, char* text,
                            int value);
void __stdcall FUN_004a0c70(Menu_476ef0* menu, const char* name, int value);
void __stdcall FUN_004afd80(Menu_476ef0* menu, char* text, int x, int y,
                            int count, int colour, float a, float b);

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

    int divisor = FUN_004c1470(FUN_004c1440()) + 2;
    int linesPerPage = gp->h / divisor;
    int textX = gp->x + 5;
    int y = gp->y + divisor / 2;
    int count = gadgets[0].u.count;
    DAT_0051e64c++;

    char* lineStart;
    if (DAT_0051e64c == 0) {
        lineStart = DAT_0051e63c;
    } else {
        char* p = DAT_0051e63c;
        int n = 0;
        int found = 0;
        if (*p != 0) {
            do {
                char c = *p;
                if (c == (char)0xff)
                    break;
                if (found)
                    break;
                p++;
                if (c == '\n') {
                    n++;
                    if (n == linesPerPage * DAT_0051e64c)
                        found = 1;
                }
            } while (*p != 0);
        }
        lineStart = found ? p : 0;
    }
    if (lineStart == 0) {
        DAT_0051e64c = 0;
        lineStart = DAT_0051e63c;
    }

    char* nextPage;
    if (DAT_0051e64c == -1) {
        nextPage = DAT_0051e63c;
    } else {
        char* q = DAT_0051e63c;
        int n = 0;
        int found = 0;
        if (*DAT_0051e63c != 0) {
            do {
                char c = *q;
                if (c == (char)0xff)
                    break;
                if (found)
                    break;
                q++;
                if (c == '\n') {
                    n++;
                    if (n == (DAT_0051e64c + 1) * linesPerPage)
                        found = 1;
                }
            } while (*q != 0);
        }
        nextPage = found ? q : 0;
    }

    if (nextPage == 0) {
        if (DAT_0051e64c == 0)
            FUN_004a0bf0(&g_game->menu, "MOREBAR", &DAT_005119b8, 0);
        else
            FUN_004a0bf0(&g_game->menu, "MOREBAR",
                         FUN_004c5740("BACK TO START"), 0);
    } else {
        FUN_004a0bf0(&g_game->menu, "MOREBAR", FUN_004c5740("MORE..."), 0);
    }
    FUN_004a0c70(&g_game->menu, "MOREBAR",
                 DAT_00507b70[g_game->field_37ef2 * 4 + 1]);

    int colourState = 1;
    char buf[0x80];
    Entry_476ef0* e = &gadgets[count];
    for (int i = linesPerPage * DAT_0051e64c;
         i < (DAT_0051e64c + 1) * linesPerPage; i++) {
        FUN_004ab1b0(dialog, "TextRegion", &DAT_005119b8, textX, y, -1, 2);
        e++;
        char* dst = e->u.text;
        e->flags = 0x411;
        e->field_28 = gp->field_28;
        e->colour_1f = DAT_00507b70[g_game->field_37ef2 * 4];
        memset(e->u.text, 0, 0x80);

        char c = *lineStart;
        y += divisor;
        if (c != '\n') {
            char* line0 = lineStart;
            for (;;) {
                if (c == 0 || c == (char)0xff)
                    break;
                if (c == '&' && colourState != 0) {
                    char code = lineStart[1];
                    lineStart++;
                    colourState = 0;
                    int sel = code == 'R' ? 3
                            : code == 'Y' ? 2
                            : code == 'G' ? 1 : 3;
                    lineStart++;
                    colourState =
                        DAT_00507b70[g_game->field_37ef2 * 4 + sel];
                    int x = FUN_004c1480(FUN_004c1440(), e->u.text) + textX;
                    int ey = e->y;
                    int k = 0;
                    char* q = lineStart;
                    while (*q != '&') {
                        buf[q - line0] = *q;
                        k++;
                        q++;
                        if (k >= 0x7f)
                            break;
                    }
                    buf[q - line0] = 0;
                    FUN_004afd80(&g_game->menu, buf, x, ey, count, 0x5e,
                                 1.0f, 0.25f);
                } else if (c == '&') {
                    lineStart++;
                    colourState = 1;
                }
                *dst++ = *lineStart++;
                c = *lineStart;
                if (c == '\n')
                    break;
            }
        }
        lineStart++;
    }
}

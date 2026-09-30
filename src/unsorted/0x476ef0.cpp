// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// Partial, 71.3%. Page scans now use an inlineable helper. Corrected coloured-run
// buffer indexing and the full-width side read; register scheduling still differs.
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
    int field_37ef2;          // +0x37ef2
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

static char* PageStart_476ef0(char* start, int lines, int page) {
    if (!page) return start;
    char* p = start;
    int found = 0;
    int count = 0;
    while (*p != 0) {
        char c = *p;
        if (c == (char)0xff) break;
        if (found) break;
        ++p;
        if (c == '\n') {
            ++count;
            if (count == page * lines) found = 1;
        }
    }
    return found ? p : 0;
}

// FUNCTION: 0x476ef0
void FUN_00476ef0()
{
    Holder_476ef0* dialog = g_game->dialog;
    Entry_476ef0* gadgets = dialog->entries;
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

    char* lineStart = PageStart_476ef0(DAT_0051e63c, linesPerPage, DAT_0051e64c);
    if (!lineStart) {
        DAT_0051e64c = 0;
        lineStart = DAT_0051e63c;
    }
    char* nextPage = PageStart_476ef0(DAT_0051e63c, linesPerPage, DAT_0051e64c + 1);

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
                        buf[k] = *q;
                        k++;
                        q++;
                        if (k >= 0x7f)
                            break;
                    }
                    buf[k] = 0;
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

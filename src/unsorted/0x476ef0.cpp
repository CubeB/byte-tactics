// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 80.0% (was 71.3%). The page-scan merge and the window-copy loop are
// now fixed:
//  * Page scans: MSVC merges `while (*p) { char c = *p; ... }` into one
//    `mov al,[p]; test al,al`, so the original's memory guard cannot come from
//    one pointer. Writing the helper with TWO pointers that advance together,
//    condition on `p` and body read on `q`, breaks the load CSE: the guard and
//    latch become `cmp byte ptr [p],0` and the body keeps its own `mov al,[q]`.
//    Scan 1 is byte exact this way.
//  * Window copy loop: `while (k < 0x7f) { if (*q == '&') break; buf[k] = *q;
//    k++; q++; }` gives the original's top-tested '&' / latch k limit (the
//    `while (*q != '&')` form rotates and duplicates the '&' test). The outer
//    text loop must be `while (1)` (a `for(;;)` is rotated so the `c == 0`
//    break moves to the latch), and `if (c == '&') { if (colourState) ... else
//    ... }` (not `&&` / `else if (c == '&')`, which re-tests the byte).
// Remaining differences (all in the second inline scan and the frame):
// 1) the second scan keeps a live zero register in edx in the original, so it
//    emits `xor edx,edx; cmp ebp,edx`, `inc eax; cmp eax,edx` and
//    `cmp byte ptr [eax],0`; ours has `test ebp,ebp`, `lea eax,[edx+1];
//    test eax,eax` and `cmp byte ptr [eax],dl`. `if (lineStart == 0)`, a named
//    zero pointer local and an `(int)` cast do not change it.
// 2) 4-byte local homes are permuted: count/colourState are [esp+0x1c]/[esp+0x20]
//    here but swapped there; dialog/gp/divisor occupy [esp+0x30]/[esp+0x38]/
//    [esp+0x28] here but [esp+0x28]/[esp+0x30]/[esp+0x38] there (a rotation).
//    Not declaration-order driven: declaring count before colourState, dumping
//    locals, or reordering the found/count initialisations does not move them.
// 3) the entry pointer `e = &gadgets[count]` is computed after the loop guard
//    in the original (MSVC's loop-invariant preheader) and before it here.
//    Moving the assignment into the loop or recomputing it from the loop index
//    is far worse (60% / 46%).
// Tried and no better: tools/headers.py (no set matches, best 80%), guarded
// do/while scans, `unsigned char`/`char`, `p[0]`, `for(;;)`, swapping the
// helper's found/count declarations (75.8%), `#include <windows.h>` (68.5%).
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
    char* q = start;
    int found = 0;
    int count = 0;
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
            while (1) {
                if (c == 0)
                    break;
                if (c == (char)0xff)
                    break;
                if (c == '&') {
                    if (colourState != 0) {
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
                        while (k < 0x7f) {
                            if (*q == '&')
                                break;
                            buf[k] = *q;
                            k++;
                            q++;
                        }
                        buf[k] = 0;
                        FUN_004afd80(&g_game->menu, buf, x, ey, count, 0x5e,
                                     1.0f, 0.25f);
                    } else {
                        lineStart++;
                        colourState = 1;
                    }
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

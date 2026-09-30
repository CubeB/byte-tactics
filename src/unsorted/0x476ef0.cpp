// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// Partial, 71.3%. Page scans use an inlineable helper; coloured-run buffer
// indexing and the full-width side read are right. Remaining differences:
// 1) both inlined page scans: the original tests the loop guard with a memory
//    compare (`cmp byte ptr [p],0` then `je`) and loads the byte again in the
//    body (`mov al,[p]`), ours merges the two into `mov al,[p]; test al,al`.
//    Tried: guarded do/while, hoisting `char c`, `unsigned char* p`, `k < 0x7f`
//    loop form; the first three compile identically, the last is much worse.
// 2) 4-byte local homes are permuted: colourState/count are [esp+0x1c]/[esp+0x20]
//    here but [esp+0x20]/[esp+0x1c] in the original; divisor is [esp+0x28] here,
//    [esp+0x38] there; `dialog` is stored at [esp+0x2c] here, [esp+0x24] there
//    (the original re-reads a split live-range home [esp+0x28] that nothing
//    writes; ours does the same trick at [esp+0x30]); gp is stored at [esp+0x40]
//    here, [esp+0x38] there. Sizes and order of the stores already agree.
// 3) the original keeps `mov dword ptr [esp+0x20],0` (colourState = 0) between
//    `cmp al,0x52` and `jne`; ours dead-store-eliminates it.
// 4) the second inline initialises its count with an immediate in the original
//    (`mov dword ptr [esp+0x14],0`); ours reuses the zero register edx.
// 5) the window-copy loop (0x4772b1) is NOT rotated in the original (the '&'
//    test is the loop head, the k limit is the latch); every source form tried
//    for it (do/while, for(;;) with two breaks, plain while) is inverted and/or
//    peeled by MSVC5 into test-at-latch order (70.7%). Same for the page-scan
//    guard: an explicit `if (*p != 0) { do/while }` keeps the cmp-mem form but
//    costs 4.6% elsewhere (66.7%), and `p[0]` reads compile identically to `*p`.
// Tried by deepseek-v4.1 (all no better): `for(;;){ if (*p==0) break; ... }` for both
// page scans (still merges into `mov al,[p]; test al,al`, 70.7%). Reordering the count
// declaration (int count; before colourState, assigned later) does not move the slots
// (71.3%). #include <windows.h> is much worse (68.5%). The slot permutation above is
// not declaration-order driven; the phantom [esp+0x28]/[esp+0x30] re-reads come from
// MSVC5 splitting dialog/gp into a store home and a lower read home.
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

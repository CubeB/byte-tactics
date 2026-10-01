// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash 10 min timebox: one variant scored, moving the
// `int colourState = 1;` declaration from the function prologue to the
// loop preheader (just before `char buf[0x80];`). It drops the score to
// 74.8% (1112 bytes), because the initialiser store moves out of the
// prologue; the frame slot swap (colourState [esp+0x20] original vs
// [esp+0x1c] ours) is not reachable this way. Reverted, file unchanged.
// deepseek-v4.1-flash retry pass (later session): still 80.4%, no variant
// scored. New evidence from the baseline diff and from disassembling the
// '&' colour arm of our own build: the ternary (`cmp al,0x52 / mov eax,3 /
// jmp`), the second `lineStart++` (`inc ebp`), the table load and the
// `colourState = DAT_00507b70[...]` store all sit at the exact offsets and
// registers the original uses, and the else arm is the original's
// `cmp al,0x26 / jne / inc ebp / dec ebx / mov [slot],1` modulo the slot
// number. The only missing instruction there is the dead
// `mov dword ptr [esp+0x20], 0`. Our colourState gets slot [esp+0x1c] (the
// initialiser `colourState = 1` proves it: original stores 1 to [esp+0x20])
// while every other reused slot (0x10, 0x14, 0x24, 0x2c, 0x34) matches, so
// the dropped store and the slot permutation are the same frame-colouring
// effect: MSVC 5 renumbered colourState out of the original's slot and can
// then prove the store dead. Fixing the slot (declaration order, extra
// scratch local, or a shape that raises colourState's live range) is the
// remaining lever; three check runs this session, best unchanged.
// deepseek-v4.1-flash retry pass: still 80.4% (1124 vs 1112). Tested
// build/scratch/0x476ef0/vA (colourState=0 moved to block entry), vB (else arm
// store before lineStart++), vC (both): all score identically, and vA still
// does not emit the missing dead `mov [esp+0x20], 0` (MSVC drops it whichever
// order it is written). The dominant remaining difference is the MSVC 5
// frame-renumbering permutation, not the source: comparing objdump of our
// build/obj/unsorted/0x476ef0.obj against the exe shows our two mismatched
// store/load variable pairs (dialog store 0x2c load 0x30, gp store 0x40 load
// 0x38) are the same phenomenon as the original's (dialog store 0x24 load
// 0x28, gp store 0x38 load 0x30), just shifted, while linesPerPage (0x18) and
// textX (0x24) already land in the original slots. Chasing the pair offsets
// with declaration reorders was the previous pass's dead end too.
// deepseek-v4.1-flash extra pass (issue #3620): hoisted an uninitialised
// `int count;` above `int colourState = 1;` and left the assignment at its old
// line, aiming to swap the two frame slots (original count [esp+0x1c] /
// colourState [esp+0x20], ours the reverse) and to let the dead
// `mov [esp+0x20], 0` reappear. Result: byte-identical, 1112 bytes, still
// 80.4%, so this slot pair is not declaration-order driven either.
// Timebox note (deepseek-v4.1-flash final pass): stopped at 80.4%, unchanged
// from the file this pass inherited (1124 original bytes vs 1112 ours).
// What still differs (all confirmed in the last check.py diff):
//  * The original emits `mov dword ptr [esp+0x20], 0` (colourState = 0) right
//    after `cmp al, 0x52` in the '&' colour arm; ours drops that store
//    entirely (MSVC 5 treats it as dead before the unconditional
//    `colourState = DAT_00507b70[...]` overwrite). That is 8 of the 12 missing
//    bytes and the single biggest win left.
//  * Scan 2 of the inlined page helper: original keeps a live zero register
//    (xor edx,edx at 0x477069) used for `cmp ebp,edx`, `mov [DAT],edx`,
//    `cmp eax,edx`, `mov [esp+0x10],edx`, while `mov [esp+0x14], 0` and the
//    loop guard `cmp byte ptr [eax], 0` use immediates. Ours has exactly the
//    mirror image (test forms plus register forms where the original uses
//    immediates).
//  * Else arm store order: original `inc ebp / mov [slot],1 / dec ebx`; ours
//    `inc ebp / dec ebx / mov [slot],1`.
//  * Frame slot permutation and the e = &gadgets[count] preheader placement
//    (see the notes below, unchanged).
// Untested scratch variants prepared in build/scratch/0x476ef0/ before the
// timebox fired (none scored, base.cpp is this file): vA.cpp moves
// `colourState = 0;` to the first statement of the `if (colourState != 0)`
// block (maybe schedules the store before the ternary join where DSE cannot
// see the kill); vB.cpp swaps the else arm to `colourState = 1; lineStart++;`;
// vC.cpp is both; vD.cpp puts `colourState = 0;` after the ternary. Score them
// with check.py --sym first.
// deepseek-v4.1-flash 10 min timebox: no variant scored. Confirmed from the
// original's full disassembly that colourState is ONE slot, [esp+0x20], with
// exactly five uses: store 1 at 0x476f15, load at 0x477244, store 0 at
// 0x477257, store the DAT byte (zero-extended, mov cl + mov [esp+0x20],ecx)
// at 0x477295, store 1 at 0x477303. The missing 0 store is therefore a plain
// DSE difference, not a second variable: the kill store is unconditional on
// every path in our source, so MSVC 5 removes it. Nothing cheap left.
// 80.4% (was 80.0%). The `&&`-free else arm needs the re-test of the byte, but
//  * the one written on the local `c` is folded away by MSVC 5 (it still knows
//    al == '&' on that edge), so it is `else if (*lineStart == '&')`: the load is
//    CSE'd with the loop-top load and only the `cmp al, 0x26 / jne` survives,
//    which is exactly the original's dead re-test at 0x4772fe. That is +4 bytes.
// Still open (unchanged from the previous notes):
//  * Scan 2 allocates `found` in memory ([esp+0x10]) and `count` in memory
//    ([esp+0x14]), while scan 1 of the same inlined helper gets registers
//    (ebp and ecx). At scan 2 ebp/esi/edi/ebx are all taken (lineStart,
//    gadgets, gp, y) and only eax/ecx/edx are free, so the original's shape is
//    a register-pressure effect, not a source shape. The original also
//    materialises the constant 0 once in edx and uses it four times
//    (0x47706b, 0x47706f, 0x47707d, 0x47708a), which is why its `if (!page)`
//    test is `cmp eax, edx` and not `test eax, eax`.
//  * Frame slot permutation: ours is dst,i,lines,colourState,count,textX,
//    divisor,ey,dialog,y,gp; the original's is dst,i,lines,count,colourState,
//    textX,dialog,ey,gp,y,divisor. NOT declaration-order driven: moving
//    `int count` above `int colourState` leaves the emitted map unchanged
//    (verified with a /Fa listing aligned against the .obj).
//  * The original's frame has two loads of never-stored slots ([esp+0x28] for
//    dialog at 0x4771b4 and [esp+0x30] for gp at 0x4771ac, whose stores went to
//    0x24 and 0x38), an MSVC 5 frame-renumbering bug. We reproduce the same
//    anomaly on the same two variables, just at other offsets.
//  * In the else arm the original orders the three instructions
//    `inc ebp / mov [colourState], 1 / dec ebx`; ours emits the store last.
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
//    ... }` (not `&&`). NOTE, corrected: the else arm DOES need a re-test of
//    the byte, and `else if (c == '&')` does not produce one, because the test
//    is then on the local and MSVC 5 still knows al == '&' on that edge. It
//    has to be `else if (*lineStart == '&')`; see the note at the top.
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
// deepseek-v4.1-flash pass 2 (confirmed in build/obj, no byte gained): in our
// .obj the colour arm is byte-for-byte the original's shape (mov al,[ebp+1] /
// inc ebp / dec ebx / cmp al,0x52 / jne ...) minus the dead store 0, so the
// ternary, the two lineStart++ positions and `if (colourState)` are all right;
// the original compiler simply failed to delete that one dead store. A
// store-to-a-local DSE sees the kill at 0x477295 only across the ternary's
// conditional branches, so the store must have been emitted where a
// block-local pass could not reach it; writing colourState = 0 as a separate,
// named temp assignment (`sel` style) or after the ternary join does not
// reproduce it, so the source shape that defeats MSVC's DSE is still unknown.
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
                 FUN_0049fdf0(gadgets, "TextRegion", 5));
    int idx = FUN_0049fdf0(gadgets, "TextRegion", 0xe);
    Entry_476ef0* gp = &gadgets[idx];
    gp->field_28 = g_game->field_37ef2 + 1;
    FUN_004a1810(gadgets, idx);

    int divisor = FUN_004c1470(FUN_004c1440()) + 2;
    int linesPerPage = gp->h / divisor;
    int textX = gp->x + 5;
    int y = gp->y + divisor / 2;
    count = gadgets[0].u.count;
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
                    } else if (*lineStart == '&') {
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

// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// 2026-10-01 retry 4 (deepseek-v4.1-flash): best stays 60.3%, 1649 bytes. New
// facts: the t-vs-walker tie is independent of the x phi. The x-register
// variant (`x += w; x -= Measure()`, 58.1%) moves i's home to frame+0x14 (the
// original's i slot) but t still lands in edx and the bound is still spilled,
// so enregistering x does not force t to memory. Forcing t out of a register
// was tried with int[2], a struct member, unsigned, long, swap-declaration and
// `t = t + 1`; every one is byte-identical to this 60.3% shape (60.3%, 1649).
// Removing windows.h drops to 55.7%. So the loop allocation is a genuine
// compiler-state tie; the remaining differences are the t/bound register swap
// (diff slots frame+0x10 vs frame+0x14) and the downstream rect base shift.
// 2026-10-01 (deepseek-v4.1-flash): best stays 60.3%, 1649 bytes. New scratch
// results (build/scratch/0x4a56b0). The loop: a guarded `if (i < bound) { do {
// ... } while (i < bound); }` (v10) and a plain `for`/`while (i < bound)` (v3,
// v4, v5, v6, v7, declaration order swapped too) all land on the 49.3% mirror
// (i memory, t in edi, bound in ecx); the unguarded `while (1)` stays 60.3%.
// So the mirror is not a declaration-order or loop-guard question.
// The x phi: BOTH `x += w; x -= Measure();` (v1) and `x = w + x; x -=
// Measure();` (v8) DO move x out of the stack and into ecx incoming / ebp
// outgoing, with no spill slot, exactly as the original. But each scores only
// 58.1%, 1647 bytes: MSVC then accumulates `w` into ecx (`add ecx, edx; mov
// ebp, ecx`) where the original loads w into ebp first (`movsx ebp, w; add
// ebp, ecx`), and the changed rect base then shifts the whole second half. So
// the spill-free x is reachable; what is still missing is the w-first operand
// order, which the natural `w + x` spelling does not give because MSVC
// reassociates into the x register.
// 2026-09-30 retry 3 (deepseek-v4.1-flash): best stays 60.3%, 1649 bytes. Tested
// twelve more loop spellings from scratch (build/scratch/0x4a56b0): guarded
// for / while / do-while and `for (;;)` with a manual break all land on the
// 49.3% mirror (i in memory, t in edi); `i > count` 55.1%, an explicit pointer
// walk 55.2%, a goto-spelled type block 41.1%, and a ternary x computed before
// the rect 41.4%. The one new datum: moving `int t = 0;` above
// `Entry* entries` scores 59.8% and only reorders the prologue (the field_14
// store moves after the arg load); it does NOT demote t, confirming the
// spill is not a declaration-order question. The target keeps bound in ecx,
// walk pointer in edx and t at frame+0x10; the unguarded shape keeps t in edx
// and spills the bound, and no plain C++ spelling moves that choice.
// 2026-09-30 retry (deepseek-v4.1-flash): best stays 60.3%, 1649 bytes. Confirmed
// again that every guarded loop shape (while (i < bound), for (; i < bound; i++),
// declaration order either way) is the 49.3% mirror: i lands in memory, the tab
// counter t in edi, bound in ecx, pointer in edx. The winning unguarded
// `while (1)` + explicit `break` keeps i in edi but MSVC does not hoist the bound
// to a register, so it spills the bound to [esp+0x14] and gives the free edx to t
// (original: bound ecx, t in memory). An explicit `Entry* e = &entries[1]` with
// e++ copy-propagates away and is byte-identical to the 60.3% version, so it does
// not consume edx. Variants tried with --sym and no better than 60.3%: guarded
// while/for (49.3%), named `int cnt` bound (48.4%), explicit index pointer (60.3%,
// identical), x assigned in all three arms as an explicit phi (42.5%, grows the
// frame), a named nx temp (identical) and `unsigned int i` (identical).
// PARTIAL: 60.3%, 1649 against 1662 bytes. What the function does: it walks the
// entry list of a layout object looking for the entry whose tab number matches
// entry[index]'s tab, sets the language from that entry, then lays the text out
// (right/centre/left), draws the text, and finally either draws a bevel
// rectangle or highlights a single character in the string.
//
// What moved the number this session, and it is the one thing every earlier
// session missed: the loop must be written as an UNGUARDED `while (1)` with the
// bound test as the first statement and an explicit `break`, NOT as a `for`.
//     while (1) {
//         if (i >= entries[0].b6.count + 1)
//             break;
//         ...
//         i++;
//     }
// That alone took 49.3% to 60.3%. It is what puts the loop counter `i` in edi,
// which is what the original does. With a `for` (or with a plain
// `while (i < ...)`) MSVC 5 keeps `i` memory resident, folds the preheader test
// to the constant `cmp ecx, 1`, and hands edi to the tab counter `t` instead;
// every later edi/i use then differs and the whole tail shifts. So the
// register-allocation question that three earlier sessions gave up on is
// actually a LOOP SHAPE question, not a variable-ordering question. Declaration
// order, `t++` vs `t = t + 1`, `i <= count`, a named bound, a named tab value
// and a hoisted `Entry*` are all byte-identical to the winning shape.
//
// Still differs (see the diff): the original keeps the loop bound
// `entries[0].b6.count + 1` in ecx and spills the tab counter `t` to
// frame+0x10, with `i`'s home at frame+0x14. This version keeps the bound in a
// memory temp at frame+0x14 and puts `t` in edx, with `i`'s home at
// frame+0x10. A named local for the bound does not fix it: the named local then
// takes edi away from `i` (48.4%), because MSVC gives a named loop-bound local
// a callee-saved register while an expression gets a scratch.
// 2026-09-30 retry 2 (deepseek-v4.1): re-trialled the loop, nothing improved on
// 60.3%. The flipped guard `entries[0].b6.count + 1 <= i` compiles
// byte-identically to the `i >= ...` winner (MSVC canonicalises the compare, the
// bound still ends up in memory). The counter as `int t[1]` with t[0] uses is
// byte-identical too, so MSVC promotes the element and a memory home is not
// reachable that way. A `const int bound` local steals edi from `i` (48.4%). A
// second break test at the loop bottom (48.7%) and wrapping the loop in
// `if (i < count + 1) { while (1) ... }` (49.3%) both reproduce the guarded
// shape: bound in ecx, counter in edi, index in memory. Every pre-test plus
// bottom-test shape lands there, so the target allocation (index edi, counter in
// memory, bound ecx, walk pointer edx) only falls out of the single-test
// unguarded shape, where MSVC chooses to spill the bound instead of the counter.
// 2026-09-30 (deepseek-v4.1): the guarded loop was re-tried in the exact shape
// that MATCHES the sibling 0x4a53c0 (`int i; int t = 0;
// for (i = 1; i < entries[0].b6.count + 1; i++)`, t in a register there):
// 49.3%, ours 1656 bytes. MSVC folds the preheader to `cmp ecx, 1`, puts i in
// memory and t in edi, the mirror image of the target (i in edi with a home
// at [esp+0x14], t in memory at [esp+0x10], bound in ecx, entry ptr in edx).
// The home slot of i is genuine: the original writes edi to [esp+0x14] at both
// loop exits (0x4a5731, 0x4a5739) and reloads it at 0x4a599b after the
// Measure calls clobber edi. So the 60.3% `while (1)` shape already has the
// right i/t split and only the bound-vs-t register choice is inverted.
// Also tried and byte-identical to the winner: `t = t + 1`, a named `int lang`
// for the call argument, `int i; i = 1;` instead of `int i = 1;`, a named
// `int cnt` for `entries[0].b6.count`, `!(i < ...)` for the bound test, and a
// dead `t = t;` in the break arm. Worse: hoisting `entries[index].tab` into a
// named local (37.2% as `int`, 35.6% as `signed char`), an
// `Entry_004a56b0* e = &entries[i]` (50.2%), a hoisted `void* surf` (50.3%),
// a guarded `do/while` (49.3%) and two independent `if`s instead of `else if`
// (27.8%, the second arm then also runs on the right-align path).
//
// The SECOND remaining difference, and the one that shifts the most bytes: the
// original resolves the x phi (right-align / centre-align / no-align) in
// REGISTERS, with the incoming x in ecx and the outgoing x in ebp
// (`mov ebp, ecx` at 0x4a587e, then each arm rewrites ebp from ecx). Here MSVC
// gives x a stack home at frame+0x1c (`mov dword ptr [esp+0x1c], ebp`) and
// reloads it after every call, which costs one extra dword slot and so pushes
// the whole rect down by four (ours 0x20/0x24/0x28/0x2c, the original
// 0x1c/0x20/0x24/0x28). It also makes the right arm compute `w - measure` and
// add x afterwards instead of the original's `w + x` before the call and
// `- measure` after. This is the same "phi through the stack" problem the
// matched sibling 0x4a53c0 hit, and none of these moved it: `short x` (52.3%),
// `int x; x = rect.left;` as a separate statement, a full ternary chain, the
// arms as `if (!(align&4)) { if (align&2) ... } else ...` (55.5%), a named
// `int mw` for each Measure result, an extra `int x2 = x` copy used by the
// first draw call (53.6%), a named `int w`, and hoisting the surface pointer
// (50.3%). The centre arm also reassociates: the original computes
// `w/2 + x` and then `- measure/2`, we compute `w/2 - measure/2` and then
// `+ x`, and the `align & 2` test must stay a separate `else if` block.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a56b0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int align;                         // +0x1b
    int colours;                       // +0x1f
    int image;                         // +0x23
    char unknown_27[1];
    signed char tab;                   // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xbc - 0xb6];        // +0xb6
    } b6;
    void* surface;                     // +0xbc
    char unknown_c0[0xd6 - 0xc0];
    int language;                      // +0xd6
    char unknown_da[0x147 - 0xda];
    unsigned char field_147;           // +0x147
    unsigned char field_148;           // +0x148
    char unknown_149[0x15b - 0x149];
};

struct Holder_004a56b0 {
    int current;                       // +0x00
    Entry_004a56b0* entries;           // +0x04
};

struct Glyph_004a56b0 { unsigned short width, height; };

struct List_004a56b0 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct LanguageRoot_004a56b0 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a56b0* language;           // +0x14
};

struct Class_004a56b0 {
    char unknown_00[0x08];
    void* field_08;                    // +0x08
    void* field_0c;                    // +0x0c
    char unknown_10[0x14 - 0x10];
    void* field_14;                    // +0x14
    Holder_004a56b0* holder;           // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colours[256];          // +0x8b2, +0x8b3, +0x8b4
};

struct Rect_004a56b0 { int left, top, right, bottom; };
#pragma pack(pop)

extern LanguageRoot_004a56b0* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
void __stdcall FUN_004c14f0(void* surface, char* text, int x, int y, int maxw);
int __stdcall FUN_004bf6f0(void* surface, Rect_004a56b0* rect, int colour);
void __stdcall FUN_004bfe10(void* surface, Rect_004a56b0* rect);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a56b0* rect, int param);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            unsigned char colour);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
int __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                           int rem, int style);

static inline int Measure_004a56b0(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    while (*p != 0) {
        char ch = *p;
        Glyph_004a56b0* glyph = (Glyph_004a56b0*)FUN_004b7f30(
            DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

static inline int LineHeight_004a56b0()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return (int)((Glyph_004a56b0*)FUN_004b7f30(
        DAT_0051fba4->language->glyphs, 0x49))->height + 2;
}

// FUNCTION: 0x4a56b0
void __stdcall FUN_004a56b0(Class_004a56b0* obj, int index)
{
    obj->field_14 = obj->field_0c;
    Entry_004a56b0* entries = obj->holder->entries;

    int i = 1;
    int t = 0;
    while (1) {
        if (i >= entries[0].b6.count + 1)
            break;
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
        i++;
    }
    if (i == entries[0].b6.count + 1) {
        FUN_004c1420(DAT_0051fba4->current);
        i = -1;
    }


    if (entries[index].x == -1)
        entries[index].x = (short)((entries[0].w - Measure_004a56b0(entries[index].b6.text)) / 2);

    Rect_004a56b0 rect;
    if (entries[index].type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = entries[index].x;
        rect.top = entries[index].y;
    }
    rect.right = entries[index].w + rect.left - 1;
    rect.bottom = entries[index].h + rect.top - 1;

    if (entries[index].image != 0)
        FUN_004bf6f0(entries->surface, &rect, obj->colours[entries[index].image]);

    int x = rect.left;
    if (entries[index].align & 4) {
        x = entries[index].w + x - Measure_004a56b0(entries[index].b6.text);
    } else if (entries[index].align & 2) {
        x = entries[index].w / 2 + x - Measure_004a56b0(entries[index].b6.text) / 2;
    }

    if (i != -1 && (entries[index].align & 8)) {
        FUN_004c13a0(obj->colours[0], FUN_004c13f0());
        FUN_004c14f0(entries->surface, entries[index].b6.text, x + 1, rect.top + 3, -1);
    }

    FUN_004c13a0(entries[index].colours, FUN_004c13f0());

    if (i == -1) {
        int lh = LineHeight_004a56b0();
        if (rect.bottom - rect.top > lh * 2)
            FUN_004a51d0(entries->surface, entries[index].b6.text, x, rect.top,
                         rect.right - rect.left + 1,
                         rect.bottom - rect.top + 1, entries[index].colours);
        else
            FUN_004a50e0(entries->surface, entries[index].b6.text, x, rect.top,
                         rect.right - rect.left + 1, entries[index].colours);
    } else {
        FUN_004c14f0(entries->surface, entries[index].b6.text, x, rect.top, -1);
    }

    if (entries[index].field_148 & 1) {
        Entry_004a56b0* entries2 = obj->holder->entries;
        Rect_004a56b0 rect2;
        if (entries2[index].type == 0) {
            rect2.left = 0;
            rect2.top = 0;
        } else {
            rect2.left = entries2[index].x;
            rect2.top = entries2[index].y;
        }
        rect2.right = entries2[index].w + rect2.left - 1;
        rect2.bottom = entries2[index].h + rect2.top - 1;
        FUN_004bfe10(entries2->surface, &rect2);
        FUN_004bf4d0(entries2->surface, &rect2, -0x14);
        obj->field_14 = obj->field_08;
        return;
    }

    unsigned char c = entries[index].field_147;
    if (c != 0) {
        char pat[2];
        pat[0] = (char)c;
        pat[1] = 0;
        char buf[0x7c];
        strcpy(buf, entries[index].b6.text);
        char* p = strstr(buf, pat);
        if (p != 0) {
            *p = 0;
            int y = rect.top;
            int x0 = rect.left;
            int w1 = Measure_004a56b0(buf);
            x0 += w1;
            int w2 = Measure_004a56b0(pat);
            int lh1 = LineHeight_004a56b0();
            int lh2 = LineHeight_004a56b0();
            FUN_004be950(entries->surface, x0, lh1 + y - 1, x0 + w2 - 1,
                         lh2 + y - 1, obj->colours[2]);
        }
    }

    obj->field_14 = obj->field_08;
}

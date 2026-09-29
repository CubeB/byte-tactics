// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 92.1% (665 bytes against 640). Every instruction from the prologue
// to the last search loop matches the original, including the two inlined name
// searches; what is left is one instruction in the third search and the closing
// float block. Details at the bottom.
//
// What the function is: the list gadget's "put line N of the entry called NAME
// at the top of the window" step, the one 0x4a99c0 (scroll down) calls at its
// end with `(char*)me + 2`. It finds the table entry whose +0x02 name matches,
// stores the line into that entry's +0xba, makes the language entry of the
// entry's tab (+0x28) current, works out the pixel step of one line from the
// current font, and when the line has fallen out of the visible window moves the
// window's top (+0xbc) to it, clamped to +0xbe. It then finds the entry of type
// 4 that shares this entry's +0x01 byte (the scrollbar), rescales that
// scrollbar's +0x140 from this entry's +0x136, and stamps +0xcca.
//
// The three structural facts that decide the code, all confirmed against the
// original and all worth double digit percentages:
//
//  1. Both name searches are the `static inline FindEntry` helper that the
//     matched siblings 0x4a0090, 0x4a0180, 0x4a0200 and 0x49ff90 use. Written
//     out by hand as `int found = -1; for (...) { found = i; break; }` the same
//     function scores 30.8% instead of 86.6%: the helper's `return i` /
//     `return -1` pair is what puts the -1 into the loop's exit block
//     (`or ecx,0xffffffff` at 0x4a2e90) and frees the index register, and the
//     caller's own index has to be declared AFTER the first call so it can have
//     that register.
//  2. The test at 0x4a2f7d is a short circuit OR, not an AND. `jg` jumps INTO
//     the block and the following `cmp di,bx / jge` jumps over it, so the source
//     is `sel > step + last - 1 || sel < last`.
//  3. `k` (the type 4 entry's index) and `pkind` (the byte it is matched on)
//     have to be declared at function scope, and `k` must NOT be initialised.
//     Declared inside the if, or initialised, MSVC 5 gives one of them a stack
//     home, pushes a fifth register and moves every [esp+N] displacement
//     (77.3% and 50.1%). With the pair at function scope the not-found path
//     compiles to the original's register with one instruction still missing,
//     see (a) below.
//
// What still differs:
//
//  a. 0x4a3037. The original's not-found path for the type 4 search is
//     `xor ecx,ecx`, that is `k` zeroed, which is what `int k = 0;` would give.
//     With the initialiser present MSVC 5 materialises k in a stack slot
//     instead and the whole function drops to 77.3%, so the initialiser is left
//     off here and this one instruction reads a stale argument slot instead of
//     zero. Behaviourally the original scans entry 0 when the list has no type 4
//     entry sharing the +0x01 byte; this build indexes whatever is in that slot.
//     That is the one place where the partial is not behaviour faithful.
//
//  b. 0x4a3077 and 0x4a307f. The original multiplies and divides with the
//     integer memory forms `fimul dword [esp+0x18]` / `fidiv dword [esp+0x18]`;
//     this file gets `fild` / `fmulp st(1),st` / `fild` / `fdivp st(1),st`, four
//     instructions longer. MSVC 5 only picks `fimul m32int` when the right
//     operand is a plain int variable; giving the two divisors int locals does
//     produce the memory forms, but they then take stack homes of their own and
//     the temporaries move to +0x20 and +0x10 (76.4%).
//
//  c. 0x4a308f to 0x4a30a2. The original is `test ah,0x40 / jne` with the
//     branch landing on the out of line `fstp st(0)`, and the store arm ending
//     in an unconditional `jmp` over that `fstp` so the two arms share one copy
//     of the tail: that is the code for "store when field_140 <= quotient", laid
//     out the way MSVC lays out an if/else. This file gets `test ah,0x41 / je`
//     with the branch landing on a second, out of line copy of the whole tail
//     (the `fstp st(0)` at 0x4a262 here is on the store path instead). Every
//     relational spelling of `<=` tried here compiles to `test ah,0x41` or
//     `test ah,0x1`; `!=` does produce 0x40 but then branches the other way
//     round the block, and writing the statement as a ternary, which is what
//     would give the if/else layout, makes MSVC spill the quotient to a qword
//     temporary (`fstp QWORD PTR` / `fcom QWORD PTR`), twelve instructions
//     worse (73.8%).
//
// Suspected original bugs: none beyond the two edges noted above. The type 4
// search is unguarded (no entry of type 4 sharing the byte means entry 0 is
// rescaled), and the second FindEntry result is used without a -1 check, which
// is safe only because the name matched on the way in and nothing has changed it
// since.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a2e40 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19;                    // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    char unknown_c0[0xd6 - 0xc0];
    int id;                            // +0xd6
    char unknown_da[0x136 - 0xda];
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140;                   // +0x140
    char unknown_142[0x15b - 0x142];
};
#pragma pack(pop)

struct List_004a2e40 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct Holder_004a2e40 {
    int current;                       // +0x00
    Entry_004a2e40* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a2e40* list;               // +0x14
};

#pragma pack(push, 1)
struct Class_004a2e40 {
    char unknown_00[0x18];
    Holder_004a2e40* holder;           // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

extern Holder_004a2e40* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
int FUN_004c1450();

static inline int FindEntry(Entry_004a2e40* entries, char* name)
{
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a2e40
void __stdcall FUN_004a2e40(Class_004a2e40* param_1, char* param_2, int param_3)
{
    Entry_004a2e40* entries = param_1->holder->entries;
    int k;
    unsigned char pkind;
    int found = FindEntry(entries, param_2);
    if (found == -1)
        return;

    Entry_004a2e40* me = &entries[found];
    me->field_ba = param_3;

    int n = 0;
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size;
    if (DAT_0051fba4->list == 0)
        size = FUN_004c1450();
    else
        size = *(unsigned short*)(FUN_004b7f30(DAT_0051fba4->list->glyphs, 0x49) + 2) + 2;
    int step = (me->field_19 - 2) / (size + 1);
    short last = me->field_bc;
    short sel = me->field_ba;
    if (sel > step + last - 1 || sel < last) {
        if (me->field_be != 0)
            me->field_bc = sel;
        if (me->field_bc > me->field_be)
            me->field_bc = me->field_be;
        int other = FindEntry(entries, param_2);
        Entry_004a2e40* peer = &entries[other];
        pkind = peer->kind;
        for (i = 1; i < entries->count + 1; i++) {
            if (entries[i].type == 4 && entries[i].kind == pkind) {
                k = i;
                break;
            }
        }
        Entry_004a2e40* e3 = &entries[k];
        if ((double)e3->field_140 <= (double)e3->field_136 * me->field_bc / me->field_be)
            e3->field_140 = (short)((double)e3->field_136 * me->field_bc / me->field_be);
    }
    param_1->field_cca = 1;
}

// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 96.0% (659 bytes against 640). Every instruction matches the original
// except the layout of the closing float block: the original shares one function
// tail (the store path ends with `jmp 0x4a30a4` over the out of line `fstp st(0)`)
// while this build duplicates the epilogue into both arms of the inner `if`.
// What the function is: the list gadget's "put line N of the entry called NAME
// at the top of the window" step, the one 0x4a99c0 (scroll down) calls at its
// end with `(char*)me + 2`.
//
// Structural facts (space-bunny-free) still hold: the two name searches are the
// `static inline FindEntry` helper (return i / return -1). The third search is
// the same idea but an inline helper returning 0 (`FindKind`), which is what
// produces the original's `xor ecx,ecx` at 0x4a3037; a hand-written `int k = 0`
// instead gives k a stack home and costs double digit percent. The test at
// 0x4a2f7d is a short circuit OR: `sel > step + last - 1 || sel < last`.
//
// Fixed here (deepseek-v4.1-flash): the quotient must be `float`, not `double`
// (`float q = (float)e3->field_136 * me->field_bc / me->field_be;`). With float,
// MSVC 5 emits the original's integer-memory FPU forms `fimul [esp+0x18]` /
// `fidiv [esp+0x18]`; with double it emits fild/fmulp/fdivp, four instructions
// longer. The compare then has to cast the field to float too, so the single
// `fcomp st(1)` (not `fld st(1)`/`fcompp`).
//
// What still differs:
//  a. 0x4a30a0. The original store arm is `call _ftol / mov [esi+0x140],ax /
//     jmp 0x4a30a4` and the equal arm is an out of line `fstp st(0)` falling
//     into the shared tail at 0x4a30a4. This build tail-duplicates the epilogue
//     into both arms instead (the store arm ends the function, the equal arm has
//     its own copy). Tried: `==` with empty then, `!((==))`, goto forms, a temp
//     short, a ternary, moving `param_1->field_cca = 1;` into the branches;
//     none make MSVC emit the single shared tail.
//
// Suspected original bugs: two edges. The type 4 search (`FindKind`) returns 0
// when no entry of type 4 shares the +0x01 byte, so the rescale then reads and
// writes entry 0 (the list header) instead of doing nothing. And the second
// FindEntry result is used without a -1 check, which is safe only because the
// name matched on the way in and nothing has changed it since.
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

static inline int FindKind(Entry_004a2e40* entries, unsigned char kind)
{
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// FUNCTION: 0x4a2e40
void __stdcall FUN_004a2e40(Class_004a2e40* param_1, char* param_2, int param_3)
{
    Entry_004a2e40* entries = param_1->holder->entries;
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
        Entry_004a2e40* peer = &entries[FindEntry(entries, param_2)];
        unsigned char pkind = peer->kind;
        Entry_004a2e40* e3 = &entries[FindKind(entries, pkind)];
        float q = (float)e3->field_136 * me->field_bc / me->field_be;
        if ((float)e3->field_140 != q)
            e3->field_140 = (short)q;
    }
    param_1->field_cca = 1;
}

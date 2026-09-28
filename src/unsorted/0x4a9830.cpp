// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// The list gadget's scroll-up step, the sibling of the scroll-down step
// 0x4a99c0. It first does what 0x4a99c0 does: picks the entry of type 7 whose
// group number matches entry `index` and makes that entry's id the current
// one (falling back to the current id of the list holder). Then it works out
// how far the visible window moves per line and, when the selected line is
// still inside the window and the window has not run off the top, moves the
// selection one line up, scrolls the window if needed and refreshes the
// gadget. A selection on line 0 is not moved, and a line whose text starts
// with "&G" is not moved either.
//
// Partial (89.4%, 393 bytes vs 390): the group loop, the break, the fallback
// and the whole scroll-up tail now match byte for byte, and writing the loop
// as `i < entries->count + 1` (rather than the equivalent `i <= count`) is
// what finally keeps the `n` counter in the dead argument slot and the
// walking pointer in ecx. The only thing left is the register the merged
// font-size value lives in. The original keeps it in eax, so `call
// FUN_004c1450` falls straight into `lea ebx, [eax + 1]` with no copy, and
// the other arm is `xor ecx, ecx; mov cx, word ptr [eax + 2]; mov eax, ecx;
// add eax, 2`. This compiler keeps the value in edx and adds one `mov edx,
// eax` after the call; because of that it sinks the divisor `lea ebx, [edx +
// 1]` below the `movsx eax, word ptr [edi + 0x19]` and the `mov cx, word ptr
// [edi + 0xba]` load of `sel` below the `idiv`, where the original has both
// above it. So the wanted form is the value in eax, the `lea ebx, [eax + 1]`
// right at the merge, and `sel` loaded before `cdq`.
//
// The original's 16-bit intermediate in a whole register (`xor ecx, ecx;
// mov cx, ..`) is what 0x4a30c0 gets from the same `if/else` spelling of the
// same expression, so the difference is register allocation rather than the
// source shape. Nothing tried here puts the value in eax: extra 16-bit, int
// and pointer locals, a `(int)` cast, swapped branches, swapped addends,
// A second pass added six more spellings of the one remaining widening, none of
// which reached the `xor ecx, ecx; mov cx, word ptr [eax + 2]; mov eax, ecx`
// form: the 16-bit local as a signed `short` (87.5%), with an explicit
// `(int)` cast on it (87.5%), with the local an `int` (74.3%), with no local at
// all (74.6%), and with an `(unsigned int)` cast instead of `(int)` (89.4%, the
// same bytes as the file). So the file's `unsigned short g` plus `size = g + 2`
// is the best of the family, and the choice between `xor ecx, ecx; mov cx` and
// `mov ax; and eax, 0xffff` for the same zero-extension is not reachable from
// the source. It is the guide's "widened returns" note with the roles reversed:
// there a byte load is masked before a return, here a 16-bit load is zeroed
// through a second register instead of in place.
// `size++` versus `size + 1` in the divisor, `unsigned int size`, the `+ 2`
// folded into the pointer or not, a separate named divisor, and a small
// `static inline` helper for either arm. Each of those either leaves the
// value in edx or breaks the group loop's allocation (the walking pointer
// spills to the stack and `n` moves into ecx, worth about ten points), so
// the if/else with a 16-bit `g` local is the best of them at 89.4%.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a9830 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x19 - 0x01];
    short field_19;                    // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    char unknown_c6[0xd6 - 0xc6];
    int id;                            // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct List_004a9830 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a9830 {
    int current;                       // +0x00
    Entry_004a9830* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a9830* list;               // +0x14
};

struct Class_004a9830 {
    char unknown_0[0x18];
    Holder_004a9830* holder;           // +0x18
};

extern Holder_004a9830* DAT_0051fba4;
extern char DAT_00502a20[];

void __stdcall FUN_004c1420(int id);
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
int FUN_004c1450();
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a1b40(Class_004a9830* param_1, int param_2);
void __stdcall FUN_004a2be0(Class_004a9830* param_1, int param_2);
void __stdcall FUN_004a2e40(Class_004a9830* param_1, char* name, int line);

// FUNCTION: 0x4a9830
void __stdcall FUN_004a9830(Class_004a9830* param_1, int index)
{
    Entry_004a9830* entries = param_1->holder->entries;
    Entry_004a9830* me = &entries[index];
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        FUN_004c1420(DAT_0051fba4->current);
    }
    int size;
    if (DAT_0051fba4->list == 0) {
        size = FUN_004c1450();
    } else {
        unsigned short g = *(unsigned short*)(FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2);
        size = g + 2;
    }
    size++;
    int step = (me->field_19 - 2) / size;
    short last = me->field_bc;
    short sel = me->field_ba;
    int isel = sel;
    if (sel < last + step && sel >= last) {
        if (me->field_c0 == 0) {
            return;
        }
        if (sel == 0) {
            return;
        }
        short prev = sel - 1;
        me->field_ba = prev;
        if (prev < last) {
            last--;
            me->field_bc = last;
        }
        if (me->field_c2 != 0) {
            char* line = FUN_004b6af0(me->field_c2, prev);
            if (strncmp(DAT_00502a20, line, 2) == 0) {
                me->field_ba = isel;
            }
        }
        FUN_004a1b40(param_1, index);
        FUN_004a2be0(param_1, index);
        return;
    }
    if (me->field_c0 != 0) {
        FUN_004a2e40(param_1, (char*)me + 2, isel);
    }
}

// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 61.2 percent by check.py (769 bytes against the original's 765,
// 248 instructions against 249). The whole shape is the original's: the
// entry-name search loop, the "Error in GUI layout" miss path, the font-height
// block, the row-fitting loop and all three later searches compile to the
// original's instruction sequences, and the prologue and the whole first
// search loop are instruction for instruction the original's.
//
// What the function does: finds the layout entry called `name`, and gives it a
// bitmap (+0xc2), a row count (+0xc0) and, when `flag` is non-zero, an id
// (+0xd6) with bit 11 of +0x1b set. It then makes the entry's line height at
// +0xda at least the font height plus one, zeroes +0xbc and +0xba, and walks the
// row counter down from count-1 while the rows still fit the entry's height
// (+0x19), each step consuming the line height, stopping at the first row that
// does not fit. If the byte at +0x29 is set it then looks the same entry up
// again, takes its +0x01 byte, finds the first entry of type 4 with that byte,
// looks THAT entry's name up in the language list and selects it; and if the
// row walk ran out of room it scrolls the list. An entry that is not found is
// reported and then treated as a null entry.
//
// The one thing that is still wrong is the frame. The original reserves one
// dword (`push ecx` in the prologue) and keeps `param_1->holder` in it: the
// store is `mov [esp+0x10], eax` at 0x4a32b8 and the two reloads are
// `mov eax, [esp+0x10]` at 0x4a3449 and 0x4a3491. This source gets the same
// store and the same two reloads but MSVC puts them in the DEAD param_1 slot
// instead, so it emits no `push ecx` at all and every `[esp+N]` displacement in
// the function is 4 too small. The prologue and first loop are otherwise
// instruction for instruction the original's, including the order
// `mov ebp, [esp+0x18]` (the name) before `push edi`, `mov edi, 1` for the
// counter, `mov esi, [eax+4]` for the entry array and then the spill.
//
// The full slot map of the original, with esp = entry_esp - 20 after the five
// prologue pushes, which is worth having because every value is homed in a
// dead argument slot except the holder:
//   [esp+0x10] = entry_esp - 4  = the `push ecx` frame dword, the holder
//   [esp+0x18] = entry_esp + 4  = param_1's dead slot, `remain`
//   [esp+0x1c] = entry_esp + 8  = the name's dead slot, `text`
//   [esp+0x24] = entry_esp + 0x10 = the count's dead slot, the language root
// so the original's home for `remain` is the dead PARAM_1 slot, which is why
// the holder had to go to a real frame slot: the two are live at the same time
// (the holder's last use is 0x4a3491, `remain` is stored at 0x4a33dc) and the
// name's slot is still live at 0x4a33dc (name is used again at 0x4a3469), so
// there is only one free dead slot at that point.
//
// What did NOT force the frame (all tried, all still no `push ecx`, all at
// 61.2% or worse): declaring `holder` and `entries` at the top, declaring them
// in the other order, spelling the later uses as `param_1->holder->entries` so
// the holder is an unnamed CSE temp again, hoisting `remain` and `text` to
// function scope, adding a `Root* root = DAT_0051fba4` local in the last block
// so a fourth value needs a home, passing the holder to the search helper
// instead of the array, and copying `name` into a local. Declaring `remain` as
// `short` rather than `int` DOES produce `push ecx` and lands the holder in the
// frame slot exactly as the original does, but it is wrong: the original
// sign-extends the height into a full dword (`movsx edi, word [esi+0x19]`,
// `sub edi, ecx`, `mov [esp+0x18], edi`), and the short version then stores only
// a word, and it scores worse besides (57.0%).
//
// Facts worth keeping for anyone finishing this:
// - The 0x15b entry: +0x00 type byte, +0x01 the byte the type-4 search matches,
//   +0x02 the 16-byte name, +0x19 the height the rows are fitted into, +0x1b
//   the flag word, +0x29 a byte that gates the whole second half, +0xb6 the
//   entry count (entry 0 only), +0xba, +0xbc, +0xbe the first fitting row,
//   +0xc0 the row count, +0xc2 the bitmap, +0xd6 the id, +0xda the line height.
// - `param_1->holder` is +0x18 and `holder->entries` is +0x04, so the first
//   instruction pair is `mov eax, [eax+0x18]` then `mov esi, [eax+4]`.
// - The name search is the `FindEntry` helper of 0x4a0180 / 0x4a0200 /
//   0x4a0280 / 0x4a35a0, inlined. Spelled as an inlined helper returning -1
//   the `or reg, 0xffffffff` lands AFTER the loop, where the original has it;
//   written as `int index = -1; for (...) { index = i; break; }` it is hoisted
//   to the top of the function and the whole register allocation changes.
// - The font height expression (`FUN_004c1450()` when the language list is
//   null, else the glyph for 0x49 plus two) is written THREE times in the
//   source: once inside the `?:` that raises +0xda (both arms recompute it, so
//   it must be spelled with the expression in both arms), once for the
//   `step`, and it must be an expression, not a local, or the second copy is
//   common-subexpression-eliminated away.
// - The stores after the entry is found are `+0xc0`, `+0xc2`, then `+0x1b`,
//   and the original reads +0x1b into edx BEFORE the `or edx, 0x10` and stores
//   it after both other stores, so the flag store is last in the source.
// - `xor edi, edi` at 0x4a3340 makes edi the zero register for the whole next
//   block: `cmp eax, edi` twice, `mov word [esi+0xbc], di`,
//   `mov word [esi+0xba], di` and `cmp eax, edi` for the flag test all read it.
// - The `?:` that raises +0xda compares `me->f_da <= FontHeight() + 1`, with
//   the field first, and stores back through the same `mov word [esi+0xda], ax`
//   from both arms, so the two arms must compute the same 16-bit store.
// - The type-4 search keeps the found index in ebp and tests `ebp == -1` even
//   though the loop can only leave 0 or a positive index there; the zero comes
//   from `xor ebp, ebp` placed at the loop's join, so `int found = 0;` with the
//   `if (found == -1)` test after the loop is what produces it. The +0x01 byte
//   it matches is read from a THIRD derivation of the entry array, loaded from
//   the holder at 0x4a3491 and kept in esi through the whole loop, and it stays
//   in `dl` for the loop; `holder` itself is dead by then, so the array has to
//   be a fresh local here, not `holder->entries`, or one more value is live in
//   the loop and the byte gets spilled to the stack instead of held in `dl`.
// - `remain` is a real local that lives in the dead param_1 slot: it is stored
//   twice (`mov [esp+0x18], edi` at 0x4a33dc before the row loop and again at
//   0x4a343a after it) and read twice at 0x4a3562 and 0x4a3579.
// - The last two arguments are pushed from the stack slots, not from registers:
//   `push edx` (the `setl` of `remain < 0`), `push esi` (the found index in the
//   language list) and `push eax` (the language root) for FUN_004a03f0.
//
// Suspected original bug: none found beyond the dead `found == -1` test above,
// which is harmless. The `+0xda` minimum is applied twice (once by the `?:`
// that raises it, once by the `me->f_da == 0` test that computes the step), so
// the step is never zero, which looks deliberate.

#include <string.h>

#pragma pack(push, 1)
struct Entry_004a32a0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01, matched by the type-4 search
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short height;                      // +0x19, the rows are fitted into this
    int flags;                         // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char f_29;                // +0x29, gates the whole second half
    char unknown_2a[0xb6 - 0x2a];
    short count;                       // +0xb6, entry 0 holds the entry count
    char unknown_b8[0xba - 0xb8];
    short f_ba;                        // +0xba
    short f_bc;                        // +0xbc
    short first;                       // +0xbe, first row that still fits
    short num;                         // +0xc0, the row count
    int bitmap;                        // +0xc2
    char unknown_c6[0xd6 - 0xc6];
    int id;                            // +0xd6
    short f_da;                        // +0xda, the line height
    char unknown_dc[0x15b - 0xdc];
};
#pragma pack(pop)

struct List_004a32a0 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct Glyph_004a32a0 {
    unsigned short width;
    unsigned short height;             // +0x02
};

struct Holder_004a32a0 {
    int unknown_0;
    Entry_004a32a0* entries;           // +0x04
};

struct Root_004a32a0 {                 // DAT_0051fba4
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a32a0* language;           // +0x14
    Holder_004a32a0* holder;           // +0x18
};

struct Class_004a32a0 {
    char unknown_00[0x18];
    Holder_004a32a0* holder;           // +0x18
};

extern Root_004a32a0* DAT_0051fba4;

void __stdcall FUN_004b6290(char* msg);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004a03f0(Root_004a32a0* menu, int index, int value);
void __stdcall FUN_004a3ef0(Root_004a32a0* param_1, int param_2);

// The line height of one row: the default font height, or the height of the
// glyph for 'I' plus two. Written out three times in the caller because the
// original evaluates it again in the second arm of the +0xda minimum.
static inline int FontHeight_004a32a0()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return (int)((Glyph_004a32a0*)FUN_004b7f30(
        DAT_0051fba4->language->glyphs, 0x49))->height + 2;
}

// The entry search of 0x4a0180, 0x4a0200, 0x4a0280 and 0x4a35a0.
static inline int FindName_004a32a0(Entry_004a32a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a32a0
void __stdcall FUN_004a32a0(Class_004a32a0* param_1, char* name, int bitmap,
                            int count, int flag)
{
    Holder_004a32a0* holder = param_1->holder;
    Entry_004a32a0* entries = holder->entries;
    int index = FindName_004a32a0(entries, name);
    Entry_004a32a0* me;
    if (index != -1) {
        me = &entries[index];
    } else {
        FUN_004b6290("Error in GUI layout");
        me = 0;
    }
    me->num = (short)count;
    me->bitmap = bitmap;
    me->flags |= 0x10;
    me->f_da = (short)((me->f_da <= FontHeight_004a32a0() + 1)
                       ? FontHeight_004a32a0() + 1 : (int)me->f_da);
    if (flag != 0) {
        me->id = flag;
        me->flags |= 0x800;
    }
    me->f_bc = 0;
    me->f_ba = 0;
    me->first = (short)(count - 1);
    int remain = me->height;
    int step;
    if (me->f_da == 0) {
        step = FontHeight_004a32a0() + 1;
    } else {
        step = me->f_da;
    }
    for (int j = count - 1; j > -1; j--) {
        remain -= step;
        if (remain < 0)
            break;
        me->first = (short)j;
    }
    if (me->f_29 == 0)
        return;
    int i2 = FindName_004a32a0(holder->entries, name);
    Entry_004a32a0* list = holder->entries;
    unsigned char kind = list[i2].kind;
    int found = 0;
    for (int k2 = 1; k2 < list->count + 1; k2++) {
        if (list[k2].type == 4 && list[k2].kind == kind) {
            found = k2;
            break;
        }
    }
    if (found == -1)
        return;
    char* text = (char*)&holder->entries[found].name;
    if (DAT_0051fba4->holder != 0) {
        int j2 = FindName_004a32a0(DAT_0051fba4->holder->entries, text);
        if (j2 != -1) {
            FUN_004a03f0(DAT_0051fba4, j2, remain < 0);
        }
    }
    if (remain < 0) {
        FUN_004a3ef0(DAT_0051fba4, found);
    }
}

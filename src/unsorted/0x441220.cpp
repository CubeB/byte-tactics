// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 97.2 percent (519 of 517 bytes). Everything matches except the
// operand size of the two `ge == 0` tests, which are the only two extra bytes:
//
//   original: test edi, edi / test esi, esi
//   ours:     test di,  di  / test si,  si
//
// The original tests the whole 32-bit register, which MSVC 5 does for an
// `int`/`bool` variable but not for a `short`. But `ge` cannot be wide here:
// `int ge = ((value & 0xff) >= (int)(signed char)((char*)g_game)[1]) ? 1 : 0;`
// lets MSVC fold `(ge == 0)` back into the comparison and emit `setl` plus a
// deferred, differently allocated block; `bool ge` spills to a stack byte and
// gives edi to the flag pointer. Only the narrowing `short` assignment keeps
// the original's materialised `xor ecx,ecx / cmp edi,edx / setge cl /
// mov edi,ecx`, the edi home for `ge`, the `[esp+0x10]` spill of the pointer
// and the matching index multiply. Tried and rejected: every `ge` spelling
// (bool/char/unsigned char/int/unsigned int/long/enum), block-scope vs
// function-scope, every `on` type and cast, the item-5 self-correction
// (`if (ge) ge = 1; else ge = 0;`, which does produce `test edi,edi` but moves
// the whole comparison after the index math), inlined helpers returning `ge`,
// taking `ge` as an `int`/`bool` parameter, and every cast on the comparison.
//
// What did move the number:
//  - `ge` as a `short` declared UNINITIALISED at function scope, first: the
//    two-byte value wins edi (WATCH) and reuses esi (JOIN), which is the
//    original's allocation.
//  - the flag fold written `(~flags & 0x80) | (flags >> 8)` (no
//    `(unsigned char)` cast), worth 3.4 points and the exact operand order.
//  - a single inlined `Apply` helper holding only the `on` test and the
//    read-modify-write, called from both blocks; it fixes the JOIN fold too.
//
// WHERE THE TWO `test` OPERAND WIDTHS ACTUALLY COME FROM. Read off the original
// at 0x441318; it is not the width of `ge` on its own:
//
//   0x441323: cmp  word ptr [eax + 0xc0], dx
//   0x44132a: sete dl
//   0x44132d: xor  eax, eax
//   0x44132f: test edi, edi        <-- differs from our `test di, di`
//   0x441331: sete al
//   0x441334: or   dl, al          <-- the two setcc results are ORed
//   0x441336: mov  eax, [esp+0x10]
//   0x44133a: and  edx, 1
//   0x44133d: or   ecx, edx
//   0x44133f: mov  dx, word ptr [eax]
//   0x441342: and  edx, 0xfffe     <-- the LOW BIT IS CLEARED HERE
//   0x441348: or   ecx, edx
//   0x44134a: mov  word ptr [eax], cx
//
// The original's shape is: materialise both flags, `or` them together, OR that
// with the old word after clearing the old word's low bit, and store 16 bits.
// Apply_00441220 as written narrows the flags first (`& 1`) and merges afterwards
// (`(r | on) | (*p & 0xfffe)`), and that narrowing is what makes MSVC choose a
// 16-bit test on `ge`. So fix the `or` order before touching `ge`'s type.
//
// Widening `ge` is NOT the answer, measured again on the free oracle: `int`,
// `unsigned int`, `unsigned long` and `long`, each with the ternary, with the
// ternary removed, and with the comparison's arms swapped, ALL compile to
// 487 bytes and 45.6%. The comparison folds away and the block is reallocated.
// The 32-bit `ge` has to come from the `or` shape keeping the comparison
// materialised, not from a wider declaration.
//
// SPACE BUNNY FREE: that `or`-shape hypothesis is disproved, and this is a
// clean negative. Apply_00441220 as written ALREADY emits the original's
// instruction order, so there is no `or`-shape defect left to fix. Ours at the
// WATCH block, from the object, side by side with the original:
//
//   103 cmp word ptr [eax + 0xc0], dx   103 cmp word ptr [eax + 0xc0], dx
//   10a sete dl                        10a sete dl
//   10d xor eax, eax                   10d xor eax, eax
//   10f test di, di     <-- ours       10f test edi, edi   <-- original
//   112 sete al                        111 sete al
//   115 or dl, al                      114 or dl, al
//   117 mov eax, [esp + 0x10]          116 mov eax, [esp + 0x10]
//   11b and edx, 0x1                   11a and edx, 0x1
//   11e or ecx, edx                    11d or ecx, edx
//   120 mov dx, word ptr [eax]         11f mov dx, word ptr [eax]
//   123 and edx, 0xfffe                122 and edx, 0xfffe
//   129 or ecx, edx                    128 or ecx, edx
//   12b mov word ptr [eax], cx         12a mov word ptr [eax], cx
//
// Both setcc results ARE ored (`or dl, al`), the `& 1` IS there
// (`and edx, 0x1`), and the old word IS masked (`and edx, 0xfffe`) before the
// merge (`or ecx, edx`), in the original's order. The source's
// `(unsigned short)` cast is invisible in the output: it does not narrow then
// re-merge the flags, it is only the barrier that stops MSVC folding the
// comparison away.
//
// And the cast is load-bearing in the OPPOSITE direction to the note above:
// removing it, i.e. writing the "original's order" as `int on = (...) & 1;`
// with 32-bit flags, collapses to 509 bytes / 46.0 percent for EVERY `ge` type,
// because with a 32-bit `on` MSVC can see that the low bit of
// `r | on | (*p & 0xfffe)` is exactly `(ge == 0)` and folds the whole
// comparison into a `setl` at the point of use. The `or` shape decides whether
// the comparison survives, but it is the surviving 16-bit `test` that comes
// with it: the two are the same defect, not two defects.
//
// Measured (free oracle only, build/scratch/0x441220, 111 variants, no
// check.py runs):
//  - `ge` as short / char / int / unsigned int / long / unsigned long / bool
//    gives BYTE-IDENTICAL code in every Apply shape. A wide declaration
//    changes nothing: MSVC 5 takes the `test` width from the narrowed
//    expression, not from the declared type. 97.2 percent always means
//    `test di, di` at 0x44132f and `test si, si` at 0x4413a9.
//  - grid 1, 7 `ge` types x 5 Apply spellings: 46.0 percent unless `on` is
//    cast to `unsigned short`, then 97.2 percent, for all 7 types.
//  - grid 2, 8 spellings of the ge test ((ge==0), (ge-1)==-1, (ge&1)==0,
//    !(ge&1), (unsigned)ge==0, ge==0u, (ge^1)&1, (ge==0)+0) x 3 assignment
//    spellings x 2 `ge` types: 97.2, 78.9 or 69.5 percent, never a 32-bit test.
//  - grid 3 and 4, 13 spellings of the narrowing: removing the cast 46.0,
//    mask before cast 46.0, cast at the store only 51.6, `short` intermediate
//    52.1, ternary 56.8, separate 32-bit arms 57.2 (same 519 bytes, worse
//    ratio), `char` intermediate 93.8, `if (on) on = 1; else on = 0;` 63.9.
//    Narrowing each setcc contribution separately, or comparing the field
//    through a 32-bit temporary, or widening `r` in the merge, all stay at
//    97.2 percent with the 16-bit test.
// Bottom line for whoever tries next: every spelling that keeps the
// materialised comparison narrows the test, and every spelling that widens the
// test folds the comparison. They are one decision, and no spelling of this
// helper does both.
#include <string.h>

struct Holder_00441220 {
    int unknown_0;
    void* gadgets;                   // +0x04
};

struct Menu_00441220 {
    char unknown_0[0x18];
    Holder_00441220* holder;         // +0x18
};

#pragma pack(push, 1)
struct Entry_00441220 {
    char unknown_0[0xba];
    short index;                     // +0xba
    char unknown_bc[0xc0 - 0xbc];
    unsigned short field_c0;         // +0xc0
    char unknown_c2[0xd2 - 0xc2];
    char* records;                   // +0xd2
    char unknown_d6[0x13c - 0xd6];
    unsigned short enabled : 1;      // +0x13c bit 0
    unsigned short bits_13c : 15;
};

struct Group_00441220 {
    int f0;
    int f1;
    int f2;
    int f3;
};

struct Msg_00441220 {
    char pad_0[0x99];
    Group_00441220 group;
    char pad_1[0xb9 - 0xa9];
};
#pragma pack(pop)

struct Game_00441220 {
    char unknown_0[0x2ab1];
    char buffer[0x200];
};

extern Game_00441220* g_game;

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a0570(void* menu, const char* name, int value);
void __stdcall FUN_0049fa90(void* menu);

static inline void Apply_00441220(unsigned short* p, unsigned short r, int ge, Entry_00441220* entry)
{
    unsigned short on = (unsigned short)(((entry->field_c0 == 0) | (ge == 0)) & 1);
    *p = (unsigned short)((r | on) | (*p & 0xfffe));
}

// FUNCTION: 0x441220
void __stdcall FUN_00441220(Menu_00441220* menu, Entry_00441220* entry)
{
    void* gadgets = menu->holder->gadgets;
    short ge;
    unsigned short* p;
    Entry_00441220* gd;
    unsigned short on;
    unsigned short r;
    int idx = entry->index;
    Group_00441220* rec = (Group_00441220*)(entry->records + idx * 0x54 + 4);
    Msg_00441220 msg;
    msg.group = *rec;
    memcpy(g_game->buffer, &msg, 185);

    unsigned short flags = *(unsigned short*)((char*)&msg.group + 2);
    int value = *(int*)((char*)&msg.group + 0xe);

    int index = FUN_0049fdf0(gadgets, "WATCH", 1);
    if (index != -1) {
        gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        ge = ((value & 0xff) >= (int)(signed char)((char*)g_game)[1]) ? 1 : 0;
        p = (unsigned short*)((char*)gd + 0x13c);
        r = (unsigned short)((~flags & 0x80) | (flags >> 8));
        r >>= 3;
        r |= flags & 0x10;
        r >>= 4;
        Apply_00441220(p, r, ge, entry);
    }
    index = FUN_0049fdf0(gadgets, "JOINGAME", 1);
    if (index != -1) {
        gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        ge = ((value & 0xff) >= (int)(signed char)((char*)g_game)[1]) ? 1 : 0;
        p = (unsigned short*)((char*)gd + 0x13c);
        r = (unsigned short)((flags >> 11) | (flags & 0x10));
        r >>= 4;
        Apply_00441220(p, r, ge, entry);
    }

    int password = msg.group.f1 & 1;
    FUN_004a0570((char*)g_game + 0x519, "PASSWORDTEXT", password);
    FUN_004a0570((char*)g_game + 0x519, "PASSWORD", password);
    FUN_0049fa90(menu);
}

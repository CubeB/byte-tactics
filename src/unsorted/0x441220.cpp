// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// PARTIAL, 65.5 percent (510 of 517 bytes), up from 52.4.
//
// Two changes did it, and both are about the order things are declared and
// combined rather than about the values:
//  - declaring `flags` before `value` so the two reloads emit as
//    `mov esi, [msg+0xbb]` then `mov bx, [msg+0xaf]`, the original's order;
//  - an `on` local, `unsigned short on = (unsigned short)(((entry->field_c0 == 0)
//    | (ge == 0)) & 1);`, then `*p = (unsigned short)((r | on) | (*p & 0xfffe));`.
//    That local is what produces the original's `or dl,al` / `and edx,1` /
//    `or ecx,edx` grouping and its `cmp edi,edx` operand order. Writing the same
//    expression as one chain does not.
//
// Measured negatives, all of which supersede the previous attempt's guesses and
// none of which beat 65.5%: reordering the OR chain, swapping the comparison
// operands, `static inline` helpers for the bit expression, for the compare and
// for the whole block, `lim` temporaries, `int`/`char`/`bool` for `ge` and `on`,
// function-scope declarations, and all 24 declaration orders of the four block
// locals.
//
// What is left is one root cause with a mechanism behind it. The original keeps
// `ge` in a register, edi in the first gadget block and esi in the second, that
// is in the register of the dead masked operand, whereas this source spills
// `ge` to a stack byte. That one choice cascades into the rest of the diff: the
// index multiply alternates ecx/edx in the original and is all-ecx here, the
// flag pointer lands in edx with a `[esp+0x10]` spill and reload, and the bit
// expression accumulates in cx. The lever is to reduce the pressure on the spill
// slot, or to make `ge` a parameter of an inlined function so the compiler
// cannot spill it. The PASSWORD call's argument registers (eax then ecx in the
// original) are a separate, small tail issue on top of that.
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
    int f0;                          // +0x00
    int f1;                          // +0x04
    int f2;                          // +0x08
    int f3;                          // +0x0c
};

struct Msg_00441220 {
    char pad_0[0x99];
    Group_00441220 group;            // +0x99
    char pad_1[0xb9 - 0xa9];
};
#pragma pack(pop)

struct Game_00441220 {
    char unknown_0[0x2ab1];
    char buffer[0x200];              // +0x2ab1
};

extern Game_00441220* g_game;

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a0570(void* menu, const char* name, int value);
void __stdcall FUN_0049fa90(void* menu);

// FUNCTION: 0x441220
void __stdcall FUN_00441220(Menu_00441220* menu, Entry_00441220* entry)
{
    void* gadgets = menu->holder->gadgets;
    int idx = entry->index;
    Group_00441220* rec = (Group_00441220*)(entry->records + idx * 0x54 + 4);
    Group_00441220 g = *rec;
    Msg_00441220 msg;
    msg.group = g;
    memcpy(g_game->buffer, &msg, 185);

    unsigned short flags = *(unsigned short*)((char*)&msg.group + 2);
    int value = *(int*)((char*)&msg.group + 0xe);

    int index = FUN_0049fdf0(gadgets, "WATCH", 1);
    if (index != -1) {
        Entry_00441220* gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        bool ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];
        unsigned short* p = (unsigned short*)((char*)gd + 0x13c);
        unsigned short r = (unsigned short)((~(unsigned char)flags & 0x80) | (flags >> 8));
        r >>= 3;
        r |= flags & 0x10;
        r >>= 4;
        unsigned short on = (unsigned short)(((entry->field_c0 == 0) | (ge == 0)) & 1);
        *p = (unsigned short)((r | on) | (*p & 0xfffe));
    }
    index = FUN_0049fdf0(gadgets, "JOINGAME", 1);
    if (index != -1) {
        Entry_00441220* gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        bool ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];
        unsigned short* p = (unsigned short*)((char*)gd + 0x13c);
        unsigned short r = (unsigned short)((flags >> 11) | (flags & 0x10));
        r >>= 4;
        unsigned short on = (unsigned short)(((entry->field_c0 == 0) | (ge == 0)) & 1);
        *p = (unsigned short)((r | on) | (*p & 0xfffe));
    }
    int password = msg.group.f1 & 1;
    FUN_004a0570((char*)g_game + 0x519, "PASSWORDTEXT", password);
    FUN_004a0570((char*)g_game + 0x519, "PASSWORD", password);
    FUN_0049fa90(menu);
}

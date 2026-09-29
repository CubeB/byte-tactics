// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 97.2 percent (519 of 517 bytes). Everything matches except the
// operand size of the two `ge == 0` tests, which are the only two extra bytes:
//
//   original: test edi, edi / test esi, esi
//   ours:     test di,  di  / test si,  si
//
// The original tests the whole 32-bit register, which MSVC 5 does for an
// `int`/`bool` variable but not for a `short`. But `ge` cannot be wide here:
// `int ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];`
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
        ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];
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
        ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];
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

// Decompiled by deepseek-v4.1-flash. Names are provisional.
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

// PARTIAL (52.4%). Everything up to and including the 185-byte copy matches, and
// the WATCH/JOINGAME bitfield stores use the same shape. What still differs:
// - the `int/bool ge = (value & 0xff) >= (signed char)g_game[1]` comparison:
//   the original computes it early (setge cl; mov edi,ecx) and re-tests it later
//   (test edi,edi; sete al); here MSVC schedules the setge next to the shift.
// - the WATCH bit expression `((~(unsigned char)flags & 0x80) | (flags >> 8)) >> 3
//   | (flags & 0x10)) >> 4`: same operations, but MSVC accumulates in ax and puts
//   hi in cl where the original accumulates in cx with hi in dl, and the original
//   hoists `flags & 0x10` (eax) before the OR. Moving the subexpression into a
//   separate variable or reordering the OR operands does not change it.
// - the trailing PASSWORD/PASSWORDTEXT call setup picks eax/ecx where the
//   original uses ecx/edx.
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

    int value = *(int*)((char*)&msg.group + 0xe);
    unsigned short flags = *(unsigned short*)((char*)&msg.group + 2);

    int index = FUN_0049fdf0(gadgets, "WATCH", 1);
    if (index != -1) {
        Entry_00441220* gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        bool ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];
        unsigned short* p = (unsigned short*)((char*)gd + 0x13c);
        unsigned short r = (unsigned short)((~(unsigned char)flags & 0x80) | (flags >> 8));
        r >>= 3;
        r |= flags & 0x10;
        r >>= 4;
        *p = (unsigned short)((*p & 0xfffe) | (r | ((entry->field_c0 == 0) | (ge == 0))));
    }
    index = FUN_0049fdf0(gadgets, "JOINGAME", 1);
    if (index != -1) {
        Entry_00441220* gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        bool ge = (value & 0xff) >= (int)(signed char)((char*)g_game)[1];
        unsigned short* p = (unsigned short*)((char*)gd + 0x13c);
        unsigned short r = (unsigned short)((flags >> 11) | (flags & 0x10));
        r >>= 4;
        *p = (unsigned short)((*p & 0xfffe) | (r | ((entry->field_c0 == 0) | (ge == 0))));
    }
    int password = msg.group.f1 & 1;
    FUN_004a0570((char*)g_game + 0x519, "PASSWORDTEXT", password);
    FUN_004a0570((char*)g_game + 0x519, "PASSWORD", password);
    FUN_0049fa90(menu);
}

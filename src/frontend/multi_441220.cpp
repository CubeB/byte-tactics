// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
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

struct Game {
    char unknown_0[0x2ab1];
    char buffer[0x200];
};

extern Game* g_game;

int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
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
    // Reference to gv: makes ge a real 32-bit read, so ge == 0 tests edi.
    int gv; int& ge = gv;
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

    int index = FindGadgetIndex(gadgets, "WATCH", 1);
    if (index != -1) {
        gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        gv = ((value & 0xff) >= (int)(signed char)((char*)g_game)[1]) ? 1 : 0;
        p = (unsigned short*)((char*)gd + 0x13c);
        r = (unsigned short)((~flags & 0x80) | (flags >> 8));
        r >>= 3;
        r |= flags & 0x10;
        r >>= 4;
        Apply_00441220(p, r, ge, entry);
    }
    index = FindGadgetIndex(gadgets, "JOINGAME", 1);
    if (index != -1) {
        gd = (Entry_00441220*)((char*)gadgets + index * 0x15b);
        gv = ((value & 0xff) >= (int)(signed char)((char*)g_game)[1]) ? 1 : 0;
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

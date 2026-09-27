// Decompiled by space-bunny-free. Names are provisional.
// Builds one player's whole unit list as a single packet and sends it: a
// header of 0x2c, a 16-bit length (written as zero, patched at the end) and
// the game's tick counter, which is also stored in the player at +0x18. Then
// every unit of the player that is live, whose owner exists and whose owner
// accepts it, until the stream is half full; then a 16-bit -1 end marker, one
// more bit, the player's own unit (the one at ticks % the unit count) written
// in full, the length patched into the header as two bytes, and the packet.
// The reader of all this is 0x48b920, and the per-unit writer is 0x48b200.
//
// The operand order of the byte-wide add in the first length byte is the one
// register choice no source rewrite settled; the compiler state these two
// headers leave behind does (36 sets in tools/headers.py's list, this the
// smallest).

#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

class Class_00415b60 {
public:
    void FUN_00415bb0();
    Class_00415b60();
};

class Class_00415c10 : public Class_00415b60 {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    void FUN_00415c10(int value, int bits);
};

// The same object seen as the byte writer of 0x415da0 and the free of 0x415b90.
struct Class_00415da0 {
    char unknown_0[0xc];
    unsigned char* data;               // +0xc
    void FUN_00415da0(int index, unsigned char value);
};

struct Class_00415b90 {
    char unknown_0[0xc];
    void* data;                        // +0xc
    void FUN_00415b90();
};

struct Link_0048b710 {
    char unknown_0[0x6f];
    unsigned short field_6f;           // +0x6f
};

// The player behind a unit: a vtable at +0, and an int at +0x20 (0x48b200).
class PlayerData_0048b710 {
public:
    virtual void vf0();
    virtual void vf1();
    virtual void vf2();
    virtual void vf3();
    virtual void vf4();
    virtual void vf5();
    virtual void vf6();
    virtual int vf7();                              // vtable +0x1c
    virtual void WriteTo(Class_00415c10* stream);  // vtable +0x20
    char unknown_4[0x1c];
    int field_20;                                   // +0x20
};

// What a unit holds at +0: a pointer to the player, and the int of 0x48b200.
struct Owner_0048b710 {
    PlayerData_0048b710* player;       // +0x0
    char unknown_4[0x1c];
    int field_20;                      // +0x20
};

struct Unit_0048b710 {                              // 0x118 bytes
    Owner_0048b710* owner;              // +0x0
    char unknown_4[0x96 - 0x4];
    Link_0048b710* link;                // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short field_a6;            // +0xa6
    unsigned short field_a8;            // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048b710 {
    char unknown_0[0x4];
    int id;                             // +0x4
    char unknown_8[0x18 - 0x8];
    int ticks;                          // +0x18
    char unknown_1c[0x67 - 0x1c];
    Unit_0048b710* units_begin;         // +0x67
    Unit_0048b710* units_end;           // +0x6b
};

struct Game_0048b710 {
    char unknown_0[0x14393];
    int field_14393;                    // +0x14393, bits for the unit type index
    char unknown_14397[0x37ee6 - 0x14397];
    unsigned short field_37ee6;         // +0x37ee6, the unit count
    char unknown_37ee8[0x38a47 - 0x37ee8];
    int ticks;                          // +0x38a47
};

#pragma pack(pop)

extern Game_0048b710* g_game;

void __stdcall FUN_0048b200(Class_00415c10* stream, Unit_0048b710* u);
int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x48b710
void __stdcall FUN_0048b710(Player_0048b710* p)
{
    Class_00415c10 stream;
    stream.FUN_00415c10(0x2c, 8);
    stream.FUN_00415c10(0, 0x10);
    stream.FUN_00415c10(g_game->ticks, 0x20);
    p->ticks = g_game->ticks;
    for (Unit_0048b710* u = p->units_begin; u <= p->units_end; u++) {
        if (!(u->flags & 0x10000000))
            continue;
        if (!u->owner)
            continue;
        if (!u->owner->player->vf7())
            continue;
        stream.FUN_00415c10(u->field_a8 - u->link->field_6f, 0x10);
        stream.FUN_00415c10(u->field_a6, g_game->field_14393);
        u->owner->player->WriteTo(&stream);
        // `>> 3`, not `/ 8`: that is the original's `add ecx, 7; sar ecx, 3`
        // with no sign fix-up.
        if (((stream.index + 7) >> 3) + stream.bit * 4 >= 0x200)
            break;
    }
    stream.FUN_00415c10(-1, 0x10);
    int i = g_game->ticks % g_game->field_37ee6;
    stream.data[stream.bit] |= 1 << stream.index;
    stream.index++;
    if (stream.index == 0x20) {
        stream.index = 0;
        stream.bit++;
        if (stream.bit == stream.capacity)
            stream.FUN_00415bb0();
        stream.data[stream.bit] = 0;
    }
    FUN_0048b200(&stream, &p->units_begin[i]);
    // The packet's length, little-endian at bytes 1 and 2, is only known here.
    // The `char` cast is what makes MSVC 5 narrow the first sum to a byte and
    // push the register unmasked; the second is pushed as a dword.
    ((Class_00415da0*)&stream)->FUN_00415da0(
        1, (char)(((stream.index + 7) >> 3) + (unsigned char)stream.bit * 4));
    ((Class_00415da0*)&stream)->FUN_00415da0(
        2, (char)((((stream.index + 7) >> 3) + stream.bit * 4) >> 8));
    FUN_00451df0(p->id, stream.data, ((stream.index + 7) >> 3) + stream.bit * 4);
    ((Class_00415b90*)&stream)->FUN_00415b90();
}

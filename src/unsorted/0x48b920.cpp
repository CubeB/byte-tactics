// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Reads one player's unit packet (the writer is the matched 0x48b710): an 8 bit
// header byte, a 16 bit length, then the game tick (32 bits), which is stored
// in the player at +0x18. Then a list of 16 bit unit indices, terminated by
// -1: for each one the unit's type index is read in g_game+0x14393 bits and,
// when it differs from the unit's own type at +0xa6, the unit is rebuilt from
// a spawn record through 0x4861d0 (whose reader 0x48b3f0 builds the same
// record, byte for byte, at 0x48b44c). Each unit then reads its own state
// through the virtual reader at its owner's iface vtable +0x24. Afterwards
// every live owned unit is cleaned up (0x43dd20 and 0x48a870), and one more
// stream bit picks the unit (tick % unit count) handed to the per unit reader
// 0x48b3f0.
//
// NOT MATCHED: 97.1%, size exact (434 bytes). Everything matches except the
// scheduling of the two whole struct copies that build the spawn record,
// `spawn.pos = unit->pos;` (12 bytes, `lea eax, [esi+0x6a]` then
// [eax]/[eax+4]/[eax+8]) and `spawn.tail = unit->tail;` (6 bytes,
// `lea ecx, [esi+0x64]` then [ecx]/[ecx+4]). The original interleaves them:
// pos.x, pos.y, tail.a, pos.z (stores: +5, +9, +0x11, +0xd, +0x15), and
// hoists the call's `unit->player` load (mov dl, [esi+0xff]) to just before
// the pos.z store, deferring the tail.b store to after the two pushes. MSVC 5
// here emits each copy whole: pos.x, pos.y, pos.z, tail.a, tail.b, then the
// player load. The whole 21 instruction set is identical, only 4 of them are
// ordered differently.
//
// Tried and failed to move the scheduler: both copy orders and every order of
// the type/id/player assignments plus the two copies (the first is
// `spawn.id`, then `spawn.type`, then `spawn.player`, which is what makes the
// first ten instructions match); copies as PosXY/partial pieces (8 byte and
// 4 byte copies get folded to direct [esi+disp] loads, only the 12 byte and
// 6 byte copies keep the lea); per field assignment with a base pointer via
// pointer/reference locals or casts (the compiler folds them back);
// static helpers with an out pointer and by value with a declaration
// (they shift `unit` out of esi); an `unsigned char who = unit->player;`
// local at every position; unused locals and a repeated player store; and
// every one of the 128 standard header sets and the 768 --cpp sets from
// tools/headers.py. The remaining difference looks like pure instruction
// scheduling in the original translation unit that the isolated file cannot
// reproduce.

// Bit reader, the counterpart of the writer used by 0x48b710 (see
// src/unsorted/0x415dc0.cpp). Fields are data, index (the dword), bit.
class Class_00415dc0 {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int FUN_00415dc0(int bits);

    int ReadBit()
    {
        int r = (data[index] & (1 << bit)) != 0;
        if (++bit == 32) {
            bit = 0;
            index++;
        }
        return r;
    }
};

// The serialisation interface behind a unit's owner. The writer 0x48b710 calls
// slot +0x20; the reader here calls slot +0x24.
class Iface_0048b920 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void ReadFrom(Class_00415dc0* reader);   // +0x24
};

struct Unit_0048b920;

class Class_0043dd20 {
public:
    Iface_0048b920* iface;             // +0x0, see src/unsorted/0x43dd20.cpp
    void FUN_0043dd20(Unit_0048b920* u);
};

#pragma pack(push, 1)
struct Pos_0048b920 { int x, y, z; };
struct Tail_0048b920 { int a; unsigned short b; };

struct Spawn_0048b920 {
    unsigned char player;              // +0x0
    unsigned short type;               // +0x1
    unsigned short id;                 // +0x3
    Pos_0048b920 pos;                  // +0x5
    Tail_0048b920 tail;                // +0x11
};

struct Unit_0048b920 {                 // 0x118 bytes
    Class_0043dd20* owner;             // +0x0
    char unknown_4[0x64 - 4];
    Tail_0048b920 tail;                // +0x64
    Pos_0048b920 pos;                  // +0x6a
    char unknown_76[0xa6 - 0x76];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char player;              // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048b920 {               // 0x14b bytes
    char unknown_0[0x18];
    int ticks;                         // +0x18
    char unknown_1c[0x67 - 0x1c];
    Unit_0048b920* units_begin;        // +0x67
    Unit_0048b920* units_end;          // +0x6b
};

struct Game_0048b920 {
    char unknown_0[0x14393];
    int field_14393;                   // +0x14393, bits of the unit type index
    char unknown_14397[0x37ee6 - 0x14397];
    unsigned short field_37ee6;        // +0x37ee6, the unit count
};
#pragma pack(pop)

extern Game_0048b920* g_game;

void __stdcall FUN_0048b3f0(Class_00415dc0* reader, Unit_0048b920* unit);
void __stdcall FUN_0048a870(Unit_0048b920* unit);
Unit_0048b920* __stdcall FUN_004861d0(unsigned char player, Spawn_0048b920* spawn);

// Still differs: the spawn.pos / spawn.tail copy interleave (see the note at
// the top of the file).
// FUNCTION: 0x48b920
void __stdcall FUN_0048b920(Player_0048b920* p, unsigned int* data)
{
    Class_00415dc0 reader;
    Spawn_0048b920 spawn;

    reader.data = data;
    reader.index = 0;
    reader.bit = 0;
    reader.FUN_00415dc0(8);
    reader.FUN_00415dc0(0x10);
    int tick = reader.FUN_00415dc0(0x20);
    p->ticks = tick;
    if (p->units_begin == 0)
        return;

    short index = (short)reader.FUN_00415dc0(0x10);
    while (index != -1) {
        Unit_0048b920* unit = &p->units_begin[index];
        unsigned short type = (unsigned short)reader.FUN_00415dc0(g_game->field_14393);
        if (unit->field_a6 != type) {
            spawn.id = unit->field_a8;
            spawn.type = type;
            spawn.player = 9;
            spawn.pos = unit->pos;
            spawn.tail = unit->tail;
            FUN_004861d0(unit->player, &spawn);
        }
        unit->owner->iface->ReadFrom(&reader);
        index = (short)reader.FUN_00415dc0(0x10);
    }

    for (Unit_0048b920* u = p->units_begin; u <= p->units_end;
         u = (Unit_0048b920*)((char*)u + 0x118)) {
        if ((u->flags & 0x10000000) && u->owner) {
            u->owner->FUN_0043dd20(u);
            FUN_0048a870(u);
        }
    }

    if (reader.ReadBit())
        FUN_0048b3f0(&reader, &p->units_begin[tick % g_game->field_37ee6]);
}

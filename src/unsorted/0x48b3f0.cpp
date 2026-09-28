// Decompiled by space-bunny-free. Names are provisional.
// The per unit reader of a player's unit packet (the writer is the matched
// 0x48b200, the packet builder 0x48b710, the packet reader 0x48b920). The unit's
// type index comes in first in g_game+0x14393 bits: a type of 0 means "this unit
// is gone", and the unit is only flagged (0x4000) if it still had one. A type
// that differs from the unit's own makes it again through 0x4861d0, from a
// spawn record built on the stack: player 9, the new type, the unit's own id,
// its position and its tail. Then the block at +0x9e is cleared, and the
// unit's own state comes in: 16 bits at +0x108, the alpha byte (which also
// rescale u->f104 when it moved), the state byte at +0x10e (set through
// 0x48b090 once set and once cleared with its complement), two bits of
// colour, and then one bit that picks the rest: set, the two ids and one more
// byte go to 0x48ab70 as a seven byte order record and the function is done.
// Clear, a unit that already has an owner sends 0xff as the order's parameter,
// and the position (three 32 bit words), the tail (three 16 bit words) and the
// owner's int are read; the position is converted from world units to the
// unit's 1/16 units against the fixed point pair at +0x7e, and if it, the tail
// owner and the colour are all unchanged the position is stored as it is, else
// the unit is unlinked, the position, the 1/16 pair and the colour are stored
// and the unit is re-registered (0x47cc30 then 0x4827b0).
//
// NOT MATCHED: 80.8%, 792 bytes against the original's 790. Everything matches
// from the prologue to 0x48b522 and again in the order records and the six
// reads, including several shapes that were not obvious:
// - `int zero = 0` after the first read is what puts a 0 in ebp, and it is
//   compared with `cmp ax, bp` (16 bit) and stored and pushed from ebp. Any
//   spelling that lets the front end fold the 0 (a literal, or the declaration
//   before the call) gives `test ax,ax`, `mov [eax+0x10],0` and `push 0`.
// - `(u->tail.a & ~0xff) | u->player` is what the first argument of 0x4861d0
//   is: MSVC 5 keeps the mask in the loaded dword and only replaces the low
//   byte (`mov edx,[u+0x64] / ... / mov dl,[u+0xff] / push edx`), so the
//   argument really does carry the tail's upper three bytes into a parameter
//   that FUN_004861d0 uses as a player index (see "suspected bug" below).
// - `__int64 alpha = (unsigned int)reader->FUN_00415dc0(8)` gives the original's
//   `fild qword` with a constant 0 in the high word. A signed int gives a
//   `cdq` the original does not have and `unsigned __int64` does not convert
//   to float at all in VC5.
// - 0x48b090's first parameter is a byte: the state byte is spilled with a
//   byte store and pushed as a dword read back from that slot, and the second
//   call pushes ebx (only its low byte is not-ed), neither of which MSVC 5
//   does for an int parameter.
// - The two order records share the slot of the 64 bit alpha, and its type
//   byte is never stored: the original leaves the alpha's low byte there.
// - The frame is 0x20 with the saved registers at the bottom, so the order
//   record, the alpha, the tail and the spawn record all share those 32 bytes,
//   the state byte and the 1/16 pair share the first dead argument slot, and
//   the fixed point copy at +0x7e lives in the second one (MSVC 5 does overlay
//   locals on dead incoming arguments).
//
// What still differs, and it is ONE allocator decision, not four: the
// original keeps the colour read at 0x48b522 in ebx and the position's x in
// ebp, with the position's z in the frame; this version spills the colour to
// [esp+0x34] and gives ebx to the position's x and ebp to its z. That flips
// the whole second half: the colour compare, the two bitfield stores (this
// version gets MSVC 5's xor/and/xor bitfield form where the original gets
// and/or, because the value arrives from a reload rather than from a named
// local in a register), the three position stores and the tail copy.
// Tried, all with the same 80.8% or worse: declaring +0x7e as a two short
// struct and copying it into a named local (that is right: it is the only way
// to get the original's `mov eax,[u+0x7e] / mov [esp+0x34],eax / movsx ecx,
// word [esp+0x36]` with the low half still in a register, but the extra 4 byte
// local does not fit the 0x20 frame, so the frame grows to 0x24 and every
// other stack offset shifts, 66%); one shared order record instead of two
// (66.8%); reading the 1/16 pair's y before its x (80.2%); an extra live
// reference on the position's z to demote it out of ebp (no change); and the
// colour compared against `u->f110.bits.colour` instead of `u->f110.all & 3`
// (identical bytes, so the compare is not what decides it). Per the brief's
// rule about one upstream cause, the next thing to try is a construct that
// raises the colour's priority or lowers the position's, not a rewrite of the
// bitfield store.

#pragma pack(push, 1)

class Class_00415dc0 {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int FUN_00415dc0(int bits);
    int FUN_00415e60(int bits);

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

struct Pos_0048b3f0 { int x, y, z; };
struct Tail_0048b3f0 { unsigned short a, b, c; };
struct Pos2_0048b3f0 { short x, y; };

struct Spawn_0048b3f0 {
    unsigned char player;              // +0x00
    unsigned short type;               // +0x01
    unsigned short id;                 // +0x03
    Pos_0048b3f0 pos;                  // +0x05
    Tail_0048b3f0 tail;                // +0x11
};

struct Order_0048b3f0 {
    unsigned char type;                // +0x00
    unsigned short id1;                // +0x01
    unsigned short id2;                // +0x03
    unsigned char param;               // +0x05
    unsigned char param2;              // +0x06
};

struct Block_0048b3f0 {
    char unknown_0[0x10];
    int field_10;                      // +0x10
};

struct Owner_0048b3f0 {
    char unknown_0[0x20];
    int field_20;                      // +0x20
};

union Flags_0048b3f0 {
    struct {
        unsigned int colour : 2;
        unsigned int r2 : 11;
        unsigned int b13 : 1;          // 0x2000
        unsigned int b14 : 1;          // 0x4000
        unsigned int b15 : 1;
        unsigned int b16 : 1;          // 0x10000
        unsigned int r17 : 11;
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int high : 2;
    } bits;
    unsigned int all;
};

class Class_0048b090 {
public:
    void* vptr;                        // +0x00
    char unknown_4[0x64 - 0x4];
    Tail_0048b3f0 tail;                // +0x64
    Pos_0048b3f0 pos;                  // +0x6a
    Pos2_0048b3f0 np;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    int fixed;                         // +0x7e, two 16.16 halves
    char unknown_82[0x86 - 0x82];
    Owner_0048b3f0* owner;             // +0x86
    char unknown_8a[0x9e - 0x8a];
    void* block;                       // +0x9e
    char unknown_a2[0xa6 - 0xa2];
    unsigned short type;               // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char player;              // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    short field_108;                   // +0x108
    char unknown_10a[0x110 - 0x10a];
    Flags_0048b3f0 f110;               // +0x110
    char unknown_114[0x118 - 0x114];

    void FUN_0048b090(unsigned char mask, int set);
};

struct Game_0048b3f0 {
    char unknown_0[0x14393];
    int field_14393;                   // +0x14393, bits of the unit type index
};

#pragma pack(pop)

extern Game_0048b3f0* g_game;
extern float DAT_004fd750;

Class_0048b090* __stdcall FUN_004861d0(unsigned char player, Spawn_0048b3f0* spawn);
void __stdcall FUN_0048ab70(Order_0048b3f0* order);
void __stdcall FUN_0047d0e0(Class_0048b090* u);
void __stdcall FUN_0047cc30(Class_0048b090* u);
void __stdcall FUN_004827b0(Class_0048b090* u);

// FUNCTION: 0x48b3f0
void __stdcall FUN_0048b3f0(Class_00415dc0* reader, Class_0048b090* u)
{
    unsigned short type = (unsigned short)reader->FUN_00415dc0(g_game->field_14393);
    int zero = 0;
    if (type == zero) {
        if (u->type != zero)
            u->f110.bits.b14 = 1;
        return;
    }
    if (u->type != type) {
        Spawn_0048b3f0 spawn;
        spawn.type = type;
        spawn.id = u->id;
        spawn.player = 9;
        spawn.pos = u->pos;
        spawn.tail = u->tail;
        FUN_004861d0((u->tail.a & ~0xff) | u->player, &spawn);
    }
    ((Block_0048b3f0*)u->block)->field_10 = zero;
    u->field_108 = reader->FUN_00415dc0(0x10);
    __int64 alpha = (unsigned int)reader->FUN_00415dc0(8);
    float scale = (float)alpha * DAT_004fd750;
    if (scale != u->field_104) {
        u->field_104 = scale;
        u->f110.bits.b13 = 1;
    }
    unsigned char state = (unsigned char)reader->FUN_00415dc0(8);
    u->FUN_0048b090(state, 1);
    u->FUN_0048b090((unsigned char)~state, zero);
    int colour = reader->FUN_00415dc0(2);
    if (reader->ReadBit()) {
        Order_0048b3f0 order;
        order.id1 = u->id;
        order.id2 = (unsigned short)reader->FUN_00415dc0(0xf);
        order.param2 = (unsigned char)~state;
        order.param = (unsigned char)reader->FUN_00415e60(8);
        FUN_0048ab70(&order);
        return;
    }
    if (u->owner) {
        Order_0048b3f0 order;
        order.id1 = u->id;
        order.id2 = 0;
        order.param = 0xff;
        order.param2 = (unsigned char)~state;
        FUN_0048ab70(&order);
    }
    Pos_0048b3f0 pos;
    pos.x = reader->FUN_00415dc0(0x20);
    pos.y = reader->FUN_00415dc0(0x20);
    pos.z = reader->FUN_00415dc0(0x20);
    Tail_0048b3f0 tail;
    tail.b = (unsigned short)reader->FUN_00415dc0(0x10);
    tail.c = (unsigned short)reader->FUN_00415dc0(0x10);
    tail.a = (unsigned short)reader->FUN_00415dc0(0x10);
    int fixed = u->fixed;
    Pos2_0048b3f0 np;
    np.x = (short)((pos.x - ((short)fixed << 19) + 0x80000) >> 20);
    np.y = (short)((pos.z - ((short)(fixed >> 16) << 19) + 0x80000) >> 20);
    if (np.x == u->np.x && np.y == u->np.y && colour == (u->f110.all & 3)) {
        u->pos = pos;
    } else {
        FUN_0047d0e0(u);
        u->pos = pos;
        u->np = np;
        u->f110.bits.colour = colour;
        FUN_0047cc30(u);
        FUN_004827b0(u);
    }
    u->f110.bits.b16 = 1;
    u->tail = tail;
    if (u->owner)
        u->owner->field_20 = reader->FUN_00415dc0(0x20);
}

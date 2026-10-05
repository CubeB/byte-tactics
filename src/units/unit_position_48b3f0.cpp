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
// and the position (three 32 bit words) and the tail (three 16 bit words) are
// read; the position is converted from world units to the unit's 1/16 units
// against the fixed point pair at +0x7e, and if it, the tail owner and the
// colour are all unchanged the position is stored as it is, else the unit is
// unlinked, the position, the 1/16 pair and the colour are stored and the unit
// is re-registered (0x47cc30 then 0x4827b0). The last word in the packet goes
// to the int at +0x20 of whatever the unit's own first pointer (at +0x00, not
// the owner at +0x86 that the order record tests) points at.
//
// MATCHES (790 bytes). What the earlier 80.8% attempt had left as "one
// allocator decision" was really three, and each needed its own source shape:
//
// 1. The order record's byte at +6 is the COLOUR, not the complement of the
//    state byte. That is what puts the colour in ebx: the original's four uses
//    of ebx between 0x48b532 and 0x48b6b3 are the two `order.param2` byte
//    stores, the compare and the bitfield store, and nothing ever nots it, so
//    the not-ed state temp is dead at the push of 0x48b516 and leaves ebx free
//    for the colour. 0x48ab70's own comments agree: the byte at +6 is blended
//    into two bits of the type's byte, and the matched writer 0x48b200 sends
//    the colour as a two bit field of its own. `~state` there is 80.8% with
//    ebx, ebp and the frame all shuffled; `colour` is 82.7% with the right
//    registers.
// 2. The colour is read as `int` but written through `(unsigned short)` in the
//    bitfield store. A plain `int` local live across nine calls gets a frame
//    home in MSVC 5 (`mov ebx,eax` *and* `mov [esp+0x34],ebx`, then a reload at
//    the compare), and only the narrow store stops the home from existing. The
//    narrowing cast is free (no instruction) and the value keeps the original's
//    32 bit `cmp ebx, eax`, where an `unsigned short` local narrows it to
//    `cmp bx, ax` (94.3%). 82.7% -> 95.2%.
// 3. `fixed.x * 0x80000` instead of `(fixed.x << 19)`. Both lower to the same
//    `shl`, but the multiply gives the front end a different tree to walk, and
//    that is what produces the original's order: spill the pair to the frame,
//    take the high half out of the frame first, then the low half out of the
//    register. With the shift the high half is extracted five instructions
//    late (95.2% -> MATCH). Reading +0x7e as a two short struct and copying it
//    into a named local is also needed: it is the only way to get the high
//    half out of the frame at all.
// 4. The unit pointer read at the very end is +0x00, not the owner at +0x86
//    that the 0x48b590 order record tests. Two different pointers, and the
//    first attempt had both at +0x86.
// 5. In the first order record the source assigns `param` before `param2`, and
//    only that order leaves MSVC 5's `mov byte [esp+0x16], bl` after the two
//    read calls, where the reverse order hoists it above them.
//
// The rest of the earlier notes still hold and are worth keeping:
// - `int zero = 0` after the first read is what puts a 0 in ebp, and it is
//   compared with `cmp ax, bp` (16 bit) and stored and pushed from ebp. Any
//   spelling that lets the front end fold the 0 (a literal, or the declaration
//   before the call) gives `test ax,ax`, `mov [eax+0x10],0` and `push 0`.
// - `(u->tail.a & ~0xff) | u->player` is what the first argument of 0x4861d0
//   is: MSVC 5 keeps the mask in the loaded dword and only replaces the low
//   byte (`mov edx,[u+0x64] / ... / mov dl,[u+0xff] / push edx`), so the
//   argument really does carry the tail's upper three bytes into a parameter
//   that CreateUnitFromPacket uses as a player index (see "suspected bug" below).
// - `__int64 alpha = (unsigned int)reader->ReadBits(8)` gives the original's
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
// Dead ends, all of which the fix above supersedes, kept so nobody repeats
// them: one shared order record instead of two (66.8%); reading the 1/16
// pair's y before its x (80.2%, and it moves the np frame slot with it); the
// colour compared through `u->f110.bits.colour` or written as a hand written
// `u->f110.all = (u->f110.all & ~3) | colour` (both give the xor/and/xor
// bitfield form, or `and al, 0xfc`); an `int colour` with an `& 3` at the read
// (89.8%, but the mask is emitted eagerly and the store then needs the
// xor/and/xor form); `unsigned short colour` as the local (94.3%, right
// registers but `cmp bx, ax`); the colour through a `static inline` read
// helper, which MSVC 5 inlines back to a range analysed int (66.4%); a union
// around the colour (61.5%); the fixed pair read as two shorts out of the unit
// (70.7%, two loads instead of one dword load and a spill).

#pragma pack(push, 1)

class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);
    int ReadSignedBits(int bits);

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
    Pos2_0048b3f0 fixed2;               // +0x7e, two 16.16 halves
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

    void SetStateBits(unsigned char mask, int set);
};

struct Game {
    char unknown_0[0x14393];
    int field_14393;                   // +0x14393, bits of the unit type index
};

#pragma pack(pop)

extern Game* g_game;
extern float DAT_004fd750;

Class_0048b090* __stdcall CreateUnitFromPacket(unsigned char player, Spawn_0048b3f0* spawn);
void __stdcall ApplyAttachUnit(Order_0048b3f0* order);
void __stdcall FUN_0047d0e0(Class_0048b090* u);
void __stdcall FUN_0047cc30(Class_0048b090* u);
void __stdcall FUN_004827b0(Class_0048b090* u);

// FUNCTION: 0x48b3f0
void __stdcall ReadUnitState(BitReader* reader, Class_0048b090* u)
{
    unsigned short type = (unsigned short)reader->ReadBits(g_game->field_14393);
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
        CreateUnitFromPacket((u->tail.a & ~0xff) | u->player, &spawn);
    }
    ((Block_0048b3f0*)u->block)->field_10 = zero;
    u->field_108 = reader->ReadBits(0x10);
    __int64 alpha = (unsigned int)reader->ReadBits(8);
    float scale = (float)alpha * DAT_004fd750;
    if (scale != u->field_104) {
        u->field_104 = scale;
        u->f110.bits.b13 = 1;
    }
    unsigned char state = (unsigned char)reader->ReadBits(8);
    u->SetStateBits(state, 1);
    u->SetStateBits((unsigned char)~state, zero);
    int colour = reader->ReadBits(2);
    if (reader->ReadBit()) {
        Order_0048b3f0 order;
        order.id1 = u->id;
        order.id2 = (unsigned short)reader->ReadBits(0xf);
        order.param = (unsigned char)reader->ReadSignedBits(8);
        order.param2 = (unsigned char)colour;
        ApplyAttachUnit(&order);
        return;
    }
    if (u->owner) {
        Order_0048b3f0 order;
        order.id1 = u->id;
        order.id2 = 0;
        order.param = 0xff;
        order.param2 = (unsigned char)colour;
        ApplyAttachUnit(&order);
    }
    Pos_0048b3f0 pos;
    pos.x = reader->ReadBits(0x20);
    pos.y = reader->ReadBits(0x20);
    pos.z = reader->ReadBits(0x20);
    Tail_0048b3f0 tail;
    tail.b = (unsigned short)reader->ReadBits(0x10);
    tail.c = (unsigned short)reader->ReadBits(0x10);
    tail.a = (unsigned short)reader->ReadBits(0x10);
    Pos2_0048b3f0 fixed = u->fixed2;
    Pos2_0048b3f0 np;
    np.x = (short)((pos.x - fixed.x * 0x80000 + 0x80000) >> 20);
    np.y = (short)((pos.z - fixed.y * 0x80000 + 0x80000) >> 20);
    if (np.x == u->np.x && np.y == u->np.y && colour == (u->f110.all & 3)) {
        u->pos = pos;
    } else {
        FUN_0047d0e0(u);
        u->pos = pos;
        u->np = np;
        u->f110.bits.colour = (unsigned int)(unsigned short)colour;
        FUN_0047cc30(u);
        FUN_004827b0(u);
    }
    u->f110.bits.b16 = 1;
    u->tail = tail;
    if (u->vptr)
        ((Owner_0048b3f0*)u->vptr)->field_20 = reader->ReadBits(0x20);
}
//
// Three shape notes, and one correction to a bug claim.
//
// The +6 byte of the order record is the COLOUR, not `~state`. Nothing in the
// disassembly ever `not`s the ebx used at 0x48b532..0x48b6b3, so the not-ed
// state temporary dies at the push of 0x48b516 and leaves ebx free for the
// colour. The already matched 0x48ab70's notes agree: the +6 byte is blended
// into two bits of the type's byte. 80.8% to 82.7%.
//
// The colour is read as an `int` but WRITTEN through a free `(unsigned short)`
// narrowing cast in the bitfield store, and that is worth twelve points. A plain
// int live across nine calls gets a frame home: `mov ebx,eax` *and*
// `mov [esp+0x34],ebx`, then a reload at the compare. The narrowing cast at the
// store removes the home while keeping the original's 32-bit `cmp ebx, eax`.
// An `unsigned short` local gives the right registers but narrows the compare
// to `cmp bx, ax`. So the two uses want different widths and a free cast is
// what lets them have them. 82.7% to 95.2%.
//
// `fixed.x * 0x80000` rather than `(fixed.x << 19)`. Both lower to the same
// `shl`, but the multiply gives the front end a different tree, and that is
// what produces the original's order of taking the high half from the frame
// first and the low half from the register afterwards. 95.2% to MATCH.
//
// CORRECTION. A bug was reported here on the strength of the call site at
// 0x48b475-0x48b491, which is `mov edx,[ecx] / mov dl,[edi+0xff] / push edx`,
// i.e. `(u->tail.a & ~0xff) | u->player`, on the grounds that the tail's upper
// three bytes are carried into an argument that is used as a player index. That
// is not a bug. CreateUnitFromPacket's third instruction is `and eax, 0xff` at
// 0x4861d8, so the callee discards the upper three bytes itself; the composite
// construction at the call site and the mask in the callee are complementary,
// and the source is almost certainly exactly `(u->tail.a & ~0xff) | u->player`
// with the callee picking the byte out. Reporting it would have been a false
// positive, and the way to see that was one instruction of the callee.

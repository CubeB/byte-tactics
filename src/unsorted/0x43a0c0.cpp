// Decompiled by space-bunny-free. Names are provisional.
// GAVE UP at 92% (274 of 274 bytes, one instruction misplaced). Everything
// matches except that the original sinks the `cmp eax, ebx` that tests the
// position pointer past the three constructor stores, so the original reads
//   mov eax, [esp+0x28] / mov [esi+0x46], ecx / mov [esi], vtable
//   mov [esi+0x1e], esi / cmp eax, ebx
// while this file reads
//   mov eax, [esp+0x28] / cmp eax, ebx / mov [esi+0x46], ecx
//   mov [esi], vtable / mov [esi+0x1e], esi
// The load of the pointer is hoisted correctly in both; only the compare is
// scheduled early here. Already ruled out (all scored, none better): all 16
// permutations of the five body statements, an if/else and a `p != 0` form
// of the position select (both drop to about 51%), a `bool` local, a pointer
// local, `(int)p` and `(p != 0)` tests, a by-value cast of the temporary, and
// an inline helper returning the selected pointer. Putting the compare in a
// separate inline helper changes the copy shape and drops to 64%. This looks
// like compiler state from the rest of the original translation unit, the
// case the guide calls "operand order that nothing changes".
// The constructor of the object whose vtable is 0x4fd2c8 (the class the
// destructor 0x43a1f0 belongs to): it first stores 0x4fd2cc, the vtable of
// the inline base constructor (a base with the single slot FUN_0043a1e0,
// which the class below overrides with FUN_00438870), then runs the member
// initialisers and stores its own vtable. It also loads the kind's default
// flag bits from the kind table at DAT_00512344 and clears bits 9 and 10 of
// them when the list owner and the position pointer are absent.
// The two 4-byte fields at +0x2e and +0x32 are pairs of shorts built through
// a temporary (a derived point, sliced into the member), which is why the
// original writes each pair with two 16-bit stores and copies the result.

#pragma pack(push, 1)
struct Entry_0043a0c0 {              // 0x19-byte entries, table at DAT_00512344
    char unknown_0[0x11];
    unsigned int flags;              // +0x11, default flags of the kind
    char unknown_15[4];
};

struct Vec3_0043a0c0 {
    int x, y, z;
};

// The temporary the position is built through (the derived type is what
// makes the compiler materialise it).
struct Vec3Init_0043a0c0 : Vec3_0043a0c0 {
    Vec3Init_0043a0c0(int a, int b, int c) { x = a; y = b; z = c; }
};

struct Game_0043a0c0 {
    char unknown_0[0x38a47];
    unsigned int ticks;              // +0x38a47
};

// A 4-byte point of two shorts, and the temporary the constructor builds it
// through (the derived type is what makes the compiler keep the temporary).
struct Point_0043a0c0 {
    short x, y;
};

struct PointInit_0043a0c0 : Point_0043a0c0 {
    PointInit_0043a0c0(short a, short b) { x = a; y = b; }
};
#pragma pack(pop)

extern Game_0043a0c0* g_game;
extern Entry_0043a0c0* DAT_00512344;

struct Owner_0043a0c0;               // the owner of a link list

class Class_004895c0 {
public:
    void* vptr;                      // +0x0
    Owner_0043a0c0* owner;           // +0x4
    Class_004895c0* next;            // +0x8

    Class_004895c0(Owner_0043a0c0* o, int v);
    void FUN_00489690(Owner_0043a0c0* o);
};

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(int);
};

#pragma pack(push, 1)
class Class_0043a0c0 : public Class_0043a1e0 {
public:
    virtual void FUN_00438870(int);  // slot 0, at 0x4fd2c8

    unsigned char kind;              // +0x4
    unsigned char flag5;             // +0x5
    unsigned int flags6;             // +0x6
    int last_id;                     // +0xa
    void* unit;                      // +0xe
    Class_004895c0 link;             // +0x12
    void* owner;                     // +0x1e
    Vec3_0043a0c0 pos;               // +0x22
    Point_0043a0c0 field_2e;         // +0x2e
    Point_0043a0c0 field_32;         // +0x32
    int field_36;                    // +0x36
    int field_3a;                    // +0x3a
    int field_3e;                    // +0x3e
    unsigned int flags;              // +0x42
    unsigned int created;            // +0x46
    int field_4a;                    // +0x4a
    int field_4e;                    // +0x4e
    void* attached;                  // +0x52

    Class_0043a0c0(int k, Owner_0043a0c0* o, Vec3_0043a0c0* p, int a, int b, int c);
};
#pragma pack(pop)

// FUNCTION: 0x43a0c0
Class_0043a0c0::Class_0043a0c0(int k, Owner_0043a0c0* o, Vec3_0043a0c0* p, int a, int b, int c)
    : kind(k), link(o, 0), field_2e(PointInit_0043a0c0(0, 0)), field_32(PointInit_0043a0c0(0, 0)),
      field_36(a), field_3a(b), field_3e(c), created(g_game->ticks)
{
    owner = this;
    flag5 = 0;
    flags6 = 0;
    field_4e = 0;
    last_id = -1;
    pos = p ? *p : Vec3Init_0043a0c0(0, 0, 0);
    flags = DAT_00512344[k & 0xff].flags;
    unit = 0;
    field_4a = 0;
    attached = 0;
    if (o == 0)
        flags &= ~0x200;
    if (p == 0)
        flags &= ~0x400;
    if (!(flags & 0x200))
        link.FUN_00489690(0);
}

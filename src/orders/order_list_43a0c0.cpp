// Decompiled by space-bunny-free, class hierarchy fixed by Claude Opus 5.5. Names are provisional.
// The constructor of Class_0043a1f0 (destructor 0x43a1f0, vtable 0x4fd2c8).
// The rest of the class is in order_queue_438870.cpp.
// It first stores 0x4fd2cc, the vtable of the inline constructor of the base
// class Class_0043a1e0, then runs the member initialisers, then stores its
// own vtable. Each vtable has one slot: the base's is the empty 0x43a1e0 and
// the derived class overrides it with 0x438870 (which ORs bits into +0x4e).
// The other constructor, 0x43a420 (which loads the object from a file),
// stores the same two vtables; the destructor stores only 0x4fd2c8, so the
// base has no destructor of its own.
// The link member's constructor puts the object in its owner's list and the
// body then points the link's value at the object; the kind's default flag
// bits come from the kind table at DAT_00512344, with bits 9 and 10 cleared
// when the list owner or the position pointer is absent.
// Stays apart from the rest of the class: it needs `kind(k)` as a plain
// member initialiser.

#pragma pack(push, 1)
struct Entry_0043a1f0 {              // 0x19-byte entries, table at DAT_00512344
    char unknown_0[0x11];
    unsigned int flags;              // +0x11, default flags of the kind
    char unknown_15[4];
};

struct Vec3_0043a1f0 {
    int x, y, z;
};

// The temporary the position is built through (the derived type is what
// makes the compiler materialise it).
struct Vec3Init_0043a1f0 : Vec3_0043a1f0 {
    Vec3Init_0043a1f0(int a, int b, int c) { x = a; y = b; z = c; }
};

struct Game {
    char unknown_0[0x38a47];
    unsigned int ticks;              // +0x38a47
};

// A 4-byte point of two shorts, and the temporary the constructor builds it
// through (the derived type is what makes the compiler keep the temporary).
struct Point_0043a1f0 {
    short x, y;
};

struct PointInit_0043a1f0 : Point_0043a1f0 {
    PointInit_0043a1f0(short a, short b) { x = a; y = b; }
};
#pragma pack(pop)

extern Game* g_game;
extern Entry_0043a1f0* DAT_00512344;

struct ListOwner_0043a1f0;            // the owner of a link list

class Class_004895c0 {
public:
    void* vptr;                      // +0x0
    ListOwner_0043a1f0* owner;       // +0x4
    Class_004895c0* next;            // +0x8
    void* value;                     // +0xc, the object the link belongs to

    void SetValue(void* v) { value = v; }

    Class_004895c0(ListOwner_0043a1f0* o, int v);
    void SetUnit(ListOwner_0043a1f0* o);
};

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);  // slot 0: 0x43a1e0, empty
};

#pragma pack(push, 1)
class Class_0043a1f0 : public Class_0043a1e0 {
public:
    // Slot 0 of vtable 0x4fd2c8, overriding the base's: 0x438870 (defined
    // in 0x438870.cpp).
    virtual void FUN_0043a1e0(unsigned int);

    unsigned char kind;              // +0x4
    unsigned char flag5;             // +0x5
    unsigned int flags6;             // +0x6
    int last_id;                     // +0xa
    void* unit;                      // +0xe
    Class_004895c0 link;             // +0x12
    Vec3_0043a1f0 pos;               // +0x22
    Point_0043a1f0 field_2e;         // +0x2e
    Point_0043a1f0 field_32;         // +0x32
    int field_36;                    // +0x36
    int field_3a;                    // +0x3a
    int field_3e;                    // +0x3e
    unsigned int flags;              // +0x42
    unsigned int created;            // +0x46
    int field_4a;                    // +0x4a
    int field_4e;                    // +0x4e
    void* attached;                  // +0x52

    Class_0043a1f0(int k, ListOwner_0043a1f0* o, Vec3_0043a1f0* p, int a, int b, int c);
};
#pragma pack(pop)

// FUNCTION: 0x43a0c0
Class_0043a1f0::Class_0043a1f0(int k, ListOwner_0043a1f0* o, Vec3_0043a1f0* p, int a, int b, int c)
    : kind(k), link(o, 0), field_2e(PointInit_0043a1f0(0, 0)), field_32(PointInit_0043a1f0(0, 0)),
      field_36(a), field_3a(b), field_3e(c), created(g_game->ticks)
{
    // Through the inline method: a plain `link.value = this` reorders the
    // position pointer's compare before the vtable store.
    link.SetValue(this);
    flag5 = 0;
    flags6 = 0;
    field_4e = 0;
    last_id = -1;
    pos = p ? *p : Vec3Init_0043a1f0(0, 0, 0);
    flags = DAT_00512344[k & 0xff].flags;
    unit = 0;
    field_4a = 0;
    attached = 0;
    if (o == 0)
        flags &= ~0x200;
    if (p == 0)
        flags &= ~0x400;
    if (!(flags & 0x200))
        link.SetUnit(0);
}

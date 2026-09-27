// Decompiled by space-bunny-free. Names are provisional.
// The constructor of the class whose vtable is 0x4fd2c8: it first stores
// 0x4fd2cc, the vtable of the base class (a single slot, FUN_0043a1e0), then
// runs the member initialisers, then stores its own vtable (slot 0 is
// FUN_00438870, slot 1 the inherited FUN_0043a1e0). The link member's
// constructor puts the object in its owner's list; the kind's default flag
// bits come from the kind table at DAT_00512344, with bits 9 and 10 cleared
// when the list owner or the position pointer is absent.
// The two 4-byte fields at +0x2e and +0x32 are pairs of shorts built through
// a temporary (a derived point, sliced into the member), which is why the
// original writes each pair with two 16-bit stores and copies the result.

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

struct Game_0043a1f0 {
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

extern Game_0043a1f0* g_game;
extern Entry_0043a1f0* DAT_00512344;

struct ListOwner_0043a1f0;            // the owner of a link list

class Class_004895c0 {
public:
    void* vptr;                      // +0x0
    ListOwner_0043a1f0* owner;       // +0x4
    Class_004895c0* next;            // +0x8

    Class_004895c0(ListOwner_0043a1f0* o, int v);
    void FUN_00489690(ListOwner_0043a1f0* o);
};

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(int);
};

// The link's value slot, where the object keeps its own pointer. Writing it
// through a method is what leaves the position select's compare in place: the
// original tests p after this store, and the method's block stops the
// scheduler from hoisting the compare above it. The cleared store is dead
// code, but its statement is what the original must have had.
struct LinkValue_0043a1f0 {
    void* value;                     // +0x1e

    void Set(void* v)
    {
        value = 0;
        value = v;
    }
};

#pragma pack(push, 1)
class Class_0043a1f0 : public Class_0043a1e0 {
public:
    virtual void FUN_00438870(int);  // slot 0, at 0x4fd2c8

    unsigned char kind;              // +0x4
    unsigned char flag5;             // +0x5
    unsigned int flags6;             // +0x6
    int last_id;                     // +0xa
    void* unit;                      // +0xe
    Class_004895c0 link;             // +0x12
    LinkValue_0043a1f0 self;         // +0x1e
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
    self.Set(this);
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
        link.FUN_00489690(0);
}

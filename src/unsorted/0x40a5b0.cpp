// Decompiled by Haiku. Names are provisional.
// Elem_0040cc40's copy constructor (a map cell and its sort key, the element
// of the vector at +0x4d of the player AI object). The other files define it
// inline in the class, and 0x40a260 calls it out of line from its inlined
// pop_heap, where /Ob2's budget ran out; here it is defined out of line so
// the compiler emits it.
struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o);
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

// FUNCTION: 0x40a5b0
Elem_0040cc40::Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key)
{
}

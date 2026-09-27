// Decompiled by Sonnet; rewritten as the compiler's own helper by Claude Opus 5.5 (#225). Names are provisional.
// The compiler-generated `vector constructor iterator` (??_H), which constructs
// each element of an array of a class with a constructor. MSVC 5 inlines it as
// a loop unless /Ob2's inline budget has run out; an element type with both a
// constructor and a destructor in an array member of a global's class keeps
// it out of line.

// FUNCTION: 0x401000 ??_H@YGXPAXIHP6EX0@Z@Z
struct Elem_00401000 {
    Elem_00401000();
    ~Elem_00401000();
    int field_0;
};

struct Holder_00401000 {
    Elem_00401000 items[4];
};

Holder_00401000 g_holder_00401000;

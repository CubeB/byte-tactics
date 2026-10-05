// Decompiled by Opus. Names are provisional.
// Out-of-line destructor of the element of std::vector<Elem_004c5bc0> (two
// reference-counted string handles, see 0x4c5bc0.cpp): called on each
// element in the vectors' destroy loops, and on the local passed to insert
// (0x4c59d0) at the end of its scope. The handles are released in reverse
// order with FUN_004c9390.

class Class_004c9390 {
public:
    char* data;

    void FUN_004c9390();
};

struct Elem_004c5bc0 {
    Class_004c9390 a;                  // +0x0
    Class_004c9390 b;                  // +0x4

    ~Elem_004c5bc0();
};

// FUNCTION: 0x4c5190
Elem_004c5bc0::~Elem_004c5bc0()
{
    b.FUN_004c9390();
    a.FUN_004c9390();
}

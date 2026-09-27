// Decompiled by Opus. Names are provisional.
// Returns the address of element n - 1 of a std::vector of
// std::vector<Elem_00434020> held at +0 (the global at 0x51e6a0). Callers
// pass size() - 1, so this is the last element. The index is narrowed to a
// short before indexing; only the real std::vector operator[] keeps _First
// loaded before the index arithmetic, as in the original.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

typedef std::vector<Elem_00434020> Inner_00433500;

class Class_00433500 {
public:
    std::vector<Inner_00433500> items;  // +0x0 (_First at +0x4)

    Inner_00433500* FUN_00433500(int n);
};

// FUNCTION: 0x433500
Inner_00433500* Class_00433500::FUN_00433500(int n)
{
    return &items[(short)(n - 1)];
}

// Decompiled by Opus. Names are provisional.
// Returns the address of element n - 1 of a std::vector of
// std::vector<Elem_00434020> held at +0 (the global at 0x51e6a0). Callers
// pass size() - 1, so this is the last element. The index is narrowed to a
// short before indexing; only the real std::vector operator[] keeps _First
// loaded before the index arithmetic, as in the original. The global is
// really a std::vector<std::vector<Elem_00434360> > (see 0x4330b0.cpp); with
// that spelling MSVC schedules the index arithmetic differently, and the
// return type is not part of the name data/symbols.csv compares.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Inner_00433500;

class LosTables {
public:
    std::vector<Inner_00433500> items;  // +0x0 (_First at +0x4)

    Inner_00433500* GetLosTable(int n);
};

// FUNCTION: 0x433500
Inner_00433500* LosTables::GetLosTable(int n)
{
    return &items[(short)(n - 1)];
}

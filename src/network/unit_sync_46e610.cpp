// Decompiled by Sonnet. Names are provisional.
// A std::vector<int> member's destructor: MSVC 5's ~vector runs an
// element-destroy loop over the (trivial, no-op) elements, which the
// optimiser empties out but still leaves one dead store (and the `push ecx`
// that makes room for it), then frees the storage and zeroes the
// {_First,_Last,_End} triple. See src/map/meteors_438480.cpp for the same note.
#include <vector>

class Class_0046e610 {
public:
    std::vector<int> vec;

    ~Class_0046e610();
};

// FUNCTION: 0x46e610
Class_0046e610::~Class_0046e610()
{
}

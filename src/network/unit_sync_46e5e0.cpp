// Decompiled by Opus. Names are provisional.
// Out-of-line destructor of a class whose only member is a std::vector of a
// trivial type: the emptied element-destroy loop leaves one dead store (the
// push ecx slot), then the storage is freed and {_First,_Last,_End} zeroed.
// Byte-identical to 0x46e610, which is called on other locals of the same
// caller (0x46dxxx).
#include <vector>

struct Elem_0046e5e0 {
    int value;                         // +0x0
};

class Class_0046e5e0 {
public:
    std::vector<Elem_0046e5e0> vec;

    ~Class_0046e5e0();
};

// FUNCTION: 0x46e5e0
Class_0046e5e0::~Class_0046e5e0()
{
}

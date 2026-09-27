// Decompiled by Sonnet. Names are provisional.
// Same shape as the matched 0x40c530: the push ecx / pop ecx pair is the
// compiler's own reserved slot for the inlined std::vector<char> destructor,
// not a saved argument.
#include <vector>

class Class_0040c5d0 {
public:
    std::vector<char> vec;
    ~Class_0040c5d0();
};

// FUNCTION: 0x40c5d0
Class_0040c5d0::~Class_0040c5d0()
{
}

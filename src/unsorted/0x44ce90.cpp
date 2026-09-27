// Decompiled by Opus. Names are provisional.
// Slot 6 of the vtable at 0x4fd2f8: the default implementation ignores the
// object and empties the caller's list (an inlined vector::clear()).
#include <vector>

struct Elem_0044ce90 {
    int unknown_0;
};

class Class_0044ce90 {
public:
    void FUN_0044ce90(std::vector<Elem_0044ce90*>* list);
};

// FUNCTION: 0x44ce90
void Class_0044ce90::FUN_0044ce90(std::vector<Elem_0044ce90*>* list)
{
    list->clear();
}

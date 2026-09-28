// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Moves the cursor to the child of the current TDF node (or of the root when
// there is none) at the given index. The node's child vector is a
// std::vector<Class_004c42a0*> at +4 (_First at +8, _Last at +0xc), and the
// out-of-range case throws out_of_range through the inlined vector::at()
// (the literal "invalid vector<T> subscript"). The inline GetChild wrapper is
// what keeps logic_error's constructor out of line: without one extra inline
// level MSVC 5 inlines it into this function too, and the call to the
// out-of-line logic_error constructor (0x4c35c0) is lost.
#include <vector>

class Class_004c42a0;

#pragma pack(push, 1)
class Class_004c42a0 {
public:
    int* name;                                      // +0x0
    std::vector<Class_004c42a0*> children;          // +0x4 (_First at +0x8)

    Class_004c42a0* GetChild(int index)
    {
        return index >= children.size() ? 0 : children.at(index);
    }
};
#pragma pack(pop)

class Class_004c3490 {
public:
    Class_004c42a0* root;                           // +0x0
    Class_004c42a0* current;                        // +0x4

    int FUN_004c3490(int index);
};

// FUNCTION: 0x4c3490
int Class_004c3490::FUN_004c3490(int index)
{
    Class_004c42a0* n = current ? current : root;
    Class_004c42a0* node = n->GetChild(index);
    current = node;
    return node != 0;
}

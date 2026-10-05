// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Moves the cursor to the child of the current TDF node (or of the root when
// there is none) at the given index. The node's child vector is a
// std::vector<TdfRecord*> at +4 (_First at +8, _Last at +0xc), and the
// out-of-range case throws out_of_range through the inlined vector::at()
// (the literal "invalid vector<T> subscript"). The inline GetChild wrapper is
// what keeps logic_error's constructor out of line: without one extra inline
// level MSVC 5 inlines it into this function too, and the call to the
// out-of-line logic_error constructor (0x4c35c0) is lost.
#include <vector>

class TdfRecord;

#pragma pack(push, 1)
class TdfRecord {
public:
    int* name;                                      // +0x0
    std::vector<TdfRecord*> children;               // +0x4 (_First at +0x8)

    TdfRecord* GetChild(int index)
    {
        return index >= children.size() ? 0 : children.at(index);
    }
};
#pragma pack(pop)

class TdfFile {
public:
    TdfRecord* root;                                // +0x0
    TdfRecord* current;                             // +0x4

    int SelectRecordAt(int index);
};

// FUNCTION: 0x4c3490
int TdfFile::SelectRecordAt(int index)
{
    TdfRecord* n = current ? current : root;
    TdfRecord* node = n->GetChild(index);
    current = node;
    return node != 0;
}

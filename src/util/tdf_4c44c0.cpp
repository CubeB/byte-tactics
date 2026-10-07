// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free. Names are provisional.
// Bounds-checked accessor into the vector of pointers held at +4 (_First at
// +8). The index is checked against size() and 0 is returned when it is out
// of range; otherwise std::vector::at returns the element.
#include <vector>

struct Named_004c44c0 {
    char* name;                         // +0x0
};

class Class_004c44c0 {
public:
    int unknown_0;
    std::vector<Named_004c44c0*> entries;   // +0x4 (_First at +0x8)

    Named_004c44c0* GetSubRecord(int index);
};

// FUNCTION: 0x4c44c0
Named_004c44c0* Class_004c44c0::GetSubRecord(int index)
{
    if ((unsigned int)index >= entries.size())
        return 0;
    return entries.at(index);
}

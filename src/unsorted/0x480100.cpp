// Decompiled by Opus. Names are provisional.
// Removes the first occurrence of a value from a std::vector by moving the
// last element into its slot and erasing the last element.
#include <vector>
#include <algorithm>

class Class_00480100 {
public:
    std::vector<int> items;            // +0x0 (_First +0x4, _Last +0x8)

    int FUN_00480100(int value);
};

// FUNCTION: 0x480100
int Class_00480100::FUN_00480100(int value)
{
    std::vector<int>::iterator it = std::find(items.begin(), items.end(), value);
    if (it == items.end()) {
        return 0;
    }
    std::vector<int>::iterator last = items.end() - 1;
    *it = *last;
    items.erase(last);
    return 1;
}

// Decompiled by Opus. Names are provisional.
// size() of the std::vector of pointers held at +4 (_First at +8), the same
// layout as Class_004c4470's entries.
#include <vector>

struct Named_004c4450 {
    char* name;                        // +0x0
};

class Class_004c4450 {
public:
    int unknown_0;
    std::vector<Named_004c4450*> entries;   // +0x4 (_First at +0x8)

    int FUN_004c4450();
};

// FUNCTION: 0x4c4450
int Class_004c4450::FUN_004c4450()
{
    return entries.size();
}

// Decompiled by Opus. Names are provisional.
#include <vector>
struct Pair_004c45e0 {
    char* name;                     // +0x0
    int value;                      // +0x4
};

#pragma pack(push, 1)
class Class_004c45e0 {
public:
    char unknown_0[0x15];
    std::vector<Pair_004c45e0> pairs;   // +0x15 (_First at +0x19)

    char* FUN_004c45e0(int index);
};
#pragma pack(pop)

// FUNCTION: 0x4c45e0
char* Class_004c45e0::FUN_004c45e0(int index)
{
    if (index < 0 || (unsigned int)index >= pairs.size())
        return 0;
    unsigned int i = 0;
    for (std::vector<Pair_004c45e0>::iterator p = pairs.begin(); p < pairs.end(); p++, i++) {
        if (i == (unsigned int)index)
            return p->name;
    }
    return 0;
}

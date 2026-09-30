// Decompiled by Opus. Names are provisional.
#include <vector>

void __cdecl FUN_004d85a0(int* p);

class Class_00470e50 {
public:
    int unknown_0;                     // +0x00
    std::vector<int*> items;           // +0x04 (_First +0x08, _Last +0x0c)
    int* extra;                        // +0x14
    void FUN_00470e50();
};

// FUNCTION: 0x470e50
void Class_00470e50::FUN_00470e50()
{
    if (extra != 0) {
        FUN_004d85a0(extra);
    }
    std::vector<int*>::iterator it = items.begin();
    while (it != items.end()) {
        FUN_004d85a0(*it);
        items.erase(it);
    }
}

// Decompiled by Opus. Names are provisional.
// Destroys the owner's ten squads allocated by 0x480190 and frees them.
#include <vector>

class Class_00480160 {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<int> items;            // +0x10
};

struct Owner_004801f0 {
    char unknown_0[0x78];
    Class_00480160* squads;            // +0x78
};

void FUN_004d85a0(int* param_1);

// FUNCTION: 0x4801f0
void __stdcall FUN_004801f0(Owner_004801f0* owner)
{
    if (owner->squads) {
        for (int i = 0; i < 10; i++)
            owner->squads[i].items.~vector();
        FUN_004d85a0((int*)owner->squads);
        owner->squads = 0;
    }
}

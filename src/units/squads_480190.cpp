// Decompiled by Opus. Names are provisional.
// Allocates the owner's ten squads and constructs each in place with the
// owner and its index (Class_00480160's constructor, inlined here).
#include <vector>

class Class_00480160 {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<int> items;            // +0x10

    Class_00480160(int a, int b)
        : field_0(a), field_4(b), field_8(0), field_c(0)
    {
    }
};

struct Owner_00480190 {
    char unknown_0[0x78];
    Class_00480160* squads;            // +0x78
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x480190
void __stdcall FUN_00480190(Owner_00480190* owner)
{
    owner->squads = (Class_00480160*)FUN_004d83b0("SQUADS", 10 * sizeof(Class_00480160));
    for (int i = 0; i < 10; i++)
        new (&owner->squads[i]) Class_00480160((int)owner, i);
}

// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of the class whose
// vtable (one slot) is at 0x4fd580. Its constructor is 0x470a90 and its
// out-of-line destructor 0x470b80; the destructor is inlined here. It frees
// field_14, then frees each item of a std::vector while erasing it from the
// front, then the vector's own destructor frees the storage.
//
// The static object below exists only to make the compiler emit the vtable
// (and with it this COMDAT) here; the game's instance is the function-local
// static DAT_0051e610 constructed with (1000, 0x4c) at 0x471c80.
#include <vector>

struct Item_00470ae0 {
    int unknown_0;
};

void FUN_004d85a0(void* p);

class Class_00470ae0 {
public:
    std::vector<Item_00470ae0*> items;  // +0x4
    void* field_14;                     // +0x14
    int field_18;                       // +0x18
    int field_1c;                       // +0x1c
    int field_20;                       // +0x20

    Class_00470ae0(int param_1, int param_2);
    virtual ~Class_00470ae0()
    {
        if (field_14 != 0)
            FUN_004d85a0(field_14);
        std::vector<Item_00470ae0*>::iterator it = items.begin();
        while (it != items.end()) {
            FUN_004d85a0(*it);
            items.erase(it);
        }
    }
};

// FUNCTION: 0x470ae0 ??_GClass_00470ae0@@UAEPAXI@Z
static Class_00470ae0 s_obj(1000, 0x4c);

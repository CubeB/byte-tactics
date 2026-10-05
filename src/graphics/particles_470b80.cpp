// Decompiled by Opus. Names are provisional.
// The out-of-line destructor of ObjectPool (vtable 0x4fd580, see
// 0x470ae0.cpp, whose ??_G inlines the same body). A matched caller
// (0x471ca0.cpp) already calls it as Class_00470b80::Destroy, so it is
// written as that method, which runs the real destructor non-virtually.
#include <vector>

struct Item_00470ae0 {
    int unknown_0;
};

void __cdecl FUN_004d85a0(void* p);

class ObjectPool {
public:
    std::vector<Item_00470ae0*> items;  // +0x4
    void* field_14;                     // +0x14
    int field_18;                       // +0x18
    int field_1c;                       // +0x1c
    int field_20;                       // +0x20

    ObjectPool(int param_1, int param_2);
    virtual ~ObjectPool()
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

class Class_00470b80 {
public:
    void Destroy();
};

// FUNCTION: 0x470b80
void Class_00470b80::Destroy()
{
    ((ObjectPool*)this)->ObjectPool::~ObjectPool();
}

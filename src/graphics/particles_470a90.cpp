// Decompiled by Opus. Names are provisional.
// The constructor of ObjectPool (vtable 0x4fd580, see 0x470ae0.cpp). Its
// only caller (0x471c80.cpp) already calls it as Class_00470a90::Construct,
// so it is written as that method, which runs the real (inline) constructor
// on this through an explicit constructor call.
#include <vector>

struct Item_00470ae0 {
    int unknown_0;
};

void __cdecl FUN_004d85a0(void* p);

class Class_00470c10 {
public:
    void Grow(int param_1, int param_2);
};

class ObjectPool {
public:
    std::vector<Item_00470ae0*> items;  // +0x4
    void* field_14;                     // +0x14
    int field_18;                       // +0x18
    int field_1c;                       // +0x1c
    int field_20;                       // +0x20

    ObjectPool(int param_1, int param_2)
    {
        field_14 = 0;
        field_18 = 0;
        field_1c = 0;
        field_20 = 0;
        if (param_1 != 0 && param_2 != 0)
            ((Class_00470c10*)this)->Grow(param_1, param_2);
    }
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

class Class_00470a90 {
public:
    ObjectPool* Construct(int param_1, int param_2);
};

// FUNCTION: 0x470a90
ObjectPool* Class_00470a90::Construct(int param_1, int param_2)
{
    ((ObjectPool*)this)->ObjectPool::ObjectPool(param_1, param_2);
    return (ObjectPool*)this;
}

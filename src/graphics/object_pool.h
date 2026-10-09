// ObjectPool: the arena at g_particlePool (vtable 0x4fd580) that hands out
// fixed-size slots to the particle systems' operator new. The one declaration
// of the class, for particles.cpp, 0x471820, 0x471a50 and 0x472630; 0x470c10
// keeps its own view, which needs the hand-written vector and its block type.
// The constructor and destructor keep their bodies inside the class, since the
// files that inline operator new inline through them. GameFreeThunk and the
// item type come with the class.
#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>

// One block of the arena: the vector holds these pointers, so the element type
// has to be complete where the destructor instantiates the vector's _Destroy.
struct Item_00470ae0 {
    int unknown_0;
};
void __cdecl GameFreeThunk(void* p);

class ObjectPool {
public:
    std::vector<Item_00470ae0*> items;  // +0x4
    void* slots;                        // +0x14, the slot table
    int slotSize;                       // +0x18
    int capacity;                       // +0x1c, the slot count
    int used;                           // +0x20, slots handed out

    ObjectPool(int param_1, int param_2)
    {
        slots = 0;
        slotSize = 0;
        capacity = 0;
        used = 0;
        if (param_1 != 0 && param_2 != 0)
            Grow(param_1, param_2);
    }
    virtual ~ObjectPool()
    {
        if (slots != 0)
            GameFreeThunk(slots);
        std::vector<Item_00470ae0*>::iterator it = items.begin();
        while (it != items.end()) {
            GameFreeThunk(*it);
            items.erase(it);
        }
    }
    void FreeBlocks();
    int Grow(int param_1, int param_2);
    int AllocSlot(int unused);
    void FreeSlot(int param_1);
    ObjectPool* Construct(int param_1, int param_2);
    void Destroy();
};

#endif

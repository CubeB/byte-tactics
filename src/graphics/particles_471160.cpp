// Decompiled by space-bunny-free. Names are provisional.
// Stays in its own file: it needs the std::vector<Elem_00473500> view of the
// lists, whose inlined insert cannot agree with particles_470a40.cpp's.
// Appends an entry to the std::vector<Elem_00473500> that the short index
// picks out of the array. When the list already holds more than 400 entries its
// oldest element is deleted and erased first, exactly as the sibling
// ParticleLists::Add in particles_470f80.cpp does for
// std::vector<ParticleSystem*>.
// 476 of 476 bytes and all ten references.
#include <vector>

class Listener_00471120 {             // what the elements point at
public:
    virtual ~Listener_00471120();
    virtual void Notify(void* param);
};

struct Elem_00473500 {                // one entry, a pointer to a listener
    Listener_00471120* p;             // +0x0
};

class ParticleLists {
public:
    std::vector<Elem_00473500> lists[1];

    void DrawList(void* param, short index);
    void AddToList(Elem_00473500 x, short index);
};

// FUNCTION: 0x471160
void ParticleLists::AddToList(Elem_00473500 x, short index)
{
    if (lists[index].size() > 400) {
        delete lists[index][0].p;
        lists[index].erase(lists[index].begin());
    }
    lists[index].push_back(x);
}

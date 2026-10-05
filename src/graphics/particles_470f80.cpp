// Decompiled by Opus. Names are provisional.
// Out-of-line constructor of the ten listener lists whose destructor is
// 0x470fb0 (0x471d90 inlines this same loop after its `new`). Each vector's
// empty allocator byte is copied from an uninitialised temporary.
#include <vector>

class Listener_00470fb0 {
public:
    virtual ~Listener_00470fb0();      // vtable +0x0
};

class ParticleLists {
public:
    std::vector<Listener_00470fb0*> lists[10];

    ParticleLists();
};

// FUNCTION: 0x470f80
ParticleLists::ParticleLists()
{
}

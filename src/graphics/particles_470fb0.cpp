// Decompiled by Opus. Names are provisional.
// Destructor of the ten listener lists that 0x471d90 allocates into the game
// object (used by 0x471f40 and 0x471f90): deletes every listener, erasing it
// from the front of its list, then the vector members are destroyed.
#include <vector>

class Listener_00470fb0 {
public:
    virtual ~Listener_00470fb0();      // vtable +0x0
};

class Class_00470fb0 {
public:
    std::vector<Listener_00470fb0*> lists[10];

    ~Class_00470fb0();
};

// FUNCTION: 0x470fb0
Class_00470fb0::~Class_00470fb0()
{
    for (int i = 0; i < 10; i++) {
        std::vector<Listener_00470fb0*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            delete *it;
            lists[i].erase(it);
        }
    }
}

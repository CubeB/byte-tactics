// Decompiled by Opus. Names are provisional.
// Updates the ten listener lists of 0x470fb0: a listener whose slot-3 check
// says it is finished is deleted and erased, every other one gets its slot-1
// call.
#include <vector>

class Listener_00470fb0 {
public:
    virtual ~Listener_00470fb0();      // vtable +0x0
    virtual void Update();             // vtable +0x4
    virtual void Notify(void* param);  // vtable +0x8
    virtual int IsDone();              // vtable +0xc
};

class ParticleLists {
public:
    std::vector<Listener_00470fb0*> lists[10];

    void UpdateAll();
};

// FUNCTION: 0x471050
void ParticleLists::UpdateAll()
{
    for (int i = 0; i < 10; i++) {
        std::vector<Listener_00470fb0*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            Listener_00470fb0* l = *it;
            if (l->IsDone()) {
                delete l;
                lists[i].erase(it);
            } else {
                l->Update();
                it++;
            }
        }
    }
}

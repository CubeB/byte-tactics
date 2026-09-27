// Decompiled by Opus. Names are provisional.
// Passes param to the slot-2 call of every listener in the ten listener lists
// of 0x470fb0.
#include <vector>

class Listener_00470fb0 {
public:
    virtual ~Listener_00470fb0();      // vtable +0x0
    virtual void Update();             // vtable +0x4
    virtual void Notify(void* param);  // vtable +0x8
    virtual int IsDone();              // vtable +0xc
};

class Class_00470fb0 {
public:
    std::vector<Listener_00470fb0*> lists[10];

    void FUN_004710e0(void* param);
};

// FUNCTION: 0x4710e0
void Class_00470fb0::FUN_004710e0(void* param)
{
    for (int i = 0; i < 10; i++) {
        for (std::vector<Listener_00470fb0*>::iterator it = lists[i].begin(); it != lists[i].end(); it++)
            (*it)->Notify(param);
    }
}

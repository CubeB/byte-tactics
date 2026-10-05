// Decompiled by Opus. Names are provisional.
// Walks the ten listener lists (see 0x471d90, 0x471f40): a listener whose
// slot 3 returns nonzero is deleted and erased from its list; the others get
// slot 1 called.
#include <vector>

class Listener_00471eb0 {
public:
    virtual ~Listener_00471eb0();
    virtual void Slot1();              // vtable +0x4
    virtual void Slot2(void* msg);     // vtable +0x8
    virtual int Slot3();               // vtable +0xc
};

struct Lists_00471eb0 {
    std::vector<Listener_00471eb0*> lists[10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_00471eb0* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x471eb0
void UpdateParticles()
{
    Lists_00471eb0* l = g_game->lists;
    for (int i = 0; i < 10; i++) {
        std::vector<Listener_00471eb0*>& v = l->lists[i];
        std::vector<Listener_00471eb0*>::iterator it = v.begin();
        while (it != v.end()) {
            Listener_00471eb0* p = *it;
            if (p->Slot3()) {
                delete p;
                v.erase(it);
            } else {
                p->Slot1();
                ++it;
            }
        }
    }
}

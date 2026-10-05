// Decompiled by Opus. Names are provisional.
// Header dependence: without <windows.h> and <math.h> MSVC loads the index
// before g_game and keeps the list base, not the scaled index, in edi.
#include <windows.h>
#include <math.h>
#include <vector>

class Listener_00471f90 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(void* msg);     // vtable +0x8
};

struct Lists_00471f90 {
    std::vector<Listener_00471f90*> lists[10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_00471f90* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// Passes msg to every listener in one list (compare 0x471f40, which does
// all ten).
// FUNCTION: 0x471f90
void __stdcall DrawParticleList(void* msg, short kind)
{
    std::vector<Listener_00471f90*>& v = g_game->lists->lists[kind];
    for (std::vector<Listener_00471f90*>::iterator it = v.begin(); it != v.end(); ++it) {
        (*it)->Slot2(msg);
    }
}

// Decompiled by Opus. Names are provisional.
#include <vector>

class Listener_00471d90 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(void* msg);
};

struct Lists_00471d90 {
    std::vector<Listener_00471d90*> lists[10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_00471d90* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// Creates the ten listener lists used by 0x471f40 and 0x471f90.
// FUNCTION: 0x471d90
void CreateParticleLists()
{
    g_game->lists = new Lists_00471d90;
}

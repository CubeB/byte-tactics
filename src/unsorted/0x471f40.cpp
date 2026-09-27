// Decompiled by Opus. Names are provisional.
#include <vector>

class Listener_471f40 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(int arg);     // vtable +0x8
};

struct Lists_471f40 {
    std::vector<Listener_471f40*> lists[10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_471f40* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x471f40
void __stdcall FUN_00471f40(int arg)
{
    Lists_471f40* l = g_game->lists;
    for (int i = 0; i < 10; i++) {
        for (std::vector<Listener_471f40*>::iterator it = l->lists[i].begin(); it != l->lists[i].end(); ++it) {
            (*it)->Slot2(arg);
        }
    }
}

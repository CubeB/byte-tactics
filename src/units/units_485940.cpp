// Decompiled by Opus. Names are provisional.
// Ordering of two items: by the key at +4 when the mode object reports 3,
// otherwise by address.

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Class_00435100* mode;              // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

struct Item_00485940 {
    char unknown_0[4];
    unsigned int key;                  // +0x4
};

// FUNCTION: 0x485940
int __stdcall FUN_00485940(Item_00485940* a, Item_00485940* b)
{
    if (g_game->mode->FUN_00435100() == 3)
        return a->key < b->key;
    return a < b;
}

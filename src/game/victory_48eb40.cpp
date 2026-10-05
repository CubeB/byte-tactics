// Decompiled by Opus. Names are provisional.
// Slot 0 (IsSatisfied) of the "destroy all units" victory condition (vtable
// 0x4fd948, state saved by 0x48eb80): satisfied while the game field at
// +0x1df2 is zero, announcing "Victory Condition" once.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1df2];
    short field_1df2;                  // +0x1df2
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0047f1a0(char* str, int flag);

class VictoryDestroyAllUnits {
public:
    int satisfied;                     // +0x4
    int announced;                     // +0x8

    virtual int IsSatisfied();
};

// FUNCTION: 0x48eb40
int VictoryDestroyAllUnits::IsSatisfied()
{
    if (g_game->field_1df2 == 0) {
        if (announced == 0) {
            FUN_0047f1a0("Victory Condition", 0);
            announced = 1;
        }
        return 1;
    }
    return 0;
}

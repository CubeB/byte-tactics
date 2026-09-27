// Decompiled by Opus. Names are provisional.
// A victory condition test: satisfied while the game field at +0x1df2 is
// zero, announcing "Victory Condition" once.

#pragma pack(push, 1)
struct Game_0048eb40 {
    char unknown_0[0x1df2];
    short field_1df2;                  // +0x1df2
};
#pragma pack(pop)

extern Game_0048eb40* g_game;

void __stdcall FUN_0047f1a0(char* str, int flag);

class Class_0048eb40 {
public:
    char unknown_0[8];
    int announced;                     // +0x8

    int FUN_0048eb40();
};

// FUNCTION: 0x48eb40
int Class_0048eb40::FUN_0048eb40()
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

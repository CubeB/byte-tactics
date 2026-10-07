// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Mission {
public:
    int GetGameType();
    int MissionExists(int index);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391ab];
    int value_391ab;                 // +0x391ab
    int value_391af;                 // +0x391af
    char unknown_391b3[0x391e9 - 0x391b3];
    Mission* ptr_391e9;              // +0x391e9
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x41f040
int FUN_0041f040()
{
    if (((Mission*)g_game->ptr_391e9)->GetGameType() == 1 &&
        ((g_game->value_391af == 0 &&
          ((Mission*)g_game->ptr_391e9)->MissionExists(g_game->value_391ab + 1) == 0) ||
         ((Mission*)g_game->ptr_391e9)->MissionExists(g_game->value_391ab + 1) != 0)) {
        return 1;
    }
    return 0;
}

// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435980 {
public:
    int MissionExists(int index);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391ab];
    int value_391ab;                 // +0x391ab
    int value_391af;                 // +0x391af
    char unknown_391b3[0x391e9 - 0x391b3];
    Class_00435100* ptr_391e9;       // +0x391e9
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x41f040
int FUN_0041f040()
{
    if (((Class_00435100*)g_game->ptr_391e9)->FUN_00435100() == 1 &&
        ((g_game->value_391af == 0 &&
          ((Class_00435980*)g_game->ptr_391e9)->MissionExists(g_game->value_391ab + 1) == 0) ||
         ((Class_00435980*)g_game->ptr_391e9)->MissionExists(g_game->value_391ab + 1) != 0)) {
        return 1;
    }
    return 0;
}

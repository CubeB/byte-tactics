// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
class Class_00409160 {
public:
    char unknown_0[0xd1];
    int* values;                        // +0xd1
    char unknown_d5[0xe1 - 0xd5];
    int* locked;                        // +0xe1
    char unknown_e5[0x10d - 0xe5];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                          // +0x1438f
};
#pragma pack(pop)

extern Class_00409160* DAT_005119c0[];
extern Game* g_game;
// FUNCTION: 0x409f20
int __stdcall FUN_00409f20(int player, unsigned short index, int value)
{
    if (index >= 1 && index < g_game->count) {
        int* values = DAT_005119c0[player]->values;
        if (values[index] == -1)
            return 1;
        return value < values[index];
    }
    return 0;
}

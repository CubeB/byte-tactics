// Decompiled by Opus. Names are provisional.
// For each entry (from 1) whose bit is set in `mask` and that is not yet
// locked, stores `value` into the player's table at +0xd1, and locks it in
// the table at +0xe1 when `lock` is non-zero.

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

// FUNCTION: 0x409e90
void __stdcall FUN_00409e90(int player, unsigned int* mask, int value, int lock)
{
    Class_00409160* p = DAT_005119c0[player];
    for (unsigned short i = 1; i < g_game->count; i++) {
        if ((mask[i >> 5] & (1 << (i & 0x1f))) && p->locked[i] == 0) {
            p->values[i] = value;
            if (lock)
                p->locked[i] = 1;
        }
    }
}

// Decompiled by deepseek-v4.1-flash. Names are provisional.
// For each entry whose bit is set in `mask` and that has not been initialised
// yet (d[i] == 0), scales the byte table c[i] by `scale`, clamps it to 0..100
// and stores it, marking the entry as done (d[i] = 1) when `lock` is set.
#include <stdlib.h>

#pragma pack(push, 1)
class PlayerAI {
public:
    char unknown_0[0xb1];
    unsigned char* c;                   // +0xb1
    char unknown_b5[0xc1 - 0xb5];
    int* d;                             // +0xc1
    char unknown_c5[0x10];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                          // +0x1438f
};
#pragma pack(pop)

extern PlayerAI* g_playerAI[];
extern Game* g_game;

// FUNCTION: 0x409dc0
void __stdcall ScaleUnitWeights(int player, unsigned int* mask, float scale, int lock)
{
    PlayerAI* p = g_playerAI[player];
    for (unsigned short i = 1; i < g_game->count; i++) {
        if (mask[i >> 5] & (1 << (i & 0x1f))) {
            // Own local: the address is then encoded as [offset + base].
            int off = i * 4;
            if (*(int*)(off + (int)p->d) == 0) {
                p->c[i] = (unsigned char)__min(__max((int)(p->c[i] * scale), 0), 100);
                if (lock)
                    *(int*)(off + (int)p->d) = 1;
            }
        }
    }
}

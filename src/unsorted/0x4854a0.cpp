// Decompiled by DeepSeek V4.1 Flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Retry #1766: GPT-6.1-sol confirmed 84.8% after four worker checks; final batch still did not MATCH. Remaining differences are std::sort stack cleanup and tail-loop registers.
// Partial: 84.8%. Using the `pool` local (not g_game->pool) for the +0xff and
// +0x96 writes made the compiler spill pool to [esp+0x14] and reload it into
// ebp before the free-list loop, which fixed the whole tail block.
// Remaining diffs:
//   0x485691, 0x48574b, 0x485778: an extra `add esp, 0xc`/`add esp, 0x10`
//     after the std::sort helper calls. The original TU was built with /Gz, so
//     its <xutility>/<algorithm> templates are __stdcall (see 0x488810.cpp,
//     0x488920.cpp, 0x488960.cpp). The real <algorithm> is __cdecl. Fix by
//     standing in for <xutility>/<algorithm> with __stdcall templates, as
//     0x424c00.cpp does for <vector>; that also fixes the jump offsets that
//     shift by +3/+6 after each call.
//   0x485894-0x485934: tail loop register allocation. Original keeps `slot` in
//     esi and zero-extends unitsPerPlayer through edx; ours uses edx/esi the
//     other way and emits an extra `xor esi, esi`.

#include <windows.h>
#include <algorithm>

class Class_00435100 {
public:
    int FUN_00435100();
};

struct Player_004854a0 {
    char unknown_0[4];
    unsigned int key;                   // +0x4
    char unknown_8[0x146 - 8];
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

#pragma pack(push, 1)
struct Game_004854a0 {
    char unknown_0[0x1b63];
    unsigned char players[10 * 0x14b];  // +0x1b63, stride 0x14b
    char unknown_2851[0x1434f - 0x2851];
    unsigned short field_1434f;         // +0x1434f
    unsigned short poolCount;           // +0x14351
    char unknown_14353[0x14357 - 0x14353];
    unsigned char* pool;                // +0x14357
    unsigned char* field_1435b;         // +0x1435b
    void* hotUnits;                     // +0x1435f
    void* hotRadar;                     // +0x14363
    char unknown_14367[0x1436f - 0x14367];
    unsigned short field_1436f;         // +0x1436f
    char unknown_14371[0x14373 - 0x14371];
    unsigned int field_14373;           // +0x14373
    char unknown_14377[0x1439b - 0x14377];
    unsigned int field_1439b;           // +0x1439b
    char unknown_1439f[0x37ee6 - 0x1439f];
    unsigned short unitsPerPlayer;      // +0x37ee6
    char unknown_37ee8[0x391e9 - 0x37ee8];
    Class_00435100* mode;               // +0x391e9
};
#pragma pack(pop)

extern Game_004854a0* g_game;

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

int __stdcall FUN_00485940(Player_004854a0* a, Player_004854a0* b)
{
    if (g_game->mode->FUN_00435100() == 3)
        return a->key < b->key;
    return a < b;
}

// FUNCTION: 0x4854a0
void __stdcall FUN_004854a0(void)
{
    g_game->field_1436f = 0;
    g_game->field_14373 &= 0xfffffffd;
    g_game->field_1434f = g_game->unitsPerPlayer;
    g_game->poolCount = (unsigned short)(g_game->unitsPerPlayer * 10 + 1);

    unsigned char* pool = g_game->pool = (unsigned char*)FUN_004d83b0("UNIT MEMORY", g_game->poolCount * 0x118);
    memset(pool, 0, g_game->poolCount * 0x118);

    unsigned int ten = g_game->unitsPerPlayer * 10;
    g_game->hotUnits = FUN_004d83b0("HOT UNITS", ten * 2);
    g_game->hotRadar = FUN_004d83b0("HOT RADAR UNITS", ten * 10);
    g_game->field_1435b = g_game->pool + g_game->poolCount * 0x118 - 0x118;

    unsigned short n;
    for (n = 0; n < g_game->poolCount; n++) {
        *(unsigned short*)(pool + n * 0x118 + 0xa8) = n;
        *(unsigned int*)(pool + n * 0x118 + 0x92) = g_game->field_1439b;
    }

    Player_004854a0* v[10];
    int k;
    for (k = 0; k < 10; k++)
        v[k] = (Player_004854a0*)(g_game->players + k * 0x14b);

    std::sort(v, v + 10, FUN_00485940);

    pool[0xff] = 0xff;
    *(unsigned int*)(pool + 0x96) = 0;
    int i;
    for (i = 0; i < 10; i++) {
        Player_004854a0* item = v[i];
        int c = g_game->unitsPerPlayer * i + 1;
        unsigned char* slot = pool + c * 0x118;
        *(unsigned char**)((char*)item + 0x67) = slot;
        *(unsigned char**)((char*)item + 0x6b) = slot + g_game->unitsPerPlayer * 0x118 - 0x118;
        *(unsigned short*)((char*)item + 0x6f) = *(unsigned short*)(slot + 0xa8);
        *(unsigned short*)((char*)item + 0x71) =
            *(unsigned short*)(*(unsigned char**)((char*)item + 0x6b) + 0xa8);
        for (unsigned char* q = slot; q <= *(unsigned char**)((char*)item + 0x6b); q += 0x118) {
            *(void**)(q + 0x96) = item;
            q[0xff] = *(unsigned char*)((char*)item + 0x146);
            *(unsigned int*)(q + 0xac) = 0xffffffffu;
        }
    }
}


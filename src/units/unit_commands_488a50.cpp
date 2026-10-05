// Decompiled by space-bunny-free. Names are provisional.
// Binary search over the 0x249-byte item table at g_game+0x1439b, comparing
// the name at +0x20 case-insensitively, returning the entry whose name matches
// exactly (0 when there is none). The search window starts one entry in, so the
// loop count is (end - lo) as a pointer difference, which MSVC turns into one
// signed divide by 0x249 (the same imul 0xe00e00e1 / sar 9 sequence as
// 0x419940's neighbours). Note the source order: `end` must be computed before
// `lo`, otherwise the two base loads swap edx/ecx for ecx/ebp.
#include <string.h>

#pragma pack(push, 1)
struct Item_00488a50 {                 // 0x249 bytes
    char unknown_0[0x20];
    char name[0x249 - 0x20];           // +0x20
};

struct Game_00488a50 {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Item_00488a50* items;              // +0x1439b
};
#pragma pack(pop)

extern Game_00488a50* g_game;
extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

static inline int Less_00488a50(const char* a, const char* b)
{
    return _strcmpi(a, b) < 0;
}

// FUNCTION: 0x488a50
Item_00488a50* __stdcall FUN_00488a50(const char* name)
{
    Item_00488a50* end = g_game->items + g_game->count;
    Item_00488a50* lo = g_game->items + 1;
    int n = (int)(end - lo);
    while (n > 0) {
        int half = n / 2;
        Item_00488a50* mid = lo + half;
        if (Less_00488a50(mid->name, name)) {
            lo = mid + 1;
            n = n - half - 1;
        } else {
            n = half;
        }
    }
    if (lo != end && _strcmpi(name, lo->name) == 0)
        return lo;
    return 0;
}

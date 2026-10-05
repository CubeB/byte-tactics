// Decompiled by space-bunny-free. Names are provisional.
// Parses a player list: binary searches the sorted item table (g_game->items,
// 0x249 bytes per item, name at +0x20, id at +0x21e) and, on a hit, sets that
// id's bit in the 0x40-byte set; on a miss it merges the mask that GetCategoryMask
// returns for an alias name. *out is 1 for a single item, 0 for an alias.
// The search and the final comparison are one inlined helper: that is what puts
// the object pointer in ebx and the walking pointer in ebp, and it is also why
// the two strcmpi calls pass their arguments in opposite order
// (item name first inside the loop, the text first after it).
#include <string.h>

#pragma pack(push, 1)
struct Item_00488d30 {                  // 0x249 bytes
    char unknown_0[0x20];
    char name[0x1fe];                  // +0x20
    unsigned short id;                 // +0x21e
    char unknown_220[0x249 - 0x220];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Item_00488d30* items;              // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;
extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

// 0x40-byte set (512 bits), as in the callers 0x406db0 and 0x406e40.
class UnitTypeSet {
public:
    int bits[16];
    void AddTypeOrCategory(char* text, int* out);
};

UnitTypeSet* __stdcall GetCategoryMask(char* name);

static inline char* FindByName(char* first, char* last, char* text)
{
    int n = (last - first) / 0x249;
    while (n > 0) {
        int m = n / 2;
        Item_00488d30* e = (Item_00488d30*)(first + m * 0x249);
        if (_strcmpi(e->name, text) < 0) {
            n = n - m - 1;
            first = (char*)e + 0x249;
        } else {
            n = m;
        }
    }
    if (first == last || _strcmpi(text, ((Item_00488d30*)first)->name) != 0)
        return 0;
    return first;
}

// FUNCTION: 0x488d30
void UnitTypeSet::AddTypeOrCategory(char* text, int* out)
{
    char* base = (char*)g_game->items;
    char* last = base + g_game->count * 0x249;
    char* first = FindByName(base + 0x249, last, text);
    unsigned short v = first ? ((Item_00488d30*)first)->id : 0;
    if (v != 0) {
        bits[v >> 5] |= 1 << (v & 31);
        *out = 1;
        return;
    }
    UnitTypeSet* other = GetCategoryMask(text);
    for (int i = 0; i < 16; i++)
        bits[i] |= other->bits[i];
    *out = 0;
}

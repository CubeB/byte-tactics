// Decompiled by space-bunny-free. Names are provisional.
// Binary search of the game's 0x249-byte table at g_game+0x1439b, starting past
// entry 0, for an entry whose name at +0x20 matches case-insensitively. Returns
// the entry's index at +0x21e, or 0 when there is no such entry (callers mask
// the result with 0xffff, so the return type is unsigned short).
// The source builds the end pointer before the first entry pointer: that
// statement order is what puts the table base straight into ebp and leaves the
// `+1` as the `add ebp, 0x249` in the middle of the end-pointer arithmetic.

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

#pragma pack(push, 1)
struct UnitType_00488b10 {              // 0x249 bytes
    char unknown_0[0x20];
    char name[0x21e - 0x20];            // +0x20
    unsigned short id;                   // +0x21e
    char unknown_220[0x249 - 0x220];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                          // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_00488b10* types;           // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x488b10
unsigned short __stdcall FUN_00488b10(const char* name)
{
    UnitType_00488b10* last = g_game->types + g_game->count;
    UnitType_00488b10* first = g_game->types + 1;
    int n = ((char*)last - (char*)first) / 0x249;
    while (n > 0) {
        int mid = n / 2;
        UnitType_00488b10* e = first + mid;
        if (_strcmpi(e->name, name) < 0) {
            first = e + 1;
            n = n - mid - 1;
        } else {
            n = mid;
        }
    }
    if (first == last || _strcmpi(name, first->name) != 0)
        first = 0;
    if (first)
        return first->id;
    return 0;
}

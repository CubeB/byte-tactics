// Decompiled by space-bunny-free. Names are provisional.
// Looks a name up in the class-name list at Mission+0xdac (one entry
// per team, each entry holding two names at +0 and +4) and returns the caller's
// parallel int array (param_2+4) at the same index, skipping entries whose int
// is 0. When the caller passes a nonzero value the search for the entry
// carrying that value starts at index 1 and the result is the match index
// plus one, so index 0 of the array is never returned: the original's
// `i++` sits inside the subscript, so the first test is array[0] with i
// already 1, and a match on array[0] leaves i = 1.
#include <string.h>

// One entry of the class list at Mission+0xdac: two names.
class Entry_00487af0 {
public:
    char* field_0;                     // +0x0
    char* field_4;                     // +0x4
    char unknown_8[0x24 - 8];
};

class Mission {
public:
    char unknown_0[0xdac];
    Entry_00487af0* list;              // +0xdac
    int count;                         // +0xdb0
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

struct Struct_00487af0 {
    char unknown_0[4];
    int* field_4;                      // +0x4
};

// The element address has to go through this helper: written inline as
// param_2->field_4[i++] the strength reducer turns the loop into a pointer
// walk (add eax, 4 plus the loaded value in a register), while the original
// recomputes esi*4 with a lea and compares straight from memory.
static inline int* Elem_00487af0(int* arr, int i)
{
    return arr + i;
}

// FUNCTION: 0x487af0
int __stdcall FindMissionUnit(char* name, Struct_00487af0* param_2, int value)
{
    int i = 0;

    if (value) {
        while (*Elem_00487af0(param_2->field_4, i++) != value) {
            if (i >= g_game->field_391e9->count)
                return 0;
        }
    }
    if (i >= g_game->field_391e9->count)
        return 0;
    for (; i < g_game->field_391e9->count; i++) {
        Entry_00487af0* e = &g_game->field_391e9->list[i];
        if (e->field_4 && _strcmpi(e->field_4, name) == 0 && param_2->field_4[i])
            return param_2->field_4[i];
        if (e->field_0 && _strcmpi(e->field_0, name) == 0 && param_2->field_4[i])
            return param_2->field_4[i];
    }
    return 0;
}

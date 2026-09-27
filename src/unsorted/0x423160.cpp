// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (65.9%), best found. The search loop, sentinel, control flow and the
// tail (FUN_00481550 / FUN_00423c50 calls) match. The one remaining difference
// is register allocation: the search result `id` must live in edi (the same
// register as the inlined search counter) so the original reads
// `cmp di,0xffff` / `mov edi,eax` / `mov ecx,edi; and ecx,0xffff`. With this
// source MSVC copies the helper's return into ebp (`mov ebp,edi`, `cmp bp,di`,
// `mov ecx,ebp`), which also pushes x/y of the position tail into edi/esi
// instead of the original's ecx/edx. `<windows.h>` fixed the later
// `and ecx,0xffff` register; a helper returning int keeps the search counter in
// edi but a helper returning unsigned short moves it to ebp, and neither
// coalesces id into edi. headers.py: no header set matches (best 65.9%).
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Rec_00423160 {
    char name[0x80];
    int x;                          // +0x80
    int y;                          // +0x84
};

struct NameEntry_00423160 {
    char name[0x94];
    short field_94;                 // +0x94
    short field_96;                 // +0x96
    char unknown_98[0xfe - 0x98];
    unsigned char flags;            // +0xfe
    char unknown_ff;
};

struct Mission_00423160 {
    char unknown_0[0xdbc];
    Rec_00423160* array;            // +0xdbc
    int count;                      // +0xdc0
};

struct Game_00423160 {
    char unknown_0[0x14253];
    int nameCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    NameEntry_00423160* names;      // +0x1426f
};
#pragma pack(pop)

extern char* g_game;

unsigned short __stdcall FUN_004224b0(char* name);
void* __stdcall FUN_00481550(int x, int y);
void* __stdcall FUN_00423c50(void* target, unsigned short id, void* pos, void* field_64, unsigned char owner);

#define MISSION (*(Mission_00423160**)(g_game + 0x391e9))
#define GAME ((Game_00423160*)g_game)

static inline int FindName(char* name)
{
    for (int i = 0; i < GAME->nameCount; i++) {
        if (_strcmpi(name, GAME->names[i].name) == 0)
            return i;
    }
    return 0xffff;
}

// FUNCTION: 0x423160
void FUN_00423160(void)
{
    for (int i = 0; i < MISSION->count; i++) {
        Rec_00423160* rec = &MISSION->array[i];
        if (rec->name[0] == 0)
            continue;
        unsigned short id = (unsigned short)FindName(rec->name);
        if (id == 0xffff) {
            id = FUN_004224b0(rec->name);
            if (id == 0xffff)
                continue;
        }
        NameEntry_00423160* e = &GAME->names[id];
        int x, y;
        if ((e->flags & 1) != 0) {
            y = rec->y;
            x = rec->x;
        } else {
            y = rec->y - e->field_96 / 2;
            x = rec->x - e->field_94 / 2;
        }
        FUN_00423c50(FUN_00481550(x, y), id, 0, 0, 10);
    }
}

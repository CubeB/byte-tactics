// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Walks the unit definition table at g_game+0x1439b (0x249-byte entries) and
// the table at g_game+0x391cb (0xbd-byte entries). For every pair that shares a
// name, if the def's bit 5 at +0x241 is clear it prints a warning and sets the
// "downloadable" bit (the table's lock is taken around the update).
//
// Best so far 100%: the loop must be `for (i = 0; i < count; i++, def += 0x249)`
// with the whole thing a single for statement. That is what puts `inc i` before
// `add esi, 0x249` and the bitfield access at +0x241 makes MSVC pick the
// bitfield storage word as the element base (name is then esi-0x221).
// The initial `i = 0` test must be MSVC's own rotation: no source guard.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Def_0042bd40 {                  // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x241 - 0x40];
    unsigned int unknown_241 : 5;      // +0x241
    unsigned int downloadable : 1;     // +0x241 bit 5
    unsigned int rest : 26;
    char unknown_245[0x249 - 0x245];
};

struct Inner_0042bd40 {                // 0xbd bytes
    char unknown_0[8];
    char name[0xbd - 8];               // +0x8
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[8];
    Def_0042bd40* defs;                // +0x1439b
    char unknown_1439f[0x391c7 - 0x1439f];
    int innerCount;                    // +0x391c7
    Inner_0042bd40* inner;             // +0x391cb
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d8780(void* param_1);
void __cdecl FUN_004d8710(void* param_1);

// FUNCTION: 0x42bd40
void FUN_0042bd40()
{
    Def_0042bd40* def = g_game->defs;
    for (int i = 0; i < g_game->count;
         i++, def = (Def_0042bd40*)((char*)def + 0x249)) {
        for (int j = 0; j < g_game->innerCount; j++) {
            if (_strcmpi(g_game->inner[j].name, def->name) == 0
                && !def->downloadable) {
                char message[128];
                sprintf(message,
                        "Hey!  Somebody forgot to set downloadable=1 for %s",
                        def->name);
                FUN_004d8780(g_game->defs);
                def->downloadable = 1;
                FUN_004d8710(g_game->defs);
            }
        }
    }
}

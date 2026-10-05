// Decompiled by Opus. Names are provisional.
// Creates the global game object at a random offset (0..6993 bytes) inside a
// zeroed allocation, using placement new.
#include <windows.h>
#include <string.h>
#include <new.h>

void __cdecl FUN_004d83a0(int);

#pragma pack(push, 1)
class Class_00463be0 {
public:
    char unknown_0[0x14b];
    Class_00463be0();
};

struct Grid_0041d920 {
    void* cells;                       // +0x0
    int width;                         // +0x4
    int height;                        // +0x8
    int field_c;                       // +0xc
    Grid_0041d920() { width = 0; height = 0; field_c = 0; cells = 0; }
};

class Game_0041d920 {
public:
    int unknown_0;
    const char* build_date;            // +0x4
    const char* build_time;            // +0x8
    char unknown_c[0x1b63 - 0xc];
    Class_00463be0 players[11];        // +0x1b63
    char unknown_299c[0x1428f - 0x299c];
    Grid_0041d920 grid_1428f;          // +0x1428f
    Grid_0041d920 grid_1429f;          // +0x1429f
    int field_142af;                   // +0x142af
    int field_142b3;                   // +0x142b3
    char unknown_142b7[0x3924d - 0x142b7];

    Game_0041d920() : build_date("Jul 30 1998"), build_time("11:16:36"),
        field_142af(0), field_142b3(0) {}
};
#pragma pack(pop)

extern Game_0041d920* g_game;

// FUNCTION: 0x41d920
void FUN_0041d920()
{
    unsigned int offset = GetTickCount() % 1000 * 7;
    unsigned int size = offset + sizeof(Game_0041d920);
    char* mem = (char*)operator new(size);
    memset(mem, 0, size);
    FUN_004d83a0((int)mem);
    g_game = new (mem + offset) Game_0041d920;
}

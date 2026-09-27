// Decompiled by Opus. Names are provisional.
// Selects entry n (a signed byte at +0x2cbe) and hands the matching table
// entry at +0x1487f to FUN_004ab400 for the object at +0x519.

struct Obj_004ab400;
struct Src_004ab400;

#pragma pack(push, 1)
struct Game_00491c80 {
    char unknown_0[0x519];
    char field_519[0x2cbe - 0x519];  // +0x519
    signed char selected;            // +0x2cbe
    char unknown_2cbf[0x1487f - 0x2cbf];
    Src_004ab400* table[1];          // +0x1487f
};
#pragma pack(pop)

extern Game_00491c80* g_game;

void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src);

// FUNCTION: 0x491c80
void __stdcall FUN_00491c80(int n)
{
    if (g_game->selected != n) {
        g_game->selected = n;
        FUN_004ab400((Obj_004ab400*)g_game->field_519, g_game->table[n]);
    }
}

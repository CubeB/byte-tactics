// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Picks the order/state table entry at +0x1487f and hands it to
// FUN_004ab400 for the object at +0x519, mirroring 0x491c80.

struct Obj_004ab400;
struct Src_004ab400;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char field_519[0x2cba - 0x519];  // +0x519
    unsigned short field_2cba;       // +0x2cba
    char unknown_2cbc[0x2cbe - 0x2cbc];
    signed char selected;            // +0x2cbe
    char unknown_2cbf[0x2cc3 - 0x2cbf];
    unsigned char mode_2cc3;         // +0x2cc3
    char unknown_2cc4[2];
    unsigned char flags_2cc6;        // +0x2cc6
    char unknown_2cc7[0x1487f - 0x2cc7];
    Src_004ab400* table[1];          // +0x1487f
};
#pragma pack(pop)

extern Game* g_game;

int __cdecl UpdatePlacementGhostValidity(void);
unsigned short __cdecl FUN_0048cd80(void);
int __stdcall FUN_0048d220(unsigned char mode);
void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src);

// FUNCTION: 0x491cc0
void __stdcall FUN_00491cc0(int unused)
{
    unsigned char flags = g_game->flags_2cc6;

    if ((flags & 2) != 0 && g_game->mode_2cc3 == 0xe) {
        UpdatePlacementGhostValidity();
        return;
    }
    if ((flags & 2) == 0 && (flags & 1) == 0) {
        if (g_game->selected != 0x13) {
            g_game->selected = 0x13;
            FUN_004ab400((Obj_004ab400*)g_game->field_519, g_game->table[0x13]);
        }
        return;
    }
    g_game->field_2cba = FUN_0048cd80();
    int n = FUN_0048d220(g_game->mode_2cc3);
    if (g_game->selected != n) {
        g_game->selected = n;
        FUN_004ab400((Obj_004ab400*)g_game->field_519, g_game->table[n]);
    }
}

// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 86.7 percent. The flag-4 branch matches the original byte for byte
// (including the abs() idiom, the `(unsigned char)(flags >> 1) & 1` shift/test
// spelling and the abs-diff-against-5 test). The else branch differs only in
// the scheduler's order for the two cell-coordinate divisions: the original
// emits pos.z / 0x200000 first (0x4826f0) and y_hi / 64 second (0x4826fe),
// keeping pos.z in edi and y_hi in eax, so cx lands in ebp and cy in edi with
// no spill. MSVC 5 here emits y_hi / 64 first, keeps it in edi, then has to
// spill the pos.z result to [esp+0x20] (5 extra instructions); cx is still ebp
// and the rest of the branch matches. Same expression as the MATCHed 0x482ac0
// (p.pos.z / 0x200000 - ((short*)&p.pos.y)[1] / 64). Tried and rejected: z/y
// local temporaries in every order, `pos.y >> 16`, packed y_lo/y_hi structs,
// static inline CellX/CellY helpers (by pointer and by value), comma operator,
// `zz = (provably-constant ? zz : zz)` dependency tricks (they get pos.z first
// but flip the allocation so cx goes to edi), splitting cx/cy into statements,
// moving the declarations to function scope, and the if + early return form.
// The near-copy 0x482830 is stuck on the very same order (see its notes).
#include <stdlib.h>

#pragma pack(push, 1)
struct Vec3_4825b0 {
    int x;                      // +0x00
    int y;                      // +0x04
    int z;                      // +0x08
};

struct Entry_4825b0 {
    int field_0;                // +0x00
    short field_4;              // +0x04
    short field_6;              // +0x06
    short field_8;              // +0x08
};

struct Cell_4825b0 {
    unsigned short count;       // +0x00
    char unknown_2[0x28 - 0x2];
    Entry_4825b0* entries;      // +0x28
};

struct Params_4825b0 {
    void* field_0;              // +0x00
    short* field_4;             // +0x04
    short field_8;              // +0x08
    unsigned char field_a;      // +0x0a
    char unknown_b;             // +0x0b
    unsigned char* field_c;     // +0x0c
    Vec3_4825b0 pos;            // +0x10
};

struct Game_4825b0 {
    char unknown_0[0x14281];
    unsigned short flags;       // +0x14281
    char unknown_14283[0x14293 - 0x14283];
    unsigned int field_14293;   // +0x14293
    unsigned int field_14297;   // +0x14297
    char unknown_1429b[0x1485b - 0x1429b];
    Cell_4825b0* field_1485b;   // +0x1485b
};
#pragma pack(pop)

extern Game_4825b0* g_game;

void __stdcall FUN_00481d50(Params_4825b0* params);
void __stdcall FUN_00482270(Params_4825b0* params);
void __stdcall FUN_00481930(Params_4825b0* params);
Entry_4825b0* __stdcall FUN_004b7f30(Cell_4825b0* table, int index);

// FUNCTION: 0x4825b0
void __stdcall FUN_004825b0(Params_4825b0* params)
{
    if ((g_game->flags & 4) == 4) {
        int x = ((short*)&params->pos.x)[1] >> 5;
        int v = params->field_a + ((short*)&params->pos.y)[1];
        if (v < 0) {
            v = 0;
        }
        if (v > 0xff) {
            v = 0xff;
        }
        int y = (((short*)&params->pos.z)[1] - (v >> 1)) >> 5;
        int diff = abs((int)*params->field_c - v);
        unsigned char c = *params->field_c;
        if (params->field_4[0] != x || params->field_4[1] != y || diff > 5) {
            if (c != 0 && (g_game->flags & 2)) {
                FUN_00481d50(params);
            }
            params->field_4[0] = (short)x;
            params->field_4[1] = (short)y;
            if ((unsigned)x >= g_game->field_14293 || (unsigned)y >= g_game->field_14297) {
                *params->field_c = 0;
                return;
            }
            *params->field_c = (unsigned char)v;
            if ((unsigned char)(g_game->flags >> 1) & 1) {
                FUN_00482270(params);
            }
            if (g_game->flags & 1) {
                FUN_00481930(params);
            }
        }
    }
    else {
        int i = (short)params->field_8 / 32 - 5;
        if (i < 0) {
            i = 0;
        } else if (i >= g_game->field_1485b->count) {
            i = g_game->field_1485b->count - 1;
        }
        int cx = params->pos.x / 0x200000;
        int cy = params->pos.z / 0x200000 - ((short*)&params->pos.y)[1] / 64;
        Entry_4825b0* e = FUN_004b7f30(g_game->field_1485b, i);
        cx -= e->field_4;
        cy -= e->field_6;
        if (params->field_4[0] != cx || params->field_4[1] != cy || *params->field_c != i) {
            if ((g_game->flags & 2) == 2) {
                FUN_00481d50(params);
                params->field_4[0] = (short)cx;
                params->field_4[1] = (short)cy;
                *params->field_c = (unsigned char)i;
                FUN_00482270(params);
            } else {
                params->field_4[0] = (short)cx;
                params->field_4[1] = (short)cy;
                *params->field_c = (unsigned char)i;
            }
            if (g_game->flags & 1) {
                FUN_00481930(params);
            }
        }
    }
}

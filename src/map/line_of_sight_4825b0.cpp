// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// MATCH. The source below is deepseek-v4.1-flash's, unchanged except for the
// one added include. The whole function is byte identical once
// `#include <windows.h>` is in the file, and nothing else had to move: with
// only <stdlib.h> the same source scored 86.7 percent, because in the else
// branch MSVC 5 then scheduled the operands of the cell-y subtraction the
// wrong way round (y_hi / 64 first, into edi, which forced a spill of the
// pos.z result to [esp+0x20] and left the function five bytes long).
// The big header changes the compiler's state enough to make the linearizer
// emit pos.z / 0x200000 first, into edi, with y_hi / 64 in eax, exactly as
// the original does, with cx in ebp and no spill. tools/headers.py showed it
// on this source: 96.6 percent with <windows.h> or <ddraw.h> before any
// source change, and the source order fix (cx declared before cy) then makes
// it a MATCH: the header is what fixed the subtraction, the statement order
// below (cx first) is what puts its magic multiply in front of the subtraction
// instead of behind it. So the order was never a wrong reading of the
// expression, only the wrong compiler state (the same expression is in the
// MATCHed 0x482ac0, which does not need the header).
// The near-copy 0x482830 is stuck on the very same order (see its notes), and
// <windows.h> is worth trying there too.
#include <windows.h>
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

struct Game {
    char unknown_0[0x14281];
    unsigned short flags;       // +0x14281
    char unknown_14283[0x14293 - 0x14283];
    unsigned int field_14293;   // +0x14293
    unsigned int field_14297;   // +0x14297
    char unknown_1429b[0x1485b - 0x1429b];
    Cell_4825b0* field_1485b;   // +0x1485b
};
#pragma pack(pop)

extern Game* g_game;

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

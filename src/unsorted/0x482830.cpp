// Decompiled by Space Bunny Free. Names are provisional.
//
// Not a match yet: 87.3 percent, ours is 10 bytes longer (231 against 221).
// Everything matches except the emission order of the two divisions that make
// the y coordinate. The original emits pos.z / 0x200000 first and the high
// half of pos.y divided by 64 second (0x4828ad and 0x4828bb), so the value of
// pos.z / 0x200000 lands in edi and the second term stays in eax for
// "sub edi, eax". Our build emits the 16 bit division first, which takes edi,
// then has to spill the value to [esp+0x18] and reload it into edx for
// "sub edi, edx" (three extra instructions, 10 bytes).
//
// The expression itself is right: the near identical function 0x482ac0 (a
// MATCH, same body inlined) writes the same thing as
//   cell_y = p.pos.z / 0x200000 - ((short*)&p.pos.y)[1] / 64;
// and that build does emit pos.z first. So this is the scheduler's choice in
// this one function, not a wrong expression. None of these source variations
// changed the order: statement order of the x, z and y_hi divisions, temporaries
// versus one expression, the high half read as a short field or as
// ((short*)&pos.y)[1] or ((short*)&pos)[3], a local copy of g_game or of
// params, short* versus struct out pointer, signed char versus unsigned char
// cell_id, the clamp with and without a local table pointer, the nested if
// against the early return form, int versus long, and removing the two
// trailing calls. Removing the FUN_004b7f30 call, or the x division, makes the
// order come out as in the original, so the pressure across that call is what
// picks the wrong order.

struct Out_482830 {
    short x;
    short y;
};

struct Entry_482830 {
    char unknown_0[4];
    short field_4;
    short field_6;
};

struct Table_482830 {
    unsigned short count;         // +0
    char unknown_2[0x28 - 0x2];
    Entry_482830* entries;        // +0x28
};

#pragma pack(push, 1)
struct Pos_482830 {
    int x;                        // +0x10
    short y_lo;                   // +0x14
    short y_hi;                   // +0x16, that is y / 0x10000
    int z;                        // +0x18
};

struct Params_482830 {
    void* field_0;                // +0
    Out_482830* field_4;          // +4
    short field_8;                // +8
    unsigned char field_a;        // +0xa
    char unknown_b;               // +0xb
    char* field_c;                // +0xc
    Pos_482830 pos;               // +0x10
};

struct Game_482830 {
    char unknown_0[0x14281];
    unsigned char flags;          // +0x14281
    char unknown_14282[0x1485b - 0x14282];
    Table_482830* field_1485b;    // +0x1485b
};
#pragma pack(pop)

extern Game_482830* g_game;

void __stdcall FUN_004825b0(Params_482830* params);
void __stdcall FUN_00482270(Params_482830* params);
void __stdcall FUN_00481930(Params_482830* params);
Entry_482830* __stdcall FUN_004b7f30(Table_482830* table, int index);

// FUNCTION: 0x482830
void __stdcall FUN_00482830(Params_482830* params)
{
    if ((g_game->flags & 2) != 2) {
        return;
    }
    *params->field_c = 0;
    if ((g_game->flags & 4) == 4) {
        FUN_004825b0(params);
        return;
    }
    int lod = params->field_8 / 32 - 5;
    if (lod < 0) {
        lod = 0;
    } else {
        Table_482830* table = g_game->field_1485b;
        if (lod >= table->count) {
            lod = table->count - 1;
        }
    }
    int x = params->pos.x / 0x200000;
    int y = params->pos.z / 0x200000 - params->pos.y_hi / 64;
    Entry_482830* entry = FUN_004b7f30(g_game->field_1485b, lod);
    x -= entry->field_4;
    y -= entry->field_6;
    params->field_4->x = (short)x;
    params->field_4->y = (short)y;
    *params->field_c = (char)lod;
    FUN_00482270(params);
    FUN_00481930(params);
}

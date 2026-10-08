// Decompiled by Space Bunny Free. Names are provisional.
// Kept out of line_of_sight.cpp: the merged file's symbol ids move its registers.

struct UnitType_482ac0 {
    char unknown_0[0x170];
    unsigned char field_170;            // +0x170
    char unknown_171[0x202 - 0x171];
    short field_202;                    // +0x202
};

struct Vec3_482ac0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Unit {
    char unknown_0[0x6a];
    Vec3_482ac0 pos;                    // +0x6a
    char unknown_76[0x7a - 0x76];
    short cell[2];                      // +0x7a
    char unknown_7e[0x92 - 0x7e];
    UnitType_482ac0* type;              // +0x92
    void* field_96;                     // +0x96
    char unknown_9a[0xf8 - 0x9a];
    unsigned char cell_id[4];           // +0xf8
};
#pragma pack(pop)

struct Entry_482ac0 {                   // one element of Cell_482ac0, 8 bytes
    int field_0;
    short field_4;
    short field_6;
    short field_8;
};

struct Cell_482ac0 {
    unsigned short count;               // +0
    char unknown_2[0x26];
    Entry_482ac0 entries[1];            // +0x28
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1427f];
    unsigned char field_1427f;          // +0x1427f
    char unknown_14280;
    unsigned char field_14281;          // +0x14281
    char unknown_14282[0x1485b - 0x14282];
    Cell_482ac0* field_1485b;           // +0x1485b
};
#pragma pack(pop)

struct Params_482ac0 {
    void* field_0;                      // +0x0
    short* field_4;                     // +0x4
    short field_8;                      // +0x8
    unsigned char field_a;              // +0xa
    char unknown_b;
    unsigned char* field_c;             // +0xc
    Vec3_482ac0 pos;                    // +0x10
    int unknown_1c;
    int unknown_20;
};

extern Game* g_game;

void __stdcall UpdateLineOfSight(Params_482ac0* params);
void __stdcall AddLineOfSight(Params_482ac0* params);
void __stdcall FUN_00481930(Params_482ac0* params);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);

// FUNCTION: 0x482ac0
void __stdcall FUN_00482ac0(Unit* unit)
{
    Params_482ac0 p;
    p.field_0 = unit->field_96;
    p.field_4 = unit->cell;
    p.field_8 = unit->type->field_202;
    p.field_c = unit->cell_id;
    p.pos = unit->pos;
    p.field_a = unit->type->field_170;
    int min_y = (g_game->field_1427f + 1) << 16;
    if (p.pos.y < min_y) {
        p.pos.y = min_y;
    }
    if ((g_game->field_14281 & 2) == 2) {
        *p.field_c = 0;
        if ((g_game->field_14281 & 4) == 4) {
            UpdateLineOfSight(&p);
        } else {
            int i = p.field_8 / 32 - 5;
            if (i < 0) {
                i = 0;
            } else if (i >= g_game->field_1485b->count) {
                i = g_game->field_1485b->count - 1;
            }
            int cell_x = p.pos.x / 0x200000;
            int cell_y = p.pos.z / 0x200000 - ((short*)&p.pos.y)[1] / 64;   // high half of y
            Entry_482ac0* e = (Entry_482ac0*)GetGafFrame((unsigned short*)g_game->field_1485b, i);
            cell_x -= e->field_4;
            cell_y -= e->field_6;
            p.field_4[0] = (short)cell_x;
            p.field_4[1] = (short)cell_y;
            *p.field_c = i;
            AddLineOfSight(&p);
            FUN_00481930(&p);
        }
    }
}

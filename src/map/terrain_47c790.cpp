// Decompiled by deepseek-v4.1-flash. Names are provisional.
#pragma pack(push, 1)

struct Point_0047c790 {
    short x;
    short y;
};

struct Unit_0047c790 {
    char unknown_0[0x14a];
    Point_0047c790 origin;              // +0x14a
    unsigned char* mask;                // +0x14e
};

struct Player_0047c790 {
    int active;                         // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                 // +0x73
};

struct UnitRec_0047c790 {               // 0x118 bytes
    char unknown_0[0x92];
    void* def;                          // +0x92
    Player_0047c790* owner;             // +0x96
    char unknown_9a[0x110 - 0x9a];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Cell_0047c790 {
    unsigned short field_0;             // +0x0
    unsigned short field_2;             // +0x2
    char unknown_4[0xd - 0x4];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    char unknown_14237[0x14287 - 0x14237];
    Cell_0047c790* cells;               // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    UnitRec_0047c790* units;            // +0x14357
};

union Flags_0047c790 {
    struct {
        unsigned int unknown_0 : 27;
        unsigned int flag27 : 1;
        unsigned int unknown_1 : 4;
    } bits;
    int all;
};

struct Obj_0047c790 {
    char unknown_0[0x76];
    Point_0047c790 pos;                 // +0x76
    char unknown_7a[4];
    Point_0047c790 size;                // +0x7e
    char unknown_82[0x92 - 0x82];
    Unit_0047c790* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short field_a8;            // +0xa8
    char unknown_aa[0x10f - 0xaa];
    unsigned char bit0 : 1;             // +0x10f
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    Flags_0047c790 flags;               // +0x110
};
#pragma pack(pop)

extern Game* g_game;

Cell_0047c790* __stdcall GetMapCell(int x, int y);

// FUNCTION: 0x47c790
void __stdcall FUN_0047c790(Obj_0047c790* obj)
{
    unsigned int f = obj->flags.all;
    unsigned char b = (unsigned char)((f & 0x8000000) >> 27);
    if (!(b & 1))
        return;
    f &= ~0x8000000;
    obj->flags.all = f;
    Point_0047c790 size = obj->size;
    if (f & 0x20000000) {
        Cell_0047c790* cell = GetMapCell(obj->pos.x, obj->pos.y);
        int index = 0;
        for (int j = size.y; j > 0; j--) {
            for (int i = size.x; i > 0; i--) {
                if (obj->unit->mask[index] & (obj->bit2 ? 2 : 4)) {
                    UnitRec_0047c790* rec;
                    unsigned short id = cell->field_0;
                    if (id == 0)
                        goto a_write;
                    rec = &g_game->units[id];
                    if (rec->owner->active == 0)
                        goto a_b;
                    if (rec->owner->type != 3)
                        goto a_b;
                    rec->flags |= 0x8000000;
                    obj->flags.all |= 0x4000000;
                a_write:
                    cell->field_0 = obj->field_a8;
                    goto a_next;
                a_b:
                    rec->flags |= 0x4000000;
                    obj->flags.all |= 0x8000000;
                a_next: ;
                } else if (cell->field_0 == obj->field_a8) {
                    cell->field_0 = 0;
                }
                index++;
                cell++;
            }
            cell += g_game->width - size.x;
        }
    } else if ((obj->flags.all & 3) == 1) {
        Cell_0047c790* cell = GetMapCell(obj->pos.x, obj->pos.y);
        for (int j = size.y; j > 0; j--) {
            for (int i = size.x; i > 0; i--) {
                UnitRec_0047c790* rec;
                unsigned short id = cell->field_0;
                if (id == 0)
                    goto b_write;
                rec = &g_game->units[id];
                if (rec->owner->active == 0)
                    goto b_b;
                if (rec->owner->type != 3)
                    goto b_b;
                rec->flags |= 0x8000000;
                obj->flags.all |= 0x4000000;
            b_write:
                cell->field_0 = obj->field_a8;
                goto b_next;
            b_b:
                rec->flags |= 0x4000000;
                obj->flags.all |= 0x8000000;
            b_next: ;
                cell++;
            }
            cell += g_game->width - size.x;
        }
    } else {
        Cell_0047c790* cell = GetMapCell(obj->pos.x, obj->pos.y);
        for (int j = size.y; j > 0; j--) {
            for (int i = size.x; i > 0; i--) {
                UnitRec_0047c790* rec;
                unsigned short id = cell->field_2;
                if (id == 0)
                    goto c_write;
                rec = &g_game->units[id];
                if (rec->owner->active == 0)
                    goto c_b;
                if (rec->owner->type != 3)
                    goto c_b;
                rec->flags |= 0x8000000;
                obj->flags.all |= 0x4000000;
            c_write:
                cell->field_2 = obj->field_a8;
                goto c_next;
            c_b:
                rec->flags |= 0x4000000;
                obj->flags.all |= 0x8000000;
            c_next: ;
                cell++;
            }
            cell += g_game->width - size.x;
        }
    }
}

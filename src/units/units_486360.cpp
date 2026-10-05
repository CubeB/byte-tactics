// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct UnitType_486360 {
    char unknown_0[0x1bc];
    unsigned short field_1bc;        // +0x1bc
    char unknown_1be[0x241 - 0x1be];
    unsigned int flags;              // +0x241
};

struct Pos_486360 {
    char unknown_0[0xc];
};

struct Unit {
    char unknown_0[0x64];
    char field_64[6];                // +0x64
    Pos_486360 pos;                  // +0x6a
    short x;                         // +0x76
    short y;                         // +0x78
    char unknown_7a[0x92 - 0x7a];
    UnitType_486360* type;           // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char owner;             // +0xff
};

struct Entry_486360 {
    char unknown_0[0xf4];
    unsigned short next;             // +0xf4
    char unknown_f6[0x100 - 0xf6];
};

struct Game {
    char unknown_0[0x1426f];
    Entry_486360* entries;           // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char limit;             // +0x1427f
};
#pragma pack(pop)

struct Result_486360 {
    char unknown_0[0x18];
    int field_18;                    // +0x18
    int field_1c;                    // +0x1c
};

// GLOBAL: 0x511de8
extern Game* g_game;

void* __stdcall FUN_00481550(int x, int y);
int __stdcall FUN_00485070(Pos_486360* pos);
Result_486360* __stdcall FUN_00423c50(void* target, unsigned short id, Pos_486360* pos, void* field_64, unsigned char owner);
void __stdcall FUN_00472630(Pos_486360* pos, int a, int b, int c);

// FUNCTION: 0x486360
void __stdcall FUN_00486360(Unit* unit, int depth, int flag)
{
    unsigned short id = unit->type->field_1bc;
    for (; depth > 1; depth--) {
        if (id >= 0xfffb) {
            return;
        }
        id = g_game->entries[id].next;
    }
    if (id < 0xfffb) {
        void* target = FUN_00481550(unit->x, unit->y);
        if (target != 0) {
            Pos_486360* pos = &unit->pos;
            if (FUN_00485070(pos) <= g_game->limit) {
                Result_486360* r = FUN_00423c50(target, id, pos, unit->field_64, unit->owner);
                if (r != 0) {
                    if (!(unit->type->flags & 0x1000000)) {
                        r->field_18 = -11468;
                        r->field_1c = 0;
                    }
                    flag = 0;
                }
            } else {
                FUN_00423c50(target, id, pos, unit->field_64, unit->owner);
            }
            if (flag) {
                FUN_00472630(pos, 0xf, 900, 9);
            }
        }
    }
}

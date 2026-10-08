// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by claude-opus-5-5. Names are provisional.
// The full windows.h stays: its declaration count decides how the cell index
// multiply folds.

#include <windows.h>

#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell_0047db20 {
    short unit;
    short unit2;
    char unknown_4[0xc - 0x4];
    unsigned char field_c;
};

struct Unit_0047db20 {
    char unknown_0[0x14e];
    unsigned char* mask;
};

union Flags_0047db20 {
    struct {
        unsigned int unknown_0 : 26;
        unsigned int flag26 : 1;
        unsigned int unknown_1 : 5;
    } bits;
    int all;
};

struct Obj_0047db20 {
    char unknown_0[0x76];
    Point pos;
    char unknown_7a[4];
    Point size;
    int spatialBucket;
    char unknown_86[0x92 - 0x86];
    Unit_0047db20* unit;
    char unknown_96[0xa8 - 0x96];
    short id;
    char unknown_aa[0x110 - 0xaa];
    Flags_0047db20 flags;
};

struct Game {
    char unknown_0[0x14233];
    int width;
    char unknown_14237[0x14287 - 0x14237];
    Cell_0047db20* cells;
    char unknown_1428b[0x142b7 - 0x1428b];
    int field_142b7;
};

class ClaimFootprintVisitor {
public:
    virtual void ClaimFootprint();
};
#pragma pack(pop)

extern Game* g_game;
extern ClaimFootprintVisitor g_claimFootprintVtable[];

void __stdcall UpdateCellHeightRange(Point pos, Point size);
void __stdcall VisitObjectsInArea(Point pos, Point size, ClaimFootprintVisitor* visitor);
void __stdcall RefreshPassMapsForUnit(Obj_0047db20* obj);

// Reads the position through a const reference: written inline, the two
// obj->pos.y reads become one common subexpression.
static inline Cell_0047db20* CellAt(const Point& p)
{
    return &g_game->cells[p.y * g_game->width + p.x];
}

// Stays in a file of its own: in the merged file the symbol count clears bit 14
// of g_game's id, which decides how the cell index multiply folds.
// FUNCTION: 0x47d0e0
void __stdcall RemoveUnitFromMap(Obj_0047db20* obj)
{
    if (obj->spatialBucket != g_game->field_142b7) {
        Point size = obj->size;
        Cell_0047db20* cell = CellAt(obj->pos);
        int index = 0;
        if (obj->flags.all & 0x20000000) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    unsigned char m = obj->unit->mask[index];
                    index++;
                    if (cell->unit == obj->id) cell->unit = 0;
                    if (m & 1) cell->field_c &= 0xfd;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            Point grown;
            grown.x = size.x + 2;
            grown.y = size.y + 2;
            Point pad;
            pad.x = obj->pos.x - 1;
            pad.y = obj->pos.y - 1;
            UpdateCellHeightRange(pad, grown);
        } else if ((obj->flags.all & 3) == 1) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    if (cell->unit == obj->id) cell->unit = 0;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        } else if ((obj->flags.all & 3) == 2) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    if (cell->unit2 == obj->id) cell->unit2 = 0;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        }
    }
    obj->flags.all &= ~0x08000000;
    if (obj->flags.bits.flag26) {
        obj->flags.all &= ~0x04000000;
        ClaimFootprintVisitor visitor;
        VisitObjectsInArea(obj->pos, obj->size, &visitor);
    }
    RefreshPassMapsForUnit(obj);
}

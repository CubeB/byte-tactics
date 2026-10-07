// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by space-bunny-free, finished by GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// Needed: removing it changes the generated code.
#include <math.h>

struct Obj_0047e5c0;

class Class_0047db20 {
public:
    virtual void FUN_0047ed30(Obj_0047e5c0* obj);
};

struct Pt_0047db20 {
    short x;
    short y;
};

#pragma pack(push, 1)
struct Obj_0047e5c0 {
    char unknown_0[0x76];
    Pt_0047db20 pos;                        // +0x76
    char unknown_7a[4];
    Pt_0047db20 size;                       // +0x7e
    char unknown_82[8];
    Obj_0047e5c0* child;                    // +0x8a
    Obj_0047e5c0* next;                     // +0x8e
};

struct Cell_0047e5c0 {                      // 10 bytes
    char unknown_0[6];
    Obj_0047e5c0* head;                     // +0x6
};

struct Grid_0047e5c0 {
    Cell_0047e5c0* cells;                   // +0x0
    unsigned int width;                     // +0x4
    unsigned int height;                    // +0x8
};

struct Game {
    char unknown_0[0x1429f];
    Grid_0047e5c0 grid;                     // +0x1429f
};
#pragma pack(pop)

extern Game* g_game;

// Scans every grid cell overlapping the rectangle [pos, pos+size), calling
// visitor->FUN_0047ed30 for each object (and child) whose own rectangle overlaps.
// FUNCTION: 0x47e5c0
void __stdcall VisitObjectsInArea(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor)
{
    // Declared apart from its assignment: keeps the sum from fusing.
    int sumy;
    Grid_0047e5c0* grid = &g_game->grid;
    // Declaration order of the bounds is fixed: other orders change allocation.
    int xstart = -1 + (pos.x >> 3);
    int ystart = -1 + (pos.y >> 3);
    int sumx = pos.x + size.x;
    sumy = pos.y + size.y;
    int xend = (sumx >> 3) + 1;
    // Recomputed from pos.y + size.y, not shifted from sumy.
    int ye = ((pos.y + size.y) >> 3) + 1;
    for (int x = xstart; x <= xend; x++) {
        for (int y = ystart; y <= ye; y++) {
            if (x >= grid->width) {
                continue;
            }
            if (y >= grid->height) {
                continue;
            }
            for (Obj_0047e5c0* o = grid->cells[grid->width * y + x].head; o != 0; o = o->next) {
                Pt_0047db20 op = o->pos;
                Pt_0047db20 os = o->size;
                if (pos.x < os.x + op.x && sumx > op.x && pos.y < os.y + op.y && sumy > op.y) {
                    visitor->FUN_0047ed30(o);
                }
                for (Obj_0047e5c0* c = o->child; c != 0; c = c->next) {
                    Pt_0047db20 cs = c->size;
                    Pt_0047db20 cp = c->pos;
                    if (pos.x < cs.x + cp.x && sumx > cp.x && pos.y < cp.y + cs.y && sumy > cp.y) {
                        visitor->FUN_0047ed30(c);
                    }
                }
            }
        }
    }
}

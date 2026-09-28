// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// 67.5%: the loop bodies and the value sequence match, but MSVC picks a
// different set of registers. The original loads size.y into edx before the
// pushes, keeps the grid pointer in edi and the y counter in esi; this source
// gets size.y into edi after the pushes, grid in esi and the y counter in ecx,
// then spills y. Hoisting ystart/yend/xend as separate variables, swapping the
// sum operand order or making Pt copies all compile to the same (wrong)
// allocation or worse; any change that moves size.y before the pushes flips
// the whole allocation.
//
// Retry notes (deepseek-v4.1-flash): computing ys right after y2 into its own
// local (instead of in the inner for-init) does put size.y back in edx and
// size.x in ecx, but the allocator still gives the grid pointer esi and the
// loop counters end up elsewhere, scoring 45-52 percent. Declaring x1/y1/x2/y2
// plus all four bounds collapses to the v1 allocation (grid esi, x ecx, y edx).
// What is still missing is making the y counter outrank the grid pointer for
// esi (so grid takes edi); no source-level ordering tried made that happen.

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

struct Game_0047e5c0 {
    char unknown_0[0x1429f];
    Grid_0047e5c0 grid;                     // +0x1429f
};
#pragma pack(pop)

extern Game_0047e5c0* g_game;

// Scans every grid cell overlapping the rectangle [pos, pos+size), calling
// visitor->FUN_0047ed30 for each object (and child) whose own rectangle overlaps.
// FUNCTION: 0x47e5c0
void __stdcall FUN_0047e5c0(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor)
{
    Grid_0047e5c0* grid = &g_game->grid;
    int sumx = pos.x + size.x;
    int sumy = pos.y + size.y;
    int xend = (sumx >> 3) + 1;
    for (int x = (pos.x >> 3) - 1; x <= xend; x++) {
        for (int y = (pos.y >> 3) - 1, ye = (sumy >> 3) + 1; y <= ye; y++) {
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
                    if (pos.x < cs.x + cp.x && sumx > cp.x && pos.y < cs.y + cp.y && sumy > cp.y) {
                        visitor->FUN_0047ed30(c);
                    }
                }
            }
        }
    }
}

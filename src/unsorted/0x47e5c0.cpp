// Decompiled by space-bunny-free. Names are provisional.

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

// FUNCTION: 0x47e5c0
void __stdcall FUN_0047e5c0(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor)
{
    Grid_0047e5c0* grid = &g_game->grid;
    int sumx = pos.x + size.x;
    int sumy = pos.y + size.y;
    for (int x = (pos.x >> 3) - 1; x <= (sumx >> 3) + 1; x++) {
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

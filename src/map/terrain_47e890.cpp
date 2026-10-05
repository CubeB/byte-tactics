// Decompiled by Space Bunny Free. Names are provisional.
// Sibling of 0x47e750, but the query is a point with a range instead of a
// rectangle: the cells the circle covers are walked and every object in them
// whose distance from the point is within the range is handed to the visitor
// (the one in 0x405d90 and its two siblings). The distance test is the fixed
// point one the game uses everywhere: only the high dword of each 64 bit product
// is kept, so the 32 bit add of the two terms can never overflow.
//
// Two spellings here are load bearing:
//  - The distance goes through a helper taking the two points as pointers, with
//    the z difference written first. Written inline the function is 6 bytes
//    shorter: the reload of the point pointer that the call clobbers then stays
//    in the loop latch instead of getting its own block at the top of the object
//    loop, and the extra reload after the loop disappears.
//  - Each of the four cell clamps goes through its own one line helper taking
//    the point pointer. All four in one helper (or written out by hand) gives the
//    same code except for the order of the two reloads at the end of the object
//    loop: the point pointer is reloaded before the x counter instead of after.
//    Only the separate helpers make x outrank pos in the allocator's priority.
#pragma pack(push, 1)
struct Vec3 {
    int x;
    int y;
    int z;
};

struct Unit {
    char unknown_0[0x6a];
    Vec3 pos;                                     // +0x6a
    char unknown_76[0x8e - 0x76];
    Unit* next;                                   // +0x8e
};

struct Cell_0047e890 {                            // 10 bytes
    char unknown_0[6];
    Unit* head;                                   // +0x6
};

struct Game {
    char unknown_0[0x1429f];
    Cell_0047e890* cells;                         // +0x1429f
    unsigned int width;                           // +0x142a3
    unsigned int height;                          // +0x142a7
};
#pragma pack(pop)

class Class_00405d90 {
public:
    virtual void FUN_00405d90(Unit* unit);
};

extern Game* g_game;

static inline int Clamp_0047e890(int v, unsigned int size)
{
    if (v < size) {
        return v;
    }
    if (v < 0) {
        return 0;
    }
    return size - 1;
}

static inline int ClampX_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->x - range) >> 23, size);
}

static inline int ClampZ_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->z - range) >> 23, size);
}

static inline int ClampX2_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->x + range) >> 23, size);
}

static inline int ClampZ2_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->z + range) >> 23, size);
}

static inline int Dist2_0047e890(Vec3* b, Vec3* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x47e890
void __stdcall VisitObjectsInRange(Vec3* pos, int range, Class_00405d90& visitor)
{
    int cx1 = ClampX_0047e890(pos, range, g_game->width);
    int cy1 = ClampZ_0047e890(pos, range, g_game->height);
    int cx2 = ClampX2_0047e890(pos, range, g_game->width);
    int cy2 = ClampZ2_0047e890(pos, range, g_game->height);
    int range2 = (int)(((__int64)range * range) >> 32);
    for (int y = cy1; y <= cy2; y++) {
        for (int x = cx1; x <= cx2; x++) {
            for (Unit* o = g_game->cells[g_game->width * y + x].head; o != 0; o = o->next) {
                if (Dist2_0047e890(pos, &o->pos) <= range2) {
                    visitor.FUN_00405d90(o);
                }
            }
        }
    }
}

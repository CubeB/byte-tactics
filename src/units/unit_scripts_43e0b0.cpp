// Decompiled by space-bunny-free. Names are provisional.
// Bounding-box centre of the vertices linked to a piece of the object's piece
// table (obj + 0x9e). The table holds a count at +0 and 0x36-byte piece
// records from +0x22; each record has a geometry pointer at +0 (its count is
// at +4) and a Vec3 vertex array at +0x22. The centre is the average of the
// per-component minima and maxima, offset by the object's world position at
// +0x6a. The minima and maxima start at 0, so a piece whose vertices are all
// positive on an axis keeps 0 as its minimum.
// The six min/max locals are declared before the piece pointer so the four
// register-resident ones are zeroed first; the loop is written as
// `while (n > 0) { ...; n--; }` so MSVC rotates it and spills the counter to
// the (dead) third argument's stack slot only after the entry test.

struct Vec3 {
    int x, y, z;
};

struct Geom_0043e0b0 {
    char pad0[4];
    int count;                         // +0x04
};

#pragma pack(push, 1)
struct Piece_0043e0b0 {
    Geom_0043e0b0* geom;               // +0x00
    char pad4[0x22 - 4];
    Vec3* verts;                       // +0x22
};

struct Obj_0043e0b0 {
    char pad0[0x6a];
    Vec3 pos;                          // +0x6a
    char pad76[0x9e - 0x76];
    char* table;                       // +0x9e
};
#pragma pack(pop)

// FUNCTION: 0x43e0b0
void __stdcall FUN_0043e0b0(Obj_0043e0b0* obj, Vec3* out, int value)
{
    int minx = 0, miny = 0, minz = 0;
    int maxx = 0, maxy = 0, maxz = 0;
    Piece_0043e0b0* p = (Piece_0043e0b0*)(obj->table + value * 0x36 + 0x22);
    Vec3* v = p->verts;
    int n = p->geom->count;
    while (n > 0) {
        int x = v->x;
        if (x < minx) minx = x;
        if (x > maxx) maxx = x;
        int y = v->y;
        if (y < miny) miny = y;
        if (y > maxy) maxy = y;
        int z = v->z;
        if (z < minz) minz = z;
        if (z > maxz) maxz = z;
        v++;
        n--;
    }
    out->x = (maxx + minx) / 2 + obj->pos.x;
    out->y = (maxy + miny) / 2 + obj->pos.y;
    out->z = (maxz + minz) / 2 + obj->pos.z;
}

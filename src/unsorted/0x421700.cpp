// Decompiled by deepseek-v4.1. Names are provisional.
// Partial: 70.9%, 1769 bytes versus 1692 (best of a Claude Opus 5.5 start plus
// deepseek-v4.1 work). Structural walk is right, but the local frame is 0xa0
// instead of the original 0x90 and the local layout differs: the original
// packs piece/count/verts/unit/desc at [esp+0x20..0x30] and i/primoffset at
// [esp+0x1c]/[esp+0x14], ours puts them 4 higher/lower respectively. The
// original also unpacks the FUN_004b6eb0 call results back into the shared
// float scratch slots ([esp+0x58..0x7c]) instead of keeping a 12-byte Vec3f
// local, so its _ftol/fld scheduling differs. What helped: declaring ab/n
// without an initializer and assigning on the next line (68.8 -> 70.9). What
// did not: a byte-offset two-pointer vertex-copy loop (64.1), unpacking the
// inner FUN_004b6eb0 into separate floats (67.1), splitting the float-vertex
// declarations (no change).
// Still differs: the frame is 0xa8 vs the original 0x90, 0x18 too big. Writing
// the three (int)(n.f * 65535.0f) results as plain ints (nx, ny, nz) shrinks
// the frame to 0x9c (12 bytes of the excess are the 12-byte ni local), and the
// original really does keep nx/ny in ebx/ebp and spill nz into the reused
// [esp+0x10] slot, but that variant scored 70.5 so it is reverted here; the
// float scratch layout (9 vertex floats plus ab/n) is what the second 12 bytes
// of excess have to come from.
#include <windows.h>
#include <memory.h>

struct Vec3_00421700 {
    int x;
    int y;
    int z;

    Vec3_00421700 operator+(const Vec3_00421700& o) const
    {
        Vec3_00421700 r;
        r.x = x + o.x;
        r.y = y + o.y;
        r.z = z + o.z;
        return r;
    }
};

struct Vec3f_00421700 {
    float x;
    float y;
    float z;

    Vec3f_00421700() {}
    Vec3f_00421700(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

struct Texture_00421700 {
    int tex;                            // +0x0
    int unknown_4;                      // +0x4
    unsigned short* texlist;            // +0x8
};

struct Prim_00421700 {
    int color;                          // +0x00
    int nverts;                         // +0x04
    int unknown_8;
    unsigned short* vindex;             // +0x0c
    Texture_00421700 tex;               // +0x10
    int flags;                          // +0x1c
};

struct Object3D_00421700 {
    unsigned char state;                // +0x00 (0xff = free)
    char unknown_1[7];
    int nprims;                         // +0x08
    int selprim;                        // +0x0c
    char unknown_10[0x14];
    Vec3_00421700* verts;               // +0x24
    Prim_00421700* prims;               // +0x28
    char unknown_2c[8];
};

struct Ref_00421700 {
    unsigned short index;               // +0x0
    unsigned short value;               // +0x2
    unsigned char kind;                 // +0x4
    char unknown_5[3];
    void* src;                          // +0x8
};

struct Debris_00421700 {
    Object3D_00421700* obj;             // +0x00
    Ref_00421700 ref1;                  // +0x04
    Ref_00421700 ref2;                  // +0x10
    Vec3_00421700 pos;                  // +0x1c
    int size_x;                         // +0x28
    int size_y;                         // +0x2c
    int size_z;                         // +0x30
    Vec3_00421700 vel;                  // +0x34
    Vec3_00421700 spin;                 // +0x40
    short angle_x;                      // +0x4c
    short angle_y;                      // +0x4e
    short angle_z;                      // +0x50
    unsigned short flag : 1;            // +0x52
};

struct Size_00421700 {
    char unknown_0[8];
    int x;                              // +0x08
    int y;                              // +0x0c
    int z;                              // +0x10
};

struct Player_00421700 {
    char unknown_0[0x96];
    unsigned char color;                // +0x96
};

#pragma pack(push, 1)
struct Owner_00421700 {
    char unknown_0[0x27];
    Player_00421700* player;            // +0x27
};

struct Piece_00421700 {
    Object3D_00421700* desc;            // +0x00
    char unknown_4[0x12];
    Vec3_00421700 offset;               // +0x16
    Vec3_00421700* verts;               // +0x22
    char unknown_26[0x36 - 0x26];
};

struct Unit_00421700 {
    Size_00421700* size;                // +0x00
    char unknown_4[0x6a - 4];
    Vec3_00421700 pos;                  // +0x6a
    char unknown_76[0x96 - 0x76];
    Owner_00421700* owner;              // +0x96
    char unknown_9a[4];
    char* pieces;                       // +0x9e
};

struct Game_00421700 {
    char unknown_0[0x14263];
    int ticks;                          // +0x14263
    char unknown_14267[0x1491b - 0x14267];
    int debrisCount;                    // +0x1491b
    Debris_00421700 debris[300];        // +0x1491f
    char unknown_1ab8f[0x1ab9f - 0x1ab8f];
    Object3D_00421700 objects[300];     // +0x1ab9f
};
#pragma pack(pop)

struct Header_00421700 {
    Unit_00421700* obj;                 // +0x00
    int index;                          // +0x04
    char unknown_8[0x1c];
    int scale;                          // +0x24
    unsigned int unknown_28 : 5;        // +0x28
    unsigned int flag : 1;
};

extern Game_00421700* g_game;

int __stdcall FUN_004b6c30(int range);
Vec3f_00421700 __stdcall FUN_004b6eb0(Vec3f_00421700 from, Vec3f_00421700 to);
Vec3f_00421700 __stdcall FUN_004b6f70(Vec3f_00421700 a, Vec3f_00421700 b);
Vec3f_00421700 __stdcall FUN_004b6ff0(Vec3f_00421700 v);
int __stdcall FUN_004b7f30(unsigned short* list, int index);

// FUN_00420920, inlined
static inline Object3D_00421700* NewObject()
{
    for (int i = 0; i < 300; i++) {
        if (g_game->objects[i].state == 0xff) {
            g_game->objects[i].state = 0;
            return &g_game->objects[i];
        }
    }
    return 0;
}

// FUNCTION: 0x421700
void __stdcall FUN_00421700(Header_00421700* param)
{
    Unit_00421700* unit = param->obj;
    Piece_00421700* piece = (Piece_00421700*)(unit->pieces + 0x22 + param->index * 0x36);
    Object3D_00421700* desc = piece->desc;
    int* count = &g_game->debrisCount;
    Vec3_00421700* verts = piece->verts;
    if (param->scale == 0)
        param->scale = 1;
    for (int i = 0; i < desc->nprims; i++) {
        if (*count >= 300)
            return;
        if (desc->prims[i].nverts == 4 && !(desc->prims[i].flags & 1) && desc->selprim != i) {
            Debris_00421700* d = (Debris_00421700*)(count + 1) + (*count)++;
            d->pos = piece->offset + unit->pos;
            d->obj = NewObject();
            if (d->obj == 0)
                return;
            d->flag = param->flag;
            if (unit->size) {
                d->size_x = unit->size->x >> 1;
                d->size_z = unit->size->z >> 1;
                d->size_y = unit->size->y >> 1;
            }
            d->vel.x = (0x50 - FUN_004b6c30(0xa0)) << 9;
            d->vel.z = (0x50 - FUN_004b6c30(0xa0)) << 9;
            d->vel.y = ((0x50 - FUN_004b6c30(0xa0)) << 9) + g_game->ticks * 30;
            d->spin.x = 800 - FUN_004b6c30(0x640);
            d->spin.y = 800 - FUN_004b6c30(0x640);
            d->spin.z = 800 - FUN_004b6c30(0x640);
            d->angle_x = 0;
            d->angle_y = 0;
            d->angle_z = 0;
            d->ref1.src = 0;
            d->ref2.src = 0;
            Object3D_00421700* o = d->obj;
            int k;
            for (k = 0; k < desc->prims[i].nverts; k++) {
                o->verts[k] = verts[desc->prims[i].vindex[k]];
                o->verts[7 - k] = verts[desc->prims[i].vindex[k]];
            }
            Vec3_00421700* v = o->verts;
            float ax = v[0].x * (1.0f / 65535.0f);
            float ay = v[0].y * (1.0f / 65535.0f);
            float az = v[0].z * (1.0f / 65535.0f);
            float bx = v[1].x * (1.0f / 65535.0f);
            float by = v[1].y * (1.0f / 65535.0f);
            float bz = v[1].z * (1.0f / 65535.0f);
            float cx = v[2].x * (1.0f / 65535.0f);
            float cy = v[2].y * (1.0f / 65535.0f);
            float cz = v[2].z * (1.0f / 65535.0f);
            Vec3f_00421700 ab;
            ab = FUN_004b6eb0(Vec3f_00421700(bx, by, bz), Vec3f_00421700(ax, ay, az));
            Vec3f_00421700 n;
            n = FUN_004b6ff0(FUN_004b6f70(FUN_004b6eb0(Vec3f_00421700(bx, by, bz), Vec3f_00421700(cx, cy, cz)), ab));
            d->vel.x += FUN_004b6c30(200) * (short)(n.x * 512.0f);
            d->vel.z -= FUN_004b6c30(200) * (short)(n.z * 512.0f);
            Vec3_00421700 ni;
            ni.x = (int)(n.x * 65535.0f);
            ni.y = (int)(n.y * 65535.0f);
            ni.z = (int)(n.z * 65535.0f);
            for (k = 0; k < 4; k++) {
                o->verts[k + 4].x -= ni.x * param->scale;
                o->verts[k + 4].y -= ni.y * param->scale;
                o->verts[k + 4].z -= ni.z * param->scale;
            }
            int sx = 0;
            int sy = 0;
            int sz = 0;
            for (k = 0; k < 8; k++) {
                sx += o->verts[k].x;
                sy += o->verts[k].y;
                sz += o->verts[k].z;
            }
            sx /= 8;
            sy /= 8;
            sz /= 8;
            for (k = 0; k < 8; k++) {
                o->verts[k].x -= sx;
                o->verts[k].y -= sy;
                o->verts[k].z -= sz;
            }
            for (int m = 0; m < o->nprims; m++) {
                o->prims[m].tex = desc->prims[i].tex;
                o->prims[m].flags = desc->prims[i].flags;
                o->prims[m].color = desc->prims[i].color;
                int flags = o->prims[m].flags;
                if (!(flags & 1) && (flags & 2) && (flags & 4)) {
                    o->prims[m].tex.tex = FUN_004b7f30(o->prims[m].tex.texlist, unit->owner->player->color);
                    o->prims[m].flags &= ~2;
                }
            }
        }
    }
}

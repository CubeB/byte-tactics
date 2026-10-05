// Decompiled by deepseek-v4.1, finished by xiaomi/mimo-v2.6-pro, finished by fledge-alpha-free, finished by Claude Opus 5.5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Breaks a unit piece into debris: for each quad face of the piece, takes a
// free debris slot and a free 3D object (FUN_00420920, inlined), gives it a
// random velocity and spin, copies the face's four vertices into the object
// (twice, as a box), pushes the back four out along the face normal by
// param->scale, centres the box on its own origin and copies the face's
// texture and colour into the object's faces.
//
// MATCH (#5457, Claude Opus 5.5; 76.6% before). Three things were missing:
//  * The nine scaled vertex components go through a FIX2F macro,
//    `(((float)(x)) / 65535.0f)`, with the cast and the whole expression each
//    in their own parentheses. Then the stores into a, b and c are
//    list-scheduled (interleaved fild/fmul/fstp) as in the original, even
//    though the block has struct-returning calls. Dropping either pair of
//    parentheses, or any plain `x * k`, serialises them (the wall every
//    earlier pass hit). `* (1.0f / 65535.0f)` compiles the same. Only the
//    a, b, c / x, y, z statement order gives the original's schedule.
//  * The second difference goes into n itself: `ab = FUN_004b6eb0(b, a);
//    n = FUN_004b6eb0(b, c); n = FUN_004b6ff0(FUN_004b6f70(n, ab));`. Nested
//    as FUN_004b6f70(FUN_004b6eb0(b, c), ab), b's three loads were CSEd
//    across the first call, which made C2 split d and the loop invariants as
//    soon as o took esi (the whole head allocation changed: 51.6%). This
//    form also gives ab its own frame slot, as in the original.
//  * piece is computed from unit (`unit->pieces`) and desc is read before
//    verts. That order of first uses sets the candidate ids, and with them
//    the order of the reloads at the bottom of the loop and the frame slots
//    of piece, count, unit and desc (96.2% -> MATCH).
// <windows.h> and <minmax.h> are needed for the symbol count (without
// <minmax.h> one load pair in d->pos swaps).
#include <windows.h>
#include <minmax.h>

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

struct Unit {
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
    Unit* obj;                          // +0x00
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

#define FIX2F(x) (((float)(x)) / 65535.0f)
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
    Unit* unit = param->obj;
    Piece_00421700* piece =
        (Piece_00421700*)(unit->pieces + 0x22 + param->index * 0x36);
    int* count = &g_game->debrisCount;
    Object3D_00421700* desc = piece->desc;
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
            Vec3f_00421700 a, b, c;
            a.x = FIX2F(v[0].x);
            a.y = FIX2F(v[0].y);
            a.z = FIX2F(v[0].z);
            b.x = FIX2F(v[1].x);
            b.y = FIX2F(v[1].y);
            b.z = FIX2F(v[1].z);
            c.x = FIX2F(v[2].x);
            c.y = FIX2F(v[2].y);
            c.z = FIX2F(v[2].z);
            Vec3f_00421700 ab, n;
            ab = FUN_004b6eb0(b, a);
            n = FUN_004b6eb0(b, c);
            n = FUN_004b6ff0(FUN_004b6f70(n, ab));
            d->vel.x += FUN_004b6c30(200) * (short)(n.x * 512.0f);
            d->vel.z -= FUN_004b6c30(200) * (short)(n.z * 512.0f);
            int nx = (int)(n.x * 65535.0f);
            int ny = (int)(n.y * 65535.0f);
            int nz = (int)(n.z * 65535.0f);
            for (k = 0; k < 4; k++) {
                o->verts[k + 4].x -= nx * param->scale;
                o->verts[k + 4].y -= ny * param->scale;
                o->verts[k + 4].z -= nz * param->scale;
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

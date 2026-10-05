// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Expands a bounding box (lo, hi) with the vertices of an object and, if arg is
// set, of its two children, translated by offset.

struct Vec3_004cb650 {
    int x;
    int y;
    int z;
};

struct Object_004cb650 {
    int unknown_0;
    int count;                       // +0x04
    int unknown_8;
    int unknown_c;
    int x;                           // +0x10
    int y;                           // +0x14
    int z;                           // +0x18
    int unknown_1c;
    int unknown_20;
    Vec3_004cb650* verts;            // +0x24
    int unknown_28;
    Object_004cb650* child_2c;       // +0x2c
    Object_004cb650* child_30;       // +0x30
};

// FUNCTION: 0x4cb6a0
void __stdcall AddObjectBounds(Object_004cb650* obj, Vec3_004cb650* offset,
                            Vec3_004cb650* lo, Vec3_004cb650* hi, int arg)
{
    Vec3_004cb650 local;
    local.x = obj->x + offset->x;
    local.y = offset->y + obj->y;
    local.z = obj->z + offset->z;
    if (obj->count > 2) {
        for (int i = 0; i < obj->count; i++) {
            Vec3_004cb650* v = &obj->verts[i];
            if (v->x + local.x > hi->x) hi->x = v->x + local.x;
            if (v->x + local.x < lo->x) lo->x = v->x + local.x;
            if (v->y + local.y > hi->y) hi->y = v->y + local.y;
            if (v->y + local.y < lo->y) lo->y = v->y + local.y;
            if (v->z + local.z > hi->z) hi->z = v->z + local.z;
            if (v->z + local.z < lo->z) lo->z = v->z + local.z;
        }
    }
    if (arg != 0) {
        if (obj->child_30 != 0)
            AddObjectBounds(obj->child_30, &local, lo, hi, arg);
        if (obj->child_2c != 0)
            AddObjectBounds(obj->child_2c, offset, lo, hi, arg);
    }
}

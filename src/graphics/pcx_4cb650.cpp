// Decompiled by Opus. Names are provisional.
// Computes the bounding box of an object tree: clears the two output corners
// and walks the tree from a zero offset.

struct Vec3_004cb650 {
    int x;
    int y;
    int z;
};

struct Object_004cb650;

void __stdcall FUN_004cb6a0(Object_004cb650* obj, Vec3_004cb650* offset,
                            Vec3_004cb650* lo, Vec3_004cb650* hi, int arg);

// FUNCTION: 0x4cb650
void __stdcall FUN_004cb650(Object_004cb650* obj, Vec3_004cb650* lo,
                            Vec3_004cb650* hi, int arg)
{
    Vec3_004cb650 offset;
    lo->x = 0;
    lo->y = 0;
    lo->z = 0;
    hi->x = 0;
    hi->y = 0;
    hi->z = 0;
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    FUN_004cb6a0(obj, &offset, lo, hi, arg);
}

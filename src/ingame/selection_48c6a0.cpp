// Decompiled by space-bunny-free. Names are provisional.
// Builds the four corners of an object's bounding box footprint at the box's low
// y, rotates each corner by the object's three angles, converts it to screen space
// and runs a point-in-polygon test against the rotated quad.
// The three screen offsets live in one Vec3 so that only o.y is spilled; that
// 12-byte local is what fixes this function's 0x88 frame.

struct Vec3_0048c6a0 {
    int x;
    int y;
    int z;
};

struct Point_0048c6a0 {
    int x;
    int y;
};

void __stdcall FUN_004b6cc0(Vec3_0048c6a0* in, Vec3_0048c6a0* out, short* angles);
void __stdcall GetObjectBounds(void* obj, Vec3_0048c6a0* lo, Vec3_0048c6a0* hi, int arg);
int __stdcall PointInPolygon(Point_0048c6a0* pts, int n, int px, int py);

extern char* g_game;

#pragma pack(push, 2)
class Object_0048c6a0 {
public:
    char unknown_64[0x64];
    short angles[3];
    int posx;
    int posz;
    int posy;
    char unknown_76[0x30];
    unsigned short index;
};
#pragma pack(pop)

// FUNCTION: 0x48c6a0
int __stdcall FUN_0048c6a0(Object_0048c6a0* obj, Point_0048c6a0* p)
{
    void** models = (void**)(*(int*)(g_game + 0x14377));
    Vec3_0048c6a0 corners[4];
    Point_0048c6a0 pts[4];
    int i;
    Vec3_0048c6a0 o;
    Vec3_0048c6a0 lo;
    Vec3_0048c6a0 hi;
    GetObjectBounds(models[obj->index], &lo, &hi, 0);
    corners[0].x = lo.x;
    corners[0].y = lo.y;
    corners[0].z = lo.z;
    corners[1].x = hi.x;
    corners[1].y = lo.y;
    corners[1].z = lo.z;
    corners[2].x = hi.x;
    corners[2].y = lo.y;
    corners[2].z = hi.z;
    corners[3].x = lo.x;
    corners[3].y = lo.y;
    corners[3].z = hi.z;
    o.x = obj->posx - (*(int*)(g_game + 0x1431f) << 16);
    o.y = obj->posz;
    o.z = obj->posy - (*(int*)(g_game + 0x14323) << 16);
    short* angles = obj->angles;
    for (i = 0; i < 4; i++) {
        Vec3_0048c6a0 v;
        FUN_004b6cc0(&corners[i], &v, angles);
        pts[i].x = (short)((v.x + o.x) >> 16) + 0x80;
        pts[i].y = ((short)((o.z - v.z) >> 16) - ((short)((v.y + o.y) >> 16) >> 1)) + 0x20;
    }
    return PointInPolygon(pts, 4, p->x, p->y) != 0;
}

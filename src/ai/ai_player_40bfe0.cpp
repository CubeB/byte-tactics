// Decompiled by GPT-6 Astra. Names are provisional.
#include <math.h>

struct Vec3 {
    int x;
    int y;
    int z;

    int Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    void Scale(int s)                  // s is 16.16 fixed point
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

static inline Vec3 operator-(const Vec3& p, const Vec3& q)
{
    Vec3 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

static inline Vec3 MoveTowards(const Vec3* from, const Vec3* to,
                                     int maxLen)
{
    Vec3 d = *to - *from;
    int len = d.Length();
    if (maxLen >= len)
        return *to;
    d.Scale((int)(((__int64)maxLen << 16) / len));
    Vec3 r;
    r.x = from->x + d.x;
    r.y = from->y + d.y;
    r.z = from->z + d.z;
    return r;
}

struct Point { short x, y; };
#pragma pack(push, 1)
struct UnitType { char pad0[0x14a]; Point origin; char pad14e[0x1ce-0x14e]; float value; };
struct Net { char pad0[0xd30]; int threshold; };
struct Game { char pad0[0x14223]; int width, height; char pad1422b[0x391e9-0x1422b]; Net* net; };
class Class_0040a7b0 {
public:
    char pad0[0x35]; Vec3 pos; char pad41[12]; char cells[16]; char pad5d[0x109-0x5d]; int range;
    bool FUN_0040a260(UnitType*, Vec3*, void*, int, Point*);
    bool FUN_0040a5d0(UnitType*, Vec3*, int, Point*);
};
#pragma pack(pop)
extern Game* g_game;
extern Class_0040a7b0* DAT_005119c0[];
int __stdcall FUN_004b6c30(int);
static inline void CellToWorld(Vec3* out, Point p, Point origin) {
    out->x=(origin.x+p.x*2)<<19;
    out->z=(origin.y+p.y*2)<<19;
}
// FUNCTION: 0x40bfe0
int __stdcall FUN_0040bfe0(int player, const Vec3* from, UnitType* type, Vec3* out)
{
    Class_0040a7b0* ai=DAT_005119c0[player];
    int maximum=g_game->width > g_game->height ? g_game->width : g_game->height;
    if (ai->range<maximum) ai->range+=160;
    Vec3 pos=MoveTowards(from,&ai->pos,ai->range<<16);
    Point cell;
    int result;
    if (type->value!=0.0f && g_game->net->threshold<FUN_004b6c30(255))
        result=ai->FUN_0040a260(type,&pos,ai->cells,ai->range*4,&cell);
    else result=ai->FUN_0040a5d0(type,&pos,ai->range,&cell);
    if (result) {
        CellToWorld(out,cell,type->origin);
        ai->range=0;
    }
    return result;
}

// Decompiled by Opus. Names are provisional.
// Constructor of a Class_0044ce20 subclass that stores a point converted
// from fixed-point world coordinates relative to the map origin, a radius
// and (radius / 16) squared. It stores the same vtable as Class_0044d010,
// so this is probably a second constructor of that class.

struct Point_0044cf60 {
    short x;
    short y;
};

struct Map_0044cf60 {
    char unknown_0[0x7e];
    Point_0044cf60 origin;             // +0x7e
};

#pragma pack(push, 2)
struct Source_0044cf60 {
    char unknown_0[0xe];
    Map_0044cf60* map;                 // +0xe
};
#pragma pack(pop)

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd328[];

class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Source_0044cf60* field_4;          // +0x4

    Class_0044ce20(Source_0044cf60* source)
    {
        vtable = DAT_004fd2f8;
        field_4 = source;
    }
};

class Class_0044cf60 : public Class_0044ce20 {
public:
    Point_0044cf60 pos;                // +0x8
    int radius;                        // +0xc
    int radiusSq;                      // +0x10

    Class_0044cf60(Source_0044cf60* source, int x, int y, int r);
};

// FUNCTION: 0x44cf60
Class_0044cf60::Class_0044cf60(Source_0044cf60* source, int x, int y, int r)
    : Class_0044ce20(source)
{
    vtable = DAT_004fd328;
    Point_0044cf60 org = source->map->origin;
    Point_0044cf60 p;
    p.x = (x - (org.x << 19) + 0x80000) >> 20;
    p.y = (y - (org.y << 19) + 0x80000) >> 20;
    pos = p;
    radius = r;
    radiusSq = (r / 16) * (r / 16);
}

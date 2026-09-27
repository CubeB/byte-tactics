// Decompiled by Opus. Names are provisional.
// Constructor of a Class_0044ce20 subclass (vtable 0x4fd358, the same class
// as Class_0044d470's file-loading constructor) that stores a point converted
// from fixed-point world coordinates relative to the map origin, two radii
// and each (radius / 16) squared. Two-radius version of 0x44cf60; the
// radius2 store has to come after the conversion.

struct Point_0044d3b0 {
    short x;
    short y;
};

struct Map_0044d3b0 {
    char unknown_0[0x7e];
    Point_0044d3b0 origin;             // +0x7e
};

#pragma pack(push, 2)
struct Source_0044d3b0 {
    char unknown_0[0xe];
    Map_0044d3b0* map;                 // +0xe
};
#pragma pack(pop)

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd358[];

class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Source_0044d3b0* field_4;          // +0x4

    Class_0044ce20(Source_0044d3b0* source)
    {
        vtable = DAT_004fd2f8;
        field_4 = source;
    }
};

class Class_0044d3b0 : public Class_0044ce20 {
public:
    Point_0044d3b0 pos;                // +0x8
    int radius2;                       // +0xc
    int radius1;                       // +0x10
    int radius2Sq;                     // +0x14
    int radius1Sq;                     // +0x18

    Class_0044d3b0(Source_0044d3b0* source, int x, int y, int r1, int r2);
};

// FUNCTION: 0x44d3b0
Class_0044d3b0::Class_0044d3b0(Source_0044d3b0* source, int x, int y, int r1, int r2)
    : Class_0044ce20(source)
{
    vtable = DAT_004fd358;
    Point_0044d3b0 org = source->map->origin;
    Point_0044d3b0 p;
    p.x = (x - (org.x << 19) + 0x80000) >> 20;
    p.y = (y - (org.y << 19) + 0x80000) >> 20;
    pos = p;
    radius2 = r2;
    radius1 = r1;
    radius1Sq = (r1 / 16) * (r1 / 16);
    radius2Sq = (r2 / 16) * (r2 / 16);
}

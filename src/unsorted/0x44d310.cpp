// Decompiled by Opus. Names are provisional.

struct Obj_0044d310 {
    char unknown_0[0x76];
    short x;                           // +0x76
    short y;                           // +0x78
};

class Class_0044d310 {
public:
    char unknown_0[0x8];
    short x;                           // +0x8
    short y;                           // +0xa
    char unknown_c[0x10 - 0xc];
    int radiusSq;                      // +0x10

    int FUN_0044d310(Obj_0044d310* obj);
};

// Whether obj lies within the circle around (x, y).
// FUNCTION: 0x44d310
int Class_0044d310::FUN_0044d310(Obj_0044d310* obj)
{
    int dy = obj->y - y;
    int dx = obj->x - x;
    return dx * dx + dy * dy <= radiusSq;
}

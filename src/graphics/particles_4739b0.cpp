// Decompiled by Opus. Names are provisional.
// Advances a position by its velocity and steps a 1..7 animation counter
// kept in the low four bits of a flags word.

struct Vec3_004739b0 {
    int x, y, z;
    Vec3_004739b0& operator+=(const Vec3_004739b0& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

class Class_004739b0 {
public:
    Vec3_004739b0 pos;                 // +0x0
    char unknown_c[0xc];
    Vec3_004739b0 vel;                 // +0x18
    char unknown_24[4];
    int flags;                         // +0x28, low 4 bits: frame

    void Step();
};

// FUNCTION: 0x4739b0
void Class_004739b0::Step()
{
    short frame = (flags & 0xf) + 1;
    pos += vel;
    if (frame > 7)
        frame = 1;
    flags = (flags & 0xfff0) + frame;
}

// Decompiled by Opus. Names are provisional.

struct Vec3i_00474130 {
    int x;
    int y;
    int z;

    Vec3i_00474130& operator+=(const Vec3i_00474130& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

class Class_00474130 {
public:
    char unknown_0[4];
    Vec3i_00474130 pos;       // +0x4
    char unknown_10[0x1c - 0x10];
    Vec3i_00474130 vel;       // +0x1c
    int mod2_28;              // +0x28
    int val2_2c;              // +0x2c
    int val_30;               // +0x30
    int mod_34;               // +0x34

    void Step();
};

// The three adds are an inlined vector operator+=: written as plain
// statements, MSVC sinks the third store past the load of val_30.
// FUNCTION: 0x474130
void Class_00474130::Step()
{
    pos += vel;
    val_30 = (val_30 + 1) % mod_34;
    if (val_30 == 0) {
        val2_2c = (val2_2c + 1) % mod2_28;
    }
}

// Decompiled by Opus. Names are provisional.
// Moves by the velocity, then every `period` ticks steps a value that wraps
// between min and max.

struct Vec3_00474580 {
    int x, y, z;
    void operator+=(const Vec3_00474580& o) { x += o.x; y += o.y; z += o.z; }
};

class Class_00474580 {
public:
    int unknown_0;
    Vec3_00474580 pos;                 // +0x04
    char unknown_10[0x1c - 0x10];
    Vec3_00474580 vel;                 // +0x1c
    int min;                           // +0x28
    int max;                           // +0x2c
    int value;                         // +0x30
    int step;                          // +0x34
    int tick;                          // +0x38
    int period;                        // +0x3c

    void Step();
};

// FUNCTION: 0x474580
void Class_00474580::Step()
{
    pos += vel;
    tick = (tick + 1) % period;
    if (tick == 0) {
        value += step;
        if (value > max) value = min;
        if (value < min) value = max;
    }
}

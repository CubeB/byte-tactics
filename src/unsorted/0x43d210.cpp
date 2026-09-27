// Decompiled by Opus. Names are provisional.
// Changes the 2-bit state at +0x2e; entering state 1 clears the velocity and
// field_20 and clears flag 1 on the owner, any other state sets it.

struct Vec3 {
    int x;
    int y;
    int z;
    Vec3() {}
    Vec3(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
};

class Class_0048b090 {
public:
    void FUN_0048b090(int param_1, int param_2);
};

class Class_0043d210 {
public:
    char unknown_0[8];
    Vec3 velocity;                     // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                      // +0x20
    char unknown_24[0x2e - 0x24];
    unsigned char state : 2;           // +0x2e bits 0-1

    void FUN_0043d0d0(Class_0048b090* owner, Vec3* v);
    void FUN_0043d210(Class_0048b090* owner, int state);
};

// field_20 must be cleared before the zero vector is built: it is stored
// with an immediate while the vector uses zeroed registers.
// FUNCTION: 0x43d210
void Class_0043d210::FUN_0043d210(Class_0048b090* owner, int newState)
{
    if (state != newState) {
        if (newState == 1) {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
            FUN_0043d0d0(owner, &zero);
            owner->FUN_0048b090(1, 0);
        } else {
            owner->FUN_0048b090(1, 1);
        }
        state = newState;
    }
}

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

class Unit {
public:
    void SetStateBits(int param_1, int param_2);
};

class UnitMotion {
public:
    char unknown_0[8];
    Vec3 velocity;                     // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                      // +0x20
    char unknown_24[0x2e - 0x24];
    unsigned char state : 2;           // +0x2e bits 0-1

    void ApplyBankAndPitch(Unit* owner, Vec3* v);
    void SetFlightMode(Unit* owner, int state);
};

// field_20 must be cleared before the zero vector is built: it is stored
// with an immediate while the vector uses zeroed registers.
// FUNCTION: 0x43d210
void UnitMotion::SetFlightMode(Unit* owner, int newState)
{
    if (state != newState) {
        if (newState == 1) {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
            ApplyBankAndPitch(owner, &zero);
            owner->SetStateBits(1, 0);
        } else {
            owner->SetStateBits(1, 1);
        }
        state = newState;
    }
}

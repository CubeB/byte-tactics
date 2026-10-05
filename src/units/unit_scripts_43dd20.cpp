// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct UnitType_0043dd20 {
    char unknown_0[0x241];
    unsigned int flags_0 : 11;         // +0x241
    unsigned int flag_800 : 1;         // bit 11
};

struct Unit {
    char unknown_0[0x92];
    UnitType_0043dd20* type;           // +0x92
};
#pragma pack(pop)

class Iface_0043dd20 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
};

class Class_0043db50 { public: void UpdateSfxOccupy(Unit* u); };

class UnitMotion {
public:
    Iface_0043dd20* iface;             // +0x0
    void UpdateMotion(Unit* u);
    void SteerGroundUnit(Unit* u);
    void SteerAircraft(Unit* u);
    void UpdatePosition(Unit* u);
    void UpdateMoveRate(Unit* u);
};

// FUNCTION: 0x43dd20
void UnitMotion::UpdateMotion(Unit* u)
{
    iface->v2();
    if (u->type->flag_800)
        ((UnitMotion*)this)->SteerAircraft(u);
    else
        ((UnitMotion*)this)->SteerGroundUnit(u);
    ((UnitMotion*)this)->UpdatePosition(u);
    ((UnitMotion*)this)->UpdateMoveRate(u);
    ((Class_0043db50*)this)->UpdateSfxOccupy(u);
}

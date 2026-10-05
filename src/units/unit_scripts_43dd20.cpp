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

class Class_0043cd20 { public: void FUN_0043cd20(Unit* u); };
class Class_0043d290 { public: void FUN_0043d290(Unit* u); };
class Class_0043d6d0 { public: void FUN_0043d6d0(Unit* u); };
class Class_0043da70 { public: void FUN_0043da70(Unit* u); };
class Class_0043db50 { public: void FUN_0043db50(Unit* u); };

class Class_0043dd20 {
public:
    Iface_0043dd20* iface;             // +0x0
    void FUN_0043dd20(Unit* u);
};

// FUNCTION: 0x43dd20
void Class_0043dd20::FUN_0043dd20(Unit* u)
{
    iface->v2();
    if (u->type->flag_800)
        ((Class_0043d290*)this)->FUN_0043d290(u);
    else
        ((Class_0043cd20*)this)->FUN_0043cd20(u);
    ((Class_0043d6d0*)this)->FUN_0043d6d0(u);
    ((Class_0043da70*)this)->FUN_0043da70(u);
    ((Class_0043db50*)this)->FUN_0043db50(u);
}

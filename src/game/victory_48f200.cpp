// Decompiled by Opus. Names are provisional.
// The "move unit to radius" victory condition (vtable 0x4fd890, visitor
// vtable 0x4fd888, the same object as 0x48f250, 0x48f2f0 and 0x48f330): slot 0
// fills in the target height on first use (0x12345678 marks it unset), visits
// the units within the radius and returns whether the condition is met.

struct Vec3_0048f200 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

class Condition_0048f200 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int done;                          // +0x04
    int announced;                     // +0x08
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f200 {
public:
    virtual void Visit(void* unit) = 0;
};

void __stdcall FUN_00484b50(int x, int z, Vec3_0048f200* out);
void __stdcall FUN_0047e890(Vec3_0048f200* pos, int radius, UnitVisitor_0048f200* visitor);

#pragma pack(push, 2)
class VictoryMoveUnitToRadius : public Condition_0048f200, public UnitVisitor_0048f200 {
public:
    char name[0x20];                   // +0x10
    Vec3_0048f200 pos;                 // +0x30
    int radius;                        // +0x3c
    virtual int IsSatisfied();
};
#pragma pack(pop)

// FUNCTION: 0x48f200
int VictoryMoveUnitToRadius::IsSatisfied()
{
    if (pos.y == 0x12345678) {
        FUN_00484b50(pos.x, pos.z, &pos);
    }
    FUN_0047e890(&pos, radius, this);
    return done;
}

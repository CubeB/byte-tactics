// Decompiled by Opus. Names are provisional.
// Runs VisitObjectsInArea over an area with a stack visitor of Class_0047db20, as
// FUN_0047db20 does for an object's area.

class Class_0047db20 {
public:
    virtual void FUN_0047ed30();
};

struct Pt_0047db20 {
    short x;
    short y;
};

void __stdcall VisitObjectsInArea(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor);

// FUNCTION: 0x47cac0
void __stdcall FUN_0047cac0(Pt_0047db20 pos, Pt_0047db20 size)
{
    Class_0047db20 visitor;
    VisitObjectsInArea(pos, size, &visitor);
}

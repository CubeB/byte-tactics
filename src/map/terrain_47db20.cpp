// Decompiled by Opus. Names are provisional.

class Class_0047db20 {
public:
    virtual void FUN_0047ed30();
};

struct Pt_0047db20 {
    short x;
    short y;
};

#pragma pack(push, 1)
struct Obj_0047db20 {
    char unknown_0[0x76];
    Pt_0047db20 pos;                // +0x76
    char unknown_7a[4];
    Pt_0047db20 size;               // +0x7e
    char unknown_82[0x110 - 0x82];
    unsigned int flags;             // +0x110
};
#pragma pack(pop)

void __stdcall FUN_0047e5c0(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor);
void __stdcall RefreshAllPassMaps(Pt_0047db20 pos, Pt_0047db20 size);

// The visitor is declared after the flag update so that it reuses obj's
// stack slot; size and pos go through locals (in that order) so that pos is
// loaded early, as in the original.
// FUNCTION: 0x47db20
void __stdcall FUN_0047db20(Obj_0047db20* obj)
{
    obj->flags |= 0x8000000;
    Class_0047db20 visitor;
    Pt_0047db20 size = obj->size;
    Pt_0047db20 pos = obj->pos;
    FUN_0047e5c0(pos, size, &visitor);
    RefreshAllPassMaps(obj->pos, obj->size);
}

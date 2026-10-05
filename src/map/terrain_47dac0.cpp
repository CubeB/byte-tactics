// Decompiled by Opus. Names are provisional.

struct Pt_0047dac0 {
    short x;
    short y;
};

#pragma pack(push, 1)
struct Obj_0047dac0 {
    char unknown_0[0x76];
    Pt_0047dac0 pos;                   // +0x76
    char unknown_7a[4];
    Pt_0047dac0 size;                  // +0x7e
    char unknown_82[0x10f - 0x82];
    unsigned char bit0 : 1;            // +0x10f
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

int __stdcall FUN_0047d970(Obj_0047dac0* obj, int flag);
void __stdcall FUN_0047c790(Obj_0047dac0* obj);
void __stdcall RefreshAllPassMaps(Pt_0047dac0 pos, Pt_0047dac0 size);

// FUNCTION: 0x47dac0
void __stdcall FUN_0047dac0(Obj_0047dac0* obj, int flag)
{
    if (FUN_0047d970(obj, flag)) {
        obj->bit2 = flag;
        obj->flags |= 0x8000000;
        FUN_0047c790(obj);
        RefreshAllPassMaps(obj->pos, obj->size);
    }
}
